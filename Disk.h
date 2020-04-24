//
//  Disk.h
//  texsyn
//
//  Created by Craig Reynolds on 4/23/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Utilities.h"
#include "Vec2.h"
#include <set>

// Represents a disk on the 2d plane, defined by center position and radius.
// TODO stores some other properties: "future_position" for LotsOfSpotsBase,
// and "angle" for LotsOfButtons. Consider redesign of class structure.
class Disk
{
public:
    Disk(){}
    Disk(float r, Vec2 p) : radius(r), position(p), future_position(p) {}
    float area() const { return pi * sq(radius); }
    Disk translate(Vec2 t) const { return Disk(radius, t + position); }
    float radius = 0;
    Vec2 position;
    Vec2 future_position;
    float angle = 0;
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
    
    // Remove all Disks from this grid, reset based on grid_side_count.
    void clear()
    {
        grid_.clear();
        grid_.resize(grid_side_count_);
        for (auto& row : grid_) row.resize(grid_side_count_);
    }

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
