//
//  Vec2.h
//  texsyn
//
//  Created by Craig Reynolds on 12/19/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Utilities.h"

class Vec2
{
public:
    // Constructors
    Vec2() {}
    // Modified to count and report, but otherwise tolerate, "abnormal" floats.
    Vec2(float x, float y) : x_(paper_over_abnormal_values(x)),
                             y_(paper_over_abnormal_values(y)) {}
    // Accessors
    float x() const { return x_; }
    float y() const { return y_; }
    // Vector operations dot product, length (norm), normalize.
    float dot(const Vec2& v) const {return x() * v.x() + y() * v.y(); }
    float length() const { return std::sqrt(sq(x()) + sq(y())); }
    float lengthSquared() const { return sq(x()) + sq(y()); }
    Vec2 normalize() const { return *this / length(); }
    // Basic operators + - * / == != < += *=
    Vec2 operator+(Vec2 v) const { return { x() + v.x(), y() + v.y() }; }
    Vec2 operator-(Vec2 v) const { return { x() - v.x(), y() - v.y() }; }
    Vec2 operator-() const { return *this * -1; }
    Vec2 operator*(float s) const { return { x() * s, y() * s}; }
    Vec2 operator/(float s) const { return { x() / s, y() / s}; }
    bool operator==(const Vec2 v) const { return x() == v.x() && y() == v.y(); }
    bool operator!=(const Vec2 v) const { return x() != v.x() || y() != v.y(); }
    bool operator<(const Vec2 v) const {return length() < v.length();}
    Vec2 operator+=(const Vec2& rhs) { return *this = *this + rhs; }
    Vec2 operator-=(const Vec2& rhs) { return *this = *this - rhs; }
    Vec2 operator*=(float s) { return *this = *this * s; }
    Vec2 operator/=(float s) { return *this = *this / s; }
    // Rotation about origin by angle in radians (or by precomputed sin/cos).
    Vec2 rotate(float a) const { return rotate(std::sin(a), std::cos(a)); }
    inline Vec2 rotate(float sin, float cos) const
        { return Vec2(x() * cos + y() * sin, y() * cos - x() * sin); }
    // 90° (π/2) rotation
    Vec2 rotate90degCW() const { return Vec2(y(), -x()); }
    Vec2 rotate90degCCW() const { return Vec2(-y(), x()); }
    // Compute a 32 bit hash value for a Vec2.
    size_t hash() { return hash_mashup(hash_float(x_), hash_float(y_)); }
    // Angle of this vector with +Y axis.
    float atan2() const { return std::atan2(x(), y()); }
private:
    float x_ = 0;
    float y_ = 0;
};

// Is distance between vectors no more than epsilon?
inline bool withinEpsilon(Vec2 a, Vec2 b, float epsilon)
{
    return (a - b).length() <= epsilon;
}
inline bool withinEpsilon(Vec2 a, Vec2 b)
{
    return withinEpsilon(a, b, defaultEpsilon());
}

// Serialize Vec2 object to stream.
inline std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
    os << "(" << v.x() << ", " << v.y() << ")";
    return os;
}

//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// TODO 20230701 spot_utility(), grating_utility() Reconsider design. Refactor?
//               These two functions were the last contents of Utilities.cpp as
//               I was emptying it out on the road to making TexSyn into a
//               header-only library. They are the basic definition of Operators
//               like Spot, LotsOfSpots, Grating, etc. But those operators were
//               written first. This general utility was added later to support
//               PhasorNoise and EvoCamoGame. I need to back fit these into the
//               primary uses (with A/B diffs). And worse, grating_utility() was
//               apparently designed for similar reasons, but NEVER used at all.

// Returns the scalar amplitude of a co-sinusoidal spot, for a given sample
// position, and given spot parameters (center, inner_radius, outer_radius)
// as in Spot::getColor(), etc. (TODO use it there?)
inline float spot_utility(Vec2 position,
                          Vec2 center,
                          float inner_radius,
                          float outer_radius)
{
    // Distance from sample position to spot center.
    float d = (position - center).length();
    // Fraction for interpolation: 0 inside, 1 outside, ramp between.
    float f = remapIntervalClip(d, inner_radius, outer_radius, 0, 1);
    // map interval [0, 1] to cosine curve.
    return 1 - sinusoid(f);
}

// TODO 20230701 if I ever look at this, cf soft_square_wave() in Utilities.h
//
//    // TODO to be used by Gabor noise. Something like the Grating texture operator,
//    // but transform is via an angle and center point, assumes sinosoid (softness=1)
//    float grating_utility(Vec2 position, Vec2 center, float angle, float wavelength)
//    {
//        Vec2 moved = position - center;
//        Vec2 rotated = moved.rotate(angle);
//        return 1 - sinusoid(rotated.x() / (wavelength * 2));
//    }

//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
