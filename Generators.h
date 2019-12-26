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
    Gradation(Vec2 point_0, Color color_0, Vec2 point_1, Color color_1) :
        color0(color_0),
        color1(color_1),
        origin(point_0),
        distance((point_1 - point_0).length()),
        basis((point_1 - point_0) / distance) {}
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

// A circular spot, centered at a given point, with two radii. Within an inner
// radius it is uniformly one color. Beyond an outer radius it is uniformly
// another color. Between the two radii, there is a sinusoid transition from one
// color to the other. (Reverses radii if they are out of order.)
class Spot : public Texture
{
public:
    Spot(Vec2 center_,
         float inner_radius_, Color inner_color_,
         float outer_radius_, Color outer_color_) :
            center(center_),
            inner_radius(std::min(inner_radius_, outer_radius_)),
            inner_color(inner_color_),
            outer_radius(std::max(inner_radius_, outer_radius_)),
            outer_color(outer_color_)
    {
        assert(inner_radius >= 0);
        assert(outer_radius >= 0);
        assert(outer_radius >= inner_radius);
    }
    Color getColor(Vec2 position) const override
    {
        // Distance from sample position to spot center.
        float d = (position - center).length();
        // Fraction for interpolation: 0 inside, 1 outside, ramp between.
        float f = remapIntervalClip(d, inner_radius, outer_radius, 0, 1);
        // Sinusoidal interpolation between inner and outer colors.
        return interpolate(sinusoid(f), inner_color, outer_color);
    }
private:
    const Vec2 center;
    const float inner_radius;
    const float outer_radius;
    const Color inner_color;
    const Color outer_color;
};
