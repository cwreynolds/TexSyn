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

class Texture : public AbstractTexture
{
public:
    // Default constructor.
    Texture(){}
    // Get color at position, clipping to unit RGB color cube.
    Color getColorClipped(Vec2 p) const { return getColor(p).clipToUnitRGB(); }
    // Display this Texture in a pop-up OpenCV window.
    void displayInWindow(int size = 511) const;
    // Reset statistics for debugging.
    void resetStatistics() const;
    // Collect statistics for debugging.
    void collectStatistics(Vec2 position, Color color) const;
private:
    // TODO maybe we need a OOBB Bounds2d class?
    // TODO maybe should be stored in external std::map keyed on Texture pointer
    // Store bounds of sampled positions.
    static float min_x;
    static float max_x;
    static float min_y;
    static float max_y;
};
