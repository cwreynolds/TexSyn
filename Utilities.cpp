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
