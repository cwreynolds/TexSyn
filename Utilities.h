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
// maps a number from 0 to 1 into a sinusoid ramp ("slow in, slow out") from 0 to 1
inline float sinusoid (float x)
{
    return 0.5f * (1.0f + (-1.0f * cosf (x * M_PI)));
}
// ----------------------------------------------------------------------------
// remap a value specified relative to a pair of bounding values
// to the corresponding value relative to another pair of bounds.
// Inspired by (dyna:remap-interval y y0 y1 z0 z1)
//
// note that these break (NAN) if the input interval is zero


inline float remapInterval(float x,
                           float in0, float in1,
                           float out0, float out1)
{
    // uninterpolate: what is x relative to the interval in0:in1?
    float relative = (x - in0) / (in1 - in0);
    
    // now interpolate between output interval based on relative x
    return interpolate(relative, out0, out1);
}

// Like remapInterval but the result is clipped to remain between out0 and out1
inline float remapIntervalClip(float x,
                               float in0, float in1,
                               float out0, float out1)
{
    // uninterpolate: what is x relative to the interval in0:in1?
    float relative = (x - in0) / (in1 - in0);
    
    // now interpolate between output interval based on relative x
    return interpolate(clip(relative, 0, 1), out0, out1);
}
