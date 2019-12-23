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
    Vec2 normalize() const { return *this / length(); }
    // Basic operators + - * / == <
    Vec2 operator+(Vec2 v) const { return { x() + v.x(), y() + v.y() }; }
    Vec2 operator-(Vec2 v) const { return { x() - v.x(), y() - v.y() }; }
    Vec2 operator-() const { return *this * -1; }
    Vec2 operator*(float s) const { return { x() * s, y() * s}; }
    Vec2 operator/(float s) const { return { x() / s, y() / s}; }
    bool operator==(const Vec2 v) const { return x() == v.x() && y() == v.y(); }
    bool operator<(const Vec2 v) const {return length() < v.length();}
    // Return vec rotated about origin by given angle in radians.
    Vec2 rotate(float angle) const;
    // Generate a random point inside a unit diameter disk centered on origin.
    static Vec2 randomPointInUnitDiameterCircle();
private:
    float x_ = 0;
    float y_ = 0;
};

// Is distance between vectors less than epsilon?
bool withinEpsilon(Vec2 a, Vec2 b, float epsilon);

std::ostream& operator<<(std::ostream& os, const Vec2& v);
