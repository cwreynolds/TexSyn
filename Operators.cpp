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
#include <thread>

// Compare textures, print stats, optional file, display inputs and AbsDiff.
void Texture::diff(const Texture& t0,
                   const Texture& t1,
                   std::string pathname,
                   int size,
                   bool binary)
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    AbsDiff abs_diff(t0, t1);
    int pixel_count = 0;
//    Color total_color(0, 0, 0);
    Color total_color_diff(0, 0, 0);
    int mismatch_count = 0;
    Texture::rasterizeDisk(size,
                           [&](int i, int j, Vec2 position)
                           {
                               Color diff = abs_diff.getColor(position);
//                               total_color += diff;
                               total_color_diff += diff;
                               pixel_count++;
                               if (diff != Color()) mismatch_count++;
                           });
    debugPrint(pixel_count);
//    debugPrint(total_color);
//    debugPrint(total_color / pixel_count);
    debugPrint(total_color_diff);
    debugPrint(total_color_diff / pixel_count);
    debugPrint(mismatch_count);
    NotEqual not_equal(t0, t1);
    const Texture* compare = (binary ?
                              (Texture*)(&not_equal) :
                              (Texture*)(&abs_diff));
    Texture::displayAndFile3(t0, t1, *compare, pathname, size);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

// BACKWARD_COMPATIBILITY reference to new "disposable" Uniform object. This
// is called ONLY from constructors providing backward compatibility. The
// tiny Uniform texture object is allowed to "memory leak" for ease of use.
Texture& Texture::disposableUniform(Color color)
{
    std::cout << "Warning: creating disposableUniform" << color
              << " for backward compatibility." << std::endl;
    return *(new Uniform(color));
}

// Insert random Disks until density threshold is met. Disk center positions
// are uniformly distributed across center tile. Radii are chosen from the
// interval [min_radius, max_radius] with a preference for smaller values.
void LotsOfSpotsBase::insertRandomSpots()
{
    float total_area = 0;
    float half = tile_size / 2;
    // Seed the random number sequence from some operator parameters.
    RandomSequence rs(seedForRandomSequence());
    // Add random Disks until density threshold is met.
    while (total_area < (spot_density * sq(tile_size)))
    {
        // Select radius, preferring the low end of the range.
        float k = rs.frandom01();
        float i = (std::pow(k, 10) + (k / 2)) / 1.5;
        float radius = interpolate(i, min_radius, max_radius);
        Vec2 center(rs.frandom2(-half, half), rs.frandom2(-half, half));
        spots.push_back(Disk(radius, center));
        total_area += spots.back().area();
        if (spots.size() >= max_spots_allowed) break;
    }
    // Insert each new random Disk into the DiskOccupancyGrid.
    // (NB: very important this happens AFTER all Disks added to std::vector
    // spots (above). Otherwise pointers will be invalidated by reallocation.)
    for (Disk& spot : spots) disk_occupancy_grid->insertDiskWrap(spot);
}

void LotsOfSpotsBase::randomizeSpotRotations()
{
    // Seed the random number sequence from some operator parameters.
    RandomSequence rs(seedForRandomSequence());
    // Each spot gets a random rotation on [0, 2π]
    for (auto& spot : spots) spot.angle = rs.frandom01() * pi * 2;
}

// Seed the random number sequence from some operator parameters.
size_t LotsOfSpotsBase::seedForRandomSequence()
{
    return (hash_float(spot_density) ^
            hash_float(min_radius) ^
            hash_float(max_radius) ^
            hash_float(soft_edge_width));
}
