//
//  Operators.h
//  texsyn
//
//  Created by Craig Reynolds on 12/31/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Texture.h"

class Operator : public Texture {};

// Use a given "matte" texture to combine two other textures. The luminance of
// the matte at each color sample is used as the interpolation fraction between
// the corresponding samples of the other textures.
class SoftMatte : public Operator
{
public:
    SoftMatte(const Texture& _matte,
              const Texture& _texture0,
              const Texture& _texture1)
        : matte(_matte), texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        return interpolate(matte.getColor(position).luminance(),
                           texture0.getColor(position),
                           texture1.getColor(position));
    }
private:
    const Texture& matte;
    const Texture& texture0;
    const Texture& texture1;
};

// Add two textures.
class Add : public Operator
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
class Subtract : public Operator
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

// TODO TEMP FOR TESTING? I kept going back and forth about needing this,
//      instead of, or in addition to Subtract. But now I want it for texture
//      comparison while working on StretchSpot

// Names AbsDiff Distance, Norm?

// Absolute value of the difference of two textures. abs(texture0 - texture1)
class AbsDiff : public Operator
{
public:
    AbsDiff(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        Color diff = texture0.getColor(position) - texture1.getColor(position);
        // TODO define overloat of std::abs() for Color?
        return Color(std::abs(diff.r()),
                     std::abs(diff.g()),
                     std::abs(diff.b()));
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Multiply two textures: for each color sample, multiply component-wise the
// corresponding color samples of the two textures.
class Multiply : public Operator
{
public:
    Multiply(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        // TODO should * be defined between two Colors?
        Color c0 = texture0.getColor(position);
        Color c1 = texture1.getColor(position);
        return Color(c0.r() * c1.r(), c0.g() * c1.g(), c0.b() * c1.b());
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Select between two textures, sample by sample, by taking the one whose
// luminance is greater.
class Max : public Operator
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
class Min : public Operator
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

// Maps the brightness of a sample of the given Texture to a pure hue (full
// brightness, full saturation). The hue transform is offset by a given phase.
class BrightnessToHue : public Operator
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
class Wrap : public Operator
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
        float angle = atan2 (p.dot(new_x), p.dot(new_y));
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
// or a contraction (when center_magnification < 1).
class StretchSpot : public Operator
{
public:
    StretchSpot(float _center_magnification,
                float radius,
                Vec2 _center,
                const Texture& _texture) :
        center_magnification(_center_magnification),
        outer_radius(radius),
        center(_center),
        texture(_texture)
    {
        ifNeededInitializeInverseLUT();
    }
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float radius = offset.length();
        float relative_radius = radius / outer_radius;
        float s = (relative_radius == 0 ? 0 :
                   ((relative_radius > 1) ? 1 :
                    ((center_magnification < 1) ?
                     inverseRemapper (relative_radius) :
                     remapper (relative_radius))));
        return texture.getColor((offset * s) + center);
    }
    // maps from relative radius to magnification multiplier between 0 and 1
    float remapper(float rr) const
    {
        return interpolate(interpolate (rr, rr, sinusoid (rr)),
                           ((center_magnification > 1) ?
                            (1 / center_magnification) :
                            center_magnification),
                           1.0f);
    }
    float inverseRemapper(float rr) const
    {
        int i = int(std::round(rr * (lutSize() - 1)));
        return inverse_lut->at(i) / rr;
    }
    void ifNeededInitializeInverseLUT()
    {
        if (center_magnification < 1)
        {
            inverse_lut = std::make_shared<std::vector<float>>(lutSize());
            float lastQ = 0;
            int i = 0;
            inverse_lut->at(0) = 0;
            float inverseRes = 1.0 / lutSize();
            for (float r = 0; r <= 1; r = r + (inverseRes * 0.01))
            {
                float q = r * remapper (r);
                float d = q - lastQ;
                if (d > inverseRes)
                {
                    i++;
                    if (i == lutSize()) break;
                    inverse_lut->at(i) = r;
                    lastQ = q;
                }
            }
            // Fill out LUT in case of numerical error.
            for (; i<lutSize() - 1; ) { inverse_lut->at(++i) = 1; }
        }
    }
private:
    const float center_magnification;
    const float outer_radius;
    const Vec2 center;
    const Texture& texture;
    std::shared_ptr<std::vector<float>> inverse_lut;
    
    // Note that this is massive overkill. The current design looks up a single
    // value in the inverse LUT. This could be much smaller if it interpolated
    // between adjacent values in the LUT. Perhaps it make sense to trade memory
    // for code simplicity. This value was chosen by drawing 511 pixel diameter
    // test textures using this code and visually inspecting for "smooth" edges.
    //     Grating vert_stripes(Vec2(0, 0), Color(1, 1, 1),
    //                          Vec2(0.1, 0), Color(0, 0, 0), 0.2);
    //     StretchSpot(0.5, 10, Vec2(-5, 0), vert_stripes).displayInWindow();
    int lutSize() const { return 10000; }
};
