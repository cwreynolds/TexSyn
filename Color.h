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
    Color(){};
    Color(float r, float g, float b) : red_(r), green_(g), blue_(b) {}
    Color(const Color& color) : red_(color.r()),
                                green_(color.green()),
                                blue_(color.blue()) {}
    // assignment, set RGB components
    Color operator=(const Color& c)
    {
        setRGB(c.r(), c.g(), c.b());
        return *this;
    }
    void setRGB(float r, float g, float b) { red_ = r; green_ = g; blue_ = b; }
    // Equality:
    bool operator==(const Color& c) const
    {
        return ((r() == c.r()) && (g() == c.g()) && (b() == c.b()));
    }
    // Luma (relative luminance?) see https://en.wikipedia.org/wiki/Luma_(video)
    float luminance() const {return 0.2126 * r() + 0.7152 * g() + 0.0722 * b();}
    // TODO still thinking about this API and these names:
    // Set this color to the one described by the given HSV values.
    void setHSV(float h, float s, float v);
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
    // Inline operators: + - *
    Color operator+(Color v) const;
    Color operator-(Color v) const;
    Color operator*(float s) const;
    // Length in linear unit RGB space.
    float length() const { return std::sqrt(sq(r()) + sq(g()) + sq(b())); }
    // Accessors:
    float red() const { return red_; }
    float green() const { return green_; }
    float blue() const { return blue_; }
    // TODO Are these short names a good idea?
    float r() const { return red_; }
    float g() const { return green_; }
    float b() const { return blue_; }
    
private:
    float red_ = 0;
    float green_ = 0;
    float blue_ = 0;
};

// Is distance between RGB vectors less than epsilon?
bool withinEpsilon(Color a, Color b, float epsilon);
