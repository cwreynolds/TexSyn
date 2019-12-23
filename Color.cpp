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
    convertRGBtoHSV(r(), g(), b(), h, s, v);
    return h;
}
float Color::getS() const
{
    float h, s, v;
    convertRGBtoHSV(r(), g(), b(), h, s, v);
    return s;
}
float Color::getV() const
{
    float h, s, v;
    convertRGBtoHSV(r(), g(), b(), h, s, v);
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

// using the basic x-floor(x) for very small negative value produces 1 rather than 0
//
// XXX20091121 experiment
// a = -1.98682e-08
// floor(a) = -1
// a-floor(a) = 1
//
inline float wellBehavedFractionalPart (float x)
{
    x = x - floor(x);
    return x - floor(x);
}


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
//    inline void convertRGBtoHSV (const float red, const float green, const float blue,
//                                 float& H, float& S, float& V)
void Color::convertRGBtoHSV (float red, float green, float blue,
                             float& H, float& S, float& V)
{
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
// this is too big to be "inline" it should be defined in a .cpp file!
//
//    inline void convertHSVtoRGB (const float h, const float s, const float v,
//                                 float& R, float& G, float& B)
void Color::convertHSVtoRGB(float h, float s, float v,
                            float& R, float& G, float& B)
{
    // XXX20091119 oops found a bug for HSV=111 (instead wrap H around to 0)
    //    const float H = h * 360.0f;
    // XXX20091121 now I got a bug here because an H slightly above 1 was pass in from BrightnessToHue
    //    const float H = (h == 1.0f) ? 0.0f : h * 360.0f;
    //    const float H = (h - floor (h)) * 360.0f; // XXX20091121 experiment
    const float H = wellBehavedFractionalPart(h) * 360.0f; // XXX20091121 experiment
    const float S = s;
    const float V = v;
    if( V == 0 )
    { R = 0; G = 0; B = 0; }
    else if( S == 0 )
    { R = V; G = V; B = V; }
    else
    {
        const float hf = H / 60.0;
        const int i  = (int) floor( hf );
        
        // XXX20091119 oops found a bug for HSV=111  (remove this when I'm sure its fixed)
        if ((i<0) || (i>5))
            std::cout<<"convertHSVtoRGB: i="<< i<<", h,s,v = "<<h<<","<<s<<","<< v<<std::endl;
        assert (!((i<0) || (i>5)));
        
        const float f  = hf - i;
        const float pv  = V * ( 1 - S );
        const float qv  = V * ( 1 - S * f );
        const float tv  = V * ( 1 - S * ( 1 - f ) );
        switch( i )
        {
            case 0: R = V;  G = tv; B = pv; break; // Red is the dominant color
            case 1: R = qv; G = V;  B = pv; break; // Green is the dominant color
            case 2: R = pv; G = V;  B = tv; break;
            case 3: R = pv; G = qv; B = V;  break; // Blue is the dominant color
            case 4: R = tv; G = pv; B = V;  break;
            case 5: R = V;  G = pv; B = qv; break; // Red is the dominant color
                // Just in case we overshoot on our math by a little, we putthese here.
                // Since its a switch it won't slow us down at all to put these here.
            case 6: R = V;  G = tv; B = pv; break;
            case -1: R = V; G = pv; B = qv; break;
//            default:
//                // The color is not defined, we should throw an error.
// //            std::cout << "i Value error in Pixel conversion, Value is " << i << std::endl;
//                std::cout << "i Value error in Pixel conversion, Value is " << i
//                          << " Input h,s,v = " << h << "," << s << "," << v << std::endl; // XXX20090913
//                exit (-1);
//                break;
        }
    }
}

// Is distance between RGB vectors less than epsilon?
bool withinEpsilon(Color a, Color b, float epsilon)
{
    return (a - b).length() < epsilon;
}
