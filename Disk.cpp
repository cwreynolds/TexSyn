//
//  Disk.cpp
//  texsyn
//
//  Defines utility classes Disk and DiskOccupancyGrid.
//
//  Created by Craig Reynolds on 4/23/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#include "Disk.h"
#include "Texture.h"

// Relaxation process that attempts to reduce the overlaps in an arbitrary
// collection of Disks. Overlapping Disks are pushed away from each other
// along the line connecting their centers. The whole process is repeated
// "retries" times, or until none overlap.
//
// This uses parallel threads and spatial data structures. For consistent
// results, the work is broken into two sequential steps, each of which runs
// in parallel. Step one: find overlaps and compute Disk's future position.
// Step two: move Disk and update occupancy grid.
void DiskOccupancyGrid::reduceDiskOverlap(int retries,
                                          float move_scale,
                                          std::vector<Disk>& disks)
{
    // Repeat relaxation process "retries" times, or until no overlaps remain.
    for (int i = 0; i < retries; i++)
    {
        // For each Disk, find nearest overlapping neighbor, compute minimal
        // move to avoid overlap, save that position.
        bool no_move = true;
        parallelDiskUpdate(disks,
                           [&](int first_disk_index, int disk_count)
                           { return std::thread(&DiskOccupancyGrid::
                                                oneThreadAdjustingSpots,
                                                this,
                                                first_disk_index,
                                                disk_count,
                                                float(i) / retries,
                                                move_scale,
                                                std::ref(no_move),
                                                std::ref(disks)); });

        // Exit adjustment loop if no overlapping Disks found.
        if (no_move) break;
        
        // Now move the overlapping Disks, in a thread-safe way, to the
        // future_position computed in the first pass. Each Disk is erased
        // from the grid, moved, then re-inserted into the grid.
        parallelDiskUpdate(disks,
                           [&](int first_disk_index, int disk_count)
                           { return std::thread(&DiskOccupancyGrid::
                                                oneThreadMovingSpots,
                                                this,
                                                first_disk_index,
                                                disk_count,
                                                std::ref(disks)); });
        yield();  // To better support multi-threading.
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    // Runs parallel update of all Disks. Parameter is function to generate one
//    // thread, given "first_disk_index" and "disk_count" in vector "disks". Each
//    // thread updates this given block of disks, in parallel with other threads.
//    void DiskOccupancyGrid::parallelDiskUpdate(std::vector<Disk>& disks,
//                                               std::function<std::thread(int, int)>
//                                                   thread_maker)
//    {
//        // TODO I arbitrarily set this to 40 (rendering uses 512.) then noticed
//        // it got faster as I reduced it, with minimum at 8. In case that is not
//        // a coincidence (my laptop has 8 hyperthreads) I left it as one thread
//        // per hardware processor.
//        int thread_count = std::thread::hardware_concurrency();
//        int disks_per_thread = int(disks.size()) / thread_count;
//        // Collection of worker threads.
//        std::vector<std::thread> all_threads;
//        for (int t = 0; t <= thread_count; t++)
//        {
//            int first_disk_index = t * disks_per_thread;
//            int disk_count = ((t == thread_count)?
//                              int(disks.size()) - first_disk_index:
//                              disks_per_thread);
//            all_threads.push_back(thread_maker(first_disk_index, disk_count));
//        }
//        // Wait for all row threads to finish.
//        for (auto& t : all_threads) t.join();
//    }

//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// TODO very temporary hack: a global variable to hold a global value.
int threads_finished = 0;
std::mutex threads_finished_mutex;
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~


// Runs parallel update of all Disks. Parameter is function to generate one
// thread, given "first_disk_index" and "disk_count" in vector "disks". Each
// thread updates this given block of disks, in parallel with other threads.
void DiskOccupancyGrid::parallelDiskUpdate(std::vector<Disk>& disks,
                                           std::function<std::thread(int, int)>
                                               thread_maker)
{
    // TODO I arbitrarily set this to 40 (rendering uses 512.) then noticed
    // it got faster as I reduced it, with minimum at 8. In case that is not
    // a coincidence (my laptop has 8 hyperthreads) I left it as one thread
    // per hardware processor.
    int thread_count = std::thread::hardware_concurrency();
    int disks_per_thread = int(disks.size()) / thread_count;
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    threads_finished = 0;
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // Collection of worker threads.
    std::vector<std::thread> all_threads;
    for (int t = 0; t <= thread_count; t++)
    {
        int first_disk_index = t * disks_per_thread;
        int disk_count = ((t == thread_count)?
                          int(disks.size()) - first_disk_index:
                          disks_per_thread);
        all_threads.push_back(thread_maker(first_disk_index, disk_count));
    }
    
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // TODO Aug 13, turning this off for the moment -- I think I need to have a
    //      “global” clock to make sure we don't sleep in each of the (eg) 200
    //      calls since they might take very little time
    
    int TEMP_COUNTER = 0;
    
    while (threads_finished < thread_count)
    {
        Texture::checkForUserInput();
        
        if (TEMP_COUNTER++ > 100000)
        {
            std::cout <<
            "=================================================================="
            " TEMP_COUNTER=" << TEMP_COUNTER <<
            " disks.size()=" << disks.size() << std::endl;
            break;
        }
    }
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    
    // Wait for all row threads to finish.
    for (auto& t : all_threads) t.join();
}


// Top level for each worker thread moving spots. For "disk_count" Disks
// beginning at "first_disk_index": if the Disk's "future_position" has
// changed, erase it from the grid, update its position, then re-insert it
// back into the grid.
void DiskOccupancyGrid::oneThreadMovingSpots(int first_disk_index,
                                             int disk_count,
                                             std::vector<Disk>& disks)
{
    for (int disk_index = first_disk_index;
         disk_index < (first_disk_index + disk_count);
         disk_index++)
    {
        Disk& disk = disks.at(disk_index);
        if (disk.position != disk.future_position)
        {
            eraseDiskWrap(disk);
            disk.position = disk.future_position;
            insertDiskWrap(disk);
        }
        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        if (disk_index == (first_disk_index + (disk_count / 2))) { yield(); }
        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    }
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    const std::lock_guard<std::mutex> lock(threads_finished_mutex);
    threads_finished++;
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
}

// Top level for each worker thread adjusting spot overlap. For "disk_count"
// Disks beginning at "first_disk_index": look up nearest neighbor, if
// overlap compute new position.
void DiskOccupancyGrid::oneThreadAdjustingSpots(int first_disk_index,
                                                int disk_count,
                                                float retry_fraction,
                                                float move_scale,
                                                bool& no_move,
                                                std::vector<Disk>& disks)
{
    for (int disk_index = first_disk_index;
         disk_index < (first_disk_index + disk_count);
         disk_index++)
    {
        Disk& a = disks.at(disk_index);
        std::set<Disk*> disks_near_a;
        findNearbyDisks(a, disks_near_a);
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
                    float fade = interpolate(retry_fraction, 1.0, 0.5);
                    float adjust = (radius_sum - distance) * fade * move_scale;
                    a.future_position += basis * adjust;
                }
            }
        }
        Vec2 before = a.future_position;
        a.future_position = wrapToCenterTile(a.future_position);
        if (a.future_position != before) no_move = false;
        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        if (disk_index == (first_disk_index + (disk_count / 2))) { yield(); }
        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    }
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    const std::lock_guard<std::mutex> lock(threads_finished_mutex);
    threads_finished++;
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Given a reference point (say to be rendered), and the center of a Spot,
// adjust "spot_center" with regard to tiling, to be the nearest (perhaps in
// another tile) to "reference_point".
Vec2 DiskOccupancyGrid::nearestByTiling(Vec2 reference_point,
                                        Vec2 spot_center) const
{
    Vec2 nearest_point;
    float nearest_distance = std::numeric_limits<float>::infinity();
    // TODO assumes square grid, only looks at x bounds.
    float tile_size = maxXY_.x() - minXY_.x();
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
Vec2 DiskOccupancyGrid::wrapToCenterTile(Vec2 v) const
{
    // TODO assumes square grid, only looks at x bounds.
    float tile_size = maxXY_.x() - minXY_.x();

    float half = tile_size / 2;
    return Vec2(fmod_floor(v.x() + half, tile_size) - half,
                fmod_floor(v.y() + half, tile_size) - half);
}
