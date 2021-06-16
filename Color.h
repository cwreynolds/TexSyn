//
//  Color.h
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Utilities.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO expermental
// Helper class to represent a color in hue-saturation-value space.
class HSV;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// TODO move elsewhere
// Simple class to represent color as RGB float values on [0, 1]
class Color
{
public:
    // Constructors:
    Color() : Color(0, 0, 0) {};
    Color(float gray) : Color(gray, gray, gray) {}
    // Modified to count and report, but otherwise tolerate, "abnormal" floats.
    Color(float r, float g, float b)
      : red_(paper_over_abnormal_values(r)),
        green_(paper_over_abnormal_values(g)),
        blue_(paper_over_abnormal_values(b)) {}
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Convert from HSV to RGB (defined below, after HSV class).
    Color(HSV hsv);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // assignment, set RGB components
    Color operator=(const Color& c)
    {
        setRGB(c.r(), c.g(), c.b());
        return *this;
    }
    void setRGB(float r, float g, float b) { red_ = r; green_ = g; blue_ = b; }
    // Equality, inequality:
    bool operator==(const Color& c) const
    {
        return ((r() == c.r()) && (g() == c.g()) && (b() == c.b()));
    }
    bool operator!=(const Color& c) const { return !(*this == c); }
    // Luma (relative luminance?) see https://en.wikipedia.org/wiki/Luma_(video)
    float luminance() const {return 0.2126 * r() + 0.7152 * g() + 0.0722 * b();}
    // TODO still thinking about this API and these names:
    // Set this color to the one described by the given HSV values.
    void setHSV(float h, float s, float v);
    // Get the HSV values for this color. Returned by setting non-const refs.
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO obsolete?
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    void getHSV(float& h, float& s, float& v) const;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Get the HSV values for a Color. Returned as 3-tuple of floats.
    // TODO needed? Wrote first version of this before the HSV helper class.
    std::tuple<float, float, float> getHSV() const;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Return a Color made from the given HSV values
    static Color makeHSV(float h, float s, float v);
    // Get H, S, or V components of this Color.
    float getH() const;
    float getS() const;
    float getV() const;
    // Static utilities to convert RGB→HSV and HSV→RGB
    static void convertRGBtoHSV(float red, float green, float blue,
                                float& H, float& S, float& V);
    static void convertHSVtoRGB(float h, float s, float v,
                                float& R, float& G, float& B);
    // Inline operators: + - * / += *=
    Color operator+(Color v) const;
    Color operator-(Color v) const;
    Color operator*(Color c) const;  // color * color, aka "tint"
    Color operator*(float s) const;
    Color operator/(float s) const { return *this * (1 / s); };
    Color operator+=(const Color& rhs) { return *this = *this + rhs; }
    Color operator*=(float s) { return *this = *this * s; }
    // Length and normalize in linear RGB space.
    float length() const { return std::sqrt(sq(r()) + sq(g()) + sq(b())); }
    Color normalize() const { return *this / length(); }
    // Get corresponding color value clipped to unit RGB cube.
    Color clipToUnitRGB() const;
    // Exponentiate the RGB components by given gamma value ("exponent")
    Color gamma(float g) const;
    // Test that the RGB components are "normal" (neither infinite nor NaN).
    bool isNormal() const
        { return (is_normal(r()) && is_normal(g()) && is_normal(b())); }
    // Assert RGB components are "normal".
    void assertNormal() const { assert(isNormal()); }
    static Color gray(float value) { return { value, value, value }; }
    // Accessors:
    float red() const { return red_; }
    float green() const { return green_; }
    float blue() const { return blue_; }
    // TODO Are these short names a good idea?
    float r() const { return red_; }
    float g() const { return green_; }
    float b() const { return blue_; }
private:
    float red_;
    float green_;
    float blue_;
};

// Is distance between RGB vectors no more than epsilon?
bool withinEpsilon(Color a, Color b, float epsilon);

// Serialize Color object to stream.
std::ostream& operator<<(std::ostream& os, const Color& v);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// TODO expermental

// Helper class to represent a color in hue-saturation-value space.
class HSV
{
public:
    HSV(float h, float s, float v) : h_(h), s_(s), v_(v) {}  // TODO needed?
    HSV(Color c) { Color::convertRGBtoHSV(c.r(), c.g(), c.b(), h_, s_, v_); }
    float getH() const { return h_; }
    float getS() const { return s_; }
    float getV() const { return v_; }
    std::tuple<float, float, float> getHSV() const { return {h_, s_, v_}; }
private:
    float h_ = 0;
    float s_ = 0;
    float v_ = 0;
};


// Construct an RGB Color from an HSV instance.
inline Color::Color(HSV hsv)
{
    Color::convertHSVtoRGB(hsv.getH(), hsv.getS(), hsv.getV(),
                           red_, green_, blue_);
}

// Get the HSV values for a Color. Returned as 3-tuple of floats.
// TODO needed? Wrote first version of this before the HSV helper class.
inline std::tuple<float, float, float> Color::getHSV() const
{
    return HSV(*this).getHSV();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
