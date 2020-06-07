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

// Gradation between two textures with arbitrary position, width, and
// orientation. The arguments are two points, defining a line segment, and a
// texture for each end. The gradation occurs along the line segment, a given
// location on the texture is projected onto that line to determine its mix of
// the two input textures.
class Gradation : public Generator
{
public:
        Gradation(Vec2 point_0, const Texture& texture_0,
                  Vec2 point_1, const Texture& texture_1) :
            texture0(texture_0),
            texture1(texture_1),
        origin(point_0),
        distance((point_1 - point_0).length()),
        basis((point_1 - point_0) / distance) {}
    // BACKWARD_COMPATIBILITY for version before inherent matting.
    Gradation(Vec2 a, Color b, Vec2 c, Color d)
      : Gradation(a, disposableUniform(b), c, disposableUniform(d)){}
    Color getColor(Vec2 position) const override
    {
        // TODO isn't this handled inside interpolate these days?
        if (distance == 0)
        {
            return interpolate(0.5,
                               texture0.getColor(position),
                               texture1.getColor(position));
        }
        else
        {
            Vec2 offset = position - origin;
            float projection = basis.dot(offset);
            float relative = remapIntervalClip(projection, 0, distance, 0, 1);
            return interpolate(deGamma(sinusoid(relative)),
                               texture0.getColor(position),
                               texture1.getColor(position));
        }
    }
private:
    const Vec2 origin;
    const float distance;
    const Vec2 basis;
    const Texture& texture0;
    const Texture& texture1;
};

// A circular spot, centered at a given point, with two radii. Within an inner
// radius it is colored according to one input texture. Beyond an outer radius
// it is uniformly colored according a second input texture. Between the two
// radii, there is a sinusoid transition from one texture to the other.
// (Reverses radii if they are out of order.)
class Spot : public Generator
{
public:
    Spot(Vec2 center_,
         float inner_radius_, const Texture& inner_texture_,
         float outer_radius_, const Texture& outer_texture_)
      : center(center_),
        inner_radius(std::min(inner_radius_, outer_radius_)),
        inner_texture(inner_texture_),
        outer_radius(std::max(inner_radius_, outer_radius_)),
        outer_texture(outer_texture_)
    {
        assert(inner_radius >= 0);
        assert(outer_radius >= 0);
        assert(outer_radius >= inner_radius);
    }
    // BACKWARD_COMPATIBILITY for version before inherent matting.
    Spot(Vec2 a, float b, Color c, float d, Color e)
      : Spot(a, b, disposableUniform(c), d, disposableUniform(e)){}
    Color getColor(Vec2 position) const override
    {
        // Distance from sample position to spot center.
        float d = (position - center).length();
        // Fraction for interpolation: 0 inside, 1 outside, ramp between.
        float f = remapIntervalClip(d, inner_radius, outer_radius, 0, 1);
        // Sinusoidal interpolation between inner and outer colors.
        return interpolate(deGamma(sinusoid(f)),
                           inner_texture.getColor(position),
                           outer_texture.getColor(position));
    }
private:
    const Vec2 center;
    const float inner_radius;
    const float outer_radius;
    const Texture& inner_texture;
    const Texture& outer_texture;
};

// A grating of two alternating stripes each colored according to two given
// input textures. Spacing and orientation is defined by two points. Stripes are
// perpendicular to the segment between these two points. That segment's length
// is the width (wavelength) of the stripe. The softness parameter varies from a
// square wave at 0 and a sinusoid at 1. The duty_cycle parameter controls
// relative width of sub-stripes, it is the ratio of the first color's stripes
// to the stripe pair's total width.
class Grating : public Generator
{
public:
    Grating(Vec2 point_0, const Texture& texture_0,
            Vec2 point_1, const Texture& texture_1,
            float softness_,
            float duty_cycle_) :
        texture0(texture_0),
        texture1(texture_1),
        origin(point_0),
        distance((point_1 - point_0).length()),
        basis((point_1 - point_0) / distance),
        softness(clip(softness_, 0, 1)),
        duty_cycle(clip(duty_cycle_, 0, 1)) {}
    // BACKWARD_COMPATIBILITY with version before duty_cycle, inherent matting.
    Grating(Vec2 a, Color b, Vec2 c, Color d, float e)
      : Grating(a, disposableUniform(b), c, disposableUniform(d), e, 0.5) {}
    Grating(Vec2 a, Color b, Vec2 c, Color d, float e, float f)
      : Grating(a, disposableUniform(b), c, disposableUniform(d), e, f) {}
    Color getColor(Vec2 position) const override
    {
        if (distance == 0)
        {
            return interpolate(0.5,
                               texture0.getColor(position),
                               texture1.getColor(position));
        }
        else
        {
            Vec2 offset = position - origin;
            float projection = basis.dot(offset);
            // unit_modulo is the normalized "cross stripe coordinate" on [0, 1]
            float unit_modulo = fmod_floor(projection, distance) / distance;
            // It is adjusted for "duty cycle" and fed into "soft edge square
            // wave" generator, finally de-gamma-ed to preserve equal stripe
            // width (assuming duty-cycle = 0.5).
            return interpolate(deGamma(softSquareWave(dutyCycle(unit_modulo))),
                               texture0.getColor(position),
                               texture1.getColor(position));
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
    // Adjust for duty_cycle. Modifies normalized cross-strip coordinate. For
    // dc=0.5, color1 corresponds to the middle half, interval [0.25, 0.75].
    // For dc=1 interval is [0, 1], for dc=0 interval is [0.5, 0.5].
    float dutyCycle(float fraction) const
    {
        // offset phase by 0.25 aka 90° to put middle of raising edge at 0
        auto offset_phase = [](float p, float o){return std::fmod(p + o, 1.0);};
        float i = offset_phase(fraction, 0.25);
        // Two linear ramps, from (0,0) to (dc,0.5), then (dc,0.5) to (1,1).
        float result = ((i < duty_cycle) ?
                        remapInterval(i,  0, duty_cycle,  0, 0.5) :
                        remapInterval(i,  duty_cycle, 1,  0.5, 1));
        return offset_phase(result, 0.75);
    }
private:
    const Vec2 origin;
    const float distance;
    const Vec2 basis;
    const Texture& texture0;
    const Texture& texture1;
    const float softness;
    const float duty_cycle;
};

// Ken Perlin's 2002 "Improved Noise": http://mrl.nyu.edu/~perlin/noise/
// This and other noise textures below use PerlinNoise package in Utilities.h
class Noise : public Generator
{
public:
    Noise(Vec2 point_0,
          Vec2 point_1,
          const Texture& texture_0,
          const Texture& texture_1)
      : scale((point_1 - point_0).length()),
        basis((point_1 - point_0).normalize()),
        center (point_0),
        texture0(texture_0),
        texture1(texture_1) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 transformed_position = transformIntoNoiseSpace(position);
        return interpolate(getScalerNoise(transformed_position),
                           //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                           texture0.getColor(transformed_position),
//                           texture1.getColor(transformed_position));
                           texture0.getColor(position),
                           texture1.getColor(position));
                           //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    // Get scalar noise fraction on [0, 1] for the given transformed position.
    // Overridden by other noise-based textures to customize basic behavior.
    virtual float getScalerNoise(Vec2 transformed_position) const
    {
        return PerlinNoise::unitNoise2d(transformed_position);
    }
    // Transform a point from texture space into noise space.
    Vec2 transformIntoNoiseSpace(Vec2 position) const
    {
        Vec2 moved = position - center;
        Vec2 scaled = moved / scale;
        Vec2 rotated = scaled.localize(basis, basis.rotate90degCCW());
        return rotated;
    }
    // BACKWARD_COMPATIBILITY with version before "two point" specification.
    Noise(float a, Vec2 b, const Texture& c, const Texture& d)
      : Noise(b, b + Vec2(a, 0), c, d) {};
    // BACKWARD_COMPATIBILITY with version before inherent matting.
    Noise(float a, Vec2 b, Color c, Color d)
      : Noise(a, b, disposableUniform(c), disposableUniform(d)) {}
private:
    const float scale;
    const Vec2 center;
    const Vec2 basis;
    const Texture& texture0;
    const Texture& texture1;
};

// Brownian Noise -- multi octave fractal 1/f Perlin Noise
class Brownian : public Noise
{
public:
    Brownian(Vec2 point_0,
             Vec2 point_1,
             const Texture& texture_0,
             const Texture& texture_1)
      : Noise(point_0, point_1, texture_0, texture_1) {};
    float getScalerNoise(Vec2 transformed_position) const override
    {
        return PerlinNoise::brownian2d(transformed_position);
    }
    // BACKWARD_COMPATIBILITY with version before "two point" specification.
    Brownian(float a, Vec2 b, const Texture& c, const Texture& d)
      : Brownian(b, b + Vec2(a, 0), c, d) {};
    // BACKWARD_COMPATIBILITY with version before inherent matting.
    Brownian(float a, Vec2 b, Color c, Color d)
        : Brownian(a, b, disposableUniform(c), disposableUniform(d)) {};
};

// Classic Perlin turbulence.
class Turbulence : public Noise
{
public:
    Turbulence(Vec2 point_0,
               Vec2 point_1,
               const Texture& texture_0,
               const Texture& texture_1)
      : Noise(point_0, point_1, texture_0, texture_1) {};
    float getScalerNoise(Vec2 transformed_position) const override
    {
        return PerlinNoise::turbulence2d(transformed_position);
    }
    // BACKWARD_COMPATIBILITY with version before "two point" specification.
    Turbulence(float a, Vec2 b, const Texture& c, const Texture& d)
      : Turbulence(b, b + Vec2(a, 0), c, d) {};
    // BACKWARD_COMPATIBILITY with version before inherent matting.
    Turbulence(float a, Vec2 b, Color c, Color d)
        : Turbulence(a, b, disposableUniform(c), disposableUniform(d)) {};
};

// Furbulence: two "fold" version of Turbulence producing sharp features at
// both low and high ends of the output range.
class Furbulence : public Noise
{
public:
    Furbulence(Vec2 point_0,
               Vec2 point_1,
               const Texture& texture_0,
               const Texture& texture_1)
      : Noise(point_0, point_1, texture_0, texture_1) {};
    float getScalerNoise(Vec2 transformed_position) const override
    {
        return PerlinNoise::furbulence2d(transformed_position);
    }
    // BACKWARD_COMPATIBILITY with version before "two point" specification.
    Furbulence(float a, Vec2 b, const Texture& c, const Texture& d)
      : Furbulence(b, b + Vec2(a, 0), c, d) {};
    // BACKWARD_COMPATIBILITY with version before inherent matting.
    Furbulence(float a, Vec2 b, Color c, Color d)
        : Furbulence(a, b, disposableUniform(c), disposableUniform(d)) {};
};

// Wrapulence: another variation on turbulence(). noise() is scaled up in value,
// then wrapped modulo [0, 1]. It has hard edge discontinuities at all scales.
class Wrapulence : public Noise
{
public:
    Wrapulence(Vec2 point_0,
               Vec2 point_1,
               const Texture& texture_0,
               const Texture& texture_1)
      : Noise(point_0, point_1, texture_0, texture_1) {};
    float getScalerNoise(Vec2 transformed_position) const override
    {
        return PerlinNoise::wrapulence2d(transformed_position);
    }
    // BACKWARD_COMPATIBILITY with version before "two point" specification.
    Wrapulence(float a, Vec2 b, const Texture& c, const Texture& d)
      : Wrapulence(b, b + Vec2(a, 0), c, d) {};
    // BACKWARD_COMPATIBILITY with version before inherent matting.
    Wrapulence(float a, Vec2 b, Color c, Color d)
        : Wrapulence(a, b, disposableUniform(c), disposableUniform(d)) {};
};

// MultiNoise: combines five noise generators (Noise, Brownian, Turbulence,
// Furbulence, Wrapulence) into one, with an extra float argument (on [0, 1])
// that selects between them. (This is to allow all the variations without
// "overwhelming" the space of generators when randomly selecting for GP. An
// alternative would be to specify a "likeliness" weighting in the GP defs for
// making the random selections.)
class MultiNoise : public Noise
{
public:
    MultiNoise(Vec2 point_0,
               Vec2 point_1,
               const Texture& texture_0,
               const Texture& texture_1,
               float _which)
      : Noise(point_0, point_1, texture_0, texture_1),
        which(_which) {};
    float getScalerNoise(Vec2 transformed_position) const override
    {
        return PerlinNoise::multiNoise2d(transformed_position, which);
    }
    // BACKWARD_COMPATIBILITY with version before "two point" specification.
    MultiNoise(float a, Vec2 b, const Texture& c, const Texture& d, float e)
      : MultiNoise(b, b + Vec2(a, 0), c, d, e) {};
    // BACKWARD_COMPATIBILITY with version before inherent matting.
    MultiNoise(float a, Vec2 b, Color c, Color d, float e)
        : MultiNoise(a, b, disposableUniform(c), disposableUniform(d), e) {};
    const float which;
};

// Color Noise -- RGB Perlin Noise
class ColorNoise : public MultiNoise
{
public:
    ColorNoise(Vec2 point_0, Vec2 point_1, float _which)
      : MultiNoise(point_0, point_1, *this, *this, _which),
        offset1(Vec2(1, 0)),
        offset2(offset1.rotate(pi * 2 / 3)),
        offset3(offset2.rotate(pi * 2 / 3)) {};
    
    Color getColor(Vec2 position) const override
    {
        Vec2 tp1 = transformIntoNoiseSpace(position + offset1).rotate(0.3);
        Vec2 tp2 = transformIntoNoiseSpace(position + offset2).rotate(0.6);
        Vec2 tp3 = transformIntoNoiseSpace(position + offset3).rotate(0.9);
        return Color(PerlinNoise::multiNoise2d(tp1, which),
                     PerlinNoise::multiNoise2d(tp2, which),
                     PerlinNoise::multiNoise2d(tp3, which));
    }
    // BACKWARD_COMPATIBILITY with version before "two point" specification.
    ColorNoise(float a, Vec2 b, float c) : ColorNoise(b, b + Vec2(a, 0), c) {};
private:
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
