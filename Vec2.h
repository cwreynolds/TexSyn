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

// Generate a random point inside a unit diameter disk centered on origin.
inline Vec2 RandomSequence::randomPointInUnitDiameterCircle()
{
    Vec2 v;
    float h = 0.5;
    do { v = {frandom01() - h, frandom01() - h}; } while (v.length() > h);
    return v;
}

// Generate a random unit vector.
inline Vec2 RandomSequence::randomUnitVector()
{
    Vec2 v;
    do { v = randomPointInUnitDiameterCircle(); } while (v.length() == 0);
    return v.normalize();
}

inline Vec2 RandomSequence::randomPointInAxisAlignedRectangle(Vec2 a, Vec2 b)
{
    return Vec2(random2(std::min(a.x(), b.x()), std::max(a.x(), b.x())),
                random2(std::min(a.y(), b.y()), std::max(a.y(), b.y())));
}

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

// TODO should this be in this header file?
// TODO should it even exist?
class Vec3
{
public:
    // Constructors
    Vec3() {}
    Vec3(float x, float y, float z) : x_(x), y_(y), z_(z) {}
    // Accessors
    float x() const { return x_; }
    float y() const { return y_; }
    float z() const { return z_; }
    // Vector operations dot product, length (norm), normalize.
    float dot(const Vec3& v) const { return (x() * v.x() +
                                             y() * v.y() +
                                             z() * v.z()); }
    float length() const { return std::sqrt(sq(x()) + sq(y()) + sq(z())); }
    Vec3 normalize() const { return *this / length(); }
    
    // Basic operators + - * / == <
    Vec3 operator+(Vec3 v) const
        { return { x() + v.x(), y() + v.y(), z() + v.z() }; }
    Vec3 operator-(Vec3 v) const
        { return { x() - v.x(), y() - v.y(), z() - v.z() }; }
    Vec3 operator-() const { return *this * -1; }
    Vec3 operator*(float s) const { return { x() * s, y() * s, z() * s }; }
    Vec3 operator/(float s) const { return { x() / s, y() / s, z() / s }; }
    bool operator==(const Vec3 v) const
        { return x() == v.x() && y() == v.y() && z() == v.z(); }
    bool operator<(const Vec3 v) const {return length() < v.length();}
    // Cross product of this Vec3 and another given Vec3.
    Vec3 cross(Vec3 o) const { return {y() * o.z() - z() * o.y(),
                                       z() * o.x() - x() * o.z(),
                                       x() * o.y() - y() * o.x() }; }
private:
    float x_ = 0;
    float y_ = 0;
    float z_ = 0;
};

// Is distance between vectors no more than epsilon?
inline bool withinEpsilon(Vec3 a, Vec3 b, float epsilon)
{
    return (a - b).length() <= epsilon;
}

// Serialize Vec3 object to stream.
inline std::ostream& operator<<(std::ostream& os, const Vec3& v)
{
    os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    return os;
}
