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
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Convert from HSV to RGB (defined below, after HSV class).
    class HSV;
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
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO still thinking about this API and these names:
    // TODO obsolete? -- should be once _old operators removed.
    // Set this color to the one described by the given HSV values.
    void setHSV(float h, float s, float v);
    // TODO obsolete? -- should be once _old operators removed.
    // Get the HSV values for this color. Returned by setting non-const refs.
    void getHSV(float& h, float& s, float& v) const;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Get the HSV values for a Color. Returned as 3-tuple of floats.
    // TODO needed? Wrote first version of this before the HSV helper class.
    std::tuple<float, float, float> getHSV() const;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO obsolete? -- should be once _old operators removed.
    // Return a Color made from the given HSV values
    static Color makeHSV(float h, float s, float v);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Get H, S, or V components of this Color.
    float getH() const;
    float getS() const;
    float getV() const;
    
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    
    // Inline operators: + - *
    Color operator+(Color v) const
    {
        return Color(r() + v.r(), g() + v.g(), b() + v.b());
    }
    Color operator-(Color v) const
    {
        return Color(r() - v.r(), g() - v.g(), b() - v.b());
    }
    Color operator*(Color c) const  // color * color, aka "tint"
    {
        return Color(r() * c.r(), g() * c.g(), b() * c.b());
    }
    Color operator*(float s) const
    {
        return Color(r() * s, g() * s, b() * s);
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    
    
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
    // Accessors for RGB components (long and short name).
    float red() const { return red_; }
    float green() const { return green_; }
    float blue() const { return blue_; }
    float r() const { return red_; }
    float g() const { return green_; }
    float b() const { return blue_; }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Helper class to represent a color in hue-saturation-value space.
    class HSV
    {
    public:
        // Construct from H, S, and V.  (TODO needed?)
        HSV(float h, float s, float v) : h_(h), s_(s), v_(v) {}
        // Construct from (RGB) Color object
        HSV(Color c) { Color::convertRGBtoHSV(c.r(), c.g(), c.b(), h_, s_, v_); }
        // Accessors for H, S, and V.
        float getH() const { return h_; }
        float getS() const { return s_; }
        float getV() const { return v_; }
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
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
    float red_;
    float green_;
    float blue_;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO remove this "public:" once the Texture::diff() for "_old" removed
public:
    
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
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};

// Is distance between RGB vectors no more than epsilon?
bool withinEpsilon(Color a, Color b, float epsilon);

// Serialize Color object to stream.
std::ostream& operator<<(std::ostream& os, const Color& v);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Hoist the internal helper class to global scope.
typedef Color::HSV HSV;

// Construct an RGB Color from an HSV instance.
inline Color::Color(HSV h)
{
    Color::convertHSVtoRGB(h.getH(), h.getS(), h.getV(), red_, green_, blue_);
}

// Get the HSV values for a Color. Returned as 3-tuple of floats.
// TODO needed? Wrote first version of this before the HSV helper class.
inline std::tuple<float, float, float> Color::getHSV() const
{
    return HSV(*this).getHSV();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
