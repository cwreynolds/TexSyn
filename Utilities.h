//
//  Utilities.h
//  texsyn
//
//  Created by Craig Reynolds on 12/16/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once

#include <iostream>
#include <cassert>
#include <cmath>

// for debugging: prints one line with a given C expression, an equals sign,
// and the value of the expression.  For example "angle = 35.6"
#define debugPrint(e) (std::cout << #e" = " << (e) << std::endl << std::flush)

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
