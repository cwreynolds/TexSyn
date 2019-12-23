//
//  Generators.h
//  texsyn
//
//  Created by Craig Reynolds on 12/21/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Texture.h"

// Color gradation texture with two colors and arbitrary position, width, and
// orientation. The arguments are two points, defining a line segment, and a
// color for each end. The gradation occurs alone the line segment, a given
// location on the texture is projected onto that line to determine its color.
// (See discussion at http://www.red3d.com/cwr/texsyn/diary.html#20090329)
class Gradation : public Texture
{
public:
    Gradation(Vec2 p0, Color c0, Vec2 p1, Color c1) :
        color0(c0),
        color1(c1),
        origin(p0),
        distance((p1 - p0).length()),
        basis((p1 - p0) / distance) {}
    Color getColor(Vec2 position) const override
    {
        if (distance == 0)
        {
            return interpolate(0.5, color0, color1);
        }
        else
        {
            Vec2 offset = position - origin;
            float projection = basis.dot(offset);
            float relative = remapIntervalClip(projection, 0, distance, 0, 1);
            return interpolate(sinusoid(relative), color0, color1);
        }
    }
private:
    const Vec2 origin;
    const float distance;
    const Vec2 basis;
    const Color color0;
    const Color color1;
};
