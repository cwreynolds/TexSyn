//
//  Color.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

//    #include "Color.h"
//    #include "Utilities.h"

//    // Set this color to the one described by the given HSV values.
//    void Color::setHSV(float h, float s, float v)
//    {
//        float r, g, b;
//        convertHSVtoRGB(h, s, v, r, g, b);
//        setRGB(r, g, b);
//    }

//    // Get the HSV values for this color. Returned by setting non-const refs.
//    void Color::getHSV(float& h, float& s, float& v) const
//    {
//        convertRGBtoHSV(r(), g(), b(), h, s, v);
//    }

//    // Return a Color made from the given HSV values
//    Color Color::makeHSV(float h, float s, float v)
//    {
//        float r, g, b;
//        convertHSVtoRGB(h, s, v, r, g, b);
//        return Color(r, g, b);
//    }

//    // Get H, S, or V components of this Color.
//    float Color::getH() const
//    {
//        float h, s, v;
//        getHSV(h, s, v);
//        return h;
//    }
//    float Color::getS() const
//    {
//        float h, s, v;
//        getHSV(h, s, v);
//        return s;
//    }
//    float Color::getV() const
//    {
//        float h, s, v;
//        getHSV(h, s, v);
//        return v;
//    }

//    // Get corresponding color value clipped to unit RGB cube.
//    Color Color::clipToUnitRGB() const
//    {
//        Color result = *this;
//        if (length() > 0)
//        {
//            // Individually clip each component to be greater than zero.
//            auto nn = [](float x){ return std::max(0.0f, x); };
//            result = Color(nn(r()), nn(g()), nn(b()));
//            // Clip to red=1 plane, then green and blue.
//            if (result.r() > 1) result = result / result.r();
//            if (result.g() > 1) result = result / result.g();
//            if (result.b() > 1) result = result / result.b();
//        }
//        return result;
//    }

//    // Exponentiate the RGB components by given gamma value ("exponent").
//    // Also clips RGB components to be non-negative before exponentiation.
//    // If any RGB values are so large that they "overflow", returns black.
//    Color Color::gamma(float exponent) const
//    {
//        float er = pow(std::max(r(), 0.0f), exponent);
//        float eg = pow(std::max(g(), 0.0f), exponent);
//        float eb = pow(std::max(b(), 0.0f), exponent);
//        return Color(paper_over_abnormal_values(er),
//                     paper_over_abnormal_values(eg),
//                     paper_over_abnormal_values(eb));
//    }

//    // TODO experimental version in RandomSequence instead of in Color.
//    //
//    // Random color, uniformly distributed across the unit RGB cube.
//    Color RandomSequence::randomUnitRGB()
//    {
//        return Color(frandom01(), frandom01(), frandom01());
//    }

//    // Is distance between RGB vectors no more than epsilon?
//    bool withinEpsilon(Color a, Color b, float epsilon)
//    {
//        return (a - b).length() <= epsilon;
//    }

//    // Serialize Color object to stream.
//    std::ostream& operator<<(std::ostream& os, const Color& c)
//    {
//        os << "(" << c.r() << ", " << c.g() << ", " << c.b() << ")";
//        return os;
//    }
