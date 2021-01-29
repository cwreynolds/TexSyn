//
//  Vec2.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/19/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Vec2.h"

// Generate a random point inside a unit diameter disk centered on origin.
Vec2 Vec2::randomPointInUnitDiameterCircle()
{
    Vec2 v;
    float h = 0.5;
    do { v = {frandom01() - h, frandom01() - h}; } while (v.length() > h);
    return v;
}

// Generate a random unit vector.
Vec2 Vec2::randomUnitVector()
{
    Vec2 v;
    do { v = randomPointInUnitDiameterCircle(); } while (v.length() == 0);
    return v.normalize();
}

// TODO experimental versions inside RandomSequence instead of in Vec2
//
// TODO these duplicate the function of the same name in global namespace.
//  Maybe those should be replaced by defining a global RandomSequence which
// must be specifically written in source code. This may help avoid the
// "attractive nuisance" of random utilities which are non-repeatable.
//
// Generate a random point inside a unit diameter disk centered on origin.
Vec2 RandomSequence::randomPointInUnitDiameterCircle()
{
    Vec2 v;
    float h = 0.5;
    do { v = {frandom01() - h, frandom01() - h}; } while (v.length() > h);
    return v;
}
//
// Generate a random unit vector.
Vec2 RandomSequence::randomUnitVector()
{
    Vec2 v;
    do { v = randomPointInUnitDiameterCircle(); } while (v.length() == 0);
    return v.normalize();
}

Vec2 RandomSequence::randomPointInAxisAlignedRectangle(Vec2 a, Vec2 b)
{
    return Vec2(random2(std::min(a.x(), b.x()), std::max(a.x(), b.x())),
                random2(std::min(a.y(), b.y()), std::max(a.y(), b.y())));
}

// Is distance between vectors no more than epsilon?
bool withinEpsilon(Vec2 a, Vec2 b, float epsilon)
{
    return (a - b).length() <= epsilon;
}

// Serialize Vec2 object to stream.
std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
    os << "(" << v.x() << ", " << v.y() << ")";
    return os;
}

// Is distance between vectors no more than epsilon?
bool withinEpsilon(Vec3 a, Vec3 b, float epsilon)
{
    return (a - b).length() <= epsilon;
}

// Serialize Vec3 object to stream.
std::ostream& operator<<(std::ostream& os, const Vec3& v)
{
    os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    return os;
}
