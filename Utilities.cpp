//
//  Utilities.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/16/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Utilities.h"
#include "Vec2.h"

// True when a and b differ by no more than epsilon.
bool withinEpsilon(float a, float b, float epsilon)
{
    return std::abs(a - b) < epsilon;
}

// Returns a float randomly distributed between 0 and 1
float frandom01()
{
    return (((float) rand()) / ((float) RAND_MAX));
}

// Returns a float randomly distributed between lowerBound and upperBound
//
float frandom2 (float lowerBound, float upperBound)
{
    return lowerBound + (frandom01 () * (upperBound - lowerBound));
}

// Perlin Noise
// Ken Perlin's 2002 "Improved Noise": http://mrl.nyu.edu/~perlin/noise/
// This code based on a transliteration by Malcolm Kesson from Java to c:
// http://www.fundza.com/c4serious/noise/perlin/perlin.html
namespace PerlinNoise
{
    // Static data structure, initialized once (needs mutex for multi-threading)
    bool initialized = false;  // would need mutex for multi-threading
    std::vector<int> p(512, 0);
    std::vector<int> permutation = { 151,160,137,91,90,15,131,13,201,95,96,53,
        194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,
        120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,
        56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,77,146,
        158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
        102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,200,
        196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,
        124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,
        182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,
        155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,
        104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,
        145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,
        121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,
        195,78,66,215,61,156,180 };
    void init()
    {
        if (!initialized)
            for (int i = 0; i < 256 ; i++) p[256+i] = p[i] = permutation[i];
        initialized = true;
    }
    // Helper functions.
    float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    float lerp(float t, float a, float b) { return a + t * (b - a); }
    float grad(int hash, float x, float y, float z)
    {
        int h = hash & 15;        // CONVERT LO 4 BITS OF HASH CODE
        float u = h < 8 ? x : y;  // INTO 12 GRADIENT DIRECTIONS.
        float v = h < 4 ? y : h==12||h==14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
    // Raw 2d "Improved Noise" (Ken Perlin, 2002)
    float raw2d(Vec2 position)
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
        int  A = p[X  ]+Y, AA = p[A], AB = p[A+1];
        int  B = p[X+1]+Y, BA = p[B], BB = p[B+1];
        // And add blended results from 4 corners of the square.
        return lerp(v, lerp(u, grad(p[AA], x  , y  , 0),
                               grad(p[BA], x-1, y  , 0)),
                       lerp(u, grad(p[AB], x  , y-1, 0),
                               grad(p[BB], x-1, y-1, 0)));
    }
    // 2d noise with output range on [0, 1].
    float unit2d(Vec2 position)
    {
        // Remap raw noise from approximately [-0.75, 0.75] to [0, 1].
        return remapInterval(raw2d(position), -0.75, 0.75, 0, 1);
    }
}
