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
#include <chrono>
#include <any>
class Vec2;
class Color;

// for debugging: prints one line with a given C expression, an equals sign,
// and the value of the expression.  For example "angle = 35.6"
#define debugPrint(e) { grabPrintLock(); debugPrintNL(e); }

// Original (circa 2002) "non locking" version, in case it is ever useful.
#define debugPrintNL(e) (std::cout << #e" = " << (e) << std::endl << std::flush)

// Use global mutex to allow synchronizing console output from parallel threads.
// (Written as a macro since the lock_guard is released at the end of a block.)
#define grabPrintLock() \
    std::lock_guard<std::recursive_mutex> pl_(DebugPrint::getPrintMutex());

// Define a global, static std::recursive_mutex to allow synchronizing console
// output from parallel threads.
//
// TODO maybe make constructor do the work now done by "debugPrint(e)" macro?
//
class DebugPrint
{
public:
    static std::recursive_mutex& getPrintMutex()
    {
        static std::recursive_mutex print_mutex_;
        return print_mutex_;
    }
};

// Just for convenience in hand-written prototypes and tests:
const float pi = M_PI;

// True when a and b differ by no more than epsilon.
inline bool withinEpsilon(float a, float b, float epsilon)
{
    return std::abs(a - b) <= epsilon;
}

// Square a float
inline float sq(float f) { return f * f; }

// Returns a float randomly distributed between 0 and 1
inline float frandom01()
{
    return (((float) rand()) / ((float) RAND_MAX));
}

// Returns a float randomly distributed between lowerBound and upperBound
//
inline float frandom2(float lowerBound, float upperBound)
{
    return lowerBound + (frandom01 () * (upperBound - lowerBound));
}

// Generic interpolation
template<typename F,typename T>
T interpolate(const F& alpha, const T& x0, const T& x1)
{
    return (x0 * (1 - alpha)) + (x1 * alpha);
}

// True when x is between given bounds (low ≤ x ≤ high)
inline bool between(float x, float low, float high)
{
    return (low <= x) && (x <= high);
}

// TODO took this directly from TextureSynthesisTest, needs clean up:
// ----------------------------------------------------------------------------

// Constrain a given value "x" to be between two bounds: "bound0" and "bound1"
// (without regard to order). Returns x if it is between the bounds, otherwise
// returns the nearer bound.
inline float clip(float x, float bound0, float bound1)
{
    float clipped = x;
    float min = std::min(bound0, bound1);
    float max = std::max(bound0, bound1);
    if (clipped < min) clipped = min;
    if (clipped > max) clipped = max;
    return clipped;
}

inline float clip01 (const float x)
{
    return clip(x, 0, 1);
}

// Remap a value specified relative to a pair of bounding values
// to the corresponding value relative to another pair of bounds.
// Inspired by (dyna:remap-interval y y0 y1 z0 z1) circa 1984.
inline float remapInterval(float x,
                           float in0, float in1,
                           float out0, float out1)
{
    // Remap if input range is nonzero, otherwise blend them evenly.
    float input_range = in1 - in0;
    float blend = ((input_range > 0) ? ((x - in0) / input_range) : 0.5);
    return interpolate(blend, out0, out1);
}

// Like remapInterval but the result is clipped to remain between out0 and out1
inline float remapIntervalClip(float x,
                               float in0, float in1,
                               float out0, float out1)
{
    return clip(remapInterval(x, in0, in1, out0, out1), out0, out1);
}

// Maps from 0 to 1 into a sinusoid ramp ("slow in, slow out") from 0 to 1.
inline float sinusoid (float x)
{
    return (1 - std::cos(x * M_PI)) / 2;
}

// Defines a "square wave with soft edges". Produces square wave when softness
// is 0, a sinusoid when softness is 1, intermediate values interpolate between
// the two. "fraction" on [0, 1] is the input, a normalized phase parameter.
// This is the symmetrical (duty_cycle=0.5) version, else see next function.
inline float soft_square_wave(float fraction, float softness)
{
    // Clip fraction to [0, 1].
    fraction = clip(fraction, 0, 1);
    // Fold second half of range back over first. f ranges over [0, 0.5].
    float f = (fraction < 0.5) ? fraction : (1 - fraction);
    // Start/end of transition region, adjusted for softness.
    float s = remapInterval(softness, 0, 1, 0.25, 0);
    float e = remapInterval(softness, 0, 1, 0.25, 0.5);
    // Piecewise linear transition (flat, ramp, flat).
    float adjust_for_softness = ((s == e) ?
                                 (f < s ? 0 : 1):
                                 remapIntervalClip(f, s, e, 0, 1));
    // Apply sinusoid to adjusted value.
    return sinusoid(adjust_for_softness);
}

// Version of soft_square_wave with one more parameter, "duty_cycle" on [0, 1]
// to adjust the relative width of the high part to the low part. (Or vice
// versa?) Basis of the Grating texture operator, where the comment used to say:
//     Adjust for duty_cycle. Modifies normalized cross-strip coordinate. For
//     dc=0.5, color1 corresponds to the middle half, interval [0.25, 0.75].
//     For dc=1 interval is [0, 1], for dc=0 interval is [0.5, 0.5].
inline float soft_square_wave(float fraction, float softness, float duty_cycle)
{
    auto offset_phase = [](float p, float o){return std::fmod(p + o, 1.0);};
    // Offset phase by 0.25 aka 90° to put middle of raising edge at 0.
    float alpha = offset_phase(fraction, 0.25);
    // Two linear ramps, from (0,0) to (dc,0.5), then (dc,0.5) to (1,1).
    float result = ((alpha < duty_cycle) ?
                    remapInterval(alpha,  0, duty_cycle,  0, 0.5) :
                    remapInterval(alpha,  duty_cycle, 1,  0.5, 1));
    // Undo phase offset.
    float fraction_adjusted_for_dc = offset_phase(result, 0.75);
    return soft_square_wave(fraction_adjusted_for_dc, softness);
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

// Simple tool for inline timing sections of code. For example:
//    void foo()
//    {
//        Timer foo_timer("foo");
//        bar();
//        zap();
//    }
// After the block containing the Timer construction it prints:
//    foo elapsed time: 1.86984 seconds
// Added parameter to customize " elapsed time: " to any string including none:
//        Timer timer("Run time for unit test suite: ", "");
// prints:
//    Run time for unit test suite: 0.0301787 seconds
//
// TODO: perhaps customize message with a message_printing function pointer?
//
class Timer
{
public:
    Timer(const std::string& description)
      : description_(description),
        elasped_time_(" elapsed time: "),
        start_time_(std::chrono::high_resolution_clock::now()) {}
    Timer(const std::string& description, const std::string& elasped_time)
      : description_(description),
        elasped_time_(elasped_time),
        start_time_(std::chrono::high_resolution_clock::now()) {}
    ~Timer()
    {
        auto finish_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish_time - start_time_;
        std::cout << description_ << elasped_time_ << elapsed.count();
        std::cout << " seconds" << std::endl;
    }
private:
    const std::string description_;
    const std::string elasped_time_;
    const std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
};

// Hash a float to a 32 bit value.
size_t hash_float(float x);

// Combine two 32 bit hash values.
size_t hash_mashup(size_t hash0, size_t hash1);

// Takes a 32 bit value and shuffles it around to produce a new 32 bit value.
// "Robert Jenkins' 32 bit integer hash function" from "Integer Hash Function"
// (1997) by Thomas Wang (https://gist.github.com/badboy/6267743)
// Altered to accept input as uint64_t but ignores the top 32 bits.
inline uint32_t rehash32bits(uint64_t u64)
{
    uint32_t a = uint32_t(u64);
    a = (a+0x7ed55d16) + (a<<12);
    a = (a^0xc761c23c) ^ (a>>19);
    a = (a+0x165667b1) + (a<<5);
    a = (a+0xd3a2646c) ^ (a<<9);
    a = (a+0xfd7046c5) + (a<<3);
    a = (a^0xb55a4f09) ^ (a>>16);
    return a;
}

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
    // Returns an int randomly distributed between 0 and n-1.
    int randomN(int n) { return nextInt() % n; }
    int randomN(size_t n) { return nextInt() % n; }
    
    // TODO these duplicate the function of the same name in global namespace.
    //  Maybe those should be replaced by defining a global RandomSequence which
    // must be specifically written in source code. This may help avoid the
    // "attractive nuisance" of random utilities which are non-repeatable.
    Vec2 randomPointInUnitDiameterCircle();
    Vec2 randomUnitVector();
    // TODO moved from Color class to here on June 30, 2020:
    Color randomUnitRGB();
    // Set seed (RS state) to given value, or defaultSeed() if none given.
    void setSeed() { state_ = defaultSeed(); }
    void setSeed(uint32_t seed) { state_ = seed; }
    // Get state.
    uint32_t getSeed() { return state_; }
private:
    uint32_t state_;
};


////////////////////////////////////////////////////////////////////////////////
//  IncrementalHalton.cpp
//  Calculates the halton sequence incrementally
//  Andrew Willmott, public domain
//
//  [downloaded from http://www.andrewwillmott.com/s2007 ]
////////////////////////////////////////////////////////////////////////////////
/*
    Notes:
    
    This is a reference implementation for clarity -- optimizations are possible!
    
    In particular the base 2 part can be done more quickly using bit tricks.
    (E.g., branchless bit reverse followed by or'ing the result into a float
    mantissa.)
*/
struct cHaltonSequence3
/// This calculates the Halton sequence incrementally
/// for a base 2/3/5 triplet.
{
    float mX;
    float mY;
    float mZ;
    
    uint32_t mBase2;
    uint32_t mBase3;
    uint32_t mBase5;
    
    cHaltonSequence3() :
        mBase2(0),
        mBase3(0),
        mBase5(0),
        mX(0.0f),
        mY(0.0f),
        mZ(0.0f)
    {}
    
    int inc();
    ///< Advance to next point in the sequence. Returns the index of this point.
    void reset();
    ///< Move back to first point in the sequence (i.e. the origin.)
    const float kOneOverThree = float(1.0 / 3.0);
    const float kOneOverFive  = float(1.0 / 5.0);
};

// for comparison purposes
float HaltonSequence(int n, int b);

// Return 2.2, TexSyn's default output gamma, intended to approximate
// the nonlinearity of sRGB (digital display screen) color space.
float defaultGamma();
void setDefaultGamma(float gamma);

// Utility for randomized subsampling in a square 2d region. Generates 2d
// offsets from the center of the square for an NxN jittered grid. Parameters:
//     n: square root of the number of grid cells (and offsets returned).
//     square_side_length: dimension of the 2d region covered by grid.
//     rs: a RandomSequence for generating random jitter.
//     offsets: vector to be filled with Vec2d offsets
// (FYI See https://www.redblobgames.com/x/1830-jittered-grid/)
void jittered_grid_NxN_in_square(int n,
                                 float square_side_length,
                                 RandomSequence& rs,
                                 std::vector<Vec2>& offsets);

// Map/round a float value (including negative) to the nearest odd integer.
inline int nearestOddInt(float x) { return (floor(x / 2) * 2) + 1; }

// Given an std::any value, cast it to the given type, and convert to string.
template<typename T> std::string any_to_string(std::any a)
{
    std::stringstream ss;
    ss << std::any_cast<T>(a);
    return ss.str();
}
