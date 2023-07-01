//
//  RandomSequence.h
//  TexSyn
//
//  Simple self-contained generator for sequence of psuedo-random 32 bit values.
//
//  Created by Craig Reynolds on 6/28/23.
//  Copyright © 2023 Craig Reynolds. All rights reserved.

#pragma once
#include "Utilities.h"
#include "Color.h"
#include "Vec2.h"

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
    // int/float overloads of random2(), returns value between INCLUSIVE bounds.
    int random2(int i, int j) { assert(i<=j); return i + randomN(j - i + 1); }
    float random2(float i, float j) { return frandom2(i, j); }
    // Returns true or false with equal likelihood.
    bool randomBool() { return random2(0, 1); }
    // Return random element of given std::vector.
    template<typename T> T randomSelectElement(const std::vector<T>& collection)
        { return collection.at(randomN(collection.size())); }
    // Set seed (RS state) to given value, or defaultSeed() if none given.
    void setSeed() { state_ = defaultSeed(); }
    void setSeed(uint32_t seed) { state_ = seed; }
    // Get state.
    uint32_t getSeed() { return state_; }

    // TODO these duplicate the function of the same name in global namespace.
    // Maybe those should be replaced by defining a global RandomSequence which
    // must be specifically written in source code. This may help avoid the
    // "attractive nuisance" of random utilities which are non-repeatable.
    
    // Generate a random point inside a unit diameter disk centered on origin.
    Vec2 randomPointInUnitDiameterCircle()
    {
        Vec2 v;
        float h = 0.5;
        do { v = {frandom01() - h, frandom01() - h}; } while (v.length() > h);
        return v;
    }
    
    // Generate a random unit vector.
    Vec2 randomUnitVector()
    {
        Vec2 v;
        do { v = randomPointInUnitDiameterCircle(); } while (v.length() == 0);
        return v.normalize();
    }
    
    // Random point (position vector) in an axis aligned rectangle defined by
    // two diagonally opposite vertices.
    Vec2 randomPointInAxisAlignedRectangle(Vec2 a, Vec2 b)
    {
        return Vec2(random2(std::min(a.x(), b.x()), std::max(a.x(), b.x())),
                    random2(std::min(a.y(), b.y()), std::max(a.y(), b.y())));
    }

    // Random color, uniformly distributed across the unit RGB cube.
    inline Color randomUnitRGB()
    {
        return Color(frandom01(), frandom01(), frandom01());
    }
    
private:
    uint32_t state_;
};


// Some related "random utilities". Should they be inside RandomSequence class?

// Return a string of "n" random letters, using the givenRandomSequence
inline std::string n_letters(int n, RandomSequence& rs)
{
    std::string result;
    std::string letters = ("abcdefghijklmnopqrstuvwxyz"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    for (int i = 0; i < n; i++)
    {
        result += letters.at((rs.nextInt() / 61) % letters.size());
    }
    return result;
}

// Utility for randomized subsampling in a square 2d region. Generates 2d
// offsets from the center of the square for an NxN jittered grid. Parameters:
//     n: square root of the number of grid cells (and offsets returned).
//     square_side_length: dimension of the 2d region covered by grid.
//     rs: a RandomSequence for generating random jitter.
//     offsets: vector to be filled with Vec2d offsets
// (FYI See https://www.redblobgames.com/x/1830-jittered-grid/)
// September 28, 2020: if n=1 offset will be in center of cell (square)
inline void jittered_grid_NxN_in_square(int n,
                                        float square_side_length,
                                        RandomSequence& rs,
                                        std::vector<Vec2>& offsets)
{
    offsets.clear();
    float radius = square_side_length / 2;
    float cell_width = square_side_length / n;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            offsets.push_back(n == 1 ?
                              Vec2() :
                              Vec2((i * cell_width) - radius, // cell corner
                                   (j * cell_width) - radius) +
                              Vec2(rs.frandom01() * cell_width,  // jitter
                                   rs.frandom01() * cell_width));
}
