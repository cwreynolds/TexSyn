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
    do {v = {frandom01() - h, frandom01() - h}; } while (v.length() > h);
    return v;
}

Vec2 Vec2::rotate(float angle) const
{
    float cos = std::cos(angle);
    float sin = std::sin(angle);
    return Vec2(x() * cos + y() * sin,
                y() * cos - x() * sin);
}

// Is distance between vectors less than epsilon?
bool withinEpsilon(Vec2 a, Vec2 b, float epsilon)
{
    return (a - b).length() < epsilon;
}

std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
    os << "(" << v.x() << ", " << v.y() << ")";
    return os;
}
