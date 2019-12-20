//
//  UnitTests.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/16/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Vec2.h"
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
    bool color_hsv = []()
    {
        float e = 0.000001;
        auto from_rgb_to_hsv_to_rgb = [&](float r, float g, float b)
        {
            float h1, s1, v1;
            float r1, g1, b1;
            Color::convertRGBtoHSV(r, g, b, h1, s1, v1);
            Color::convertHSVtoRGB(h1, s1, v1, r1, g1, b1);
            return (withinEpsilon(r, r1, e) &&
                    withinEpsilon(g, g1, e) &&
                    withinEpsilon(b, b1, e));
        };
        Color c0(1.0, 0.5, 0.0);
        Color c1 = Color::makeHSV(c0.getH(), c0.getS(), c0.getV());
        float h0, s0, v0;
        float r0, g0, b0;
        Color::convertHSVtoRGB(0, 0, 0, r0, g0, b0);
        Color::convertRGBtoHSV(r0, g0, b0, h0, s0, v0);
        return (from_rgb_to_hsv_to_rgb(0.0, 0.0, 0.0) &&
                from_rgb_to_hsv_to_rgb(1.0, 1.0, 1.0) &&
                from_rgb_to_hsv_to_rgb(0.5, 0.5, 0.5) &&
                from_rgb_to_hsv_to_rgb(0.1, 0.5, 0.9) &&
                withinEpsilon(c0.r(), c1.r(), e) &&
                withinEpsilon(c0.g(), c1.g(), e) &&
                withinEpsilon(c0.b(), c1.b(), e) &&
                withinEpsilon(h0, 0, e) &&
                withinEpsilon(v0, 0, e) &&
                withinEpsilon(s0, 0, e));
    }();
    
    
//    debugPrint(h0);
//    debugPrint(s0);
//    debugPrint(v0);

    bool vec2_constructors = ((Vec2().x() == 0) &&
                              (Vec2().y() == 0) &&
                              (Vec2(1, -2).x() == 1) &&
                              (Vec2(1, -2).y() == -2));
    bool vec2_equality = ((Vec2() == Vec2()) &&
                          (Vec2(1, -2) == Vec2(1, -2)));
    bool vec2_assignment = []()
    {
        Vec2 v1 = Vec2();
        Vec2 v2 = Vec2(1, -2);
        return ((v1 == Vec2()) &&
                (v2 == Vec2(1, -2)) &&
                (v2 == (v2 = Vec2(1, -2))));
    }();
    bool vec2_operators = []()
    {
        return ((Vec2(2, 4).dot(Vec2(10, 20)) == 100) &&
                (Vec2(3, 4).length() == 5) &&
                (Vec2(3, 4).normalize() == Vec2(0.6, 0.8)));
    }();
    bool vec2_random_point = []()
    {
        bool all_ok = true;
        for (int i = 0; i < 1000; i ++)
            if (Vec2::randomPointInUnitDiameterCircle().length() > 0.5)
                all_ok = false;
        return all_ok;
    }();
    // Log sub-test results
    debugPrint(color_constructors);
    debugPrint(color_equality);
    debugPrint(color_assignment);
    debugPrint(color_luminance);
    debugPrint(color_hsv);
    debugPrint(vec2_constructors);
    debugPrint(vec2_equality);
    debugPrint(vec2_assignment);
    debugPrint(vec2_operators);
    debugPrint(vec2_random_point);
    bool all_tests_ok = (color_constructors &&
                         color_equality &&
                         color_assignment &&
                         color_luminance &&
                         color_hsv &&
                         vec2_constructors &&
                         vec2_equality &&
                         vec2_assignment &&
                         vec2_operators &&
                         vec2_random_point);
    debugPrint(all_tests_ok);
    return all_tests_ok;
}
