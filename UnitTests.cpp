//
//  UnitTests.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/16/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

// The various "unit test" functions below assert that some supposed property
// (usually an equality, or a "within epsilon" test) exists, and returns true
// if so. Some also print logging.
//
// The main entry point is UnitTests::allTestsOK()

#include "Color.h"
#include "Operators.h"
#include "UnitTests.h"
#include "Utilities.h"
#include "Vec2.h"

// This "sub-test" wrapper macro just returns the value of the given expression
// "e". If the value is NOT TRUE, the st() macro will also log the specific
// failing sub-test expression ("e") to aid in debugging. Unit test functions
// generally run several sub-tests ANDing the results together.
#define st(e) [&]()                                        \
{                                                          \
    bool _e_ok = (e);                                      \
    if (!_e_ok) std::cout << "fail: " << #e << std::endl;  \
    return _e_ok;                                          \
}()

// Tests Utilities.h.
bool utilities()
{
    float e = 0.0000001;
    return (st(withinEpsilon(1, 1, 0)) &&
            st(withinEpsilon(1.1, 1.2, 0.2)) &&
            st(withinEpsilon(-1.1, -1.2, 0.2)) &&
            st(!withinEpsilon(1.1, 1.2, 0.01)) &&
            st(sq(2) == 4) &&
            st(interpolate(0.1, 0, 10) == 1) &&
            st(interpolate(0.1, 0, -10) == -1) &&
            st(clip(2, 1, 3) == 2) &&
            st(clip(0, 1, 2) == 1) &&
            st(clip(3, 1, 2) == 2) &&
            st(clip(0, 1, 1) == 1) &&
            st(clip(3, 1, 1) == 1) &&
            st(clip(2, 3, 1) == 2) &&
            st(clip(0, 3, 1) == 1) &&
            st(clip(4, 3, 1) == 3) &&
            st(sinusoid(0) == 0) &&
            st(sinusoid(0.25) < 0.25) &&
            st(sinusoid(0.5) == 0.5) &&
            st(sinusoid(0.75) > 0.75) &&
            st(sinusoid(1) == 1) &&
            st(remapInterval(1.5, 1, 2, 20, 30) == 25) &&
            st(remapInterval(2, 1, 4, 10, 40) == 20) &&
            st(remapIntervalClip(5, 1, 4, 10, 40) == 40) &&
            st(remapInterval(1.5, 1, 2, 30, 20) == 25) &&
            st(remapInterval(2, 1, 3, 30, 10) == 20) &&
            st(remapIntervalClip(5, 1, 4, 40, 10) == 10) &&
            st(!std::isnan(remapInterval(1, 1, 1, 2, 3))) &&
            st(!std::isnan(remapIntervalClip(1, 1, 1, 2, 3))) &&
            st(withinEpsilon(fmod_floor(1, 1.23), 1, e)) &&
            st(withinEpsilon(fmod_floor(2, 1.23), 0.77, e)) &&
            st(withinEpsilon(fmod_floor(-1, 1.23), 0.23, e)) &&
            st(withinEpsilon(fmod_floor(-2, 1.23), 0.46, e)) &&
            st(withinEpsilon(fmod_floor(1.23, 1.23), 0, e)) &&
            st(nearestOddInt(0.1) == 1) &&
            st(nearestOddInt(1.0) == 1) &&
            st(nearestOddInt(1.9) == 1) &&
            st(nearestOddInt(2.1) == 3) &&
            st(nearestOddInt(-0.1) == -1) &&
            st(nearestOddInt(-1.0) == -1) &&
            st(nearestOddInt(-1.9) == -1) &&
            st(nearestOddInt(-2.1) == -3));
};

// Tests for Color class.
bool color_constructors()
{
    return (st(Color().r() == 0) &&
            st(Color().g() == 0) &&
            st(Color().b() == 0) &&
            st(Color(1, 2, 3).r() == 1) &&
            st(Color(1, 2, 3).g() == 2) &&
            st(Color(1, 2, 3).b() == 3));
}

bool color_equality()
{
    return (st(Color() == Color()) &&
            st(Color() != Color(1, 2, 3)) &&
            st(Color(1, 2, 3) == Color(1, 2, 3)));
}

bool color_assignment()
{
    Color ca1 = Color();
    Color ca2 = Color(1, 2, 3);
    return (st(ca1 == Color()) &&
            st(ca2 == Color(1, 2, 3)) &&
            st(ca2 == (ca2 = Color(1, 2, 3))));
}

bool color_basic_operators()
{
    float e = 0.000001;
    Color wec1(0.1, 0.2, 0.3);
    Color wec2(0.1, 0.2, 0.3 + (e / 2));
    Color wec3(0.1, 0.2, 0.3 + (e * 2));
    Color black(0, 0, 0);
    Color gray50(0.5, 0.5, 0.5);
    Color white(1, 1, 1);
    Color c(0.1, 0.1, 0.1);
    return (st(withinEpsilon(wec1, wec1, 0)) &&
            st(withinEpsilon(wec1, wec2, e)) &&
            st(withinEpsilon(wec2, wec1, e)) &&
            st(!withinEpsilon(wec1, wec3, e)) &&
            st((white + black) == white) &&
            st((white - black) == white) &&
            st((white - white) == black) &&
            st((white - gray50) == gray50) &&
            st((gray50 * 2) == white) &&
            st((white / 2) == gray50) &&
            st((gray50 * gray50) == (white / 4)) &&
            st((c += c) == Color(0.2, 0.2, 0.2)) &&
            st((c *= 4) == Color(0.8, 0.8, 0.8)) &&
            st(gray50.length() == (std::sqrt(3 * sq(0.5)))) &&
            st(gray50.normalize() == white.normalize()) &&
            st(Color(0.3, 0, 0).normalize() == Color(1, 0, 0)) &&
            st(gray50.gamma(2.2) == (white * std::pow(0.5f, 2.2f))));
}

bool color_luminance()
{
    return (st(Color(0, 0, 0).luminance() == 0) &&
            st(Color(1, 1, 1).luminance() == 1) &&
            st(Color(1, 0, 0).luminance() == 0.2126f) &&
            st(Color(0, 1, 0).luminance() == 0.7152f) &&
            st(Color(0, 0, 1).luminance() == 0.0722f) &&
            st(Color(0.5, 0.5, 0.5).luminance() == 0.5));
}

bool color_hsv()
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
    bool randoms_ok = true;
    RandomSequence rs(38284732);
    for (int i = 0; i < 10000; i++)
    {
        Color c = rs.randomUnitRGB();
        randoms_ok = randoms_ok && from_rgb_to_hsv_to_rgb(c.r(), c.g(), c.b());
    }
    return (st(from_rgb_to_hsv_to_rgb(0.0, 0.0, 0.0)) &&
            st(from_rgb_to_hsv_to_rgb(1.0, 1.0, 1.0)) &&
            st(from_rgb_to_hsv_to_rgb(0.5, 0.5, 0.5)) &&
            st(from_rgb_to_hsv_to_rgb(0.1, 0.5, 0.9)) &&
            st(withinEpsilon(c0.r(), c1.r(), e)) &&
            st(withinEpsilon(c0.g(), c1.g(), e)) &&
            st(withinEpsilon(c0.b(), c1.b(), e)) &&
            st(withinEpsilon(h0, 0, e)) &&
            st(withinEpsilon(v0, 0, e)) &&
            st(withinEpsilon(s0, 0, e)) &&
            st(randoms_ok));
}

bool color_clip()
{
    float e = 0.000001;
    bool all_ok = true;
    RandomSequence rs(66426174);
    for (int i = 0; i < 1000; i ++)
    {
        Color a(rs.frandom2(-1, 10), rs.frandom2(-1, 10), rs.frandom2(-1, 10));
        Color b = a.clipToUnitRGB();
        bool in_range = ((b.r() >= 0) && (b.g() >= 0) && (b.b() >= 0) &&
                         (b.r() <= 1) && (b.g() <= 1) && (b.b() <= 1));
        bool skip = ((a.length() == 0) ||
                     (a.r() <= 0) || (a.g() <= 0) || (a.b() <= 0));
        bool direction_ok = withinEpsilon(a.normalize(), b.normalize(), e);
        if (!in_range || !(skip || direction_ok)) all_ok = false;
    }
    return all_ok;
}

bool vec2_constructors()
{
    return (st(Vec2().x() == 0) &&
            st(Vec2().y() == 0) &&
            st(Vec2(1, -2).x() == 1) &&
            st(Vec2(1, -2).y() == -2));
}

bool vec2_equality()
{
    return (st(Vec2() == Vec2()) &&
            st(Vec2(1, -2) == Vec2(1, -2)));
}

bool vec2_assignment()
{
    Vec2 v1 = Vec2();
    Vec2 v2 = Vec2(1, -2);
    return (st(v1 == Vec2()) &&
            st(v2 == Vec2(1, -2)) &&
            st(v2 == (v2 = Vec2(1, -2))));
}

bool vec2_vector_operations()
{
    return (st(Vec2(2, 4).dot(Vec2(10, 20)) == 100) &&
            st(Vec2(3, 4).length() == 5) &&
            st(Vec2(3, 4).normalize() == Vec2(0.6, 0.8)));
}

bool vec2_basic_operators()
{
    float e = 0.000001;
    Vec2 wev1(0.1, 0.2);
    Vec2 wev2(0.1, 0.2 + (e / 2));
    Vec2 wev3(0.1, 0.2 + (e * 2));
    Vec2 v(2, 3);
    return (st(withinEpsilon(wev1, wev1, 0)) &&
            st(withinEpsilon(wev1, wev2, e)) &&
            st(withinEpsilon(wev2, wev1, e)) &&
            st(!withinEpsilon(wev1, wev3, e)) &&
            st(-Vec2(1, 2) == Vec2(-1, -2)) &&
            st((Vec2(1, 2) + Vec2(10, 20)) == Vec2(11, 22)) &&
            st((Vec2(10, 20) - Vec2(1, 2)) == Vec2(9, 18)) &&
            st((Vec2(1, 2) * 5) == Vec2(5, 10)) &&
            st((Vec2(5, 10) / 5) == Vec2(1, 2)) &&
            st(Vec2(1, 2) < Vec2(-3, -4)) &&
            st((v + v) == (v += v)) &&
            st((v * 3) == (v *= 3)));
}

bool vec2_random_point()
{
    bool all_ok = true;
    RandomSequence rs(23807653);
    for (int i = 0; i < 1000; i ++)
        if (rs.randomPointInUnitDiameterCircle().length() > 0.5)
            all_ok = false;
    return all_ok;
}

bool vec2_rotate()
{
    bool all_ok = true;
    RandomSequence rs(85085172);
    for (int i = 0; i < 100; i ++)
    {
        float angle = rs.frandom2(-60, +60);  // In radians, tests large angles.
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        Vec2 v = Vec2(1, 0).rotate(angle);
        if ((v.x() != cos) || (v.y() != -sin)) all_ok = false;
    }
    return all_ok;
}

bool gradation_test()
{
    Vec2 point1(0.2, 0.2);
    Vec2 point2(0.8, 0.8);
    Color color1(1, 0, 1);
    Color color2(0, 1, 1);
    Uniform uniform1(color1);
    Uniform uniform2(color2);
    Gradation graduation(point1, uniform1, point2, uniform2);
    Vec2 midpoint = interpolate(0.5, point1, point2);
    Color midcolor = interpolate(0.5, color1, color2);
    float e = 0.00001;
    RandomSequence rs(38336022);
    auto off_axis_sample = [&](float f)
    {
        Vec2 on_axis = interpolate(f, point1, point2);
        Vec2 off_axis = Vec2(-1, 1) * rs.frandom2(-10, 10);
        // TODO maybe instead of predicting what color we expect to find
        // (which requires internal knowledge of the texure) maybe compare
        // two samples, say from plus and minus off_axis.
        Color expected_color = interpolate(sinusoid(f), color1, color2);
        Color sampled_color = graduation.getColor(on_axis + off_axis);
        return withinEpsilon(sampled_color, expected_color, e);
    };
    return (st(graduation.getColor(point1) == color1) &&
            st(graduation.getColor(point2) == color2) &&
            st(withinEpsilon(graduation.getColor(midpoint), midcolor, e)) &&
            st(withinEpsilon(graduation.getColor(Vec2(0, 0)), color1, e)) &&
            st(withinEpsilon(graduation.getColor(Vec2(1, 1)), color2, e)) &&
            st([&](){
                for (int i = 0; i < 10; i++)
                    if (!off_axis_sample(i * 0.1)) return false;
                return true;
            }()));
}

bool spot_test()
{
    Vec2 center(-0.4, -0.4);
    float inner_radius = 0.1;
    float outer_radius = 0.3;
    Color inner_color(1, 1, 0);
    Color outer_color(0, 1, 0);
    Uniform uniform_ic(inner_color);
    Uniform uniform_oc(outer_color);
    Spot spot(center, inner_radius, uniform_ic, outer_radius, uniform_oc);
    Color midcolor = interpolate(0.5, inner_color, outer_color);
    float midradius = (inner_radius + outer_radius) / 2;
    Vec2 midpoint = center + (Vec2(1, 0) * midradius);
    float e = 0.000001;
    RandomSequence rs(64577036);
    return (st(spot.getColor(center) == inner_color) &&
            st(spot.getColor(midpoint * 2) == outer_color) &&
            st(withinEpsilon(spot.getColor(midpoint), midcolor, e)) &&
            st([&](){
                for (int i = 0; i < 100; i++) // try 100 times
                {
                    // Two random vectors, with the same random radius in
                    // transition zone, should have the same color.
                    float r_radius = rs.frandom2(inner_radius, outer_radius);
                    Vec2 rv1 = rs.randomUnitVector() * r_radius;
                    Vec2 rv2 = rs.randomUnitVector() * r_radius;
                    Color color1 = spot.getColor(center + rv1);
                    Color color2 = spot.getColor(center + rv2);
                    if (!withinEpsilon(color1, color2, e)) return false;
                }
                return true;
            }()));
}

bool grating_test()
{
    float e = 0.0001;
    RandomSequence rs(9635451);
    return ([&]()
            {
                for (int i = 0; i < 100; i++) // try 100 times
                {
                    // Define a random Grating
                    Vec2 p1 = rs.randomPointInUnitDiameterCircle();
                    Vec2 p2 = rs.randomPointInUnitDiameterCircle();
                    Color c1 = rs.randomUnitRGB();
                    Color c2 = rs.randomUnitRGB();
                    Uniform u1(c1);
                    Uniform u2(c2);
                    Grating grating(p1, u1, p2, u2, rs.frandom01(), 0.5);
                    // Pick a random point between p1 and p2.
                    Vec2 between = interpolate(rs.frandom01(), p1, p2);
                    // Pick another point along the line p1,p2 which is
                    // some random integer multiple of offset away.
                    Vec2 offset = p2 - p1;
                    Vec2 other = between + (offset * int(rs.frandom2(-5, 5)));
                    // Read back colors from midpoint, between, and other.
                    Color gc_midpoint = grating.getColor((p1 + p2) / 2);
                    Color gc_between = grating.getColor(between);
                    Color gc_other = grating.getColor(other);
                    // Check if everything is as expected
                    bool ok = (st(grating.getColor(p1) == c1) &&
                               st(grating.getColor(p2) == c1) &&
                               st(withinEpsilon(gc_midpoint, c2, e)) &&
                               st(withinEpsilon(gc_between, gc_other, e)));
                    if (!ok) return false;
                }
                return true;
            }());
}

bool operators_minimal_test()
{
    float e = 0.000001;
    Color black(0, 0, 0);
    Color white(1, 1, 1);
    Color gray(0.5, 0.5, 0.5);
    Uniform bt(black);  // black texture
    Uniform gt(gray);   // gray texture
    Uniform wt(white);  // white texture
    Max mx(bt, wt);
    Min mn(bt, wt);
    Add ad(wt, gt);
    Subtract s1(wt, gt);
    Subtract s2(bt, gt);
    float ri = 0.2;  // spot radius inner
    float ro = 0.8;  // spot radius outer
    Spot sp(Vec2(0, 0), ri, wt, ro, bt);
    SoftMatte sm(sp, bt, wt);
    RandomSequence rs(59049567);
    return ([&]()
            {
                bool all_ok = true;
                for (int i = 0; i < 1000; i++) // try 1000 times
                {
                    Vec2 r_pos = rs.randomPointInUnitDiameterCircle() * 2;
                    float r = r_pos.length();
                    float r_remap = remapIntervalClip(r, ri, ro, 0, 1);
                    float spot_profile = sinusoid(r_remap);
                    Color sm_color = interpolate(spot_profile, white, black);
                    bool ok =
                    (st(withinEpsilon(bt.getColor(r_pos), black, e)) &&
                     st(withinEpsilon(wt.getColor(r_pos), white, e)) &&
                     st(withinEpsilon(mx.getColor(r_pos), white, e)) &&
                     st(withinEpsilon(mn.getColor(r_pos), black, e)) &&
                     st(withinEpsilon(ad.getColor(r_pos), white + gray, e)) &&
                     st(withinEpsilon(s1.getColor(r_pos), white - gray, e)) &&
                     st(withinEpsilon(s2.getColor(r_pos), black - gray, e)) &&
                     st(withinEpsilon(sm.getColor(r_pos), sm_color, e)));
                    if (!ok) all_ok = false;
                }
                return all_ok;
            }());
}

bool noise_ranges()
{
    auto test_range = [](std::function<float(Vec2)> noise_function,
                         float min_threshold, float max_threshold)
    {
        std::pair<float, float> min_max =
            PerlinNoise::measure_range(noise_function);
        // std::cout << "noise_ranges: ";
        // std::cout << "min_range = " << min_max.first << ", ";
        // std::cout << "max_range = " << min_max.second << std::endl;
        return ((min_max.first <= min_threshold) &&
                (min_max.second >= max_threshold));
    };
    return (st(test_range(PerlinNoise::noise2d,     -1, 1)) &&
            st(test_range(PerlinNoise::unitNoise2d,  0, 1)) &&
            st(test_range(PerlinNoise::turbulence2d, 0, 1)) &&
            st(test_range(PerlinNoise::brownian2d,   0, 1)) &&
            st(test_range(PerlinNoise::furbulence2d, 0, 1)) &&
            st(test_range(PerlinNoise::wrapulence2d, 0, 1)));
}

bool interpolate_float_rounding()
{
    // See https://cwreynolds.github.io/TexSyn/#20200614 about a change to the
    // interpolate() template to avoid floating point rounding issues. I changed
    // from basing noise1/noise2 on Noise/Brownian to Grating. I had been unable
    // to reproduce the June 14 fail (today on July 16) perhaps because of a
    // change to noise ranges on June 24.
    Vec2 p1(0, 0);
    Vec2 p2(0.1, 0.2);
    Uniform red(1, 0, 0);
    Uniform blue(0, 0, 1);
    Uniform green(0, 1, 0);
    Grating noise0(Vec2(), red, Vec2(0, 0.1), blue, 0.2, 0.5);
    Grating noise1(Vec2(), blue, Vec2(0.1, 0), green, 0.2, 0.5);
    bool ok = true;
    int subtest_count = 10000;
    RandomSequence rs(38430341);
    for (int i = 0; i < subtest_count; i++)
    {
        Vec2 p = rs.randomPointInUnitDiameterCircle() * 2;
        Color color0 = noise0.getColor(p);
        Color color1 = noise1.getColor(p);
        Color interpolated = interpolate(1, color0, color1);
        if (!st(color1 == interpolated)) ok = false;
        // Enable this code to verify failing case:
        if ((false))
        {
            // This was the body of interpolation() before June 14.
            auto old_interp = [](float alpha, const Color& x0, const Color& x1)
                { return x0 + ((x1 - x0) * alpha); };
            interpolated = old_interp(1, color0, color1);
            if (color1 != interpolated)
            {
                std::cout << "old_interp() fails with color difference of: ";
                std::cout << (color1 - interpolated) << std::endl;
            }
        }
    }
    return ok;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool two_point_transform()
{
    float e = 0.000001;
    // Define identity transform two ways.
    TwoPointTransform identity;
    TwoPointTransform identity2(Vec2(0, 0), Vec2(1, 0));
    // Define an "arbitrary" transform.
    Vec2 ao(-1, -2);     // global position of local space's origin
    Vec2 axb(3, 5);      // basis vector along local space's x axis (1, 0).
    Vec2 ayb = axb.rotate90degCCW(); // basis vector along y axis (0, 1)
    Vec2 ahl(0.5, 0.5);  // (0.5, 0.5) in local space, which correponds to:
    Vec2 ahg(-2, 2);     // (-2, 2) in global space
    TwoPointTransform arbitrary(ao, ao + axb);
    return (// Test default constructor, identity transform.
            st(identity.scale() == 1) &&
            st(identity.origin() == Vec2(0, 0)) &&
            st(identity.xBasis() == Vec2(1, 0)) &&
            st(identity.yBasis() == Vec2(0, 1)) &&
            st(identity.xBasisUnit() == Vec2(1, 0)) &&
            st(identity.yBasisUnit() == Vec2(0, 1)) &&
            // Test constructor with two point parameters, identity transform.
            st(identity2.scale() == 1) &&
            st(identity2.origin() == Vec2(0, 0)) &&
            st(identity2.xBasis() == Vec2(1, 0)) &&
            st(identity2.yBasis() == Vec2(0, 1)) &&
            st(identity2.xBasisUnit() == Vec2(1, 0)) &&
            st(identity2.yBasisUnit() == Vec2(0, 1)) &&
            // Construct arbitrary transform with scale, rotation, and translate.
            st(arbitrary.scale() == axb.length()) &&
            st(arbitrary.origin() == ao) &&
            st(arbitrary.xBasis() == axb) &&
            st(arbitrary.yBasis() == ayb) &&
            st(arbitrary.xBasisUnit() == axb.normalize()) &&
            st(arbitrary.yBasisUnit() == ayb.normalize()) &&

            // Test localize() and globalize() Vec2 operators.
            st(identity.globalize(ao) == ao) &&
            st(identity.localize(ao) == ao) &&
            st(arbitrary.globalize(Vec2(0, 0)) == ao) &&
            st(arbitrary.globalize(Vec2(1, 1)) == Vec2(-3, 6)) &&
            st(arbitrary.globalize(Vec2(0, 1)) == Vec2(-6, 1)) &&
            st(arbitrary.globalize(Vec2(1, 0)) == Vec2(2, 3)) &&
            st(arbitrary.globalize(ahl) == ahg) &&
            st(arbitrary.localize(ao) == Vec2(0, 0)) &&
            st(withinEpsilon(arbitrary.localize(Vec2(-3, 6)), Vec2(1, 1), e)) &&
            st(withinEpsilon(arbitrary.localize(Vec2(-6, 1)), Vec2(0, 1), e)) &&
            st(withinEpsilon(arbitrary.localize(Vec2(2, 3)), Vec2(1, 0), e)) &&
            st(withinEpsilon(arbitrary.localize(ahg), ahl, e)) &&
            true);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Used only in UnitTests::allTestsOK()
#define logAndTally(e)                       \
{                                            \
    bool _e_ok = e();                        \
    std::cout << "\t";                       \
    std::cout << (_e_ok ? "pass" : "FAIL");  \
    std::cout << " " << #e;                  \
    std::cout << std::endl << std::flush;    \
    if (!_e_ok) all_tests_passed = false;    \
}

bool UnitTests::allTestsOK()
{
    Timer timer("Run time for unit test suite: ", "");
    bool all_tests_passed = true;
    logAndTally(utilities);
    logAndTally(color_constructors);
    logAndTally(color_equality);
    logAndTally(color_assignment);
    logAndTally(color_basic_operators);
    logAndTally(color_luminance);
    logAndTally(color_hsv);
    logAndTally(color_clip);
    logAndTally(vec2_constructors);
    logAndTally(vec2_equality);
    logAndTally(vec2_assignment);
    logAndTally(vec2_vector_operations);
    logAndTally(vec2_basic_operators);
    logAndTally(vec2_random_point);
    logAndTally(vec2_rotate);
    logAndTally(gradation_test);
    logAndTally(spot_test);
    logAndTally(grating_test);
    logAndTally(operators_minimal_test);
    logAndTally(noise_ranges);
    logAndTally(interpolate_float_rounding);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    logAndTally(two_point_transform);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    std::cout << std::endl;
    std::cout << (all_tests_passed ? "All tests PASS." : "Some tests FAIL.");
    std::cout << std::endl << std::endl;
    return all_tests_passed;
}

// This utility is intended to verify that all Texture types exist, can be
// constructed, and produce a "reasonable" output. It is currently not used
// anywhere except when it was called "manually" from main() on June 7, 2020
// for testing. Note that no mechanism automatically adds clauses to this
// function when new texture types are defined, so it needs to be updated
// manually, which of course reduces its effectiveness for catching (e.g.)
// accidentally deleted definitions.
void UnitTests::instantiateAllTextureTypes()
{
    Vec2 p1(-0.1, 0);
    Vec2 p2(0.1, 0);
    Vec2 p3(0.4, 0.6);
    Uniform black(Color(0, 0, 0));
    Uniform white(Color(1, 1, 1));
    Uniform red(Color(1, 0, 0));
    Uniform cyan(Color(0, 1, 1));
    Grating white_cyan(Vec2(0, 0.2), white, Vec2(0, 0), cyan, 0.1, 0.5);
    Grating black_red(Vec2(0.1, 0), black, Vec2(0, 0), red, 0.1, 0.5);
    Grating& t1 = white_cyan;
    Grating& t2 = black_red;
    ColorNoise t3(p1, p3, 0.2);
    
    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200607_";
    int counter = 0;
    
    auto do_thumbnail = [&](const Texture& texture)
    {
        std::string s = path + "thumbnail_" + std::to_string(counter++);
        // TODO TEMP
        s = "";
        Texture::displayAndFile(texture, s, 101);
    };

    do_thumbnail(Uniform(0.5));
    do_thumbnail(Spot(p1, 0.1, t1, 0.2, t2));
    do_thumbnail(Gradation(p1, t1, p2, t2));
    do_thumbnail(Grating(p1, t1, p3, t2, 1, 0.5));
    do_thumbnail(SoftMatte(t1, t2, t3));
    do_thumbnail(Add(t1, t2));
    do_thumbnail(Subtract(t1, t2));
    do_thumbnail(Multiply(t1, t2));
    do_thumbnail(Max(t1, t2));
    do_thumbnail(Min(t1, t2));
    do_thumbnail(AbsDiff(t1, t2));
    do_thumbnail(Noise(p1, p2, t1, t2));
    do_thumbnail(Brownian(p1, p2, t1, t2));
    do_thumbnail(Turbulence(p1, p2, t1, t2));
    do_thumbnail(Furbulence(p1, p2, t1, t2));
    do_thumbnail(Wrapulence(p1, p2, t1, t2));
    do_thumbnail(MultiNoise(p1, p2, t1, t2, 0.5));
    do_thumbnail(ColorNoise(p1, p2, 0.5));
    do_thumbnail(BrightnessToHue(0.5, t1));
    do_thumbnail(Wrap(2, p1, p2, t1));
    do_thumbnail(StretchSpot(5, 1, p1, t1));
    do_thumbnail(Stretch(Vec2(2, 3), p2, t1));
    do_thumbnail(SliceGrating(p3, p2, t1));
    do_thumbnail(SliceToRadial(p3, p2, t1));
    do_thumbnail(SliceShear(p3, p2, t1, Vec2(0.4, 0.1), p1, t2));
    do_thumbnail(Colorize(Vec2(1, 0.2), p1, t2, t3));
    do_thumbnail(MobiusTransform(p3, p1, Vec2(0.4, 0.1), p2, t1));
    do_thumbnail(Scale(0.5, t1));
    do_thumbnail(Rotate(0.5, t1));
    do_thumbnail(Translate(p1, t1));
    do_thumbnail(Blur(0.2, t1));
    do_thumbnail(SoftThreshold(0, 1, t1));
    do_thumbnail(EdgeDetect(0.1, t1));
    do_thumbnail(EdgeEnhance(0.1, 1, t1));
    do_thumbnail(AdjustHue(0.25, t1));
    do_thumbnail(AdjustSaturation(0.5, t1));
    do_thumbnail(AdjustBrightness(0.5, t1));
    do_thumbnail(Twist(10, 2, p1, t1));
    do_thumbnail(BrightnessWrap(0.4, 0.6, t3));
    do_thumbnail(Mirror(p3, p2, t1));
    do_thumbnail(Ring(9, p3, p1, t1));
    do_thumbnail(Row(Vec2(0.1, 0.1), p1, t1));
    do_thumbnail(Shader(Vec3(1, 1, 1), 0.2, t1, t3));
    do_thumbnail(LotsOfSpots(0.8, 0.1, 0.4, 0.05, 0.01, t1, t2));
    do_thumbnail(ColoredSpots(0.8, 0.1, 0.4, 0.05, 0.01, t1, t2));
    do_thumbnail(LotsOfButtons(0.8, 0.1, 0.4, 0.05, 0.01, p1, t1, 1, t2));
    do_thumbnail(Gamma(0.5, t3));
    do_thumbnail(RgbBox(0.2, 1, 0, 0.2, 0.2, 1, t1));
    do_thumbnail(CotsMap(p1, p2, p3, Vec2(-1, -1), white_cyan));
    do_thumbnail(Hyperbolic(p3, 1.5, 4, 2, black_red, white_cyan));
    do_thumbnail(Affine(p3, p3 + Vec2(0.3, 0.3), white_cyan));
    do_thumbnail(HueOnly(1, 1, AdjustBrightness(0.1, Add(white_cyan, white))));

    std::cout << "Total thumbnails constructed: " << counter << std::endl;
}
