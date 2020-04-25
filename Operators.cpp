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

// Relaxation process that attempts to move an arbitrary collection of Disks
// to have no overlaps, or at least "nearly so". When two Disks overlap they
// are pushed away from each other along the line connecting their centers.
// The whole process is repeated "move_count" times, or until none overlap.
//
// This uses parallel threads and spatial data structures. For consistent
// results, there are two sequential steps, each of which runs in parallel.
// Step one: find overlaps (accelerated by DiskOccupancyGrid) and compute
// Disk's future position. Step two: move Disk and update occupancy grid.
void LotsOfSpotsBase::adjustOverlappingSpots()
{
    // Runs parallel update of all Disks. Parameter is function to generate one
    // such threads, given "first_disk_index" and "disk_count" in vector "spots".
    auto parallelDiskUpdate = [&]
    (std::function<std::thread(int first_index, int count)> thread_maker)
    {
        // TODO I arbitrarily set this to 40 (rendering uses 512.) then noticed
        // it got faster as I reduced it, with minimum at 8. In case that is not
        // a coincidence (my laptop has 8 hyperthreads) I left it as one thread
        // per hardware processor.
        int thread_count = std::thread::hardware_concurrency();
        int disks_per_thread = int(spots.size()) / thread_count;
        // Collection of worker threads.
        std::vector<std::thread> all_threads;
        for (int t = 0; t <= thread_count; t++)
        {
            int first_disk_index = t * disks_per_thread;
            int disk_count = ((t == thread_count)?
                              int(spots.size()) - first_disk_index:
                              disks_per_thread);
            all_threads.push_back(thread_maker(first_disk_index, disk_count));
        }
        // Wait for all row threads to finish.
        for (auto& t : all_threads) t.join();
    };

    // Repeat relaxation process move_count times, or until no overlaps remain.
    for (int i = 0; i < move_count; i++)
    {
        // For each Disk, find nearest overlapping neighbor, compute minimal
        // move to avoid overlap, save that position.
        bool no_move = true;
        parallelDiskUpdate([&]
                           (int first_disk_index, int disk_count)
                           { return std::thread(&LotsOfSpotsBase::
                                                oneThreadAdjustingSpots,
                                                this,
                                                first_disk_index,
                                                disk_count,
                                                i,
                                                std::ref(no_move)); });
        // Exit adjustment loop if no overlapping Disks found.
        if (no_move) break;
        
        // Now move the overlapping Disks, in a thread-safe way, to the
        // future_position computed in the first pass. Each Disk is erased
        // from the grid, moved, then re-inserted into the grid.
        parallelDiskUpdate([&]
                           (int first_disk_index, int disk_count)
                           { return std::thread(&LotsOfSpotsBase::
                                                oneThreadMovingSpots,
                                                this,
                                                first_disk_index,
                                                disk_count); });
    }
}

// Top level for each worker thread moving spots. For "disk_count" Disks
// beginning at "first_disk_index": if the Disk's "future_position" has
// changed, erase it from the grid, update its position, then re-insert it
// back into the grid.
void LotsOfSpotsBase::oneThreadMovingSpots(int first_disk_index, int disk_count)
{
    for (int disk_index = first_disk_index;
         disk_index < (first_disk_index + disk_count);
         disk_index++)
    {
        Disk& disk = spots.at(disk_index);
        if (disk.position != disk.future_position)
        {
            disk_occupancy_grid->eraseDiskWrap(disk);
            disk.position = disk.future_position;
            disk_occupancy_grid->insertDiskWrap(disk);
        }
    }
}

// Top level for each worker thread adjusting spot overlap. For "disk_count"
// Disks beginning at "first_disk_index": look up nearest neighbor, if
// overlap compute new position.
void LotsOfSpotsBase::oneThreadAdjustingSpots(int first_disk_index,
                                              int disk_count,
                                              int move_index,
                                              bool& no_move)
{
    for (int disk_index = first_disk_index;
         disk_index < (first_disk_index + disk_count);
         disk_index++)
    {
        Disk& a = spots.at(disk_index);
        std::set<Disk*> disks_near_a;
        disk_occupancy_grid->findNearbyDisks(a, disks_near_a);
        for (Disk* pointer_to_disk : disks_near_a)
        {
            Disk& b = *pointer_to_disk;
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
                    
                    float f = move_index;
                    float fade = interpolate(f / move_count, 1.0, 0.5);
                    float adjust = (radius_sum - distance) * fade;
                    a.future_position += basis * adjust;
                }
            }
        }
        Vec2 before = a.future_position;
        a.future_position = wrapToCenterTile(a.future_position);
        if (a.future_position != before) no_move = false;
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
