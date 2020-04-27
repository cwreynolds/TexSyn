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
    // Get the HSV values for this color. Returned by setting non-const refs.
    void getHSV(float& h, float& s, float& v) const;
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
    Color operator*(Color c) const;  // color * color, aka "tint"
    Color operator*(float s) const;
    Color operator/(float s) const { return *this * (1 / s); };
    Color operator+=(const Color& rhs) { return *this = *this + rhs; }
    // Length and normalize in linear RGB space.
    float length() const { return std::sqrt(sq(r()) + sq(g()) + sq(b())); }
    Color normalize() const { return *this / length(); }
    // Get corresponding color value clipped to unit RGB cube.
    Color clipToUnitRGB() const;
    // Exponentiate the RGB components by given gamma value ("exponent")
    Color gamma(float g) const;
    // Random color uniformly distributed across the unit RGB cube.
    static Color randomUnitRGB();
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
    float red_ = 0;
    float green_ = 0;
    float blue_ = 0;
};

// Is distance between RGB vectors less than epsilon?
bool withinEpsilon(Color a, Color b, float epsilon);

// Serialize Color object to stream.
std::ostream& operator<<(std::ostream& os, const Color& v);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <map>
#include <unordered_map>

// TODO experimental cache for Blur at discrete subsample grid cell
// make it spectic to int x,y keys and Color values. Maybe generalize later.

// Try timing tests with std::unordered_map instead of std::map

class ColorGridCache
{
public:
    typedef std::pair<std::pair<int, int>, Color> ColorGrid;
//    ColorGridCache(){};
    // TODO new March 16
    ColorGridCache(){ cache_.reserve(511 * 511); };

    // TODO for testing, keep?
    bool cached(int x, int y)
    {
        auto find_result = cache_.find(std::make_pair(x, y));
        return find_result != cache_.end();
    }
    
    size_t size() { return cache_.size(); }
    void insert(int x, int y, Color color)
    {
        cache_[std::make_pair(x, y)] = color;
    }
    Color lookup(int x, int y, bool& present_in_cache)
    {
        Color result(0, 0, 0);
        present_in_cache = false;
        auto find_result = cache_.find(std::make_pair(x, y));
        if (find_result != cache_.end())
        {
            present_in_cache = true;
            result = find_result->second;
        }
        return result;
    }
private:
    // TODO map (red-black tree) version
    // std::map<std::pair<int, int>, Color> cache_;
    
    // TODO hash table version
    struct pair_hash
    {
        template <class T1, class T2>
        std::size_t operator() (const std::pair<T1, T2> &pair) const
        {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };
    std::unordered_map<std::pair<int, int>, Color, pair_hash> cache_;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
