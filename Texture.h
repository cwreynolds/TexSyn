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
#include <vector>

// Nickname for the type of PixelFunction used for rasterization.
typedef std::function<void(int i, int j, Vec2 position)> PixelFunction;

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
    // Provide a default so Texture is a concrete (non-virtual) class.
    Color getColor(Vec2 position) const override { return Color(0, 0, 0); }
    // Get color at position, clipping to unit RGB color cube.
    Color getColorClipped(Vec2 p) const { return getColor(p).clipToUnitRGB(); }
    // Display this Texture in a pop-up OpenCV window, wait for key, then close.
    void displayInWindow(int size = 511, bool wait = true) const;
    // Display a collection of Textures, each in a window, then wait for a char.
    static void displayInWindow(std::vector<const Texture*> textures,
                                int size = 511, bool wait = true);
    // Writes Texture to a file using cv::imwrite(). Generally used with JPEG
    // codec, but pathname's extension names the format to be used. Converts to
    // "24 bit" image (8 bit unsigned values for each of red, green and blue
    // channels) because most codecs do not support 3xfloat format.
    void writeToFile(int size,
                     const std::string& pathname,
                     Color bg_color = Color(0.5, 0.5, 0.5),
                     int margin = 0,
                     const std::string& file_type = ".png") const;
    // Reset statistics for debugging.
    void resetStatistics() const;
    // Collect statistics for debugging.
    void collectStatistics(Vec2 position, Color color) const;
    // Utilities for rasterizing a Texture to tiling of pixels, with versions
    // for a square and a disk of pixels. Each require a "size" (width of the
    // square or diameter of the disk) and a function to be applied at each
    // pixel. The function's parameters are i/j (column/row) indexes of the
    // pixel raster, and the corresponding Vec2 in Texture space.
    static void rasterizeSquare(int size, PixelFunction pixel_function);
    static void rasterizeDisk(int size, PixelFunction pixel_function);
    // Compare two textures, print stats, display inputs and AbsDiff of them
    static void diff(const Texture& t0, const Texture& t1);
    // Combines display on screen and writing file, but primary benefit is that
    // this allows writing an arbitrarily nested expression of TexSyn
    // constructors, whose lifetime extends across both operations.
    // See: https://cwreynolds.github.io/TexSyn/docs/index.html#20200305
    static void displayAndFile(const Texture& texture,
                               std::string pathname = "",
                               int size = 511);
    static void waitKey();
private:
    // TODO maybe we need a OOBB Bounds2d class?
    // TODO maybe should be stored in external std::map keyed on Texture pointer
    // Store bounds of sampled positions.
    static float min_x;
    static float max_x;
    static float min_y;
    static float max_y;
};
