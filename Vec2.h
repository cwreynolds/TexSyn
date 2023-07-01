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

// Serialize Vec2 object to stream.
inline std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
    os << "(" << v.x() << ", " << v.y() << ")";
    return os;
}
