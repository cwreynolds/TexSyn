//
//  Texture.h
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
    Color(){};
    Color(const Color& color)
        : red_(color.r()), green_(color.g()), blue_(color.b()) {}
    
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

class AbstractTexture
{
    AbstractTexture(){}
    virtual Color pixel(float x, float y) = 0;
};
