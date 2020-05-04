//
//  Generators.h
//  texsyn
//
//  Created by Craig Reynolds on 12/21/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Texture.h"

class Generator : public Texture {};

// Color gradation texture with two colors and arbitrary position, width, and
// orientation. The arguments are two points, defining a line segment, and a
// color for each end. The gradation occurs alone the line segment, a given
// location on the texture is projected onto that line to determine its color.
// (See discussion at http://www.red3d.com/cwr/texsyn/diary.html#20090329)
class Gradation : public Generator
{
public:
    Gradation(Vec2 point_0, Color color_0, Vec2 point_1, Color color_1) :
        color0(color_0),
        color1(color_1),
        origin(point_0),
        distance((point_1 - point_0).length()),
        basis((point_1 - point_0) / distance) {}
    Color getColor(Vec2 position) const override
    {
        if (distance == 0)
        {
            return interpolate(0.5, color0, color1);
        }
        else
        {
            Vec2 offset = position - origin;
            float projection = basis.dot(offset);
            float relative = remapIntervalClip(projection, 0, distance, 0, 1);
            return interpolate(sinusoid(relative), color0, color1);
        }
    }
private:
    const Vec2 origin;
    const float distance;
    const Vec2 basis;
    const Color color0;
    const Color color1;
};

// A circular spot, centered at a given point, with two radii. Within an inner
// radius it is uniformly one color. Beyond an outer radius it is uniformly
// another color. Between the two radii, there is a sinusoid transition from one
// color to the other. (Reverses radii if they are out of order.)
class Spot : public Generator
{
public:
    Spot(Vec2 center_,
         float inner_radius_, Color inner_color_,
         float outer_radius_, Color outer_color_) :
            center(center_),
            inner_radius(std::min(inner_radius_, outer_radius_)),
            inner_color(inner_color_),
            outer_radius(std::max(inner_radius_, outer_radius_)),
            outer_color(outer_color_)
    {
        assert(inner_radius >= 0);
        assert(outer_radius >= 0);
        assert(outer_radius >= inner_radius);
    }
    Color getColor(Vec2 position) const override
    {
        // Distance from sample position to spot center.
        float d = (position - center).length();
        // Fraction for interpolation: 0 inside, 1 outside, ramp between.
        float f = remapIntervalClip(d, inner_radius, outer_radius, 0, 1);
        // Sinusoidal interpolation between inner and outer colors.
        return interpolate(sinusoid(f), inner_color, outer_color);
    }
private:
    const Vec2 center;
    const float inner_radius;
    const float outer_radius;
    const Color inner_color;
    const Color outer_color;
};

// A grating of two alternating colored stripes. The spacing and orientation is
// defined by two points. The segment between them is perpendicular to the
// stripes, the length of the segment is the width (wavelength) of the stripes.
// The softness parameter varies from a square wave at 0 and a sinusoid at 1.
class Grating : public Generator
{
public:
    Grating(Vec2 point_0, Color color_0,
            Vec2 point_1, Color color_1,
            float softness_) :
        color0(color_0),
        color1(color_1),
        origin(point_0),
        distance((point_1 - point_0).length()),
        basis((point_1 - point_0) / distance),
        softness(softness_) {}
    Color getColor(Vec2 position) const override
    {
        if (distance == 0)
        {
            return interpolate(0.5, color0, color1);
        }
        else
        {
            Vec2 offset = position - origin;
            float projection = basis.dot(offset);
            float unit_modulo = fmod_floor(projection, distance) / distance;
            return interpolate(softSquareWave(unit_modulo), color0, color1);
        }
    }
    // Defines a "square wave with soft edges". When softness is 0 it is a
    // square wave. When softness is 1 it is a sinusoid.
    float softSquareWave(float fraction) const
    {
        // Clip fraction to [0, 1].
        fraction = clip(fraction, 0, 1);
        // Fold second half of range back over first. f ranges over [0, 0.5].
        float f = (fraction < 0.5) ? fraction : (1 - fraction);
        // Start/end of transition region, adjusted for softness.
        float s = remapInterval(softness, 0, 1, 0.25, 0);
        float e = remapInterval(softness, 0, 1, 0.25, 0.5);
        // Piecewise linear transition (flat, ramp, flat).
        float adjust_for_softness = remapIntervalClip(f, s, e, 0, 1);
        // Apply sinusoid to adjusted value.
        return sinusoid(adjust_for_softness);
    }
private:
    const Vec2 origin;
    const float distance;
    const Vec2 basis;
    const Color color0;
    const Color color1;
    const float softness;
};

// Perlin Noise
// Ken Perlin's 2002 "Improved Noise": http://mrl.nyu.edu/~perlin/noise/
// This code based on a transliteration by Malcolm Kesson from Java to c:
// http://www.fundza.com/c4serious/noise/perlin/perlin.html
class Noise : public Generator
{
public:
    Noise (float _scale, Vec2 _center, Color _color0, Color _color1) :
        scale (_scale), center (_center), color0(_color0), color1(_color1) {};
    Color getColor(Vec2 position) const override
    {
        float noise = PerlinNoise::unitNoise2d((position - center) / scale);
        return interpolate(noise, color0, color1);
    }
private:
    const float scale;
    const Vec2 center;
    const Color color0;
    const Color color1;
};

// Brownian Noise -- fractal 1/f Perlin Noise
class Brownian : public Generator
{
public:
    Brownian(float _scale, Vec2 _center, Color _color0, Color _color1)
        : scale (_scale), center (_center), color0(_color0), color1(_color1) {};
    Color getColor(Vec2 position) const override
    {
        float noise = PerlinNoise::brownian2d((position - center) / scale);
        return interpolate(noise, color0, color1);
    }
private:
    const float scale;
    const Vec2 center;
    const Color color0;
    const Color color1;
};

// Classic Perlin turbulence.
class Turbulence : public Generator
{
public:
    Turbulence(float _scale, Vec2 _center, Color _color0, Color _color1)
        : scale (_scale), center (_center), color0(_color0), color1(_color1) {};
    Color getColor(Vec2 position) const override
    {
        float t = PerlinNoise::turbulence2d((position - center) / scale);
        return interpolate(t, color0, color1);
    }
private:
    const float scale;
    const Vec2 center;
    const Color color0;
    const Color color1;
};

// Furbulence: two "fold" version of Turbulence producing sharp features at
// both low and high ends of the output range.
class Furbulence : public Generator
{
public:
    Furbulence(float _scale, Vec2 _center, Color _color0, Color _color1)
        : scale (_scale), center (_center), color0(_color0), color1(_color1) {};
    Color getColor(Vec2 position) const override
    {
        float f = PerlinNoise::furbulence2d((position - center) / scale);
        return interpolate(f, color0, color1);
    }
private:
    const float scale;
    const Vec2 center;
    const Color color0;
    const Color color1;
};

// Wrapulence: another variation on turbulence(). noise() is scaled up in
// value, then wrapped modulo [0, 1]. It has hard edge discontinuities at
// all scales.
class Wrapulence : public Generator
{
public:
    Wrapulence(float _scale, Vec2 _center, Color _color0, Color _color1)
        : scale (_scale), center (_center), color0(_color0), color1(_color1) {};
    Color getColor(Vec2 position) const override
    {
        float w = PerlinNoise::wrapulence2d((position - center) / scale);
        return interpolate(w, color0, color1);
    }
private:
    const float scale;
    const Vec2 center;
    const Color color0;
    const Color color1;
};

// MultiNoise: combines five noise generators (Noise, Brownian, Turbulence,
// Furbulence, Wrapulence) into one, with an extra float argument (on [0, 1])
// that selects between them. (This is to allow all the variations without
// "overwhelming" the space of generators when randomly selecting for GP. An
// alternative would be to specify a "likeliness" weighting in the GP defs for
// making the random selections.)
class MultiNoise : public Generator
{
public:
    MultiNoise(float _scale, Vec2 _center,
               Color _color0, Color _color1,
               float _which)
      : scale(_scale),
        center(_center),
        color0(_color0),
        color1(_color1),
        which(_which) {};
    Color getColor(Vec2 position) const override
    {
        Vec2 adjusted = (position - center) / scale;
        float noise = PerlinNoise::multiNoise2d(adjusted, which);
        return interpolate(noise, color0, color1);
    }
private:
    const float scale;
    const Vec2 center;
    const Color color0;
    const Color color1;
    const float which;
};

// Color Noise -- RGB Perlin Noise
// (TODO should this have a "which" parameter like MultiNoise? Maybe there
// should be a PerlinNoise::multiNoise2d() helper function?)
class ColorNoise : public Generator
{
public:
    ColorNoise(float _scale, Vec2 _center, float _which)
      : scale(_scale),
        center(_center),
        which(_which),
        offset1(Vec2(1, 0)),
        offset2(offset1.rotate(pi * 2 / 3)),
        offset3(offset2.rotate(pi * 2 / 3)) {};
    Color getColor(Vec2 position) const override
    {
        Vec2 p1 = ((position + offset1 - center) / scale).rotate(0.3);
        Vec2 p2 = ((position + offset2 - center) / scale).rotate(0.6);
        Vec2 p3 = ((position + offset3 - center) / scale).rotate(0.9);
        return Color(PerlinNoise::multiNoise2d(p1, which),
                     PerlinNoise::multiNoise2d(p2, which),
                     PerlinNoise::multiNoise2d(p3, which));
    }
private:
    const float scale;
    const Vec2 center;
    const float which;
    const Vec2 offset1;
    const Vec2 offset2;
    const Vec2 offset3;
};

// I had been resisting this as unncessary (not minimal) but now (March 1, 2020)
// adding it in the same spirit as Scale/Rotate/Translate: convenient for hand
// written code.
class Uniform : public Generator
{
public:
    Uniform(Color _color) : color(_color) {};
    Uniform(float red, float green, float blue) : color(red, green, blue) {};
    Uniform(float _gray_value) : color(Color::gray(_gray_value)) {};
    Color getColor(Vec2 position) const override { return color; }
private:
    const Color color;
};
