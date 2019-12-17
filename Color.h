//
//  Color.h
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once

// TODO move elsewhere
// Simple class to represent color as RGB float values on [0, 1]
class Color
{
    // Constructors:
    Color(){};
    Color(float r, float g, float b) : red_(r), green_(r), blue_(b) {}
    Color(const Color& color) : red_(color.r()),
                                green_(color.green()),
                                blue_(color.blue()) {}
    // assignment, set rgb
    /////////////////////////////
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
