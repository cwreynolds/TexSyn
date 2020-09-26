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
namespace cv {class Mat;}

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
    Texture() : raster_(emptyCvMat()) {}
    // Provide a default so Texture is a concrete (non-virtual) class.
    Color getColor(Vec2 position) const override { return Color(0, 0, 0); }
    // Get color at position, clipping to unit RGB color cube.
    Color getColorClipped(Vec2 p) const { return getColor(p).clipToUnitRGB(); }
    // Utility for getColor(), special-cased for when alpha is 0 or 1.
    Color interpolatePointOnTextures(float alpha, Vec2 position0, Vec2 position1,
                                     const Texture& t0, const Texture& t1) const;
    // Rasterize this texture into size² OpenCV image, display in pop-up window.
    void displayInWindow(int size = getDefaultRenderSize(),
                         bool wait = true) const;
    // Display a collection of Textures, each in a window, then wait for a char.
    static void displayInWindow(std::vector<const Texture*> textures,
                                int size = getDefaultRenderSize(),
                                bool wait = true);
    // Display cv::Mat in pop-up window. Stack diagonally from upper left.
    static void windowPlacementTool(cv::Mat& mat);
    // Rasterize this texture into a size² OpenCV image. Arg "disk" true means
    // draw a round image, otherwise a square. Run parallel threads for speed.
    void rasterizeToImageCache(int size, bool disk) const;
    // Rasterize the j-th row of this texture into a size² OpenCV image. Expects
    // to run in its own thread, uses mutex to synchonize access to the image.
    void rasterizeRowOfDisk(int j, int size, bool disk,
                            cv::Mat& opencv_image,
                            std::mutex& ocv_image_mutex) const;
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
    // pixel raster, and the corresponding Vec2 in Texture space. [DEPRECATED]
    static void rasterizeSquare(int size, PixelFunction pixel_function);
    static void rasterizeDisk(int size, PixelFunction pixel_function);
    // Combines display on screen and writing file, but primary benefit is that
    // this allows writing an arbitrarily nested expression of TexSyn
    // constructors, whose lifetime extends across both operations.
    // See: https://cwreynolds.github.io/TexSyn/#20200305
    static void displayAndFile(const Texture& texture,
                               std::string pathname = "",
                               int size = getDefaultRenderSize());
    static void waitKey();
    // BACKWARD_COMPATIBILITY reference to new "disposable" Uniform object. This
    // is called ONLY from constructors providing backward compatibility. The
    // tiny Uniform texture object is allowed to "memory leak" for ease of use.
    static Texture& disposableUniform(Color color);
    // Special utility for Texture::diff() maybe refactor to be more general?
    // Compare textures, print stats, optional file, display inputs and AbsDiff.
    static void diff(const Texture& t0, const Texture& t1,
                     std::string pathname, int size, bool binary);
    static void diff(const Texture& t0, const Texture& t1,
                     std::string pathname, int size)
        { diff(t0, t1, pathname, getDiffSize(), false); }
    static void diff(const Texture& t0, const Texture& t1, std::string pathname)
        { diff(t0, t1, pathname, getDiffSize()); }
    static void diff(const Texture& t0, const Texture& t1)
        { diff(t0, t1, "", getDiffSize()); }
    static void displayAndFile3(const Texture& t1,
                                const Texture& t2,
                                const Texture& t3,
                                std::string pathname = "",
                                int size = 333);
    // Each rendered pixel uses an NxN jittered grid of subsamples, where N is:
    static int sqrt_of_aa_subsample_count;
    // Get/set global default render size.
    static int getDefaultRenderSize() { return render_size_; }
    static void setDefaultRenderSize(int size) { render_size_ = size; }
    static int getDiffSize()
        { return nearestOddInt(getDefaultRenderSize() * 0.666); }
    // Get/set global default "render as disk" flag: disk if true, else square.
    static bool getDefaultRenderAsDisk() { return render_as_disk_; }
    static void setDefaultRenderAsDisk(bool disk) { render_as_disk_ = disk; }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    thread_local static inline int expensive_to_nest = 0;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
    // TODO maybe we need a OOBB Bounds2d class?
    // TODO maybe should be stored in external std::map keyed on Texture pointer
    // Store bounds of sampled positions.
    static float min_x;
    static float max_x;
    static float min_y;
    static float max_y;
    // Allocate a generic, empty, cv::Mat. Optionally used for rasterization.
    std::shared_ptr<cv::Mat> emptyCvMat() const;
    const std::shared_ptr<cv::Mat> raster_;
    // Global default render size.
    static int render_size_;
    // Global default "render as disk" flag: disk if true, else square.
    static bool render_as_disk_;
};
