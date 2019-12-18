//
//  UnitTests.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/16/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Color.h"
#include "UnitTests.h"
#include "Utilities.h"

bool UnitTests::allTestsOK()
{
    // Tests for Color class.
    bool color_constructors = ((Color().r() == 0) &&
                               (Color().g() == 0) &&
                               (Color().b() == 0) &&
                               (Color(1, 2, 3).r() == 1) &&
                               (Color(1, 2, 3).g() == 2) &&
                               (Color(1, 2, 3).b() == 3));
    bool color_equality = ((Color() == Color()) &&
                           (Color(1, 2, 3) == Color(1, 2, 3)));
    bool color_assignment = [](){
                                    Color ca1 = Color();
                                    Color ca2 = Color(1, 2, 3);
                                    return ((ca1 == Color()) &&
                                            (ca2 == Color(1, 2, 3)) &&
                                            (ca2 == (ca2 = Color(1, 2, 3))));
                                }();
    bool color_luminance = []()
    {
        float e = 0.000001;
        return (withinEpsilon(Color(       ).luminance(), 0,      e) &&
                withinEpsilon(Color(1, 1, 1).luminance(), 1,      e) &&
                withinEpsilon(Color(1, 0, 0).luminance(), 0.2126, e) &&
                withinEpsilon(Color(0, 1, 0).luminance(), 0.7152, e) &&
                withinEpsilon(Color(0, 0, 1).luminance(), 0.0722, e));
    }();
    // Log sub-test results
    debugPrint(color_constructors);
    debugPrint(color_equality);
    debugPrint(color_assignment);
    debugPrint(color_luminance);
    bool all_tests_ok = (color_constructors &&
                         color_equality &&
                         color_assignment &&
                         color_luminance);
    debugPrint(all_tests_ok);
    return all_tests_ok;
}
