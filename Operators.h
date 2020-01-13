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
