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
#include "Generators.h"

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
    bool color_basic_operators = []()
    {
        float e = 0.000001;
        Color wec1(0.1, 0.2, 0.3);
        Color wec2(0.1, 0.2, 0.3 + (e / 2));
        return ((withinEpsilon(wec1, wec2, e)) &&
                (withinEpsilon(wec2, wec1, e)));
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
    bool vec2_vector_operations = []()
    {
        return ((Vec2(2, 4).dot(Vec2(10, 20)) == 100) &&
                (Vec2(3, 4).length() == 5) &&
                (Vec2(3, 4).normalize() == Vec2(0.6, 0.8)));
    }();
    bool vec2_basic_operators = []()
    {
        return ((-Vec2(1, 2) == Vec2(-1, -2)) &&
                ((Vec2(1, 2) + Vec2(10, 20)) == Vec2(11, 22)) &&
                ((Vec2(10, 20) - Vec2(1, 2)) == Vec2(9, 18)) &&
                ((Vec2(1, 2) * 5) == Vec2(5, 10)) &&
                ((Vec2(5, 10) / 5) == Vec2(1, 2)) &&
                (Vec2(1, 2) < Vec2(-3, -4)));
    }();
    bool vec2_random_point = []()
    {
        bool all_ok = true;
        for (int i = 0; i < 1000; i ++)
            if (Vec2::randomPointInUnitDiameterCircle().length() > 0.5)
                all_ok = false;
        return all_ok;
    }();
    bool vec2_rotate = []()
    {
        bool all_ok = true;
        for (int i = 0; i < 100; i ++)
        {
            float angle = frandom2(-60, +60);
            float cos = std::cos(angle);
            float sin = std::sin(angle);
            Vec2 v = Vec2(1, 0).rotate(angle);
            if ((v.x() != cos) || (v.y() != -sin)) all_ok = false;
        }
        return all_ok;
    }();
    bool gradation_test = []()
    {
        Vec2 point1(0.2, 0.2);
        Vec2 point2(0.8, 0.8);
        Color color1(1, 0, 1);
        Color color2(0, 1, 1);
        Gradation graduation(point1, color1, point2, color2);
        Vec2 midpoint = interpolate(0.5, point1, point2);
        Color midcolor = interpolate(0.5, color1, color2);
        float e = 0.000001;
        auto off_axis_sample = [&](float f)
        {
            Vec2 on_axis = interpolate(f, point1, point2);
            Vec2 off_axis = Vec2(-1, 1) * frandom2(-10, 10);
            Color expected_color = interpolate(sinusoid(f), color1, color2);
            Color sampled_color = graduation.getColor(on_axis + off_axis);
            return withinEpsilon(sampled_color, expected_color, e);
        };
        return ((graduation.getColor(point1) == color1) &&
                (graduation.getColor(point2) == color2) &&
                withinEpsilon(graduation.getColor(midpoint), midcolor, e) &&
                withinEpsilon(graduation.getColor(Vec2(0, 0)), color1, e) &&
                withinEpsilon(graduation.getColor(Vec2(1, 1)), color2, e) &&
                [&](){
                    for (int i = 0; i < 10; i++)
                        if (!off_axis_sample(i * 0.1)) return false;
                    return true;
                }());
    }();
    bool spot_test = []()
    {
        Vec2 center(-0.4, -0.4);
        float inner_radius = 0.1;
        float outer_radius = 0.3;
        Color inner_color(1, 1, 0);
        Color outer_color(0, 1, 0);
        Spot spot(center, inner_radius, inner_color, outer_radius, outer_color);
        Color midcolor = interpolate(0.5, inner_color, outer_color);
        float midradius = (inner_radius + outer_radius) / 2;
        Vec2 midpoint = center + (Vec2(1, 0) * midradius);
        float e = 0.000001;
        return ((spot.getColor(center) == inner_color) &&
                (spot.getColor(midpoint * 2) == outer_color) &&
                withinEpsilon(spot.getColor(midpoint), midcolor, e) &&
                [&](){
                    for (int i = 0; i < 100; i++) // try 100 times
                    {
                        // Two random vectors, with the same random radius in
                        // transition zone, should have the same color.
                        float r_radius = frandom2(inner_radius, outer_radius);
                        Vec2 rv1 = Vec2::randomUnitVector() * r_radius;
                        Vec2 rv2 = Vec2::randomUnitVector() * r_radius;
                        Color color1 = spot.getColor(center + rv1);
                        Color color2 = spot.getColor(center + rv2);
                        if (!withinEpsilon(color1, color2, e)) return false;
                    }
                    return true;
                }());
    }();

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Log sub-test results
    std::cout << "\t"; debugPrint(color_constructors);
    std::cout << "\t"; debugPrint(color_equality);
    std::cout << "\t"; debugPrint(color_assignment);
    std::cout << "\t"; debugPrint(color_basic_operators);
    std::cout << "\t"; debugPrint(color_luminance);
    std::cout << "\t"; debugPrint(color_hsv);
    std::cout << "\t"; debugPrint(vec2_constructors);
    std::cout << "\t"; debugPrint(vec2_equality);
    std::cout << "\t"; debugPrint(vec2_assignment);
    std::cout << "\t"; debugPrint(vec2_vector_operations);
    std::cout << "\t"; debugPrint(vec2_basic_operators);
    std::cout << "\t"; debugPrint(vec2_random_point);
    std::cout << "\t"; debugPrint(vec2_rotate);
    std::cout << "\t"; debugPrint(gradation_test);
    std::cout << "\t"; debugPrint(spot_test);
    bool ALL_TESTS_OK = (color_constructors &&
                         color_equality &&
                         color_assignment &&
                         color_basic_operators &&
                         color_luminance &&
                         color_hsv &&
                         vec2_constructors &&
                         vec2_equality &&
                         vec2_assignment &&
                         vec2_vector_operations &&
                         vec2_basic_operators &&
                         vec2_random_point &&
                         vec2_rotate &&
                         gradation_test &&
                         spot_test);
    std::cout << std::endl;
    debugPrint(ALL_TESTS_OK);
    std::cout << std::endl;
    return ALL_TESTS_OK;
}
