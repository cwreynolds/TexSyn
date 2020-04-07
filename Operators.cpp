//
//  Operators.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/31/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//
// The bulk of Operator classes are defined in Operators.h, a few method are
// defined here "out of line". Some for order of definition reasons, some
// because they seemed to long to be inline in the .h file, and storage for
// some static data members.

#include "Operators.h"

void Texture::diff(const Texture& t0, const Texture& t1)
{
    AbsDiff abs_diff(t0, t1);
    int pixel_count = 0;
    Color total_color(0, 0, 0);
    int size = 511;
    Texture::rasterizeDisk(size,
                           [&](int i, int j, Vec2 position)
                           {
                               Color diff = abs_diff.getColor(position);
                               total_color += diff;
                               pixel_count++;
                           });
    debugPrint(pixel_count);
    debugPrint(total_color);
    debugPrint(total_color / pixel_count);
    Texture::displayInWindow({ &t0, &t1, &abs_diff }, 451);
}

// Each Blur::getColor() uses an NxN jiggled grid of subsamples, where N is:
int Blur::sqrt_of_subsample_count = 11;

// Insert random spots until density threshold is met. Positions are
// uniformly distributed across center tile. Radii are chosen from interval
// [min_radius, max_radius] with a preference for smaller values.
void LotsOfSpotsBase::insertRandomSpots()
{
    float total_area = 0;
    float half = tile_size / 2;
    // Seed the random number sequence from some operator parameters.
    RandomSequence rs(hash_float(spot_density) ^
                      hash_float(min_radius) ^
                      hash_float(max_radius) ^
                      hash_float(soft_edge_width));
    while (total_area < (spot_density * sq(tile_size)))
    {
        // Select radius, preferring the low end of the range.
        float k = rs.frandom01();
        float i = (std::pow(k, 10) + (k / 2)) / 1.5;
        float radius = interpolate(i, min_radius, max_radius);
        Vec2 center(rs.frandom2(-half, half), rs.frandom2(-half, half));
        spots.push_back(Dot(radius, center));
        total_area += spots.back().area();
    }
    debugPrint(spots.size());
}

// Considers all pairs of spots (so O(n²)). When two overlap they are pushed
// away from each other along the line connecting their centers. The whole
// process is repeated "move_count" times, or until no spots overlap.
void LotsOfSpotsBase::adjustOverlappingSpots()
{
    // Move spots away from regions of overlap, repeat move_count times.
    for (int i = 0; i < move_count; i++)
    {
        debugPrint(i);
        bool no_move = true;
        for (auto& a : spots)
        {
            for (auto& b : spots)
            {
                if (&a != &b)  // Ignore self overlap.
                {
                    Vec2 b_tile = nearestByTiling(a.position, b.position);
                    Vec2 offset = a.position - b_tile;
                    float distance = offset.length();
                    float radius_sum = a.radius + b.radius;
                    if (distance < radius_sum)
                    {
                        no_move = false;
                        Vec2 basis = offset / distance;
                        float f = i;
                        float fade = interpolate(f / move_count, 1.0, 0.5);
                        float adjust = (radius_sum - distance) * fade;
                        a.position += basis * adjust;
                        b.position += basis * -adjust;
                    }
                }
            }
            // Wrap "a"s position inside tile, clear "no_move" if was outside.
            Vec2 before = a.position;
            a.position = wrapToCenterTile(a.position);
            if (a.position != before) no_move = false;
        }
        if (no_move) break;
    }
}

// Given a reference point (say to be rendered), and the center of a Spot,
// adjust "spot_center" with regard to tiling, to be the nearest (perhaps in
// another tile) to "reference_point".
Vec2 LotsOfSpotsBase::nearestByTiling(Vec2 reference_point, Vec2 spot_center) const
{
    Vec2 nearest_point;
    float nearest_distance = std::numeric_limits<float>::infinity();
    for (float x : {-tile_size, 0.0f, tile_size})
    {
        for (float y : {-tile_size, 0.0f, tile_size})
        {
            Vec2 tiled = spot_center + Vec2(x, y);
            float d = (reference_point - tiled).lengthSquared();
            if (nearest_distance > d)
            {
                nearest_distance = d;
                nearest_point = tiled;
            }
        }
    }
    return nearest_point;
};

// Given a position, find corresponding point on center tile, via fmod/wrap.
Vec2 LotsOfSpotsBase::wrapToCenterTile(Vec2 v) const
{
    float half = tile_size / 2;
    return Vec2(fmod_floor(v.x() + half, tile_size) - half,
                fmod_floor(v.y() + half, tile_size) - half);
}
