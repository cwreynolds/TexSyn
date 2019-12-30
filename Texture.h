//
//  Texture.h
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Vec2.h"
#include "Color.h"
#include "Utilities.h"

// TODO does it make sense to call it a “pixel” since they need not be arranged
// in a regular rectangular grid? Possibly a ColorSample or ColorPoint (or
// Sample or Point)?

class AbstractTexture
{
public:
    AbstractTexture(){}
    virtual Color getColor(Vec2 position) const = 0;
};

//    // TODO not sure if I want to use this (may not be worth the effort) so
//    // make it a stub for now:
//    class ColorSampleCache
//    {
//    };

class Texture : public AbstractTexture
{
public:
    // Default constructor.
    Texture(){}
    // Display this Texture in a pop-up OpenCV window.
    void displayInWindow(int size = 512) const;
};
