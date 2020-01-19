//
//  Utilities.h
//  texsyn
//
//  Created by Craig Reynolds on 12/16/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include <cassert>
class Vec2;

// for debugging: prints one line with a given C expression, an equals sign,
// and the value of the expression.  For example "angle = 35.6"
#define debugPrint(e) (std::cout << #e" = " << (e) << std::endl << std::flush)

// Just for convenience in hand-written prototypes and tests:
const float pi = M_PI;

// True when a and b differ by no more than epsilon.
bool withinEpsilon(float a, float b, float epsilon);

// Square a float
inline float sq(float f) { return f * f; }

// Returns a float randomly distributed between 0 and 1
float frandom01(void);

// Returns a float randomly distributed between lowerBound and upperBound
float frandom2(float lowerBound, float upperBound);

// Generic interpolation
template<typename F,typename T>
T interpolate (const F& alpha, const T& x0, const T& x1)
{
    return x0 + ((x1 - x0) * alpha);
}

// TODO took this directly from TextureSynthesisTest, needs clean up:
// ----------------------------------------------------------------------------
// Constrain a given value (x) to be between two (ordered) bounds: min
// and max.  Returns x if it is between the bounds, otherwise returns
// the nearer bound.

inline float clip (const float x, const float min, const float max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Maps from 0 to 1 into a sinusoid ramp ("slow in, slow out") from 0 to 1.
inline float sinusoid (float x)
{
    return (1 - std::cos(x * M_PI)) / 2;
}

// remap a value specified relative to a pair of bounding values
// to the corresponding value relative to another pair of bounds.
// Inspired by (dyna:remap-interval y y0 y1 z0 z1)
inline float remapInterval(float x,
                           float in0, float in1,
                           float out0, float out1)
{
    // If the input range is well defined do the linear remapping.
    float input_range = in1 - in0;
    if (input_range > 0)
    {
        // Uninterpolate: what is x relative to input interval?
        float relative = (x - in0) / input_range;
        // Interpolate relative to output interval.
        return interpolate(relative, out0, out1);
    }
    // Otherwise treat as step function.
    return (x <= in0) ? out0 : out1;
}

// Like remapInterval but the result is clipped to remain between out0 and out1
inline float remapIntervalClip(float x,
                               float in0, float in1,
                               float out0, float out1)
{
    return clip(remapInterval(x, in0, in1, out0, out1), out0, out1);
}

// This variation on fmod() is analogous to the difference between "rounding
// down" (as in floor()) and "rounding toward zero" (as in std::round())
inline float fmod_floor(float x, float y)
{
    return std::fmod(x, y) + ((x < 0) ? y : 0);
}

// Perlin Noise
// Ken Perlin's 2002 "Improved Noise": http://mrl.nyu.edu/~perlin/noise/
// This code based on a transliteration by Malcolm Kesson from Java to c:
// http://www.fundza.com/c4serious/noise/perlin/perlin.html
namespace PerlinNoise
{
    // Classic Perlin noise, in 2d, output range approximately on [-1, 1].
    float noise2d(Vec2 position);
    // Classic Perlin noise, in 2d, output range on [0, 1].
    float unitNoise2d(Vec2 position);
    // Classic Perlin turbulence, in 2d, output range on [0, 1].
    float turbulence2d(Vec2 position);
    // Brownian Noise, fractal 1/f Perlin noise, output range on [0, 1].
    float brownian2d(Vec2 position);
    // Furbulence: two "fold" version of Turbulence producing sharp features at
    // both low and high ends of the output range.
    float furbulence2d(Vec2 position);
    // Wrapulence: another variation on turbulence(). noise() is scaled up in
    // value, then wrapped modulo [0, 1]. It has hard edge discontinuities at
    // all scales.
    float wrapulence2d(Vec2 position);
    // Tool to measure typical range of a noise function. Returns min and max
    // range from calling given noise function 100000 times for random points
    // in a circle at origin with diameter of 100.
    std::pair<float, float> measure_range(std::function<float(Vec2)> noise_func);
};

// Generic look up table. Currently only used in Texture Operator StretchSpot,
// so current design is skewed toward that. Could be generalized and made into a
// type agnostic template, but for now (2020-01-18) my goal is just to move some
// code from StretchSpot to here.
//
// A table of N "output" (y) values correspoding to "input" (x) values between.
// TODO interpolation
//
class LookupTable
{
public:
    LookupTable(int size) { table_.resize(size, 0); }
    LookupTable(int size, float value) { table_.resize(size, value); }
    size_t size() const { return table_.size(); }
    float lookup(float input) const { return table_.at(std::floor(input)); }
private:
    std::vector<float> table_;
    float minimum_input_value_ = 0;
    float maximum_input_value_ = 1;
};
