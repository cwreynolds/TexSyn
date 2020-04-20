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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <thread>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef THREADS_FOR_ADJUST
    for (Disk& spot : spots) spot.future_position = spot.position;
#else // THREADS_FOR_ADJUST
#endif // THREADS_FOR_ADJUST
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

//    // Considers all pairs of spots (so O(n²)). When two overlap they are pushed
//    // away from each other along the line connecting their centers. The whole
//    // process is repeated "move_count" times, or until no spots overlap.
//    void LotsOfSpotsBase::adjustOverlappingSpots()
//    {
//    #ifdef USE_DOG_FOR_ADJUST
//        // Move spots away from regions of overlap, repeat move_count times.
//        for (int i = 0; i < move_count; i++)
//        {
//            debugPrint(i);
//            bool no_move = true;
//            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    #ifdef THREADS_FOR_ADJUST
//            for (int disk_index = 0; disk_index < spots.size(); disk_index++)
//    #else // THREADS_FOR_ADJUST
//            for (auto& a : spots)
//    #endif // THREADS_FOR_ADJUST
//            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//            {
//                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    #ifdef THREADS_FOR_ADJUST
//                Disk& a = spots.at(disk_index);
//    #else // THREADS_FOR_ADJUST
//    #endif // THREADS_FOR_ADJUST
//                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//                //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//                std::set<Disk*> disks_near_a;
//    //            disk_occupancy_grid->findNearbyDisks(a.position, disks_near_a);
//                disk_occupancy_grid->findNearbyDisks(a, disks_near_a);
//                for (Disk* pointer_to_disk : disks_near_a)
//                {
//                    Disk& b = *pointer_to_disk;
//                //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//                    if (&a != &b)  // Ignore self overlap.
//                    {
//                        Vec2 b_tile = nearestByTiling(a.position, b.position);
//                        Vec2 offset = a.position - b_tile;
//                        float distance = offset.length();
//                        float radius_sum = a.radius + b.radius;
//                        if (distance < radius_sum)
//                        {
//                            no_move = false;
//                            Vec2 basis = offset / distance;
//                            float f = i;
//                            float fade = interpolate(f / move_count, 1.0, 0.5);
//                            float adjust = (radius_sum - distance) * fade;
//
//                            //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//    //                        a.position += basis * adjust;
//    //                        b.position += basis * -adjust;
//
//    #ifdef THREADS_FOR_ADJUST
//                            a.future_position += basis * adjust;
//    #else // THREADS_FOR_ADJUST
//                            disk_occupancy_grid->eraseDiskWrap(a);
//                            disk_occupancy_grid->eraseDiskWrap(b);
//                            a.position += basis * adjust;
//                            b.position += basis * -adjust;
//                            disk_occupancy_grid->insertDiskWrap(a);
//                            disk_occupancy_grid->insertDiskWrap(b);
//    #endif // THREADS_FOR_ADJUST
//                            //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//                        }
//                    }
//                }
//                //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//    //            // Wrap "a"s position inside tile, clear "no_move" if was outside.
//    //            Vec2 before = a.position;
//    //            a.position = wrapToCenterTile(a.position);
//    //            if (a.position != before) no_move = false;
//
//    #ifdef THREADS_FOR_ADJUST
//                Vec2 before = a.future_position;
//                a.future_position = wrapToCenterTile(a.future_position);
//                if (a.future_position != before) no_move = false;
//    #else // THREADS_FOR_ADJUST
//                // If "a" is outside the central tile, wrap it in, clear "no_move".
//                Vec2 wrapped_position = wrapToCenterTile(a.position);
//                if (a.position != wrapped_position)
//                {
//                    disk_occupancy_grid->eraseDiskWrap(a);
//                    a.position = wrapped_position;
//                    disk_occupancy_grid->insertDiskWrap(a);
//                    no_move = false;
//                }
//    #endif // THREADS_FOR_ADJUST
//
//
//                //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//            }
//            if (no_move) break;
//
//    #ifdef THREADS_FOR_ADJUST
//            for (auto& disk : spots)
//            {
//                if (disk.position != disk.future_position)
//                {
//                    disk_occupancy_grid->eraseDiskWrap(disk);
//                    disk.position = disk.future_position;
//                    disk_occupancy_grid->insertDiskWrap(disk);
//                }
//            }
//    #else // THREADS_FOR_ADJUST
//    #endif // THREADS_FOR_ADJUST
//        }
//
//    #else // USE_DOG_FOR_ADJUST
//
//        // Move spots away from regions of overlap, repeat move_count times.
//        for (int i = 0; i < move_count; i++)
//        {
//    //        debugPrint(i);
//            bool no_move = true;
//            for (auto& a : spots)
//            {
//                for (auto& b : spots)
//                {
//                    if (&a != &b)  // Ignore self overlap.
//                    {
//                        Vec2 b_tile = nearestByTiling(a.position, b.position);
//                        Vec2 offset = a.position - b_tile;
//                        float distance = offset.length();
//                        float radius_sum = a.radius + b.radius;
//                        if (distance < radius_sum)
//                        {
//                            no_move = false;
//                            Vec2 basis = offset / distance;
//                            float f = i;
//                            float fade = interpolate(f / move_count, 1.0, 0.5);
//                            float adjust = (radius_sum - distance) * fade;
//                            a.position += basis * adjust;
//                            b.position += basis * -adjust;
//                        }
//                    }
//                }
//                // Wrap "a"s position inside tile, clear "no_move" if was outside.
//                Vec2 before = a.position;
//                a.position = wrapToCenterTile(a.position);
//                if (a.position != before) no_move = false;
//            }
//            if (no_move) break;
//        }
//
//    #endif // USE_DOG_FOR_ADJUST
//
//    }

// TODO April 19, prototyping the multi threaded version

//    // Considers all pairs of spots (so O(n²)). When two overlap they are pushed
//    // away from each other along the line connecting their centers. The whole
//    // process is repeated "move_count" times, or until no spots overlap.
//    void LotsOfSpotsBase::adjustOverlappingSpots()
//    {
//    #ifdef USE_DOG_FOR_ADJUST
//        // Move spots away from regions of overlap, repeat move_count times.
//        for (int i = 0; i < move_count; i++)
//        {
//            debugPrint(i);
//            bool no_move = true;
//            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    //#ifdef THREADS_FOR_ADJUST
//            // TODO the multithreaded version QQQ
//            // later...
//            //const int max_threads = 100;
//            //const int min_disks_per_thread = 100;
//            // for rigth now...
//            int thread_count = 40;
//            int disks_per_thread = int(spots.size()) / thread_count;
//
//            // Collection of all row threads. (Use clear() to remove initial threads,
//            // see https://stackoverflow.com/a/38130584/1991373 )
//            std::vector<std::thread> all_threads(thread_count);
//            all_threads.clear();
//
//            // Launch "thread_count" threads, each working on a given range of
//            // "disks_per_thread" Disks.
//            for (int t = 0; t <= thread_count; t++)
//            {
//
//                int first_disk_index = t * disks_per_thread;
//                int disk_count = ((t = thread_count)?
//                                  int(spots.size()) - first_disk_index:
//                                  disks_per_thread);
//                all_threads.push_back(std::thread(&LotsOfSpotsBase::
//                                                  oneThreadAdjustingSpots,
//                                                  this,
//                                                  first_disk_index,
//                                                  disk_count,
//                                                  i,
//                                                  std::ref(no_move)));
//            }
//            // Wait for all row threads to finish.
//            for (auto& t : all_threads) t.join();
//
//
//    //#else // THREADS_FOR_ADJUST
//
//    //        // TODO the April 18 version 60x60 grid, does 2.76 on LOS benchmark
//    //        for (auto& a : spots)
//    //        {
//    //            std::set<Disk*> disks_near_a;
//    //            disk_occupancy_grid->findNearbyDisks(a, disks_near_a);
//    //            for (Disk* pointer_to_disk : disks_near_a)
//    //            {
//    //                Disk& b = *pointer_to_disk;
//    //                if (&a != &b)  // Ignore self overlap.
//    //                {
//    //                    Vec2 b_tile = nearestByTiling(a.position, b.position);
//    //                    Vec2 offset = a.position - b_tile;
//    //                    float distance = offset.length();
//    //                    float radius_sum = a.radius + b.radius;
//    //                    if (distance < radius_sum)
//    //                    {
//    //                        no_move = false;
//    //                        Vec2 basis = offset / distance;
//    //                        float f = i;
//    //                        float fade = interpolate(f / move_count, 1.0, 0.5);
//    //                        float adjust = (radius_sum - distance) * fade;
//    //
//    //                        disk_occupancy_grid->eraseDiskWrap(a);
//    //                        disk_occupancy_grid->eraseDiskWrap(b);
//    //                        a.position += basis * adjust;
//    //                        b.position += basis * -adjust;
//    //                        disk_occupancy_grid->insertDiskWrap(a);
//    //                        disk_occupancy_grid->insertDiskWrap(b);
//    //                    }
//    //                }
//    //            }
//    //            // If "a" is outside the central tile, wrap it in, clear "no_move".
//    //            Vec2 wrapped_position = wrapToCenterTile(a.position);
//    //            if (a.position != wrapped_position)
//    //            {
//    //                disk_occupancy_grid->eraseDiskWrap(a);
//    //                a.position = wrapped_position;
//    //                disk_occupancy_grid->insertDiskWrap(a);
//    //                no_move = false;
//    //            }
//    //#endif // THREADS_FOR_ADJUST
//            }
//            if (no_move) break;
//
//    #ifdef THREADS_FOR_ADJUST
//            for (auto& disk : spots)
//            {
//                if (disk.position != disk.future_position)
//                {
//                    disk_occupancy_grid->eraseDiskWrap(disk);
//                    disk.position = disk.future_position;
//                    disk_occupancy_grid->insertDiskWrap(disk);
//                }
//            }
//    #else // THREADS_FOR_ADJUST
//    #endif // THREADS_FOR_ADJUST
//        }
//
//    #else // USE_DOG_FOR_ADJUST
//
//        // Move spots away from regions of overlap, repeat move_count times.
//        for (int i = 0; i < move_count; i++)
//        {
//    //        debugPrint(i);
//            bool no_move = true;
//            for (auto& a : spots)
//            {
//                for (auto& b : spots)
//                {
//                    if (&a != &b)  // Ignore self overlap.
//                    {
//                        Vec2 b_tile = nearestByTiling(a.position, b.position);
//                        Vec2 offset = a.position - b_tile;
//                        float distance = offset.length();
//                        float radius_sum = a.radius + b.radius;
//                        if (distance < radius_sum)
//                        {
//                            no_move = false;
//                            Vec2 basis = offset / distance;
//                            float f = i;
//                            float fade = interpolate(f / move_count, 1.0, 0.5);
//                            float adjust = (radius_sum - distance) * fade;
//                            a.position += basis * adjust;
//                            b.position += basis * -adjust;
//                        }
//                    }
//                }
//                // Wrap "a"s position inside tile, clear "no_move" if was outside.
//                Vec2 before = a.position;
//                a.position = wrapToCenterTile(a.position);
//                if (a.position != before) no_move = false;
//            }
//            if (no_move) break;
//        }
//
//    #endif // USE_DOG_FOR_ADJUST
//
//    }


// Considers all pairs of spots (so O(n²)). When two overlap they are pushed
// away from each other along the line connecting their centers. The whole
// process is repeated "move_count" times, or until no spots overlap.
void LotsOfSpotsBase::adjustOverlappingSpots()
{
    // Move spots away from regions of overlap, repeat move_count times.
    for (int i = 0; i < move_count; i++)
    {
//        debugPrint(i);
        bool no_move = true;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO the multithreaded version QQQ
        // later...
        //const int max_threads = 100;
        //const int min_disks_per_thread = 100;
        // for rigth now...
        int thread_count = 40;
        int disks_per_thread = int(spots.size()) / thread_count;
        
        // Collection of all row threads. (Use clear() to remove initial threads,
        // see https://stackoverflow.com/a/38130584/1991373 )
        std::vector<std::thread> all_threads(thread_count);
        all_threads.clear();

        // Launch "thread_count" threads, each working on a given range of
        // "disks_per_thread" Disks.
        for (int t = 0; t <= thread_count; t++)
        {
            
            int first_disk_index = t * disks_per_thread;
            int disk_count = ((t == thread_count)?
                              int(spots.size()) - first_disk_index:
                              disks_per_thread);
            all_threads.push_back(std::thread(&LotsOfSpotsBase::
                                              oneThreadAdjustingSpots,
                                              this,
                                              first_disk_index,
                                              disk_count,
                                              i,
                                              std::ref(no_move)));
        }
        // Wait for all row threads to finish.
        for (auto& t : all_threads) t.join();
        
        if (no_move) break;
        
        for (auto& disk : spots)
        {
            if (disk.position != disk.future_position)
            {
                disk_occupancy_grid->eraseDiskWrap(disk);
                disk.position = disk.future_position;
                disk_occupancy_grid->insertDiskWrap(disk);
            }
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//std::mutex print_mutex;



// Top level for each worker thread adjusting spot overlap.
void LotsOfSpotsBase::oneThreadAdjustingSpots(int first_disk_index,
                                              int disk_count,
                                              int move_index,
                                              bool& no_move)
{
//    {
//        // Wait to grab lock for access to image. (Lock released at end of block)
//        const std::lock_guard<std::mutex> lock(print_mutex);
//        std::cout << "in oneThreadAdjustingSpots " << first_disk_index
//                  << std::endl;
//    }
//    for (int disk_index = 0; disk_index < spots.size(); disk_index++)
    
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
//                    float fade = 1;
//                    float f = move_index;
//                    float fade = interpolate(f / move_count, 1.0, 0.1);
                    
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
