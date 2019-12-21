//
//  Utilities.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/16/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Utilities.h"

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
