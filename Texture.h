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
    virtual ~AbstractTexture() = default;
    virtual Color getColor(Vec2 position) const = 0;
};

class Texture : public AbstractTexture
{
public:
    // Default constructor.
    Texture() : raster_(emptyCvMat()) { constructor_count_++; }
    virtual ~Texture();
    
    // Provide a default so Texture is a concrete (non-virtual) class.
    Color getColor(Vec2 position) const override { return Color(0, 0, 0); }
    // Get color at position, clipping to unit RGB color cube.
    Color getColorClipped(Vec2 p) const { return getColor(p).clipToUnitRGB(); }
    // Get color at position, clipping to unit RGB color cube, and anti-aliased.
    Color getColorClippedAntialiased(Vec2 position, float size) const;
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
    static inline int window_counter = 0;
    static inline int window_x = 0;
    static inline int window_y = 0;
    // Rasterize this texture into a size² OpenCV image. Arg "disk" true means
    // draw a round image, otherwise a square. Run parallel threads for speed.
    void rasterizeToImageCache(int size, bool disk) const;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    // Rasterize the j-th row of this texture into a size² OpenCV image. Expects
//    // to run in its own thread, uses mutex to synchonize access to the image.
//    void rasterizeRowOfDisk(int j, int size, bool disk,
//                            cv::Mat& opencv_image,
//                            std::mutex& ocv_image_mutex) const;
    // Rasterize the j-th row of this texture into a size² OpenCV image. Expects
    // to run in its own thread, uses mutex to synchonize access to the image.
    void rasterizeRowOfDisk(int j, int size, bool disk,
                            cv::Mat& opencv_image,
                            int& row_counter,
                            std::mutex& ocv_image_mutex) const;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Copies disk-shaped portion of image cache onto given background cv::Mat.
    // Assumes "bg" is a CV "ROI", a "submat" of a presumably larger cv::Mat.
    void matteImageCacheDiskOverBG(int size, cv::Mat& bg);
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
    
    // Added to debug memory leak, but kept as an alarm for potential problems.
    // Used to test if a given Texture (eg a pointer's target) is still valid.
    bool valid() const
    {
        bool v = ((valid_top_ == validity_key_) &&
                  (valid_bot_ == validity_key_ / 2));
        if (!v) invalid_instance_counter_++;
        if (!v)
        {
            std::cout << "fail Texture::valid(), should be (";
            std::cout << validity_key_ << ", " << validity_key_ / 2;
            std::cout << ") but are (";
            std::cout << valid_top_ << ", " << valid_bot_ / 2 << ")";
            std::cout << std::endl;
        }
        return v;
    }
    void markAsInvalid()
    {
        valid_top_ = 0;
        valid_bot_ = 0;
    }
    static void invalidInstanceReport()
    {
        std::cout << "Texture invalid instance count = ";
        std::cout << invalid_instance_counter_ << std::endl;
    }
    // Utilities for rasterizing a Texture to tiling of pixels, with versions
    // for a square and a disk of pixels. Each require a "size" (width of the
    // square or diameter of the disk) and a function to be applied at each
    // pixel. The function's parameters are i/j (column/row) indexes of the
    // pixel raster, and the corresponding Vec2 in Texture space. [DEPRECATED]
    static void rasterizeSquare(int size, PixelFunction pixel_function);
    static void rasterizeDisk(int size, PixelFunction pixel_function);
    // Combines display on screen and (if non-empty "pathname" provided) writing
    // to file. Displays at default resolution (typically 511x511) unless "size"
    // parameter is given. Combined to allow writing inline an arbitrarily
    // nested expression of TexSyn constructors, whose lifetime extends across
    // both operations.
    // See: https://cwreynolds.github.io/TexSyn/#20200305
    static void displayAndFile(const Texture& texture)
        { displayAndFile(texture, ""); }
    static void displayAndFile(const Texture& texture, std::string pathname)
        { displayAndFile(texture, pathname, getDefaultRenderSize()); }
    static void displayAndFile(const Texture& texture,
                               std::string pathname,
                               int size);
    static void waitKey();
    static void waitKey(int delay_in_milliseconds);
    // close the window
    //static void closeWindow(const std::string name);
    static void closeAllWindows();

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
    // Display row of three textures, at given size, optionally saving to file.
    static void displayAndFile3(const Texture& t1,
                                const Texture& t2,
                                const Texture& t3,
                                std::string pathname,
                                int size);
    static void displayAndFile3(const Texture& t1,
                                const Texture& t2,
                                const Texture& t3,
                                std::string pathname)
        { displayAndFile3(t1, t2, t3, pathname, getDiffSize()); }
    static void displayAndFile3(const Texture& t1,
                                const Texture& t2,
                                const Texture& t3)
        { displayAndFile3(t1, t2, t3, ""); }
    // Each rendered pixel uses an NxN jittered grid of subsamples, where N is:
    static inline int sqrt_of_aa_subsample_count = 1;
    // Get/set global default render size.
    static int getDefaultRenderSize() { return render_size_; }
    static void setDefaultRenderSize(int size) { render_size_ = size; }
    static int getDiffSize()
        { return nearestOddInt(getDefaultRenderSize() * 0.666); }
    // Get/set global default "render as disk" flag: disk if true, else square.
    static bool getDefaultRenderAsDisk() { return render_as_disk_; }
    static void setDefaultRenderAsDisk(bool disk) { render_as_disk_ = disk; }
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // Get/set global switch for "parallel render": one thread per row.
    static int getParallelRender() { return render_thread_per_row_; }
    static void setParallelRender(bool p) { render_thread_per_row_ = p; }
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    thread_local static inline int expensive_to_nest = 0;
    // Score a Texture on how much "high" frequency it has.
    // TODO temp? Similar in intent to wiggliness() in GP.h
    void fft_test();
    float highFrequencyScore();
    // Optional cache of 100 colors randomly sampled in unit-diameter disk.
    const std::vector<Color>& cachedRandomColorSamples(RandomSequence& rs);
    // Print report on constructor vs. destructor count, eg at end of run.
    static void leakCheck()
    {
        std::cout << "Texture";
        std::cout << ": constructions=" << constructor_count_;
        std::cout << ", destructions=" << destructor_count_;
        std::cout << ", leaked=" << constructor_count_ - destructor_count_;
        std::cout << std::endl;
    }
    static int getDefaultOpencvMatType() { return default_opencv_mat_type_; }
    static void setDefaultOpencvMatType(int opencv_mat_type)
        { default_opencv_mat_type_ = opencv_mat_type; }
    // TODO 20201204 experiment-- expose a Texture's cv::mat
    const cv::Mat& getCvMat() const { return *raster_; }
    // Return a "submat"/"ROI" reference into a portion of a given cv::Mat.
    static cv::Mat getCvMatRect(const Vec2& upper_left_position,
                                const Vec2& size_in_pixels,
                                const cv::Mat cv_mat);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    static void checkForUserInput();
    static inline int last_key_read_;
    static int getLastKeyPushed() { return last_key_read_; }
    static void setLastKeyPushed(int key) { last_key_read_ = key; }
    // TODO maybe there should be a hook, a void() std::function, to handle
    // per-app key commands?
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    static inline bool seed_from_hashed_args = true;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    class RasterizeRowHelper
    {
    public:
        RasterizeRowHelper(int j, int size, bool disk) :
            j_(j), size_(size), disk_(disk)
        {
            odd_size = size % 2;
//            half = size / float(2);
            half = size / 2;
            
            // Which row we are on? [0, size-1]
//            row_index = (odd_size ?
//                         j + half :
//                         j + half - 1);
            row_index = j + half + (odd_size ? 0 : -1);

//            auto half_step_toward_0 =[](float x)
//                { return x + (x > 0 ? -0.5 : 0.5); };
//            row_y = odd_size ? j : (j + (j > 0 ? -0.5 : 0.5));
//            row_y = odd_size ? j : half_step_toward_0(j);

            row_y = (odd_size ? j : j - 0.5);

            // define float offsets for even-diameter case??????????????

//            last_pixel_index =
//                (disk ?
//                 (odd_size ?
//                  std::sqrt(sq(half) - sq(j)) :
//                  half_step_toward_0(std::sqrt(sq(half) - sq(row_y)))) :
////                  half_step_toward_0(std::sqrt(sq(half) - sq(row_y)) + 1)) :
//                 half);
//            last_pixel_index = disk ? std::sqrt(sq(half) - sq(j)) : half;
            
//                last_pixel_index = (disk ?
//                                    (odd_size ?
//                                     std::sqrt(sq(half) - sq(j)) :
//                                     std::sqrt(sq(size * 0.5) - sq(row_y))) :
//                                    half);
//
//    //            first_pixel_index = -last_pixel_index;
//                first_pixel_index = -last_pixel_index + (odd_size ? 0 : -1);

//            last_pixel_index = (disk ?
//                                (odd_size ?
//                                 std::sqrt(sq(half) - sq(j)) :
//                                 std::sqrt(sq(size * 0.5) - sq(row_y))) :
//                                half);

            last_pixel_index = (disk ?
                                (odd_size ?
                                 std::sqrt(sq(half) - sq(j)) :
                                 std::sqrt(sq(size * 0.5) - sq(row_y))) :
                                (odd_size ? half : half - 1));

//            first_pixel_index = -last_pixel_index;
//            first_pixel_index = -last_pixel_index + (odd_size ? 0 : -1);
//                first_pixel_index = (-last_pixel_index +
//    //                                 (odd_size ? 0 : -1) +
//                                     (disk && odd_size ? 0 : 1));
//            first_pixel_index = -last_pixel_index;
            first_pixel_index = -last_pixel_index + (odd_size ? 0 : -1);

//            std::cout << "RR:";
//            std::cout << " j=" << j;
//            std::cout << " size=" << size;
//            std::cout << " odd_size=" << odd_size;
//            std::cout << " half=" << half;
//            std::cout << " row_y=" << row_y;
//            std::cout << " row_index=" << row_index;
//            std::cout << " first_pixel_index=" << first_pixel_index;
//            std::cout << " last_pixel_index=" << last_pixel_index;
//            std::cout << std::endl;
        }
        bool odd_size;
        int half;
        int first_pixel_index;
        int last_pixel_index;
        float row_y;
        int row_index;
    private:
        int j_;
        int size_;
        bool disk_;
    };
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    
    private:
    static inline const int validity_key_ = 1234567890;
    static inline int invalid_instance_counter_ = 0;
    int valid_top_ = validity_key_;
    // Allocate a generic, empty, cv::Mat. Optionally used for rasterization.
    std::shared_ptr<cv::Mat> emptyCvMat() const;
    std::shared_ptr<cv::Mat> raster_;
    // Optional cache of 100 colors randomly sampled in unit-diameter disk.
    std::vector<Color> cached_random_color_samples_;
    float cached_high_frequency_score_ = 0;
    // Global default render size.
    static inline int render_size_ = 511;
    // Global default "render as disk" flag: render disk if true, else square.
    static inline bool render_as_disk_ = true;
    static inline int constructor_count_ = 0;
    static inline int destructor_count_ = 0;
    static int default_opencv_mat_type_;
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
public:
    // Global for one-thread-per-row rendering.
    // TODO should have accessors if kept.
//    static inline bool render_thread_per_row = true;
    static inline bool render_thread_per_row_ = true;
private:
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    int valid_bot_ = validity_key_ / 2;
};
