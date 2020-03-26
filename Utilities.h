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
#include <complex>
#include <iostream>
#include <limits>
#include <vector>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <chrono>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
    // Returns result of one of the noise functions (unitNoise2d, turbulence2d,
    // brownian2d, furbulence2d, wrapulence2d -- selected according to "which")
    // applied to "position".
    float multiNoise2d(Vec2 position, float which);
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


// Compute the inverse Möbius transformation of the complex plane. It is
// parameterized by four "points" (aka complex numbers). The Wikipedia
// article (https://en.wikipedia.org/wiki/Möbius_transformation) says the four
// points should satisfy: ad − bc ≠ 0.
// See Tim Hutton cool app: http://timhutton.github.io/mobius-transforms/
typedef std::complex<float> Complex;
Complex inverse_mobius_transform(Complex z,
                                 Complex a, Complex b, Complex c, Complex d);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Simple tool for inline timing sections of code. For example:
//
//    void foo()
//    {
//        Timer foo_timer("foo");
//        bar();
//        zap();
//    }
//
// After the block containing the Timer construction it prints:
//
//    foo elapsed time: 1.86984 seconds
//
class Timer
{
public:
    Timer(const std::string& description)
      : description_(description),
        start_time_(std::chrono::high_resolution_clock::now())
    {}
    ~Timer()
    {
        auto finish_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish_time - start_time_;
        std::cout << description_ << " elapsed time: " << elapsed.count();
        std::cout << " seconds" << std::endl;
    }
private:
    const std::string description_;
    const std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Hash a float to a 32 bit value.
size_t hash_float(float x);

// Combine two 32 bit hash values.
size_t hash_mashup(size_t hash0, size_t hash1);

// Takes a 32 bit value and shuffles it around to produce a new 32 bit value.
uint32_t rehash32bits(uint64_t a);

// Simple self-contained generator for a sequence of psuedo-random 32 bit values
class RandomSequence
{
public:
    // Constructor with default seed.
    RandomSequence() : state_(defaultSeed()) {}
    // Constructor with given seed.
    RandomSequence(uint64_t seed) : state_(uint32_t(seed)) {}
    // Next random number in sequence as a 31 bit positive int.
    uint32_t nextInt() { return bitMask() & nextUint32(); }
    // Next random number in sequence as a 32 bit unsigned int.
    uint32_t nextUint32() { return state_ = rehash32bits(state_); }
    // A 32 bit word with zero sign bit and all other 31 bits on, max pos int.
    uint32_t bitMask() { return 0x7fffffff; } // 31 bits
    // The largest (31 bit) positive integer that can be returned.
    int maxIntValue() { return bitMask(); }
    // A "large" 32 bit "random" number.
    static uint32_t defaultSeed() { return 688395321; }
    
    // TODO look at removing the old versions of these utilities.
    // Returns a float randomly distributed between 0 and 1    
    float frandom01() { return float(nextInt()) / float(maxIntValue()); }
    // Returns a float randomly distributed between lowerBound and upperBound
    float frandom2(float a, float b) { return interpolate(frandom01(), a, b); }
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Vec2 randomPointInUnitDiameterCircle();
    Vec2 randomUnitVector();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
    uint32_t state_;
};
