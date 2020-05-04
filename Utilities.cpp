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

    // Classic Perlin noise, in 2d, output range approximately on [-1, 1].
    // Based on "Improved Noise" (Ken Perlin, 2002)
    float noise2d(Vec2 position)
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
        float raw = lerp(v, lerp(u, grad(p[AA], x  , y  , 0),
                                    grad(p[BA], x-1, y  , 0)),
                            lerp(u, grad(p[AB], x  , y-1, 0),
                                    grad(p[BB], x-1, y-1, 0)));
        // Experimentally "raw" ranges on [-0.88, 0.88], very occasionally up to
        // [-0.88, 1]. For predictable output, clip smaller range to [-1, 1].
        return remapIntervalClip(raw, -0.88, 0.88, -1, 1);
    }

    // Classic Perlin noise, in 2d, output range on [0, 1].
    float unitNoise2d(Vec2 position)
    {
        // Remap raw noise from approximately [-1, 1] to [0, 1].
        return remapIntervalClip(noise2d(position), -1, 1, 0, 1);
    }

    // For 1/f subdivision recursion from "image scale" to "pixel scale"
    // TODO this recursion criteria should be pixel-aware like Perlin's
    int recursion_levels = 10;
    // For disalignment rotation of two radians at each recursion level.
    float dis_sin = std::sin(2);
    float dis_cos = std::cos(2);
    Vec2 disalignment_rotation(Vec2 v) { return v.rotate(dis_sin, dis_cos); }

    // Classic Perlin turbulence. 2d noise with output range on [0, 1].
    float turbulence2d(Vec2 position)
    {
        float value = 0.0f;
        float octave = 1.0f;
        for (int i = 0; i < recursion_levels; i++)
        {
            value += fabs(noise2d(position * octave) / octave);
            octave *= 2;
            position = disalignment_rotation(position);
        }
        return remapIntervalClip(value, 0, 1.5, 0, 1);
    }

    // Brownian Noise, fractal 1/f Perlin noise, output range on [0, 1].
    float brownian2d(Vec2 position)
    {
        float value = 0.0f;
        float octave = 1.0f;
        for (int i = 0; i < recursion_levels; i++)
        {
            value += noise2d(position * octave) / octave;
            octave *= 2;
            position = disalignment_rotation(position);
        }
        return remapIntervalClip(value, -1.4, 1.4, 0, 1);
    }

    // Furbulence: two "fold" version of Turbulence producing sharp features at
    // both low and high ends of the output range.
    float furbulence2d(Vec2 position)
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
        // TODO revisit Furbulence clip bounds after running Release build.
        return remapIntervalClip(value, 0.07, 1.9, 0, 1);
    }

    // Wrapulence: another variation on turbulence(). noise() is scaled up in
    // value, then wrapped modulo [0, 1]. It has hard edge discontinuities at
    // all scales.
    float wrapulence2d(Vec2 position)
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
        // TODO revisit Wrapulence clip bounds after running Release build.
        return remapIntervalClip(value, 0, 1.9, 0, 1);
    }

    // Returns result of one of the noise functions (unitNoise2d, turbulence2d,
    // brownian2d, furbulence2d, wrapulence2d -- selected according to "which")
    // applied to "position".
    float multiNoise2d(Vec2 position, float which)
    {
        switch(int(fmod_floor(which, 1) * 5))
        {
            case  0: return PerlinNoise::unitNoise2d(position);
            case  1: return PerlinNoise::brownian2d(position);
            case  2: return PerlinNoise::turbulence2d(position);
            case  3: return PerlinNoise::furbulence2d(position);
            default: return PerlinNoise::wrapulence2d(position);
        }
    }

    // Tool to measure typical range of a noise function. Returns min and max
    // range from calling given noise function 100000 times for random points
    // in a circle at origin with diameter of 100.
    std::pair<float, float> measure_range(std::function<float(Vec2)> noise_func)
    {
        float max_range = -std::numeric_limits<float>::infinity();
        float min_range = +std::numeric_limits<float>::infinity();
        float steps = 1000;
        float magnify = 50;
        for (int i = -steps / 2; i < steps / 2; i++)
        {
            for (int j = -steps / 2; j < steps / 2; j++)
            {
                Vec2 v((i * magnify) / (steps / 2),
                       (j * magnify) / (steps / 2));
                float noise = noise_func(v);
                if (max_range < noise) max_range = noise;
                if (min_range > noise) min_range = noise;
            }
        }
        debugPrint(min_range);
        debugPrint(max_range);
        std::cout << std::endl;
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

}

// Compute the inverse Möbius transformation of the complex plane. It is
// parameterized by four "points" (aka complex numbers). The Wikipedia
// article (https://en.wikipedia.org/wiki/Möbius_transformation) says the four
// points should satisfy: ad − bc ≠ 0.
// See Tim Hutton cool app: http://timhutton.github.io/mobius-transforms/
Complex inverse_mobius_transform(Complex z,
                                 Complex a, Complex b, Complex c, Complex d)
{
    Complex numerator = (d * z) - b;
    Complex denominator = a - (c * z);
    if (denominator == Complex(0, 0)) debugPrint(denominator);
    return numerator / denominator;
}

// Hash a float to a 32 bit value.
size_t hash_float(float x)
{
    return rehash32bits(std::hash<float>()(x));
}

// Combine two 32 bit hash values.
size_t hash_mashup(size_t hash0, size_t hash1)
{
    return rehash32bits((hash0 ^ (hash1 >> 16 ^ hash1 << 16)) + 918273645);
}

// Takes a 32 bit value and shuffles it around to produce a new 32 bit value.
//
// "Robert Jenkins' 32 bit integer hash function" from "Integer Hash Function"
// (1997) by Thomas Wang (https://gist.github.com/badboy/6267743)
// Altered to accept input as uint64_t but ignores the top 32 bits.
uint32_t rehash32bits(uint64_t u64)
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

///< Advance to next point in the sequence. Returns the index of this point.
int cHaltonSequence3::inc()
{
    /////////////////////////////////////
    // base 2
    
    uint32_t oldBase2 = mBase2;
    mBase2++;
    uint32_t diff = mBase2 ^ oldBase2;

    // bottom bit always changes, higher bits
    // change less frequently.
    float s = 0.5f;

    // diff will be of the form 0*1+, i.e. one bits up until the last carry.
    // expected iterations = 1 + 0.5 + 0.25 + ... = 2
    do
    {
        if (oldBase2 & 1)
            mX -= s;
        else
            mX += s;
        
        s *= 0.5f;
        
        diff = diff >> 1;
        oldBase2 = oldBase2 >> 1;
    }
    while (diff);

    
    /////////////////////////////////////
    // base 3: use 2 bits for each base 3 digit.
    
    uint32_t mask = 0x3;  // also the max base 3 digit
    uint32_t add  = 0x1;  // amount to add to force carry once digit==3
    s = kOneOverThree;

    mBase3++;

    // expected iterations: 1.5
    while (1)
    {
        if ((mBase3 & mask) == mask)
        {
            mBase3 += add;          // force carry into next 2-bit digit
            mY -= 2 * s;
            
            mask = mask << 2;
            add  = add  << 2;
            
            s *= kOneOverThree;
        }
        else
        {
            mY += s;     // we know digit n has gone from a to a + 1
            break;
        }
    };

    /////////////////////////////////////
    // base 5: use 3 bits for each base 5 digit.
    mask = 0x7;
    add  = 0x3;  // amount to add to force carry once digit==dmax
    uint32_t dmax = 0x5;  // max digit
    
    s = kOneOverFive;

    mBase5++;
    
    // expected iterations: 1.25
    while (1)
    {
        if ((mBase5 & mask) == dmax)
        {
            mBase5 += add;          // force carry into next 3-bit digit
            mZ -= 4 * s;
            
            mask = mask << 3;
            dmax = dmax << 3;
            add  = add  << 3;
            
            s *= kOneOverFive;
        }
        else
        {
            mZ += s;     // we know digit n has gone from a to a + 1
            break;
        }
    };

    return mBase2; // return the index of this sequence point
}

///< Move back to first point in the sequence (i.e. the origin.)
void cHaltonSequence3::reset()
{
    mBase2 = 0;
    mBase3 = 0;
    mBase5 = 0;
    mX = 0.0f;
    mY = 0.0f;
    mZ = 0.0f;
}

// for comparison purposes
float HaltonSequence(int n, int b)
/// return term i of the base b Halton sequence
/// You can think of this as, for example, just a generalization of Heckbert's bit-
/// reversal distribution trick.
/// E.g., when b=3, write n as a base 3 number, digit 0 -> which third of interval the
/// sample is in, 1 -> which third of that, 2 -> which third of that, etc.
{
    float result = 0;
    float ip = 1.0f / b;
    float p = ip;

    while (n > 0)
    {
        result += (n % b) * p;
        n = n / b;
        p *= ip;
    }

    return result;
}
