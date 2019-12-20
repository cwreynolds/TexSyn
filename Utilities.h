//
//  Utilities.h
//  texsyn
//
//  Created by Craig Reynolds on 12/16/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once

#include <iostream>
#include <cassert>
#include <cmath>

// for debugging: prints one line with a given C expression, an equals sign,
// and the value of the expression.  For example "angle = 35.6"
#define debugPrint(e) (std::cout << #e" = " << (e) << std::endl << std::flush)

// True when a and b differ by no more than epsilon.
bool withinEpsilon(float a, float b, float epsilon);

// Square a float
inline float sq(float f) { return f * f; }

// Returns a float randomly distributed between 0 and 1
float frandom01(void);

// Returns a float randomly distributed between lowerBound and upperBound
float frandom2(float lowerBound, float upperBound);
