//
//  Color.h
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Utilities.h"

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
    // Equality, inequality:
    bool operator==(const Color& c) const
    {
        return ((r() == c.r()) && (g() == c.g()) && (b() == c.b()));
    }
    bool operator!=(const Color& c) const { return !(*this == c); }
    // Luma (relative luminance?) see https://en.wikipedia.org/wiki/Luma_(video)
    float luminance() const {return 0.2126 * r() + 0.7152 * g() + 0.0722 * b();}
    // Get hue, saturation, or value component of this Color.
    float h() const { return HSV(*this).h(); };
    float s() const { return HSV(*this).s(); };
    float v() const { return HSV(*this).v(); };
    // Inline operators: + - *
    Color operator+(Color v) const
        { return Color(r() + v.r(), g() + v.g(), b() + v.b()); }
    Color operator-(Color v) const
        { return Color(r() - v.r(), g() - v.g(), b() - v.b()); }
    Color operator*(Color c) const  // color * color, aka "tint"
        { return Color(r() * c.r(), g() * c.g(), b() * c.b()); }
    Color operator*(float s) const
        { return Color(r() * s, g() * s, b() * s); }
    Color operator/(float s) const { return *this * (1 / s); };
    Color operator+=(const Color& rhs) { return *this = *this + rhs; }
    Color operator*=(float s) { return *this = *this * s; }
    // Length and normalize in linear RGB space.
    float length() const { return std::sqrt(sq(r()) + sq(g()) + sq(b())); }
    Color normalize() const { return *this / length(); }
    // Test that the RGB components are "normal" (neither infinite nor NaN).
    bool isNormal() const
        { return (is_normal(r()) && is_normal(g()) && is_normal(b())); }
    // Assert RGB components are "normal".
    void assertNormal() const { assert(isNormal()); }
    static Color gray(float value) { return { value, value, value }; }
    // Accessors for RGB components (long and short name).
    float red() const { return red_; }
    float green() const { return green_; }
    float blue() const { return blue_; }
    float r() const { return red_; }
    float g() const { return green_; }
    float b() const { return blue_; }
    // Get corresponding color value clipped to unit RGB cube.
    Color clipToUnitRGB() const
    {
        Color result = *this;
        if (length() > 0)
        {
            // Individually clip each component to be greater than zero.
            auto nn = [](float x){ return std::max(0.0f, x); };
            result = Color(nn(r()), nn(g()), nn(b()));
            // Clip to red=1 plane, then green and blue.
            if (result.r() > 1) result = result / result.r();
            if (result.g() > 1) result = result / result.g();
            if (result.b() > 1) result = result / result.b();
        }
        return result;
    }
    // Exponentiate the RGB components by given gamma value ("exponent").
    // Also clips RGB components to be non-negative before exponentiation.
    // If any RGB values are so large that they "overflow", returns black.
    Color gamma(float exponent) const
    {
        float er = pow(std::max(r(), 0.0f), exponent);
        float eg = pow(std::max(g(), 0.0f), exponent);
        float eb = pow(std::max(b(), 0.0f), exponent);
        return Color(paper_over_abnormal_values(er),
                     paper_over_abnormal_values(eg),
                     paper_over_abnormal_values(eb));
    }
    // Computes a measure of similarity. For unit RGB values this is on [0, 1].
    // Similarity of identical colors is 1, colors in opposite corners of the
    // unit RGB cube (eg black and white, or blue and yellow) have similarity 0.
    // Colors outside unit RGB cube can have similarity less than 0.
    static float similarity(Color a, Color b)
    {
        // Length of difference, divided by unit cube diagonal, subtract from 1.
        return 1 - ((a - b).length() / std::sqrt(3.0));
    }
    // Helper class to represent a color in hue-saturation-value space.
    class HSV
    {
    public:
        // Construct from H, S, and V.  (TODO needed?)
        HSV(float h, float s, float v) : h_(h), s_(s), v_(v) {}
        // Construct from (RGB) Color object
        HSV(Color c) { Color::convertRGBtoHSV(c.r(), c.g(), c.b(), h_, s_, v_); }
        // Accessors for hue, saturation, and value.
        float h() const { return h_; }
        float s() const { return s_; }
        float v() const { return v_; }
        // Return a copy of this HSV with one of its components overwritten.
        HSV newH(float h) const { return {h, s_, v_}; }
        HSV newS(float s) const { return {h_, s, v_}; }
        HSV newV(float v) const { return {h_, s_, v}; }
        // Return H, S, and V as 3-tuple of floats.
        std::tuple<float, float, float> getHSV() const { return {h_, s_, v_}; }
    private:
        float h_ = 0;
        float s_ = 0;
        float v_ = 0;
    };
    // Construct an RGB Color from an HSV instance.
    inline Color(HSV hsv)
    {
        Color::convertHSVtoRGB(hsv.h(), hsv.s(), hsv.v(), red_, green_, blue_);
    }
private:
    float red_;
    float green_;
    float blue_;
    
    // Transform color space from "Red Green Blue" to "Hue Saturation Value"
    //
    // Method due to Alvy Ray Smith, while at NYIT, first published in 1978.
    // See http://en.wikipedia.org/wiki/HSV_color_space
    //
    // Code due to T. Nathan Mundhenk of the USC iLab and generously provided
    // on the web: http://ilab.usc.edu/wiki/index.php/HSV_And_H2SV_Color_Space
    //
    // I made all six parameters (RGBHSV) range on [0 1] and cosmetic changes.
    //
    static void convertRGBtoHSV (float red, float green, float blue,
                                 float& H, float& S, float& V)
    {
        // Modified to count and report, but otherwise tolerate, "abnormal" floats.
        const float R = 255 * paper_over_abnormal_values(red);
        const float G = 255 * paper_over_abnormal_values(green);
        const float B = 255 * paper_over_abnormal_values(blue);
        if((B > G) && (B > R)) // Blue Is the dominant color
        {
            V = B; // Value is set as the dominant color
            if(V != 0)
            {
                float min;
                if(R > G) min = G;
                else min = R;
                const float delta = V - min;
                if(delta != 0)
                { S = (delta/V); H = 4 + (R - G) / delta; }
                else
                { S = 0; H = 4 + (R - G); }
                H *=60; if(H < 0) H += 360;
                V =(V/255);
            }
            else
            { S = 0; H = 0;}
        }
        else if(G > R) // Green is the dominant color
        {
            V = G;
            if(V != 0)
            {
                float min;
                if(R > B) min = B;
                else  min = R;
                const float delta = V - min;
                if(delta != 0)
                { S = (delta/V); H = 2 + (B - R) / delta; }
                else
                { S = 0; H = 2 + (B - R); }
                H *=60; if(H < 0) H += 360;
                V =  (V/255);
            }
            else
            { S = 0; H = 0;}
        }
        else // Red is the dominant color
        {
            V = R;
            if(V != 0)
            {
                float min;
                if(G > B) min = B;
                else  min = G;
                const float delta = V - min;
                if(delta != 0)
                { S = (delta/V); H = (G - B) / delta; }
                else
                { S = 0; H = (G - B); }
                H *=60; if(H < 0) H += 360;
                V =  (V/255);
            }
            else
            { S = 0; H = 0;}
        }
        H /= 360.0f;
        
        // Modified to count and report, but otherwise tolerate, "abnormal" floats.
        H = paper_over_abnormal_values(H);
        S = paper_over_abnormal_values(S);
        V = paper_over_abnormal_values(V);
    }
    
    // Transform color space from "Hue Saturation Value" to "Red Green Blue"
    //
    // Method due to Alvy Ray Smith, first published in 1978.
    // See http://en.wikipedia.org/wiki/HSV_color_space
    //
    // Code due to T. Nathan Mundhenk of the USC iLab and generously provided
    // on the web: http://ilab.usc.edu/wiki/index.php/HSV_And_H2SV_Color_Space
    //
    // I made all six parameters (RGBHSV) range on [0 1] and cosmetic changes.
    //
    // I removed some work-arounds dated "20091119", cleaning them up and adding a
    // single numeric validity check at the end.
    //
    static void convertHSVtoRGB(float h, float s, float v,
                                float& R, float& G, float& B)
    {
        const float H = fmod_floor(h, 1) * 360;
        const float S = s;
        const float V = v;
        if (V == 0)
            { R = 0; G = 0; B = 0; }
        else if (S == 0)
            { R = V; G = V; B = V; }
        else
        {
            const float hf = H / 60.0;
            int i = std::max(0, std::min(5, int(std::floor(hf))));
            const float f  = hf - i;
            const float pv  = V * ( 1 - S );
            const float qv  = V * ( 1 - S * f );
            const float tv  = V * ( 1 - S * ( 1 - f ) );
            switch (i)
            {
                case 0: R = V;  G = tv; B = pv; break; // Red is dominant color
                case 1: R = qv; G = V;  B = pv; break; // Green is dominant color
                case 2: R = pv; G = V;  B = tv; break;
                case 3: R = pv; G = qv; B = V;  break; // Blue is dominant color
                case 4: R = tv; G = pv; B = V;  break;
                case 5: R = V;  G = pv; B = qv; break; // Red is dominant color
            }
        }
        Color(R, G, B).assertNormal();
    }
};

// Is distance between RGB vectors no more than epsilon?
inline bool withinEpsilon(Color a, Color b, float epsilon)
{
    return (a - b).length() <= epsilon;
}

// Serialize Color object to stream.
inline std::ostream& operator<<(std::ostream& os, const Color& c)
{
    os << "(" << c.r() << ", " << c.g() << ", " << c.b() << ")";
    return os;
}

// RandomSequence: random color, uniformly distributed across the unit RGB cube.
inline Color RandomSequence::randomUnitRGB()
{
    return Color(frandom01(), frandom01(), frandom01());
}

// Hoist the internal helper class to global scope.
typedef Color::HSV HSV;
