//
//  Operators.h
//  texsyn
//
//  Created by Craig Reynolds on 12/31/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Texture.h"
#include "Disk.h"
#include "COTS.h"

// Minimal texture, a uniform color everywhere on the texture plane. Its single
// parameter is that color. As a convenience for hand written code, also can be
// constructed from three RGB floats, or a single gray level float.
class Uniform : public Texture
{
public:
    Uniform(Color _color) : color(_color) {};
    Uniform(float red, float green, float blue) : color(red, green, blue) {};
    Uniform(float _gray_value) : color(Color::gray(_gray_value)) {};
    Color getColor(Vec2 position) const override { return color; }
private:
    const Color color;
};

// A circular spot, centered at a given point, with two radii. Within an inner
// radius it is colored according to one input texture. Beyond an outer radius
// it is uniformly colored according a second input texture. Between the two
// radii, there is a sinusoid transition from one texture to the other.
// (Reverses radii if they are out of order.)
class Spot : public Texture
{
public:
    Spot(Vec2 center_,
         float inner_radius_, const Texture& inner_texture_,
         float outer_radius_, const Texture& outer_texture_)
      : center(center_),
        inner_radius(std::max(0.0f, std::min(inner_radius_, outer_radius_))),
        inner_texture(inner_texture_),
        outer_radius(std::max(0.0f, std::max(inner_radius_, outer_radius_))),
        outer_texture(outer_texture_) {}
    Color getColor(Vec2 position) const override
    {
        // Distance from sample position to spot center.
        float d = (position - center).length();
        // Fraction for interpolation: 0 inside, 1 outside, ramp between.
        float f = remapIntervalClip(d, inner_radius, outer_radius, 0, 1);
        // Sinusoidal interpolation between inner and outer colors.
        return interpolatePointOnTextures(sinusoid(f), position, position,
                                          inner_texture, outer_texture);
    }
    // BACKWARD_COMPATIBILITY for version before inherent matting.
    Spot(Vec2 a, float b, Color c, float d, Color e)
      : Spot(a, b, disposableUniform(c), d, disposableUniform(e)){}
private:
    const Vec2 center;
    const float inner_radius;
    const float outer_radius;
    const Texture& inner_texture;
    const Texture& outer_texture;
};

// Gradation between two textures with arbitrary position, width, and
// orientation. The arguments are two points, defining a line segment, and a
// texture for each end. The gradation occurs along the line segment, a given
// location on the texture is projected onto that line to determine its mix of
// the two input textures.
class Gradation : public Texture
{
public:
        Gradation(Vec2 point_0, const Texture& texture_0,
                  Vec2 point_1, const Texture& texture_1) :
            texture0(texture_0),
            texture1(texture_1),
        origin(point_0),
        distance((point_1 - point_0).length()),
        basis((point_1 - point_0) / distance) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - origin;
        float projection = basis.dot(offset);
        float relative = remapIntervalClip(projection, 0, distance, 0, 1);
        return interpolatePointOnTextures(sinusoid(relative),
                                          position,
                                          position,
                                          texture0,
                                          texture1);
    }
    // BACKWARD_COMPATIBILITY for version before inherent matting.
    Gradation(Vec2 a, Color b, Vec2 c, Color d)
      : Gradation(a, disposableUniform(b), c, disposableUniform(d)){}
private:
    const Vec2 origin;
    const float distance;
    const Vec2 basis;
    const Texture& texture0;
    const Texture& texture1;
};

// A grating of two alternating stripes each colored according to two given
// input textures. Spacing and orientation is defined by two points. Stripes are
// perpendicular to the segment between these two points. That segment's length
// is the width (wavelength) of the stripe. The softness parameter varies from a
// square wave at 0 and a sinusoid at 1. The duty_cycle parameter controls
// relative width of sub-stripes, it is the ratio of the first color's stripes
// to the stripe pair's total width.
class Grating : public Texture
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
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - origin;
        float projection = basis.dot(offset);
        // unit_modulo is the normalized "cross stripe coordinate" on [0, 1]
        float unit_modulo = fmod_floor(projection, distance) / distance;
        // Adjust for "duty cycle" then then for "soft-edge square wave".
        float alpha = softSquareWave(dutyCycle(unit_modulo));
        if (distance == 0) alpha = 0.5;
        return interpolatePointOnTextures(alpha,
                                          position, position,
                                          texture0, texture1);
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
    // BACKWARD_COMPATIBILITY with version before duty_cycle, inherent matting.
    Grating(Vec2 a, Color b, Vec2 c, Color d, float e)
      : Grating(a, disposableUniform(b), c, disposableUniform(d), e, 0.5) {}
    Grating(Vec2 a, Color b, Vec2 c, Color d, float e, float f)
      : Grating(a, disposableUniform(b), c, disposableUniform(d), e, f) {}
private:
    const Vec2 origin;
    const float distance;
    const Vec2 basis;
    const Texture& texture0;
    const Texture& texture1;
    const float softness;
    const float duty_cycle;
};

class SoftMatte : public Texture
{
public:
    SoftMatte(const Texture& _matte,
              const Texture& _texture0,
              const Texture& _texture1)
        : matte(_matte), texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        return interpolatePointOnTextures(matte.getColor(position).luminance(),
                                          position, position,
                                          texture0, texture1);
    }
private:
    const Texture& matte;
    const Texture& texture0;
    const Texture& texture1;
};

// Add two textures.
class Add : public Texture
{
public:
    Add(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        return texture0.getColor(position) + texture1.getColor(position);
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Subtract two textures. (texture0 - texture1)
class Subtract : public Texture
{
public:
    Subtract(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        return texture0.getColor(position) - texture1.getColor(position);
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Multiply two textures: for each color sample, multiply component-wise the
// corresponding color samples of the two textures.
class Multiply : public Texture
{
public:
    Multiply(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        return texture0.getColor(position) * texture1.getColor(position);
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Select between two textures, sample by sample, by taking the one whose
// luminance is greater.
class Max : public Texture
{
public:
    Max(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        Color c0 = texture0.getColor(position);
        Color c1 = texture1.getColor(position);
        return (c0.luminance() > c1.luminance()) ? c0 : c1;
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Select between two textures, sample by sample, by taking the one whose
// luminance is less.
class Min : public Texture
{
public:
    Min(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        Color c0 = texture0.getColor(position);
        Color c1 = texture1.getColor(position);
        return (c0.luminance() < c1.luminance()) ? c0 : c1;
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Absolute value of the difference of two textures. abs(texture0 - texture1)
// (TODO Names AbsDiff Distance, Norm?)
class AbsDiff : public Texture
{
public:
    AbsDiff(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        Color diff = texture0.getColor(position) - texture1.getColor(position);
        // TODO define overload of std::abs() for Color?
        return Color(std::abs(diff.r()),
                     std::abs(diff.g()),
                     std::abs(diff.b()));
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Ken Perlin's 2002 "Improved Noise": http://mrl.nyu.edu/~perlin/noise/
// This and other noise textures below use PerlinNoise package in Utilities.h
class Noise : public Texture
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
        float blend = getScalerNoise(transformIntoNoiseSpace(position));
        if (scale == 0) blend = 0.5;
        return interpolatePointOnTextures(blend,
                                          position, position,
                                          texture0, texture1);
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

// Maps the brightness of a sample of the given Texture to a pure hue (full
// brightness, full saturation). The hue transform is offset by a given phase.
class BrightnessToHue : public Texture
{
public:
    BrightnessToHue (float _huePhase, const Texture& _texture)
        : huePhase (_huePhase), texture (_texture) {}
    Color getColor(Vec2 position) const override
    {
        float luminance = texture.getColor(position).luminance();
        float red, green, blue;
        Color::convertHSVtoRGB(luminance + huePhase, 1.0f, 1.0f,
                               red, green, blue);
        return Color(red, green, blue);
    }
private:
    const float huePhase;
    const Texture& texture;
};

// Wrap a given portion of a half-plane radially around a given center point,
// keeping the texture along a given ray unchanged. Related to a rectangular-
// (Cartesian)-to-polar transform.
//
// The parameters are the "width" of a vertical slice of the half-plane which
// will be wrapped, the "center" of rotation, a "fixed_ray" vector indicating
// the orientation of the fixed point ray extending from the center, and the
// source "texture".
//
// Consider the "fixed_ray" to be the +Y axis of a space with "center" at its
// origin. For any point on the X axis from [-width, width] consider the ray
// parallel to +Y. The X coordinate is mapped to an angle of rotation from +Y.
//
class Wrap : public Texture
{
public:
    Wrap(float _width, Vec2 _center, Vec2 _fixed_ray, const Texture& _texture)
      : width(_width),
        center(_center),
        fixed_ray(_fixed_ray),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        // Position relative to "center".
        Vec2 p = position - center;
        // X and Y basis vectors of transformed space.
        Vec2 new_y = fixed_ray.normalize();
        Vec2 new_x = new_y.rotate90degCW();
        // Measure angle (0 parallel to new_y axis, pi/2 parallel to new_x).
        float angle = Vec2(p.dot(new_x), p.dot(new_y)).atan2();
        // Distance from "center".
        float radius = p.length();
        Vec2 new_point = (center +
                          new_x * remapInterval(angle, -pi, pi, -width, width) +
                          new_y * radius);
        return texture.getColor(new_point);
    }
private:
    const float width;
    const Vec2 center;
    const Vec2 fixed_ray;
    const Texture& texture;
};

// Modifies the given texture within a disk of "radius" around "center", doing a
// "fisheye" expansion of the center of the disk (when center_magnification > 1)
// or a contraction (when center_magnification < 1). Rewritten on July 2, 2020.
class StretchSpot : public Texture
{
public:
    StretchSpot(float _center_magnification,
                float _spot_radius,
                Vec2 _center,
                const Texture& _texture) :
        center_magnification(std::max(_center_magnification, 0.0f)),
        spot_radius(std::max(_spot_radius, 0.0f)),
        center(_center),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float r = offset.length();  // radius from center
        float rr = clip01(r / spot_radius);  // "relative radius" on [0, 1]
        float taper = interpolate(std::pow(rr, 5), rr, sinusoid(rr));
        float scale = interpolate(taper, center_magnification, 1.0f);
        return texture.getColor((offset / scale) + center);
    }
private:
    const float center_magnification;
    const float spot_radius;
    const Vec2 center;
    const Texture& texture;
};

// Stretch (scale in one dimension) the given input Texture. The stretching is
// defined by the first Vec2 argument "stretch". Its magnitude is the scale
// factor, and its direction is the axis scaling. The "center" argument is the
// origin of the scaling. It is unchanged by the transform, as is the line
// through it perpendicular to the "stretch" vector.
class Stretch : public Texture
{
public:
    Stretch(Vec2 _stretch, Vec2 _center, const Texture& _texture)
      : scale(_stretch.length()),
        main_basis(_stretch / scale),
        perp_basis(main_basis.rotate90degCW()),
        center(_center),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float main_distance = offset.dot(main_basis);
        float perp_distance = offset.dot(perp_basis);
        float stretched = main_distance / scale;
        return texture.getColor(center +
                                main_basis * stretched +
                                perp_basis * perp_distance);
    }
private:
    const float scale;
    const Vec2 main_basis;
    const Vec2 perp_basis;
    const Vec2 center;
    const Texture& texture;
};

// Creates a Color grating by taking a slice(/transit/1d texture/waveform) from
// a given Texture, then sweeping that pattern in a perpendicular direction. The
// slice is defined by a tangent vector and a center. The tangent basis vector
// give the direction of the slice and its length is a scaling factor along the
// slice. (Nearly identical to a very large first arg to Stretch.)
class SliceGrating : public Texture
{
public:
    SliceGrating(Vec2 _slice_tangent, Vec2 _center, const Texture& _texture)
      : slice_tangent(_slice_tangent),
        center(_center),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float projection = offset.dot(slice_tangent);
        return texture.getColor(center + (slice_tangent * projection));
    }
private:
    const Vec2 slice_tangent;
    const Vec2 center;
    const Texture& texture;
};

// Creates a radial color pattern based on a slice(/transit/1d texture/waveform)
// from another Texture. The slice is defined by a "center" point on it and a
// tangent vector indicating its orientation and scaling. The radial pattern
// consists of rays of colors read from a portion of the slice, addressed by
// angle relative to the tangent direction.
class SliceToRadial : public Texture
{
public:
    SliceToRadial(Vec2 _slice_tangent, Vec2 _center, const Texture& _texture)
      : slice_tangent(_slice_tangent),
        perpendicular(slice_tangent.rotate90degCW()),
        center(_center),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float angle = std::atan2(offset.dot(perpendicular),
                                 offset.dot(slice_tangent));
        return texture.getColor(center + (slice_tangent * angle));
    }
private:
    const Vec2 slice_tangent;
    const Vec2 perpendicular;
    const Vec2 center;
    const Texture& texture;
};

// Shear one Texture, along a given axis, by an amount based on luminance values
// from a slice of another Texture. Parameters are the two Textures, along with
// an axis for each, defined as a center point and tangent vector.
class SliceShear : public Texture
{
public:
    SliceShear(Vec2 _slice_tangent,
               Vec2 _slice_center,
               const Texture& _texture_for_slice,
               Vec2 _shear_tangent,
               Vec2 _shear_center,
               const Texture& _texture_to_shear)
      : slice_tangent(_slice_tangent),
        slice_center(_slice_center),
        texture_for_slice(_texture_for_slice),
        shear_tangent(_shear_tangent),
        shear_center(_shear_center),
        texture_to_shear(_texture_to_shear),
        perpendicular(shear_tangent.rotate90degCCW()) {}
    Color getColor(Vec2 position) const override
    {
        // Find point on texture_to_shear: decompose into x,y in shear space,
        // offset x by luminince from slice sample, recombine to new position.
        Vec2 shear_offset = position - shear_center;
        float local_x = shear_offset.dot(shear_tangent);
        float local_y = shear_offset.dot(perpendicular);
        // Use "local_y" to select a color along the slice.
        Vec2 slice_sample_position = slice_center + slice_tangent * local_y;
        // Sample color from "texture_for_slice" then take its luminance.
        Color slice_color = texture_for_slice.getColor(slice_sample_position);
        float luminance = slice_color.luminance();
        // Reconstruct sample location with its "x" coord offset by "luminance".
        return texture_to_shear.getColor(shear_center +
                                         shear_tangent * (local_x + luminance) +
                                         perpendicular * local_y);
    }
private:
    const Vec2 slice_tangent;
    const Vec2 slice_center;
    const Texture& texture_for_slice;
    const Vec2 shear_tangent;
    const Vec2 shear_center;
    const Texture& texture_to_shear;
    const Vec2 perpendicular;
};

// Colorize one texture by mapping its luminance to the sequence of colors
// along a slice(/transit/1d texture/waveform) of another texture.
class Colorize : public Texture
{
public:
    Colorize(Vec2 _slice_tangent,
             Vec2 _center,
             const Texture& _texture_for_slice,
             const Texture& _texture_to_color)
      : slice_tangent(_slice_tangent),
        center(_center),
        texture_for_slice(_texture_for_slice),
        texture_to_color(_texture_to_color) {}
    Color getColor(Vec2 position) const override
    {
        // Look up color to be tinted/colorized an get its luminance.
        Color original = texture_to_color.getColor(position);
        float luminance = original.luminance();
        // Look up color by luminance along slice in texture_for_slice.
        Vec2 on_slice = center + (slice_tangent * luminance);
        return texture_for_slice.getColor(on_slice);
    }
private:
    const Vec2 slice_tangent;
    const Vec2 center;
    const Texture& texture_for_slice;
    const Texture& texture_to_color;
};


// Use the Möbius transformation of the complex plane as a Texture Operator by
// considering it isomorphic to the Cartesian Texture plane. The transformation
// is parameterized by four "points" (aka complex numbers). The Wikipedia
// article (https://en.wikipedia.org/wiki/Möbius_transformation) says the four
// points should satisfy: ad − bc ≠ 0.
// See Tim Hutton cool app: http://timhutton.github.io/mobius-transforms/
class MobiusTransform : public Texture
{
public:
    MobiusTransform(Vec2 _a, Vec2 _b, Vec2 _c, Vec2 _d, const Texture& _texture)
      : a(Vec2ToComplex(_a)),
        b(Vec2ToComplex(_b)),
        c(Vec2ToComplex(_c)),
        d(Vec2ToComplex(_d)),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Complex z = Vec2ToComplex(position);
        Complex imt = inverse_mobius_transform(z, a, b, c, d);
        return texture.getColor(ComplexToVec2(imt));
    }
    static Vec2 ComplexToVec2(Complex z) { return {z.real(), z.imag()}; }
    static Complex Vec2ToComplex(Vec2 v) { return {v.x(), v.y()}; }
private:
    const Complex a;
    const Complex b;
    const Complex c;
    const Complex d;
    const Texture& texture;
};

// Scale rigid geometric transformation. Mostly for hand-written code.
// TODO should this have a "center" parameter? For GP it would make sense,
// but if this is just for hand-written code it can be handled by ordering.
class Scale : public Texture
{
public:
    Scale(float _scale, const Texture& _texture)
        : scale(_scale), texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        return texture.getColor(position / scale);
    }
private:
    const float scale;
    const Texture& texture;
};

// Rotate rigid geometric transformation. Mostly for hand-written code.
// TODO should this have a "center" parameter? For GP it would make sense,
// but if this is just for hand-written code it can be handled by ordering.
class Rotate : public Texture
{
public:
    Rotate(float _angle, const Texture& _texture)
        : angle(_angle), texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        return texture.getColor(position.rotate(-angle));
    }
private:
    const float angle;
    const Texture& texture;
};

// Translate rigid geometric transformation. Mostly for hand-written code.
// TODO should this have a "center" parameter? For GP it would make sense,
// but if this is just for hand-written code it can be handled by ordering.
class Translate : public Texture
{
public:
    Translate(Vec2 _translation, const Texture& _texture)
        : translation(_translation), texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        return texture.getColor(position - translation);
    }
private:
    const Vec2 translation;
    const Texture& texture;
};

// Uses a "distributed sampling" approach. Takes subsamples on a jiggled NxN
// grid spaning the bounding square of a cosinusoidal kernel with the given
// diameter. N can be statically adjusted with Blur::sqrt_of_subsample_count.
// For each subsample, it looks up a color in the input texture, adjusts it
// by the kernel weight, and averages the result.
class Blur : public Texture
{
public:
    Blur(const float _width, const Texture& _texture)
        : width(_width), texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        float radius = width / 2;
        std::vector<Vec2> offsets;
        RandomSequence rs(position.hash());
        jittered_grid_NxN_in_square(sqrt_of_subsample_count, width, rs, offsets);
        Color sum_of_weighted_colors(0, 0, 0);
        float sum_of_weights = 0;
        for (Vec2 offset : offsets)
        {
            float length = offset.length();
            if (length <= radius)
            {
                float weight = 1 - sinusoid(length / radius);
                Color color_at_offset = texture.getColor(position + offset);
                sum_of_weighted_colors += color_at_offset * weight;
                sum_of_weights += weight;
            }
        }
        return sum_of_weighted_colors / sum_of_weights;
    }
    // Each Blur::getColor() uses an NxN jiggled grid of subsamples, where N is:
    static int sqrt_of_subsample_count;
private:
    const float width;
    const Texture& texture;
};

// Given two intensity thresholds and an input texture, remap the texture colors
// between those thresholds to "full intensity range". Converts the texture to
// hue-saturation-value color space. Areas darker than the lower threshold get
// value=0, areas brighter than the upper threshold get value=1, areas between
// the two thresholds are mapped to values on [0, 1]. The hue and saturation
// components remain unchanged.
class SoftThreshold : public Texture
{
public:
    SoftThreshold (float _intensity0, float _intensity1, const Texture& _texture)
      : intensity0(std::min(_intensity0, _intensity1)),
        intensity1(std::max(_intensity0, _intensity1)),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Color color = texture.getColor(position);
        float hue, saturation, value;
        color.getHSV(hue, saturation, value);
        float new_v = remapIntervalClip(value, intensity0, intensity1, 0, 1);
        color.setHSV(hue, saturation, new_v);
        return color;
    }
private:
    const float intensity0;
    const float intensity1;
    const Texture& texture;
};

// Finds (colored) edges based on Blur and "unsharp masking" -- subtracting the
// blurred texture from the original.
//
// TODO this is a "modernized" version of the old August 2009 version. It finds
// the edges, a signal which is symmetric around zero brightness, then biases
// that up by adding middle gray. That last part seems questionable, but
// definitely produces a result more likely to be visable.
class EdgeDetect : public Texture
{
public:
    EdgeDetect(const float width, const Texture& texture)
      : blur(width, texture),
        edges(texture, blur) {}
    Color getColor(Vec2 position) const override
    {
        return edges.getColor(position) + Color::gray(0.5);
    }
private:
    Blur blur;
    Subtract edges;
};

// Enhances the edges (emphasize the high frequencies) of a given texture. Based
// on EdgeDetect, which is based on Blur via the technique of “unsharp masking”.
// Parameters include a filter (kernel) width, and a scale factor controlling
// the strength of the edge enhancement.
class EdgeEnhance : public Texture
{
public:
    EdgeEnhance(const float _width,
                const float _strength,
                const Texture& _texture)
      : texture(_texture),
        strength(_strength),
        blurred(_width, texture) {}
    Color getColor(Vec2 position) const override
    {
        Color orig_color = texture.getColor(position);
        Color blur_color = blurred.getColor(position);
        return orig_color + ((orig_color - blur_color) * strength);
    }
private:
    const float strength;
    const Texture& texture;
    const Blur blurred;
};

// In HSV space, rotates the hue by the given "offset". "H" is on [0, 1] so
// H+offset is taken "f-modulo 1.0". Only the fractional part of offset is
// meaningful.
class AdjustHue : public Texture
{
public:
    AdjustHue(float _offset, const Texture& _texture)
      : offset(_offset),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Color color = texture.getColor(position);
        float hue, saturation, value;
        color.getHSV(hue, saturation, value);
        color.setHSV(std::fmod(hue + offset, 1), saturation, value);
        return color;
    }
private:
    const float offset;
    const Texture& texture;
};

// Adjust saturation: in HSV space, scale (and clip) saturation.
class AdjustSaturation : public Texture
{
public:
    AdjustSaturation (float _factor, const Texture& _texture)
      : factor(_factor),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Color color = texture.getColor(position);
        float hue, saturation, value;
        color.getHSV(hue, saturation, value);
        color.setHSV(hue, clip(saturation * factor, 0, 1), value);
        return color;
    }
private:
    const float factor;
    const Texture& texture;
};

// Adjust brightness: scale all colors by a given factor. RGB components are
// multiplied by the factor. See also Multiply which forms the product of two
// textures. In the previous version of this library there was a Tint operator
// that multiplied a Texture by a Color.
class AdjustBrightness : public Texture
{
public:
    AdjustBrightness (float _factor, const Texture& _texture)
      : factor(_factor),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        return texture.getColor(position) * factor;
    }
private:
    const float factor;
    const Texture& texture;
};

// Twist an input texture around a given "center". The twist has infinite
// extent but falls off as 1/r. This creates a spiral tightly curved near
// "center" and asymptotically approaching zero curvature for increasing radius.
// The Twist is parameterized by an "angle_scale" (bigger values mean more
// twisting) and a "radius_scale" which adjusts the rate of falloff (bigger
// values pull the twisting closed to "center"). The twist angle is:
// angle = angle_scale / ((radius * radius_scale) + 1)
class Twist : public Texture
{
public:
    Twist (float _angle_scale,
           float _radius_scale,
           const Vec2 _center,
           const Texture&
           _texture)
      : angle_scale(_angle_scale),
        radius_scale(_radius_scale),
        center(_center),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float radius = offset.length();
        float angle = angle_scale / ((radius * radius_scale) + 1);
        Vec2 rotated_offset = offset.rotate(angle);
        return texture.getColor(center + rotated_offset);
    }
private:
    const float angle_scale;
    const float radius_scale;
    const Vec2 center;
    const Texture& texture;
};

// BrightnessWrap, analogous to SoftThreshold, takes two brightness thresholds
// and an input texture. The brightness of the input texture is “wrapped around”
// in the sense of modulus (fmod) the brightness interval between the two
// thresholds. Then that brightness interval is adjusted to cover the interval
// between black and white. (That adjustment to full range had not been done in
// the previous version of this library. I now think it makes more sense. But I
// need to think about it some more.) These operations on brightness happen in
// hue-saturation-value color space, so only brightness (value) is changed. The
// hue and saturation remain unchanged.
class BrightnessWrap : public Texture
{
public:
    BrightnessWrap (float _intensity0, float _intensity1, const Texture& _texture)
      : intensity0(std::min(_intensity0, _intensity1)),
        intensity1(std::max(_intensity0, _intensity1)),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Color color = texture.getColor(position);
        float hue, saturation, value;
        color.getHSV(hue, saturation, value);
        float new_v = value;
        float interval = intensity1 - intensity0;
        if (interval > 0)
        {
            float between = fmod_floor(value - intensity0, interval);
            new_v = remapIntervalClip(between + intensity0,
                                      intensity0, intensity1,
                                      0, 1);
        }
        color.setHSV(hue, saturation, new_v);
        return color;
    }
private:
    const float intensity0;
    const float intensity1;
    const Texture& texture;
};

// Mirror across a line in texture space, defined by a tangent and center point.
class Mirror : public Texture
{
public:
    Mirror(Vec2 _line_tangent, Vec2 _center, const Texture& _texture)
      : line_tangent(_line_tangent),
        perpendicular(line_tangent.rotate90degCW()),
        center(_center),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float along = offset.dot(line_tangent);
        float across = std::abs(offset.dot(perpendicular));
        return texture.getColor(center +
                                (line_tangent * along) +
                                (perpendicular * across));
    }
private:
    const Vec2 line_tangent;
    const Vec2 perpendicular;
    const Vec2 center;
    const Texture& texture;
};

// The Ring operator takes a pie-slice-shaped sector from an input texture,
// copying and rotating around a center to create the whole “pie.” Parameters to
// Ring are: copies (how many “pie pieces” there are), basis (the direction of
// the bisector of the sector to be copied), center (around which the sectors
// are rotated), and the input texture. The ray from center, along the direction
// of basis, is a “fixed point” of the Ring operator: texture within the fixed
// sector (centered on that ray) is identical to the input texture.
class Ring : public Texture
{
public:
    Ring(float _copies, Vec2 _basis, Vec2 _center, const Texture& _texture)
      : copies(std::max(1.0f, std::round(std::abs(_copies)))),
        basis(_basis),
        basis_angle(basis.atan2()),
        center(_center),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float angle = offset.atan2() - basis_angle;
        float segment_angle = 2 * pi / copies;
        int segment_index = std::round(angle / segment_angle);
        float lookup_angle = angle - (segment_angle * segment_index);
        float radius = offset.length();
        Vec2 spoke = basis.rotate(lookup_angle).normalize() * radius;
        return texture.getColor(center + spoke);
    }
private:
    const float copies;
    const Vec2 basis;
    const float basis_angle;
    const Vec2 center;
    const Texture& texture;
};

// The Row operator tiles the texture plane by copying and translating a portion
// (a fixed width “stripe”) of an input texture. Its parameters are two Vec2
// values: basis and center. The basis vector spans from one boundary of a
// “stripe” to the other (the length of basis is the width of the stripe, and
// its orientation is normal to the stripe's “axis”). The center position adjust
// the “phase” of stripes, effectively sliding the input texture relative to the
// stripe. The stripe that is centered on center is a “fixed point” of the Row
// transform, it remains identical to the input texture.
class Row : public Texture
{
public:
    Row(Vec2 _basis, Vec2 _center, const Texture& _texture)
      : width(_basis.length()),
        basis(_basis / width),
        perpendicular(basis.rotate90degCW()),
        center(_center - basis * width / 2),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float basis_proj = basis.dot(offset);
        float perp_proj = perpendicular.dot(offset);
        float within_stripe = fmod_floor(basis_proj, width);
        return texture.getColor(center +
                                basis * within_stripe +
                                perpendicular * perp_proj);
    }
private:
    const float width;
    const Vec2 basis;
    const Vec2 perpendicular;
    const Vec2 center;
    const Texture& texture;
};

// Returns Lambertian shading factor given a Vec3 unit surface normal and a Vec3
// unit vector toward the light source.
// TODO This is shared between ShadedSphereTest and Shade. Needs clean up.
inline float lambertian_shading(const Vec3& surface_normal,
                                const Vec3& toward_light)
{
    return std::max(0.0f, surface_normal.dot(toward_light));
}

// TODO experimental
// This Shader operator takes two input textures, one for colors and one whose
// luminance defines a height field. For each getColor() it samples a triangle
// near the given position to determine a surface normal. A Lambertian shade is
// determined according to a given illumination direction and a given ambient
// illumination. The corresponding spot on the color texture is scaled by this
// shade.
class Shader : public Texture
{
public:
    Shader(Vec3 _toward_light,
           float _ambient_level,
           const Texture& _color_texture,
           const Texture& _bump_texture)
      : toward_light(_toward_light.normalize()),
        ambient_level(_ambient_level),
        color_texture(_color_texture),
        bump_texture(_bump_texture) {}
    Color getColor(Vec2 position) const override
    {
        // Local random number generator seeded by "position" parameter.
        RandomSequence rs(position.hash());
        // Make a small offset (from position) in a random direction.
        Vec2 offset = rs.randomUnitVector() * 0.004; // roughly a pixel (~2/511)
        // Gets 3d vertex of triangle on bump map, centered on "position".
        auto getOneVertex = [&]()
        {
            // Rotate offset by 1/3.
            offset = offset.rotate(2 * pi / 3);
            // 2d position of vertex on bump map.
            Vec2 vertex2d = position + offset;
            // Form 3d triangle vertex using Z from bump map luminance.
            float height = bump_texture.getColor(vertex2d).luminance();
            return Vec3(vertex2d.x(), vertex2d.y(), height);
        };
        // Three triangle vertices
        Vec3 a = getOneVertex();
        Vec3 b = getOneVertex();
        Vec3 c = getOneVertex();
        // Cross edge tangents to get surface normal.
        Vec3 surface_normal = (a - b).cross(c - b).normalize();
        // Get Lambertian reflectance factor.
        float shade = lambertian_shading(surface_normal, toward_light);
        // Shade input color.
        return color_texture.getColor(position) * (shade + ambient_level);
    }
private:
    const Vec3 toward_light;
    const float ambient_level;
    const Texture& color_texture;
    const Texture& bump_texture;
};

// A new LotsOfSpots (see http://www.red3d.com/cwr/texsyn/diary.html#20100208).
// This is defined across entire texture plane. Current approach: just tile the
// plane, using a “large” square tile (10x10 compared to the typical Texture
// rendering size of 2x2), centered on the origin.
//
// Ideas: the spots themselves could be like the Spot operator, or more in the
// sense of SoftMatte(Spot(), a, b) where the spot's texture is taken from
// another texture. Spot size would be in an interval, or another distribution,
// or taken from the luninence of another texture. The “colored spots” demo
// (http://www.red3d.com/cwr/texsyn/diary.html#20100209) was also a nice idea,
// sampling spot colors from another texture.
//
// Basic geometric framework is in base class:
//     LotsOfSpotsBase(density, min_r, max_r, soft)
// Derived from that are:
//     LotsOfSpots(density, min_r, max_r, soft, spot_color, bg_color)
//     ColoredSpots(density, min_r, max_r, soft, color_texture, bg_color)
//     LotsOfButtons(density, min_r, max_r, soft, button_center,
//                   button_texture, bg_color)
//
class LotsOfSpotsBase : public Texture
{
public:
      LotsOfSpotsBase(float _spot_density,
                      float _min_radius,
                      float _max_radius,
                      float _soft_edge_width,
                      float _margin)
      : spot_density(std::min(_spot_density, 1.0f)),
        min_radius(std::min(_min_radius, _max_radius) + _margin),
        max_radius(std::max(_min_radius, _max_radius) + _margin),
        soft_edge_width(_soft_edge_width),
        margin(_margin),
        disk_occupancy_grid
            (std::make_shared<DiskOccupancyGrid>(Vec2(-5, -5), Vec2(5, 5), 60))
    {
        Timer timer("LotsOfSpots constructor");  // TODO temp
        insertRandomSpots();
        disk_occupancy_grid->reduceDiskOverlap(200, spots);
    }
    // Find nearest spot (Dot) and the soft-edged opacity at "position".
    typedef std::pair<Disk, float> DiskAndSoft;
    DiskAndSoft getSpot(Vec2 position) const
    {
        float gray_level = 0;
        Disk nearest_spot;
        Vec2 tiled_pos = disk_occupancy_grid->wrapToCenterTile(position);
        std::set<Disk*> disks;
        disk_occupancy_grid->findNearbyDisks(tiled_pos, disks);
        for (auto& disk : disks)
        {
            Disk spot = *disk;
            spot.radius -= margin;
            // Adjust spot center to be nearest "tiled_pos" maybe in other tile.
            Vec2 tiled_spot =
                disk_occupancy_grid->nearestByTiling(tiled_pos, spot.position);
            // Distance from sample position to spot center. Ignore if too far.
            float d = (tiled_pos - tiled_spot).length();
            if (d <= spot.radius)
            {
                float inner = std::max(0.0f, spot.radius - soft_edge_width);
                // Interpolation fraction: 0 inside, 1 outside, ramp between.
                float f = remapIntervalClip(d, inner, spot.radius, 0, 1);
                // Sinusoidal interpolation between inner and outer colors.
                float spot_level = interpolate(sinusoid(f), 1.0f, 0.0f);
                gray_level = std::max(gray_level, spot_level);
                nearest_spot = spot;
            }
        }
        return std::make_pair(nearest_spot, gray_level);
    }
    // Insert random Disks until density threshold is met. Disk center positions
    // are uniformly distributed across center tile. Radii are chosen from the
    // interval [min_radius, max_radius] with a preference for smaller values.
    void insertRandomSpots();
    void randomizeSpotRotations();
    size_t seedForRandomSequence();
private:
    std::vector<Disk> spots;
    std::shared_ptr<DiskOccupancyGrid> disk_occupancy_grid;
    const float tile_size = 10;
    const int move_count = 200;
    const float spot_density;
    const float min_radius;
    const float max_radius;
    const float soft_edge_width;
    const float margin;
};

// Collection of spots matte "spot_texture" over "background_texture".
class LotsOfSpots : public LotsOfSpotsBase
{
public:
    LotsOfSpots(float _spot_density,
                float _min_radius,
                float _max_radius,
                float _soft_edge_width,
                float _margin,
                const Texture& _spot_texture,
                const Texture& _background_texture)
      : LotsOfSpotsBase(_spot_density, _min_radius, _max_radius,
                        _soft_edge_width, _margin),
        spot_texture(_spot_texture),
        background_texture(_background_texture) {}
    // BACKWARD_COMPATIBILITY for version before "margin", "background_texture"
    LotsOfSpots(float a, float b, float c, float d, Color e, Color f)
      : LotsOfSpots(a, b, c, d, 0, disposableUniform(e), disposableUniform(f)){}
    Color getColor(Vec2 position) const override
    {
        DiskAndSoft das = getSpot(position);
        return interpolatePointOnTextures(das.second,
                                          position, position,
                                          background_texture, spot_texture);
    }
private:
    const Texture& spot_texture;
    const Texture& background_texture;
};

// Collection of spots take their color from "color_texture" sampled at the
// center of each spot. These are matted over "background_texture".
class ColoredSpots : public LotsOfSpotsBase
{
public:
    ColoredSpots(float _spot_density,
                 float _min_radius,
                 float _max_radius,
                 float _soft_edge_width,
                 float _margin,
                 const Texture& _color_texture,
                 const Texture& _background_texture)
      : LotsOfSpotsBase(_spot_density, _min_radius, _max_radius,
                        _soft_edge_width, _margin),
        color_texture(_color_texture),
        background_texture(_background_texture) {}
    // BACKWARD_COMPATIBILITY for version before "margin", "background_texture"
    ColoredSpots(float a, float b, float c, float d, const Texture& e, Color f)
      : ColoredSpots(a, b, c, d, 0, e, disposableUniform(f)) {}
    Color getColor(Vec2 position) const override
    {
        DiskAndSoft das = getSpot(position);
        return interpolatePointOnTextures(das.second,
                                          position,
                                          das.first.position,
                                          background_texture,
                                          color_texture);
    }
private:
    const Texture& background_texture;
    const Texture& color_texture;
};

// Collection of spots, each containing a portion of "button_texture" near
// "button_center". If "button_random_rotate" is > 0.5, each spot will be
// rotated about its center. The spots are matted over "background_texture."
class LotsOfButtons : public LotsOfSpotsBase
{
public:
    LotsOfButtons(float _spot_density,
                  float _min_radius,
                  float _max_radius,
                  float _soft_edge_width,
                  float _margin,
                  Vec2 _button_center,
                  const Texture& _button_texture,
                  float _button_random_rotate,
                  const Texture& _background_texture)
      : LotsOfSpotsBase(_spot_density, _min_radius, _max_radius,
                        _soft_edge_width, _margin),
        button_center(_button_center),
        button_texture(_button_texture),
        button_random_rotate(_button_random_rotate),
        background_texture(_background_texture)
    {
        if (button_random_rotate > 0.5) randomizeSpotRotations();
    }
    // BACKWARD_COMPATIBILITY for version before "margin", "background_texture"
    LotsOfButtons(float a, float b, float c, float d,
                  Vec2 e, const Texture& f, float g, Color h)
      : LotsOfButtons(a, b, c, d, 0, e, f, g, disposableUniform(h)) {}
    Color getColor(Vec2 position) const override
    {
        DiskAndSoft das = getSpot(position);
        float matte = das.second;
        Vec2 spot_center = das.first.position;
        Vec2 rotated = (position - spot_center).rotate(das.first.angle);
        Vec2 button_sample_point = button_center + rotated;
        return interpolatePointOnTextures(matte,
                                          position,
                                          button_sample_point,
                                          background_texture,
                                          button_texture);
    }
private:
    const Vec2 button_center;
    const Texture& button_texture;
    const float button_random_rotate;
    const Texture& background_texture;
};

// Gamma provides the standard operation used in digital image processing:
// exponentiating the RGB components by a given parameter gamma (𝛾). Values
// other than 1 cause a nonlinear contrast mapping. When gamma is less than 1:
// mid-range colors will be brighter. When gamma is greater than 1: mid-range
// colors will be darker. The brightest and darkest colors stay about the same.
// Normally gamma correction is used on color values within the positive unit
// RGB cube. In TexSyn colors are three arbitrary floats.
class Gamma : public Texture
{
public:
    Gamma(float _exponent, const Texture& _texture)
      : exponent(_exponent),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        return texture.getColor(position).gamma(exponent);
    }
private:
    const float exponent;
    const Texture& texture;
};

// Linear remap of positive unit RGB color cube to a given "box" within it.
// Parameters are the min and max bounds of the box for each of red, green and
// blue. So for example The clipped input color's red component is remapped from
// the interval [0, 1] to [min_r, max_r].
class RgbBox : public Texture
{
public:
    RgbBox(float minr, float maxr,
           float ming, float maxg,
           float minb, float maxb,
           const Texture& _texture)
      : min_r(clip01(std::min(minr, maxr))),
        max_r(clip01(std::max(minr, maxr))),
        min_g(clip01(std::min(ming, maxg))),
        max_g(clip01(std::max(ming, maxg))),
        min_b(clip01(std::min(minb, maxb))),
        max_b(clip01(std::max(minb, maxb))),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Color input = texture.getColor(position).clipToUnitRGB();
        return Color(remapInterval(input.r(), 0, 1, min_r, max_r),
                     remapInterval(input.g(), 0, 1, min_g, max_g),
                     remapInterval(input.b(), 0, 1, min_b, max_b));
    }
private:
    const float min_r;
    const float max_r;
    const float min_g;
    const float max_g;
    const float min_b;
    const float max_b;
    const Texture& texture;
};

// Image warp via Jarek Rossignac's 2020 “Corner-operated Tran-similar (COTS)
// Maps, Patterns, and Lattices”. See COTS.h for full citation and more details.
// The four Vec2 parameters specify the corners onto which the input texture's
// unit square will be projected.
class CotsMap : public Texture
{
public:
    CotsMap(Vec2 A, Vec2 B, Vec2 C, Vec2 D, const Texture& _texture)
      : cots_map(A, B, C, D),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        return texture.getColor(cots_map.Inverse(position));
    }
private:
    const COTS cots_map;
    const Texture& texture;
};

// Maps entire "texture_to_warp" onto a given circle (defined by "center" and
// "radius") using a hyperbolic projection. The result is large in the center,
// becoming asymptotically tiny at the circle's edge. "exponent" controls the
// rate of compression approaching the edge. "scale" is the magnification at
// the center, which falls off toward the edge. Pixels outside the given disk
// are taken from "background_texture". When "scale" is 0.5 and "exponent" is
// 2, this projection corresponds (I think) to Poincaré's disk model of the
// hyperbolic plane. See: https://cwreynolds.github.io/TexSyn/#20200711
class Hyperbolic : public Texture
{
public:
    Hyperbolic(Vec2 _center,
               float _radius,
               float _scale,
               float _exponent,
               const Texture& _texture_to_warp,
               const Texture& _background_texture)
      : center(_center),
        radius(_radius),
        scale(_scale),
        exponent(_exponent),
        texture_to_warp(_texture_to_warp),
        background_texture(_background_texture) {}
    // Original constructor, assumed scale and exponent were 1.
    Hyperbolic(Vec2 _center,
               float _radius,
               const Texture& _texture_to_warp,
               const Texture& _background_texture)
      : Hyperbolic(_center, _radius, 1, 1,
                   _texture_to_warp, _background_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = (position - center) / radius;
        float relative_radius = offset.length();
        float e = std::pow(relative_radius, exponent);
        return ((relative_radius < 1) ?
                texture_to_warp.getColor(offset / (scale * (1 - e))) :
                background_texture.getColor(position));
    }
private:
    const Vec2 center;
    const float radius;
    const float scale;
    const float exponent;
    const Texture& texture_to_warp;
    const Texture& background_texture;
};

// Simple "affine transformation" parameterized by the "two point" specification
// used by several other operators. Its result is the combination of a scale,
// rotation, and traslation of the input texture. The two-point specification
// provides the positions of Vec2(0, 0) and Vec2(1, 0) after the transformation.
class Affine : public Texture
{
public:
    Affine(Vec2 point_0, Vec2 point_1, const Texture& texture)
      : scale((point_1 - point_0).length()),
        basis((point_1 - point_0).normalize()),
        perp(basis.rotate90degCCW()),
        center (point_0),
        texture(texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 moved = position - center;
        Vec2 scaled = moved / scale;
        Vec2 rotated = scaled.localize(basis, perp);
        return texture.getColor(rotated);
    }
private:
    const float scale;
    const Vec2 center;
    const Vec2 basis;
    const Vec2 perp;
    const Texture& texture;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO experimental analog of RgbBox but for HSV color space.

// Linear remap of positive unit RGB color cube to a given "box" within it.
// Parameters are the min and max bounds of the box for each of red, green and
// blue. So for example The clipped input color's red component is remapped from
// the interval [0, 1] to [min_r, max_r].

//    class RgbBox : public Texture
//    {
//    public:
//        RgbBox(float minr, float maxr,
//               float ming, float maxg,
//               float minb, float maxb,
//               const Texture& _texture)
//          : min_r(clip01(std::min(minr, maxr))),
//            max_r(clip01(std::max(minr, maxr))),
//            min_g(clip01(std::min(ming, maxg))),
//            max_g(clip01(std::max(ming, maxg))),
//            min_b(clip01(std::min(minb, maxb))),
//            max_b(clip01(std::max(minb, maxb))),
//            texture(_texture) {}
//        Color getColor(Vec2 position) const override
//        {
//            Color input = texture.getColor(position).clipToUnitRGB();
//            return Color(remapInterval(input.r(), 0, 1, min_r, max_r),
//                         remapInterval(input.g(), 0, 1, min_g, max_g),
//                         remapInterval(input.b(), 0, 1, min_b, max_b));
//        }
//    private:
//        const float min_r;
//        const float max_r;
//        const float min_g;
//        const float max_g;
//        const float min_b;
//        const float max_b;
//        const Texture& texture;
//    };

class HsvBox : public Texture
{
public:
    HsvBox(float minh, float maxh,
           float mins, float maxs,
           float minv, float maxv,
           const Texture& _texture)
        // trying it without imposing an ordering
//      : min_h(clip01(std::min(minh, maxh))),
//        max_h(clip01(std::max(minh, maxh))),
//        min_s(clip01(std::min(mins, maxs))),
//        max_s(clip01(std::max(mins, maxs))),
//        min_v(clip01(std::min(minv, maxv))),
//        max_v(clip01(std::max(minv, maxv))),
        : min_h(clip01(minh)),
          max_h(clip01(maxh)),
          min_s(clip01(mins)),
          max_s(clip01(maxs)),
          min_v(clip01(minv)),
          max_v(clip01(maxv)),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        // TODO do I want to keep the clip?
        Color input = texture.getColor(position).clipToUnitRGB();
        float h1, s1, v1;
        Color::convertRGBtoHSV(input.r(), input.g(), input.b(), h1, s1, v1);
        
        //float h2 = remapInterval(h1, 0, 1, min_h, max_h);
        float h_folded = sinusoid(h1 < 0.5 ? h1 * 2 : 2 - (h1 * 2));
        float h2 = remapInterval(h_folded, 0, 1, min_h, max_h);

        float s2 = remapInterval(s1, 0, 1, min_s, max_s);
        float v2 = remapInterval(v1, 0, 1, min_v, max_v);
        return Color::makeHSV(h2, s2, v2);
    }
private:
    const float min_h;
    const float max_h;
    const float min_s;
    const float max_s;
    const float min_v;
    const float max_v;
    const Texture& texture;
};

// TODO temporary, just or testing
class HueRing : public Texture
{
public:
    HueRing(){}
//    Color getColor(Vec2 position) const override
//    {
//        Color color(0, 0, 0);
//        if (between(position.length(), 0.5, 0.9))
//        {
//            float angle = position.atan2();
//            color = Color::makeHSV(angle / (2 * pi), 1, 1);
//        }
//        return color;
//    }
//        Color getColor(Vec2 position) const override
//        {
//            Color color(0, 0, 0);
//            float radius = position.length();
//    //        float ri = 0.1;
//    //        float rm = 0.5;
//    //        float ro = 0.9;
//            float ri = 0.0;
//            float rm = 0.5;
//            float ro = 1.0;
//            if (between(radius, ri, ro))
//            {
//                color = Color::makeHSV(position.atan2() / (2 * pi),
//                                       remapInterval(radius, ri, rm, 0, 1),
//                                       remapInterval(radius, rm, ro, 1, 0));
//            }
//            return color;
//        }
//        Color getColor(Vec2 position) const override
//        {
//            Color color(0, 0, 0);
//            float radius = position.length();
//    //        float r1 = 0.3;
//    //        float r2 = 0.8;
//    //        float r1 = 0.5;
//    //        float r2 = 0.6;
//            float r1 = 0.4;
//            float r2 = 0.6;
//            if (radius < 1)
//            {
//    //            color = Color::makeHSV(position.atan2() / (2 * pi),
//    //                                   remapInterval(radius, 0, r1, 0, 1),
//    //                                   remapInterval(radius, r2, 1, 1, 0));
//    //            color = Color::makeHSV(position.atan2() / (2 * pi),
//    //                                   remapIntervalClip(radius, 0, r1, 0, 1),
//    //                                   remapIntervalClip(radius, r2, 1, 1, 0));
//    //            color = Color::makeHSV(position.atan2() / (2 * pi),
//    //                                   remapIntervalClip(radius, 0, r1, 0, 1),
//    ////                                   remapIntervalClip(radius, r2, 1, 1, 0)
//    //                                   1 - remapIntervalClip(radius, r2, 1, 0, 1)
//    //                                   );
//
//    //            color = Color::makeHSV(position.atan2() / (2 * pi),
//    //                                   sinusoid(remapIntervalClip(radius, 0, r1, 0, 1)),
//    ////                                   sinusoid(remapIntervalClip(radius, r2, 1, 1, 0))
//    //                                   sinusoid(1 - remapIntervalClip(radius,
//    //                                                                  r2, 1, 0, 1))
//    //                                   );
//    //
//
//
//                color = Color::makeHSV(position.atan2() / (2 * pi),
//                                       sinusoid(remapIntervalClip(radius, 0, r1, 0, 1)),
//                                       sinusoid(remapIntervalClip(radius, r2, 1, 1, 0))
//                                       );
//
//
//
//            }
//            return color;
//        }

    Color getColor(Vec2 position) const override
    {
        Color color(0, 0, 0);
        float radius = position.length();
        if (radius < 1)
        {
            float r1 = 0.4;
            float r2 = 0.6;
            float h = position.atan2() / (2 * pi);
            float s = sinusoid(remapIntervalClip(radius,  0, r1,  0, 1));
            float v = sinusoid(remapIntervalClip(radius,  r2, 1,  1, 0));
            color = Color::makeHSV(h, s, v);
        }
        return color;
    }
};

//    // TODO super prototype
//
//    class HueIfAny : public Texture
//    {
//    public:
//        HueIfAny(float _saturation, float _value, const Texture& _texture)
//          : saturation(_saturation), value(_value), texture(_texture) {}
//        Color getColor(Vec2 position) const override
//        {
//            Color input = texture.getColor(position);
//            Color clipped = input.clipToUnitRGB();
//            float h, s, v;
//            Color::convertRGBtoHSV(clipped.r(), clipped.g(), clipped.b(), h, s, v);
//            return (s < min_sat ? input : Color::makeHSV(h, saturation, value));
//        }
//    private:
//        const float min_sat = 0.000001;
//        const float saturation;
//        const float value;
//        const Texture& texture;
//    };


class HueOnly : public Texture
{
public:
    HueOnly(float _saturation, float _value, const Texture& _texture)
      : saturation(_saturation), value(_value), texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Color input = texture.getColor(position);
        Color clipped = input.clipToUnitRGB();
        float h, s, v;
        Color::convertRGBtoHSV(clipped.r(), clipped.g(), clipped.b(), h, s, v);
        return (s < min_sat ? input : Color::makeHSV(h, saturation, value));
    }
private:
    const float min_sat = 0.000001;
    const float saturation;
    const float value;
    const Texture& texture;
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
