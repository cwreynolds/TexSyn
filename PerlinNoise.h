//
//  PerlinNoise.h
//  TexSyn
//
//  Created by Craig Reynolds on 6/28/23.
//  Copyright © 2023 Craig Reynolds. All rights reserved.
//
//  Utilities for Perlin noise textures.
//  Ken Perlin's 2002 "Improved Noise": http://mrl.nyu.edu/~perlin/noise/
//
// This code derived from on a transliteration by Malcolm Kesson from Java to c:
// http://www.fundza.com/c4serious/noise/perlin/perlin.html
//
//  API:
//  // Classic Perlin noise, in 2d, output range approximately on [-1, 1].
//  float noise2d(Vec2 position);
//  // Classic Perlin noise, in 2d, output range on [0, 1].
//  float unitNoise2d(Vec2 position);
//  // Classic Perlin turbulence, in 2d, output range on [0, 1].
//  float turbulence2d(Vec2 position);
//  // Brownian Noise, fractal 1/f Perlin noise, output range on [0, 1].
//  float brownian2d(Vec2 position);
//  // Furbulence: two "fold" version of Turbulence producing sharp features
//  // at both low and high ends of the output range.
//  float furbulence2d(Vec2 position);
//  // Wrapulence: another variation on turbulence(). noise() is scaled up in
//  // value, then wrapped modulo [0, 1]. It has hard edge discontinuities at
//  // all scales.
//  float wrapulence2d(Vec2 position);
//  // Returns result of one of the noise functions (unitNoise2d, turbulence2d,
//  // brownian2d, furbulence2d, wrapulence2d -- selected according to "which")
//  // applied to "position".
//  float multiNoise2d(Vec2 position, float which);
//  // Tool to measure typical range of a noise function. Returns min and max
//  // range from calling given noise function 100000 times for random points
//  // in a circle at origin with diameter of 100.
//  std::pair<float, float> measure_range(std::function<float(Vec2)> noise_func);
//
//  Note: prior to June 2023 this was a namespace defined in Utilities.cpp.
//  Refactored to this form so it can be a pure header-only library. Now it is
//  defined as a class, which is never instantiated (not even a singleton -- a
//  "zeroton"? "nonton"?) to use static class members as global variables for
//  constants like "p" and "recursion_levels". All member functions are static.

#pragma once
#include "Utilities.h"
#include "Vec2.h"
#include "RandomSequence.h"

class PerlinNoise
{
public:
    
    // Static data structure, initialized once (need mutex for multi-threading?)
    inline static std::vector<int> p;
    static void init()
    {
        static bool initialized = false;
        if (!initialized)
        {
            std::vector<int> permutation =
            {
                151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,
                103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,
                26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,
                87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,77,
                146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,
                245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,
                208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,
                173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,
                85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,213,
                119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,129,22,
                39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
                228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,
                249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,
                121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,
                141,128,195,78,66,215,61,156,180
            };
            p = permutation;
            p.insert(p.end(), permutation.begin(), permutation.end());
            // std::cout << "in PerlinNoise2::init()" << std::endl;
        }
        initialized = true;
    }

    // Helper functions.
    static float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    static float lerp(float t, float a, float b) { return a + t * (b - a); }
    static float grad(int hash, float x, float y, float z)
    {
        int h = hash & 15;        // CONVERT LO 4 BITS OF HASH CODE
        float u = h < 8 ? x : y;  // INTO 12 GRADIENT DIRECTIONS.
        float v = h < 4 ? y : h==12||h==14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    // Classic Perlin noise, in 2d, output range approximately on [-1, 1].
    // Based on "Improved Noise" (Ken Perlin, 2002)
    static float noise2d(Vec2 position)
    {
        init();
        float x = position.x();
        float y = position.y();
        // Find unit square that contains position.
        int X = int(floor(x)) & 255;
        int Y = int(floor(y)) & 255;
        // Relative xy of position inside square
        x -= floor(x);
        y -= floor(y);
        // Compute fade curves for each of x and y.
        float  u = fade(x);
        float  v = fade(y);
        // Hash coordinates of the 4 corners of the square.
        int A = p[X  ]+Y, AA = p[A], AB = p[A+1];
        int B = p[X+1]+Y, BA = p[B], BB = p[B+1];
        // And add blended results from 4 corners of the square.
        float raw = lerp(v,
                         lerp(u,
                              grad(p[AA], x  , y  , 0),
                              grad(p[BA], x-1, y  , 0)),
                         lerp(u,
                              grad(p[AB], x  , y-1, 0),
                              grad(p[BB], x-1, y-1, 0)));
        // Experimentally (see https://cwreynolds.github.io/TexSyn/#20200523)
        // "raw" range is about [-0.7, 0.7]. Remap and clip that to [-1, 1].
        return remapIntervalClip(raw, -0.70, 0.70, -1, 1);
    }

    // Classic Perlin noise, in 2d, output range on [0, 1].
    static float unitNoise2d(Vec2 position)
    {
        // Remap trditional noise from [-1, 1] like a sine wave, to [0, 1].
        return remapIntervalClip(noise2d(position), -1, 1, 0, 1);
    }

    // For 1/f subdivision recursion from "image scale" to "pixel scale"
    // TODO this recursion criteria should be pixel-aware like Perlin's.
    inline static int recursion_levels = 10;

    // For disalignment rotation of two radians at each recursion level.
    static Vec2 disalignment_rotation(Vec2 v)
    {
        static float dis_sin = std::sin(2);
        static float dis_cos = std::cos(2);
        return v.rotate(dis_sin, dis_cos);
    }

    // Classic Perlin turbulence. 2d noise with output range on [0, 1].
    static float turbulence2d(Vec2 position)
    {
        float value = 0.0f;
        float octave = 1.0f;
        for (int i = 0; i < recursion_levels; i++)
        {
            value += fabs(noise2d(position * octave) / octave);
            octave *= 2;
            position = disalignment_rotation(position);
        }
        return remapIntervalClip(value, 0.1, 1.5, 0, 1);
    }

    // Brownian Noise, fractal 1/f Perlin noise, output range on [0, 1].
    static float brownian2d(Vec2 position)
    {
        float value = 0.0f;
        float octave = 1.0f;
        for (int i = 0; i < recursion_levels; i++)
        {
            value += noise2d(position * octave) / octave;
            octave *= 2;
            position = disalignment_rotation(position);
        }
        return remapIntervalClip(value, -1.3, 1.3, 0, 1);
    }

    // Furbulence: two "fold" version of Turbulence producing sharp features at
    // both low and high ends of the output range.
    static float furbulence2d(Vec2 position)
    {
        float value = 0.0f;
        float octave = 1.0f;
        for (int i = 0; i < recursion_levels; i++)
        {
            float pn = noise2d(position * octave);
            value += fabs (0.66f - fabs(pn + 0.33f)) * 1.5f / octave;
            octave *= 2;
            position = disalignment_rotation(position);
        }
        return remapIntervalClip(value, 0.16, 1.8, 0, 1);
    }

    // Wrapulence: another variation on turbulence(). noise() is scaled up in
    // value, then wrapped modulo [0, 1]. It has hard edge discontinuities at
    // all scales.
    static float wrapulence2d(Vec2 position)
    {
        float value = 0.0f;
        float octave = 1.0f;
        for (int i = 0; i < recursion_levels; i++)
        {
            float pn = noise2d(position * octave);
            float sn = pn * 3;
            value += (sn - floor (sn)) / octave;
            octave *= 2;
            position = disalignment_rotation(position);
        }
        return remapIntervalClip(value, 0.11, 1.8, 0, 1);
    }

    // Returns result of one of the noise functions (unitNoise2d, turbulence2d,
    // brownian2d, furbulence2d, wrapulence2d -- selected according to "which")
    // applied to "position".
    static float multiNoise2d(Vec2 position, float which)
    {
        switch(int(fmod_floor(which, 1) * 5))
        {
            case  0: return unitNoise2d(position);  // which 0
            case  1: return brownian2d(position);   // which 0.2
            case  2: return turbulence2d(position); // which 0.4
            case  3: return furbulence2d(position); // which 0.6
            default: return wrapulence2d(position); // which 0.8
        }
    }

    // Tool to measure typical range of a noise function. Returns min and max
    // range from calling given noise function 100000 times for random points
    // in a circle at origin with diameter of 100.
    //std::pair<float, float> measure_range(std::function<float(Vec2)> noise_func)
    //inline std::pair<float, float> measure_range(std::function<float(Vec2)> noise_func)
    static std::pair<float, float> measure_range(std::function<float(Vec2)> noise_func)
    {
        float max_range = -std::numeric_limits<float>::infinity();
        float min_range = +std::numeric_limits<float>::infinity();
        float measurements = 10000;
        RandomSequence rs(30304668);
        for (int i = 0; i < measurements; i ++)
        {
            float diameter = 200;
            Vec2 v = rs.randomPointInUnitDiameterCircle() * diameter;
            float noise = noise_func(v);
            if (max_range < noise) max_range = noise;
            if (min_range > noise) min_range = noise;
        }
        return std::pair<float, float>(min_range, max_range);
    }

    // Copying 3d code from 2010 TextureSynthesisTest in case I need it later.
    //float improvedPerlinNoise (float x, float y, float z)
    //{
    //    int   X = (int)floor(x) & 255,                     // FIND UNIT CUBE THAT
    //          Y = (int)floor(y) & 255,                     // CONTAINS POINT.
    //          Z = (int)floor(z) & 255;
    //    x -= floor(x);                                     // FIND RELATIVE X,Y,Z
    //    y -= floor(y);                                     // OF POINT IN CUBE.
    //    z -= floor(z);
    //    float  u = fade(x),                                // COMPUTE FADE CURVES
    //           v = fade(y),                                // FOR EACH OF X,Y,Z.
    //           w = fade(z);
    //    int  A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,     // HASH COORDINATES OF
    //         B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;     // THE 8 CUBE CORNERS,
    //
    //    return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),  // AND ADD
    //                                   grad(p[BA  ], x-1, y  , z   )), // BLENDED
    //                           lerp(u, grad(p[AB  ], x  , y-1, z   ),  // RESULTS
    //                                   grad(p[BB  ], x-1, y-1, z   ))),// FROM  8
    //                   lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  // CORNERS
    //                                   grad(p[BA+1], x-1, y  , z-1 )), // OF CUBE
    //                           lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
    //                                   grad(p[BB+1], x-1, y-1, z-1 ))));
    //}
    
};
