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
    Vec2(float x, float y) : x_(x), y_(y) {}
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
    Vec2 operator*=(float s) { return *this = *this * s; }
    // Rotation about origin by angle in radians (or by precomputed sin/cos).
    Vec2 rotate(float a) const { return rotate(std::sin(a), std::cos(a)); }
    inline Vec2 rotate(float sin, float cos) const
        { return Vec2(x() * cos + y() * sin, y() * cos - x() * sin); }
    // 90° (π/2) rotation
    Vec2 rotate90degCW() const { return Vec2(y(), -x()); }
    Vec2 rotate90degCCW() const { return Vec2(-y(), x()); }
    // "Localize" this Vec2 into a local space defined by x and y basis vectors.
    Vec2 localize(Vec2 bx, Vec2 by) const { return Vec2(dot(bx), dot(by)); }
    // "Globalize" this Vec2 from local space defined by x and y basis vectors.
    // TODO This has not been tested. I cribbed localize() from OpenSteer, and
    // TODO decided to write this while I had the file open. "Might" be right.
    Vec2 globalize(Vec2 bx, Vec2 by) const { return (bx * x()) + (by * y()); }
    // Compute a 32 bit hash value for a Vec2.
    size_t hash() { return hash_mashup(hash_float(x_), hash_float(y_)); }
    // Angle of this vector with +Y axis.
    float atan2() const { return std::atan2(x(), y()); }
    // Generate a random point inside a unit diameter disk centered on origin.
    static Vec2 randomPointInUnitDiameterCircle();
    // Generate a random unit vector.
    static Vec2 randomUnitVector();
private:
    float x_ = 0;
    float y_ = 0;
};

// Is distance between vectors no more than epsilon?
bool withinEpsilon(Vec2 a, Vec2 b, float epsilon);

// Serialize Vec2 object to stream.
std::ostream& operator<<(std::ostream& os, const Vec2& v);


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
//    // Rotation about origin by angle in radians (or by precomputed sin/cos).
//    Vec2 rotate(float a) const { return rotate(std::sin(a), std::cos(a)); }
//    inline Vec2 rotate(float sin, float cos) const
//        { return Vec2(x() * cos + y() * sin, y() * cos - x() * sin); }
//    // 90° (π/2) rotation
//    Vec2 rotate90degCW() const { return Vec2(y(), -x()); }
//    Vec2 rotate90degCCW() const { return Vec2(-y(), x()); }
//    // Compute a 32 bit hash value for a Vec2.
//    size_t hash() { return hash_mashup(hash_float(x_), hash_float(y_)); }
//    // Generate a random point inside a unit diameter disk centered on origin.
//    static Vec2 randomPointInUnitDiameterCircle();
//    // Generate a random unit vector.
//    static Vec2 randomUnitVector();
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
bool withinEpsilon(Vec3 a, Vec3 b, float epsilon);

// Serialize Vec2 object to stream.
std::ostream& operator<<(std::ostream& os, const Vec3& v);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO prototype, this seems wrong place
//      yes, wrote a note in DynaList, maybe put in own file.
//
// TODO oh but do we want a normalized basis or the original differene vector?

class TwoPointTransform
{
public:
    TwoPointTransform(){}
    TwoPointTransform(Vec2 point_0, Vec2 point_1)
      : scale_((point_1 - point_0).length()),
        origin_(point_0),
        x_basis_(point_1 - point_0),
        y_basis_(xBasis().rotate90degCCW()),
        x_basis_unit_(xBasis().normalize()),
        y_basis_unit_(yBasis().normalize()) {}
    float scale() const { return scale_; }
    Vec2 origin() const { return origin_; }
    Vec2 xBasis() const { return x_basis_; }
    Vec2 yBasis() const { return y_basis_; }
    Vec2 xBasisUnit() const { return x_basis_unit_; }
    Vec2 yBasisUnit() const { return y_basis_unit_; }
    
    
    // TODO
    // note: Vec2::localize() used only in classes Affine and Noise
    
    // "Localize" a Vec2 into this transform's "local space".
    Vec2 localize(Vec2 v) const
    {
        return (v - origin()).localize(xBasis() / sq(scale()),
                                       yBasis() / sq(scale()));
    }
    // "Globalize" a Vec2 from this transform's "local space" to "global space".
    Vec2 globalize(Vec2 v) const
    {
        return origin() + v.globalize(xBasis(), yBasis());
    }
    
    // TODO I want to move this code into TwoPointTransform class:

//    // "Localize" this Vec2 into a local space defined by x and y basis vectors.
//    Vec2 localize(Vec2 bx, Vec2 by) const { return Vec2(dot(bx), dot(by)); }
//    // "Globalize" this Vec2 from local space defined by x and y basis vectors.
//    // TODO This has not been tested. I cribbed localize() from OpenSteer, and
//    // TODO decided to write this while I had the file open. "Might" be right.
//    Vec2 globalize(Vec2 bx, Vec2 by) const { return (bx * x()) + (by * y()); }
private:
    const float scale_ = 1;
    const Vec2 origin_ = Vec2(0, 0);
    const Vec2 x_basis_ = Vec2(1, 0);
    const Vec2 y_basis_ = Vec2(0, 1);
    const Vec2 x_basis_unit_ = Vec2(1, 0);
    const Vec2 y_basis_unit_ = Vec2(0, 1);
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
