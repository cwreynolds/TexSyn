//
//  Color.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Color.h"
#include "Utilities.h"

// Set this color to the one described by the given HSV values.
void Color::setHSV(float h, float s, float v)
{
    float r, g, b;
    convertHSVtoRGB(h, s, v, r, g, b);
    setRGB(r, g, b);
}

// Get the HSV values for this color. Returned by setting non-const refs.
void Color::getHSV(float& h, float& s, float& v) const
{
    convertRGBtoHSV(r(), g(), b(), h, s, v);
}

// Return a Color made from the given HSV values
Color Color::makeHSV(float h, float s, float v)
{
    float r, g, b;
    convertHSVtoRGB(h, s, v, r, g, b);
    return Color(r, g, b);
}

// Get H, S, or V components of this Color.
float Color::getH() const
{
    float h, s, v;
    getHSV(h, s, v);
    return h;
}
float Color::getS() const
{
    float h, s, v;
    getHSV(h, s, v);
    return s;
}
float Color::getV() const
{
    float h, s, v;
    getHSV(h, s, v);
    return v;
}

// Inline operators: + - *
Color Color::operator+(Color v) const
{
    return Color(r() + v.r(), g() + v.g(), b() + v.b());
}
Color Color::operator-(Color v) const
{
    return Color(r() - v.r(), g() - v.g(), b() - v.b());
}
Color Color::operator*(Color c) const
{
    return Color(r() * c.r(), g() * c.g(), b() * c.b());
}
Color Color::operator*(float s) const
{
    return Color(r() * s, g() * s, b() * s);
}

//------------------------------------------------------------------------------
// TODO Dec 18, 2019 -- I copied the code below from the Utilities.h file of the
// 2009 version of TextureSynthesisTest. It needs to be cleaned up.
//------------------------------------------------------------------------------

// for tracking down NAN/INF problems
#define assert_Not_A_Not_A_Number(x) assert(!std::isnan (x))
#define assert_Valid_Float(x) assert(!(std::isnan(x)||std::isinf(x)))

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
void Color::convertRGBtoHSV (float red, float green, float blue,
                             float& H, float& S, float& V)
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    Color(red, green, blue).assertNormal();
    
    if (!Color(red, green, blue).isNormal())
    {
        std::cout << "TODO bad input to Color::convertRGBtoHSV()" << std::endl;
        red = 0;
        green = 0;
        blue = 0;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    const float R = 255.0f * red;
    const float G = 255.0f * green;
    const float B = 255.0f * blue;
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
//    if (!((H >= 0.0) && ( H <= 1.0)))
//        std::cout << "h Value error in Pixel conversion, Value is " << H
//        << " Input r,g,b = " << red << "," << green << "," << blue << std::endl;
    assert_Valid_Float (H);
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
void Color::convertHSVtoRGB(float h, float s, float v,
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

// Get corresponding color value clipped to unit RGB cube.
Color Color::clipToUnitRGB() const
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
// Also clips RGB components to be non-negative before exponentiation, and if
// any RGB values are so large that they "overflow", clips result to white.
Color Color::gamma(float exponent) const
{
    Color exponentiated(pow(std::max(r(), 0.0f), exponent),
                        pow(std::max(g(), 0.0f), exponent),
                        pow(std::max(b(), 0.0f), exponent));
    bool valid = isNormal() && exponentiated.isNormal();
    return valid ? exponentiated : Color(1, 1, 1);
}

// TODO experimental version in RandomSequence instead of in Color.
//
// Random color, uniformly distributed across the unit RGB cube.
Color RandomSequence::randomUnitRGB()
{
    return Color(frandom01(), frandom01(), frandom01());
}

// Is distance between RGB vectors no more than epsilon?
bool withinEpsilon(Color a, Color b, float epsilon)
{
    return (a - b).length() <= epsilon;
}

// Serialize Color object to stream.
std::ostream& operator<<(std::ostream& os, const Color& c)
{
    os << "(" << c.r() << ", " << c.g() << ", " << c.b() << ")";
    return os;
}
