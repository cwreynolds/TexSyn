//
//  Disk.h
//  texsyn
//
//  Defines utility classes Disk and DiskOccupancyGrid.
//
//  Created by Craig Reynolds on 4/23/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Utilities.h"
#include "Vec2.h"
#include <set>
#include <thread>

// Represents a disk on the 2d plane, defined by center position and radius.
// TODO stores some other properties: "future_position" for LotsOfSpotsBase,
// and "angle" for LotsOfButtons. Consider redesign of class structure.
class Disk
{
public:
    Disk(){}
    Disk(float r, Vec2 p) : radius(r), position(p), future_position(p) {}
    Disk(float r, Vec2 p, float a, float w)
      : radius(r), position(p), future_position(p), angle(a), wavelength(w) {}
    float area() const { return pi * sq(radius); }
    Disk translate(Vec2 t) const { return Disk(radius, t + position,
                                               angle, wavelength); }
    float radius = 0;
    Vec2 position;
    Vec2 future_position;
    float angle = 0;
    float wavelength = 0; // TODO experimental for Gabor noise kernels. Needed?
    // Lightweight utility used by Camouflage.
    // TODO viz_func used only for debugging, can be removed eventually.
    static std::vector<Disk>
        randomNonOverlappingDisksInRectangle(int count,
                                             float radius_min,
                                             float radius_max,
                                             float radius_margin,
                                             Vec2 corner_min,
                                             Vec2 corner_max,
                                             RandomSequence& rs,
                                             std::function
                                             <void(const std::vector<Disk>&)>
                                             viz_func);
};

// DiskOccupancyGrid -- 2d spatial data structure for collections of Disks.
// (TODO currently assumes grid is square, both in 2d space and cell layout.)
class DiskOccupancyGrid
{
public:
    DiskOccupancyGrid(Vec2 minXY, Vec2 maxXY, int grid_side_count)
      : minXY_(minXY),
        maxXY_(maxXY),
        grid_side_count_(grid_side_count)
    {
        assert(((maxXY.x() - minXY.x()) == (maxXY.y() - minXY.y())) &&
               "expecting square spatial dimentions");
        clear();
    }
    
    // Insert (or erase) a given Disk from this occupancy grid. Assumes the grid
    // is used to represent a tiling pattern, so handles wrap-around of disks
    // that intersect grid boundary.
    void insertDiskWrap(Disk& d)
    {
        diskWrapUtility(d, [&](int i, int j) { insertIntoCell(i, j, &d); });
    }
    void eraseDiskWrap(Disk& d)
    {
        diskWrapUtility(d, [&](int i, int j) { eraseFromCell(i, j, &d); });
    }
    
    // Used by insertDiskWrap() and eraseDiskWrap() for wrap-around tiling.
    // Handles cases when given Disk crosses a boundary of the grid, inserting
    // up to 4 images of the Disk, each offset by tiling spacing in x, y, both.
    void diskWrapUtility(Disk& d, std::function<void(int i, int j)> function)
    {
        // TODO assumes square grid, only looks at x bounds.
        float tile_size = maxXY_.x() - minXY_.x();
        // Offsets to wrap around to the other side of nearest grid boundary.
        Vec2 wrap_x(d.position.x() > 0 ? -tile_size : tile_size, 0);
        Vec2 wrap_y(0, d.position.y() > 0 ? -tile_size : tile_size);
        // Clone up to four Disks into the grid. 4 if Disk center is near a grid
        // corner, 2 if Disk center is near a grid edge, 1 if fully inside.
        // applyToCellsInRect() clips Disks which are wholly outside the grid.
        applyToCellsInRect(d,                            function);
        applyToCellsInRect(d.translate(wrap_x),          function);
        applyToCellsInRect(d.translate(         wrap_y), function);
        applyToCellsInRect(d.translate(wrap_x + wrap_y), function);

    }

    // Insert (erase) a given Disk from this occupancy grid.
    void insertDisk(Disk& d)
    {
        applyToCellsInRect(d, [&](int i, int j){ insertIntoCell(i, j, &d); });

    }
    void eraseDisk(Disk& d)
    {
        applyToCellsInRect(d, [&](int i, int j){ eraseFromCell(i, j, &d); });
    }
    
    // Lookup which Disks in this grid overlap with a given Disk (or point as a
    // zero radius Disk). Adds them to given output arg, a set of Disk pointers.
    void findNearbyDisks(Vec2 point, std::set<Disk*>& disks)
    {
        // Treat point as a disk of zero radius.
        findNearbyDisks(Disk(0, point), disks);
    }
    void findNearbyDisks(const Disk& query, std::set<Disk*>& disks)
    {
        // Each grid cell contains an std::set of disk pointers
        // Add each pointer in the Cell's set to the output argument set.
        auto collectCellDisks = [&](int i, int j)
        {
             for (auto& d : *getSetFromGrid(i, j)) disks.insert(d);
        };
        applyToCellsInRect(query, collectCellDisks);
    }
    
    // Is any portion of the given Disk inside the bounds of our grid?
    bool gridOverlapsDisk(const Disk& disk)
    {
        return ((disk.position.x() + disk.radius >= minXY_.x()) &&
                (disk.position.y() + disk.radius >= minXY_.y()) &&
                (disk.position.x() - disk.radius <= maxXY_.x()) &&
                (disk.position.y() - disk.radius <= maxXY_.y()));
    }

    // Apply function to each Cell in 2d bounding square of given Disk.
    void applyToCellsInRect(const Disk& disk,
                            std::function<void(int i, int j)> function)
    {
        if (gridOverlapsDisk(disk))
        {
            applyToCellsInRect(xToI(disk.position.x() - disk.radius),
                               xToI(disk.position.y() - disk.radius),
                               xToI(disk.position.x() + disk.radius),
                               xToI(disk.position.y() + disk.radius),
                               function);
        }
    }

    // Apply given function to each (i,j) cell within given inclusive bounds.
    void applyToCellsInRect(int i_min,
                            int j_min,
                            int i_max,
                            int j_max,
                            std::function<void(int i, int j)> function)
    {
        for (int i = i_min; i <= i_max; i++)
            for (int j = j_min; j <= j_max; j++)
                function(i, j);
    }
    
    // TODO temp for debugging
    void printCellCounts()
    {
        int total_count = 0;
        for (int j = grid_side_count_ - 1; j >= 0 ; j--)
        {
            for (int i = 0; i < grid_side_count_; i++)
            {
                size_t count = getSetFromGrid(i, j)->size();
                std::cout << count << " ";
                total_count += count;
            }
            std::cout << std::endl;
        }
        std::cout << "total count: " << total_count << std::endl;
    }
    // Convert float x coordinate to int i cell index. Since we currently assume
    // a square grid, this is used for both x to i and y to j. NB: the remapping
    // goes from the 2d float bounds to [0, grid_side_count] then is clipped
    // onto [0, grid_side_count]. Took me a while to get that right.
    int xToI(float x) const
    {
        return int(clip(remapInterval(x,
                                      minXY_.x(),
                                      maxXY_.x(),
                                      0,
                                      grid_side_count_),
                        0,
                        grid_side_count_ - 1));
    };
    
    float area() const
    {
        Vec2 diagonal = maxXY_ - minXY_;
        return diagonal.x() * diagonal.y();
    }
    
    // Random position uniformly distributed between "minXY_" and "maxXY_".
    Vec2 randomPointOnGrid(RandomSequence& rs) const
    {
        return Vec2(rs.frandom2(minXY_.x(), maxXY_.x()),
                    rs.frandom2(minXY_.y(), maxXY_.y()));
    }

    // Remove all Disks from this grid, reset based on grid_side_count.
    void clear()
    {
        grid_.clear();
        grid_.resize(grid_side_count_);
        for (auto& row : grid_) row.resize(grid_side_count_);
    }
    
    //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // These functions specifically support use case of LotsOfSpotsBase.
    //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // Relaxation process that attempts to reduce the overlaps in an arbitrary
    // collection of Disks. Overlapping Disks are pushed away from each other
    // along the line connecting their centers. The whole process is repeated
    // "retries" times, or until none overlap.
    //
    // This uses parallel threads and spatial data structures. For consistent
    // results, the work is broken into two sequential steps, each of which runs
    // in parallel. Step one: find overlaps and compute Disk's future position.
    // Step two: move Disk and update occupancy grid.
    //
    // 20210429 added optional "move_scale" arg, defaulting to 1, to reduce the
    //          movement used to remove overlap. For phasor noise kernels which
    //          NEED to overlap, but we want to "spread out" just a little bit.
    void reduceDiskOverlap(int retries, std::vector<Disk>& disks)
    {
        reduceDiskOverlap(retries, 1, disks);
    }
    void reduceDiskOverlap(int retries,
                           float move_scale,
                           std::vector<Disk>& disks);
    // Runs parallel update of all Disks. Parameter is function to generate one
    // thread, given "first_disk_index" and "disk_count" in vector "disks". Each
    // thread updates this given block of disks, in parallel with other threads.
    void parallelDiskUpdate(std::vector<Disk>& disks,
                            std::function<std::thread(int, int)> thread_maker);
    // Top level for each worker thread adjusting spot overlap. For "disk_count"
    // Disks beginning at "first_disk_index": look up nearest neighbor, if
    // overlap compute new position.
    void oneThreadAdjustingSpots(int first_disk_index,
                                 int disk_count,
                                 float retry_fraction,
                                 float move_scale,
                                 bool& no_move,
                                 std::vector<Disk>& disks);
    // Top level for each worker thread moving spots. For "disk_count" Disks
    // beginning at "first_disk_index": if the Disk's "future_position" has
    // changed, erase it from the grid, update its position, then re-insert it
    // back into the grid.
    void oneThreadMovingSpots(int first_disk_index,
                              int disk_count,
                              std::vector<Disk>& disks);
    // Given a reference point (say to be rendered), and the center of a Spot,
    // adjust "spot_center" with regard to tiling, to be the nearest (perhaps in
    // another tile) to "reference_point".
    Vec2 nearestByTiling(Vec2 reference_point, Vec2 spot_center) const;
    // Given a position, find corresponding point on center tile, via fmod/wrap.
    Vec2 wrapToCenterTile(Vec2 v) const;
    //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    
private:
    // Returns pointer to set of disk pointers at grid cell (i, j).
    std::set<Disk*>* getSetFromGrid(int i, int j)
    {
        return grid_.at(i).at(j).getSet();
    }
    void insertIntoCell(int i, int j, Disk* d)
    {
        grid_.at(i).at(j).insert(d);
    }
    void eraseFromCell(int i, int j, Disk* d)
    {
        grid_.at(i).at(j).erase(d);
    }
    const Vec2 minXY_;
    const Vec2 maxXY_;
    const int grid_side_count_;
    
    // The grid is a 2d array of these Cell objects, each containing an std::set of Disk pointers and a mutex to lock it for thread safety.
    class Cell
    {
    public:
        Cell(){}
        Cell(const Cell& cell) : set_(cell.set_) {}
        std::set<Disk*>* getSet() { return &set_; }
        void insert(Disk* d)
        {
            // Wait to grab cell lock. (Lock released at end of block)
            const std::lock_guard<std::mutex> lock(cell_mutex_);
            set_.insert(d);
        }
        void erase(Disk* d)
        {
            // Wait to grab cell lock. (Lock released at end of block)
            const std::lock_guard<std::mutex> lock(cell_mutex_);
            set_.erase(d);
        }
    private:
        std::set<Disk*> set_;
        std::mutex cell_mutex_;
    };
    std::vector<std::vector<Cell>> grid_;
};

// Lightweight utility to randomly place "count" non-overlapping Disks, with
// radii in the given range, inside an axis-aligned rectangle defined by two
// diagonally opposite corners. Result returned by value (copied) in an
// std::vector<Disk>. This is used by the Camouflage module for placing three
// Textures on a background. I initially tried using DiskOccupancyGrid, but it
// (currently) requires a square (versus rectangular) grid, and appeared to not
// adjust such a small number of Disks in its "industrial strength" multi-
// threaded solver. Rather than try to debug that, and possibly break the
// LotsOfSpots family of Textures, I wrote this smaller brut force solver. So
// sue me. TODO maybe merge the two versions at some point?
// TODO viz_func used only for debugging, can be removed eventually.
inline std::vector<Disk>
    Disk::randomNonOverlappingDisksInRectangle(int count,
                                               float radius_min,
                                               float radius_max,
                                               float radius_margin,
                                               Vec2 corner_min,
                                               Vec2 corner_max,
                                               RandomSequence& rs,
                                               std::function
                                               <void(const std::vector<Disk>&)>
                                               viz_func)
{
    // Collection of random nonoverlapping Disks, initially empty.
    std::vector<Disk> disks;
    // Initialize to "count" random disks, inside rectangle, may overlap.
    for (int i = 0; i < count; i++)
    {
        float r = rs.random2(radius_min, radius_max);
        Vec2 center(rs.random2(corner_min.x() + r, corner_max.x() - r),
                    rs.random2(corner_min.y() + r, corner_max.y() - r));
        disks.push_back(Disk(r, center));
    }
    // Used below for forcing re-positioned Disks back inside rectangle.
    auto clip_to_rec = [&]
    (int index, Vec2 point, float radius)
    {
        disks.at(index).position = Vec2(clip(point.x(),
                                             corner_min.x() + radius,
                                             corner_max.x() - radius),
                                        clip(point.y(),
                                             corner_min.y() + radius,
                                             corner_max.y() - radius));
    };
    // For all pairs of Disks, push apart, repeat up to "max_retry" times.
    int max_retry = 100;
    bool done = true;
    for (int retry = 0; retry < max_retry; retry++)
    {
        done = true;
        for (int i = 0; i < count; i++)
        {
            for (int j = 0; j < count; j++)
            {
                if (i != j)
                {
                    // Copy center position and radius for Disks i and j.
                    Vec2 pi = disks.at(i).position;
                    Vec2 pj = disks.at(j).position;
                    float ri = disks.at(i).radius;
                    float rj = disks.at(j).radius;
                    // To prevent overlap and keep given margin between Disks.
                    float min_distance = ri + rj + radius_margin;
                    Vec2 offset = pi - pj;
                    float distance = offset.length();
                    if (distance < min_distance)
                    {
                        // Unit vector along line through both Disk centers.
                        Vec2 direction = offset / distance;
                        // Add randomization, vector no longer normalized, but
                        // I can't see how that would matter as used here.
                        direction = direction + rs.randomUnitVector() * 0.4;
                        // Adjustment rate.
                        float speed = 0.02;
                        // Vector to incrementally push Disk centers apart.
                        Vec2 push = direction * min_distance * speed;
                        pi += push;
                        pj -= push;
                        // Constrain repositioned Disks back inside rectangle
                        clip_to_rec(i, pi, ri);
                        clip_to_rec(j, pj, rj);
                        // Continue to adjust until no overlaps found
                        done = false;
                        viz_func(disks);
                    }
                }
            }
        }
        if (done) { break; }
    }
    if (!done)
    {
        std::cout << "Disk::randomNonOverlappingDisksInRectangle, ";
        std::cout << "unable to position without overlap" << std::endl;
    }
    return disks;
}

// Serialize Disk object to stream.
inline std::ostream& operator<<(std::ostream& os, const Disk& d)
{
    os << "Disk(radius=" << d.radius;
    os << ", position=" << d.position;
    if (d.position != d.future_position)
        { os << ", future_position=" << d.future_position; }
    os << ", angle=" << d.angle;
    os << ", wavelength=" << d.wavelength << ")";
    return os;
}
