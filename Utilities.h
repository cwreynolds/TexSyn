//
//  Utilities.h
//  texsyn
//
//  Created by Craig Reynolds on 12/16/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once

#include <iostream>


// for debugging: prints one line with a given C expression, an equals sign,
// and the value of the expression.  For example "angle = 35.6"
#define debugPrint(e) (std::cout << #e" = " << (e) << std::endl << std::flush)

