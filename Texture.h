//
//  Texture.h
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//
//  A procedural Texture instance is something like an image which is infinite
//  in extent and resolution. It is defined by the AbstractTexture::getColor()
//  virtual function. All Operators overload this method. Its argument is a Vec2
//  whose x and y components range over the entire floating point range. Its
//  output is a similarly unbounded Color. A texture can be rendered by sampling
//  this color over some portion of the texture plane.
//
//  Texture class is large and probably too much trouble to understand to be
//  worth the effort unless you have a pressing need. A few key bits of API are
//  described in these comments. The intent of Texture is probably better
//  understood through the lens of a texture Operator (e.g. Add in Operator.h).
//  Texture provides the shared infrastructure, plumbing, and grunt work, that
//  allows an Operator definition to be relatively compact.
//
//  Texture class largely hides the interface to OpenCV, which does the heavy
//  lifting for cross-platform image manipulation, such as storing, displaying,
//  and writing images to file. It also contains various tools for "rendering"
//  a procedural texture to more traditional image format with stored pixels,
//  for which it provides a cache. It supports antialiased sampling, clipping to
//  the unit RGB color space, gamma correction, matting rendered (square or
//  round) images over others, tools for comparing textures (diff), support for
//  multi-threaded rendering, prevent locking up the machine during long runs of
//  multiple renders, and checking for user input (key presses, mouse clicks)
//  during long runs.
//
//  Selected API:
//      getColor()        maps a 2d location to a color, both defined over the
//                        full float range.
//      displayAndFile()  display a Texture in a window, rasterizing/rendering
//                        if needed, and caching the result. Optionally writes
//                        the image to a file.
//      diff()            compare two textures, often to check exact equivalence
//                        between, say, an old and new implementation.

#pragma once
#include "Vec2.h"
#include "Color.h"
#include "Utilities.h"
#include "RandomSequence.h"
#include <vector>
#include <limits>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>  // TODO temp(?) for fft_test()
#include <opencv2/highgui/highgui.hpp>
#pragma clang diagnostic pop


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
    // Default constructor and virtual destructor.
    Texture() : raster_(emptyCvMat()) { constructor_validity_util(); }
    ~Texture() { destructor_validity_util(); }
    
    // Provide a default so Texture is a concrete (non-virtual) class.
    Color getColor(Vec2 position) const override { return Color(0, 0, 0); }
    // Get color at position, clipping to unit RGB color cube.
    Color getColorClipped(Vec2 p) const { return getColor(p).clipToUnitRGB(); }

    // Get color at position, clipping to unit RGB color cube, and anti-aliased.
    Color getColorClippedAntialiased(Vec2 position, float size) const
    {
        // Read TexSyn Color from Texture at (i, j).
        Color color(0, 0, 0);
        resetExpensiveToNest();
        if (sqrt_of_aa_subsample_count > 1) // anti-aliasing?
        {
            float pixel_radius = 2.0 / size;
            std::vector<Vec2> offsets;
            RandomSequence rs(position.hash());
            jittered_grid_NxN_in_square(sqrt_of_aa_subsample_count,
                                        pixel_radius * 2, rs, offsets);
            for (Vec2 offset : offsets)
                color += getColorClipped(position + offset);
            color = color / sq(sqrt_of_aa_subsample_count);
        }
        else
        {
            color = getColorClipped(position);
        }
        return color;
    }

    // Utility for getColor(), special-cased for when alpha is 0 or 1.
    Color interpolatePointOnTextures(float alpha,
                                     Vec2 position0,
                                     Vec2 position1,
                                     const Texture& t0,
                                     const Texture& t1) const
    {
        return ((alpha == 0) ?
                // For alpha==0 evaluate only t0.
                t0.getColor(position0) :
                ((alpha == 1) ?
                 // For alpha==1 evaluate only t1.
                 t1.getColor(position1) :
                 // Otherwise evaluate both and interpolate between them.
                 interpolate(alpha,
                             t0.getColor(position0),
                             t1.getColor(position1))));
    }

    // Rasterize this texture into size² OpenCV image, display in pop-up window.
    void displayInWindow(int size, bool wait) const
    {
        rasterizeToImageCache(size, getDefaultRenderAsDisk());
        windowPlacementTool(*raster_);
        if (wait) waitKey();  // Wait for a keystroke in the window.
    }

    // Display a collection of Textures, each in a window, then wait for a char.
    static void displayInWindow(std::vector<const Texture*> textures,
                                int size = getDefaultRenderSize(),
                                bool wait = true)
    {
        for (auto& t : textures) t->displayInWindow(size, false);
        // Wait for keystroke, close windows, exit function.
        if (wait) waitKey();
    }

    // Display cv::Mat in pop-up window. Stack diagonally from upper left.
    static void windowPlacementTool(cv::Mat& mat)
    {
        std::string window_name = "TexSyn" + std::to_string(window_counter++);
        cv::namedWindow(window_name);       // Create a window for display.
        int tm = 23;  // TODO approximate top margin height
        cv::moveWindow(window_name, window_x, window_y);
        window_x += tm;
        window_y += tm;
        cv::imshow(window_name, mat);  // Show our image inside it.
        waitKey(1);  // TODO Trying to force window to display if not "on top."
        // TODO pure hack, assumes 511x511, screen size of my MacBook Pro (Mid 2014)
        if ((window_counter % 15) == 0) window_y =0 ;
    }

    static inline int window_counter = 0;
    static inline int window_x = 0;
    static inline int window_y = 0;
    
    // Rasterize this texture into a size² OpenCV image. Arg "disk" true means
    // draw a round image, otherwise a square. Run parallel threads for speed.
    void rasterizeToImageCache(int size, bool disk) const
    {
        //Timer t("rasterizeToImageCache");
        // If size changed, including from initial value of 0x0, generate raster.
        // (TODO also ought to re-cache if "disk" changes. Issue ignored for now.)
        if ((size != raster_->rows) || (size != raster_->cols))
        {
            // Reset our OpenCV Mat to be (size, size) at default depth.
            raster_->create(size, size, getDefaultOpencvMatType());
            startRenderTimer();
            // Synchronizes access to opencv_image to allow multiple render threads.
            std::mutex ocv_image_mutex;
            RasterizeHelper rh(size, disk);
            if (getParallelRender())
            {
                // Make helper threads, each to work on a horizontal "stripe" of the
                // texture in parallel. The goal is roughly one thread per hardware
                // processor but that is just hardcoded inline. (So "TODO 20220710")
                int number_of_threads = 6;
                int rows_per_thread = size / number_of_threads;
                int rows_left_over = size % number_of_threads;
                // Collection of all row threads.
                std::vector<std::thread> all_threads;
                // Threads increment "row_counter" when a row of pixels is finished.
                int row_counter = 0;
                // Veritcal (y/j) bounds for each stripe. First is bigger if needed.
                // These value updated in the loop below.
                int stripe_top = rh.top_j;
                int stripe_bot = stripe_top + rows_per_thread + rows_left_over;
                for (int s = 0; s < number_of_threads; s++)
                {
                    all_threads.push_back(std::thread
                                          (&Texture::rasterizeStripeOfDisk,
                                           this,
                                           stripe_top,
                                           stripe_bot - stripe_top,
                                           size,
                                           disk,
                                           std::ref(*raster_),
                                           std::ref(row_counter),
                                           std::ref(ocv_image_mutex)));
                    stripe_top = stripe_bot;
                    stripe_bot = stripe_top + rows_per_thread;
                }
                // Wait for helper threads to finish, join them with this thread.
                while (row_counter < size) { checkForUserInput(); }
                for (auto& t : all_threads) { t.join(); }
            }
            else
            {
                // Sequential case, single threaded. Loop over all image rows.
                int i = 0;  // Counter whose value is ignore in single thread case.
                for (int j = rh.top_j; j <= rh.bot_j; j++)
                {
                    rasterizeRowOfDisk(j, size, disk, *raster_, i, ocv_image_mutex);
                    checkForUserInput();
                }
            }
            // If optional render timeout was reached, return uniform black texture.
            if (renderTimeOut())
            {
                std::cout << "RENDER TIMEOUT: returning black texture." << std::endl;
                *raster_ = cv::Scalar::all(0);
            }
        }
    }

    // Rasterize the j-th row of this texture into a size² OpenCV image. Expects
    // to run in its own thread, uses mutex to synchonize access to the image.
    //void Texture::rasterizeRowOfDisk(int j, int size, bool disk,
    void rasterizeRowOfDisk(int j,                    // starting row index
                            int size,                 // total texture size
                            bool disk,                // disk or square?
                            cv::Mat& opencv_image,
                            int& row_counter,
                            std::mutex& ocv_image_mutex) const
    {
        // Half the rendering's size corresponds to the disk's center.
        int half = size / 2;
        RasterizeHelper rh(j, size, disk);
        // Create temp cv::Mat to accumulate pixels for this row.
        assert(default_opencv_mat_type_ == CV_8UC3);
        cv::Scalar gray(127, 127, 127);  // Note: assumes CV_8UC3.
        cv::Mat row_image(1, size, getDefaultOpencvMatType(), gray);
        for (int i = rh.first_pixel_index; i <= rh.last_pixel_index; i++)
        {
            if ((i == rh.first_pixel_index) && renderTimeOut()) { break; }
            // Read TexSyn Color from Texture at (i, j).
            Vec2 pixel_center = Vec2(i, j) / half;
            resetExpensiveToNest();
            // Render one pixel to produce a color value.
            Color color = getColorClippedAntialiased(pixel_center, size);
            // Adjust for display gamma.
            color = color.gamma(1 / defaultGamma());
            // Make OpenCV color, with reversed component order.
            cv::Vec3b opencv_color(color.b() * 255,  // Note: assumes CV_8UC3.
                                   color.g() * 255,
                                   color.r() * 255);
            // Write OpenCV color to corresponding pixel on row image:
            row_image.at<cv::Vec3b>(cv::Point(half + i, 0)) = opencv_color;
            // Near midpoint of rendering this Texture row, yield to other threads,
            // to avoid locking up the whole machine during a lengthy render run.
            if (i == 0) { yield(); }
        }
        
        // Define a new image which is a "pointer" to j-th row of opencv_image.
        cv::Mat row_in_full_image(opencv_image, cv::Rect(0, half - j, size, 1));
        // Wait to grab lock for access to image. (Lock released at end of block)
        const std::lock_guard<std::mutex> lock(ocv_image_mutex);
        // Copy line_image into the j-th row of opencv_image.
        row_image.copyTo(row_in_full_image);
        row_counter++;
    }

    // Rasterizes (renders) a horizontal "stripe" -- a range of vertically
    // adjacent pixel rows. Calls rasterizeRowOfDisk() to render each row.
    void rasterizeStripeOfDisk(int j,            // starting row index
                               int n_rows,       // number of row in stripe
                               int size,         // total texture size
                               bool disk,        // disk or square?
                               cv::Mat& opencv_image,
                               int& row_counter,
                               std::mutex& ocv_image_mutex) const
    {
        for (int row_index = j; row_index < (j + n_rows); row_index++)
        {
            rasterizeRowOfDisk(row_index, size, disk, opencv_image,
                               row_counter, ocv_image_mutex);
        }
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20220621 gathering data -- should this be kept?
    // TODO How tall are the per-thread "stripes"? On Intel laptop it was 1.
    // On M1 seems to provide almost no benefit. Collecting data to diagnose.
    static inline int rows_per_render_thread = 1;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Copies disk-shaped portion of image cache onto given background cv::Mat.
    // Normally "bg" is a CV "ROI", a "submat" of a presumably larger cv::Mat.
    void matteImageCacheDiskOverBG(int size, cv::Mat& bg)
    {
        // Ensure the Texture has been rendered to image cache as disk.
        rasterizeToImageCache(size, true);
        // Matte cached disk image over bg.
        matteImageCacheDiskOverBG(*raster_, bg);
    }

    // Copies disk-shaped portion of one cv::Mat onto a background cv::Mat.
    static void matteImageCacheDiskOverBG(const cv::Mat& disk, cv::Mat& bg)
    {
        assert(disk.rows == disk.cols);
        int size = disk.rows;
        // For each row.
        RasterizeHelper rh(size);
        for (int j = rh.top_j; j <= rh.bot_j; j++)
        {
            // Update rh for current row.
            rh = RasterizeHelper(j, size);
            // On j-th row, from first to last pixel.
            cv::Rect row_rect(rh.row_rect_x, rh.row_index, rh.row_rect_w, 1);
            // Create two submats (ROIs) of the disk mat and the destination mat.
            cv::Mat disk_row(disk, row_rect);
            cv::Mat bg_row(bg, row_rect);
            // Copy the disk row into the destination row.
            disk_row.copyTo(bg_row);
        }
    }

    // TODO 20211112: using for debugging, make part of UnitTest?
    // Verify that given mat is: square and symmetric (vertically, horizontally,
    // and diagonally (90° rotation)). Note: this function has multiple return
    // statements. I try to avoid that, but doing so here made it 9 lines longer.
    static bool isDiskSymmetric(const cv::Mat& mat)
    {
        if (mat.cols != mat.rows) { return false; }
        auto pixels_differ = [&](int x0, int y0, int x1, int y1)
        {
            auto pixel0 = mat.at<cv::Vec3b>(cv::Point(x0, y0));
            auto pixel1 = mat.at<cv::Vec3b>(cv::Point(x1, y1));
            return pixel0 != pixel1;
        };
        // Check for x/horizontal symmetry.
        for (int y = 0; y < mat.rows; y++)
        {
            int x0 = 0;
            int x1 = mat.rows - 1;
            do
            {
                if (pixels_differ(x0, y, x1, y)) { return false; }
                x0++;
                x1--;
            }
            while (x0 < x1);
        }
        // Check for y/vertical symmetry.
        for (int x = 0; x < mat.rows; x++)
        {
            int y0 = 0;
            int y1 = mat.rows - 1;
            do
            {
                if (pixels_differ(x, y0, x, y1)) { return false; }
                y0++;
                y1--;
            }
            while (y0 < y1);
        }
        // Check for 90°/diagonal symmetry.
        for (int p = 0; p < mat.rows; p++)
        {
            for (int q = 0; q < mat.rows; q++)
            {
                if (pixels_differ(p, q, q, p)) { return false; }
            }
        }
        return true;
    }

    // Writes Texture to a file using cv::imwrite(). Generally used with JPEG
    // codec, but pathname's extension names the format to be used. Converts to
    // "24 bit" image (8 bit unsigned values for each of red, green and blue
    // channels) because most codecs do not support 3xfloat format.
    void writeToFile(int size, const std::string& pathname) const
    {
        writeToFile(size, pathname, Color(0.5, 0.5, 0.5), 0, ".png");
    }
    void writeToFile(int size,
                     const std::string& pathname,
                     Color bg_color,
                     int margin,
                     const std::string& file_type) const
    {
        // Make OpenCV Mat of type CV_8UC3 (3 by unsigned 8 bit primaries).
        cv::Mat opencv_image(size + margin * 2,
                             size + margin * 2,
                             default_opencv_mat_type_,  // normally CV_8UC3
                             cv::Scalar(255 * bg_color.b(),
                                        255 * bg_color.g(),
                                        255 * bg_color.r()));
        // Ensure cached rendering of Texture is available.
        rasterizeToImageCache(size, getDefaultRenderAsDisk());
        // Define a new image, a "pointer" to portion of opencv_image inside margin.
        cv::Mat render_target(opencv_image, cv::Rect(margin, margin, size, size));
        raster_->copyTo(render_target);
        bool ok = cv::imwrite(pathname + file_type, opencv_image);
        std::cout << (ok ? "OK " : "bad") << " write Texture: size=" << size;
        std::cout << ", margin=" << margin << ", bg_color=" << bg_color;
        std::cout << ", path=\"" << pathname + file_type << "\", " << std::endl;
    }

    
    // TODO 20230714 -- hmm these functions do not seem to have implementations
    // anywhere in TexSyn. I commented them out and they should eventually be
    // removed unless I can figure out if they have any purpose.
    //    // Reset statistics for debugging.
    //    void resetStatistics() const;
    //    // Collect statistics for debugging.
    //    void collectStatistics(Vec2 position, Color color) const;
    
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
    static void rasterizeSquare(int size, PixelFunction pixel_function)
    {
        int half = size / 2;
        for (int i = -half; i <= half; i++)
        {
            for (int j = -half; j <= half; j++)
            {
                pixel_function(i, j, Vec2(i / float(half), j / float(half)));
            }
        }
    }
    static void rasterizeDisk(int size, PixelFunction pixel_function)
    {
        int half = size / 2;
        for (int i = -half; i <= half; i++)
        {
            for (int j = -half; j <= half; j++)
            {
                float radius = std::sqrt(sq(i) + sq(j));
                if (radius <= half)
                {
                    pixel_function(i, j, Vec2(i / float(half), j / float(half)));
                }
            }
        }
    }

    
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
                               int size)
    {
        texture.displayInWindow(size, false);
        if (pathname != "") texture.writeToFile(size, pathname);
    }

    
    // Wait for "any key" to be pressed. Typically used after a call to e.g.
    // displayAndFile(). Just a wrapper around the corresponding OpenCV utility.
    // Allows an optional "delay_in_milliseconds" where zero means forever,
    // otherwise returns after that delay regardless of key presses.
    static void waitKey()
    {
        waitKey(0);
    }
    static void waitKey(int delay_in_milliseconds)
    {
        cv::waitKey(delay_in_milliseconds);
    }

    // Close all open OpenCV windows.
    static void closeAllWindows()
    {
        cv::destroyAllWindows();
        window_counter = 0;
        window_x = 0;
        window_y = 0;
    }

    // BACKWARD_COMPATIBILITY reference to new "disposable" Uniform object. This
    // is called ONLY from constructors providing backward compatibility. The
    // tiny Uniform texture object is allowed to "memory leak" for ease of use.
    static Texture& disposableUniform(Color color);
    
    // Compare textures, print stats, optional file, display inputs and AbsDiff.
    static void diff(const Texture& t0, const Texture& t1)
        { diff(t0, t1, "", getDiffSize()); }
    static void diff(const Texture& t0, const Texture& t1, std::string pathname)
        { diff(t0, t1, pathname, getDiffSize()); }
    static void diff(const Texture& t0,
                     const Texture& t1,
                     std::string pathname,
                     int size)
        { diff(t0, t1, pathname, getDiffSize(), false); }
    // Special utility for Texture::diff() implementation defined in Operators.h
    // since it uses functionality from there.
    static void diff(const Texture& t0, const Texture& t1,
                     std::string pathname, int size, bool binary);

    // Display row of three textures, at given size, optionally saving to file.
    // Special utility for Texture::diff() maybe refactor to be more general?
    static void displayAndFile3(const Texture& t1,
                                const Texture& t2,
                                const Texture& t3,
                                std::string pathname,
                                int size)
    {
        // Make OpenCV Mat instance of type CV_8UC3 which is size*3 x size pixels.
        cv::Mat mat(size, size * 3, default_opencv_mat_type_);//normally CV_8UC3
        // Function to handle each Texture.
        auto subwindow = [&](const Texture& t, int x)
        {
            // Render Texture to its raster_ cv::Mat.
            t.rasterizeToImageCache(size, getDefaultRenderAsDisk());
            // Define a size*size portion of "mat" whose left edge is at "x".
            cv::Mat submat = cv::Mat(mat, cv::Rect(x, 0, size, size));
            // Copy into submat while conveting from rgb float to rgb uint8_t
            t.raster_->copyTo(submat);
        };
        subwindow(t1, 0);
        subwindow(t2, size);
        subwindow(t3, size * 2);
        // Write "mat" to file if non-empty "pathname" given.
        std::string file_type = ".png";  // Maybe should be an optional parameter?
        if (pathname != "") cv::imwrite(pathname + file_type, mat);
        // Display "mat" in the TexSyn fashion.
        windowPlacementTool(mat);
    }

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
    // Get/set global switch for "parallel render": one thread per row.
    static int getParallelRender() { return render_thread_per_row_; }
    static void setParallelRender(bool p) { render_thread_per_row_ = p; }
    
    // Count nesting of certain "expensive_to_nest" convolution operators.
    static void resetExpensiveToNest() { expensive_to_nest_ = 0; }
    static void incrementExpensiveToNest() { expensive_to_nest_++; }
    static void decrementExpensiveToNest() { expensive_to_nest_--; }
    static void setMaxExpensiveNest(int m) { max_expensive_nest_ = m; }
    static bool tooExpensiveToNest() { return (expensive_to_nest_ >
                                               max_expensive_nest_); }

    // Experimental utility. Maybe to be deleted or moved?
    // Score a Texture on how much "high" frequency it has.
    // TODO temp? Similar in intent to wiggliness() in GP.h
    void fft_test() // const
    {
        Texture::rasterizeToImageCache(201, false);
        cv::Mat monochrome;
        cv::cvtColor(*raster_, monochrome, cv::COLOR_BGR2GRAY);
        cv::imshow("monochrome", monochrome);
        
        // Complex plane to contain the DFT coefficients {[0]-Real,[1]-Img}
        cv::Mat zeros = cv::Mat::zeros(monochrome.size(), CV_32F);
        std::vector<cv::Mat> planes = { monochrome, zeros };
        cv::Mat complexI;
        cv::merge(planes, complexI);
        
        // Applying DFT
        cv::dft(complexI, complexI);
        
        // Reconstructing original image from the DFT coefficients
        cv::Mat invDFT;
        // Applying IDFT
        cv::idft(complexI, invDFT, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT );
        cv::imshow("DFT-iDFT reconstruction", invDFT);
        
        // Split the image into different channels
        std::vector<cv::Mat> fftChannels(2);
        split(complexI, fftChannels);
        cv::imshow("DFT real part", fftChannels[0]);
        cv::imshow("DFT imginary part", fftChannels[1]);
        
        cv::Mat& real = fftChannels[0];
        int width = real.rows;
        int y = width / 2;
        
        for (int x = width / 2; x < width; x++)
        {
            std::cout << real.at<float>(y, x) << " ";
        }
        std::cout << std::endl << std::endl;
        for (int x = width / 2; x < width; x++)
        {
            std::cout << int(10 * real.at<float>(y, x)) << " ";
        }
        std::cout << std::endl << std::endl;
        
        debugPrint(highFrequencyScore());
    }
    float highFrequencyScore()
    {
        // TODO maybe cache the rendered image used here, or just case the score?
        Timer t("    highFrequencyScore");
        float score = cached_high_frequency_score_;
        if (score == 0)
        {
            // Render this texture to monochrome (square image, size x size).
            int size = 101;
            cv::Mat temp = *raster_;  // Save raster_
            Texture::rasterizeToImageCache(size, false);
            cv::Mat monochrome;
            cv::cvtColor(*raster_, monochrome, cv::COLOR_BGR2GRAY);
            // restore raster_
            *raster_ = temp;
            
            // Complex plane to contain the DFT coefficients {[0]-Real,[1]-Img}
            cv::Mat complexI;
            cv::Mat zeros = cv::Mat::zeros(monochrome.size(), CV_32F);
            std::vector<cv::Mat> planes = { monochrome, zeros };
            cv::merge(planes, complexI);
            
            // Applying DFT
            cv::dft(complexI, complexI);
            
            // Split the image into different channels
            std::vector<cv::Mat> fftChannels(2);
            split(complexI, fftChannels);
            
            cv::Mat& real = fftChannels[0];
            int width = real.rows;
            float half_width = width * 0.5;
            Vec2 center(half_width, half_width);
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < width; y++)
                {
                    float length = (Vec2(x, y) - center).length();
                    float weight = length / half_width;
                    if (weight < 1)
                    {
                        float real_part = real.at<float>(y, x);
                        score += std::abs(real_part * weight);
                    }
                }
            }
            cached_high_frequency_score_ = score;
        }
        return score;
    }

    // Optional cache of 100 colors randomly sampled in unit-diameter disk.
    const std::vector<Color>& cachedRandomColorSamples(RandomSequence& rs)
    {
        if (cached_random_color_samples_.empty())
        {
            int n = 10;
            std::vector<Vec2> positions;
            jittered_grid_NxN_in_square(n, 1.4, rs, positions);
            for (auto& p : positions)
            {
                Color sample = getColor(p).clipToUnitRGB();
                cached_random_color_samples_.push_back(sample);
            }
        }
        return cached_random_color_samples_;
    }

    
    
    // Print report on constructor vs. destructor count, eg at end of run.
    static void leakCheck()
    {
        std::cout << "Texture";
        std::cout << ": constructions=" << constructor_count_;
        std::cout << ", destructions=" << destructor_count_;
        std::cout << ", leaked=" << constructor_count_ - destructor_count_;
        std::cout << std::endl;
    }
    void constructor_validity_util()
    {
        constructor_count_++;
    }
    void destructor_validity_util()
    {
        assert("already invalid at top of ~Texture" && valid());
        markAsInvalid();
        destructor_count_++;
    }
    static int getDefaultOpencvMatType() { return default_opencv_mat_type_; }
    static void setDefaultOpencvMatType(int opencv_mat_type)
        { default_opencv_mat_type_ = opencv_mat_type; }
    // TODO 20201204 experiment-- expose a Texture's cv::mat
    const cv::Mat& getCvMat() const { return *raster_; }
    
    // Return a "submat"/"ROI" reference into a portion of a given cv::Mat.
    static cv::Mat getCvMatRect(const Vec2& upper_left_position,
                                const Vec2& size_in_pixels,
                                const cv::Mat cv_mat)
    {
        {
            // Try for slightly more readable error messages than OpenCV's.
            Vec2 ul = upper_left_position;
            Vec2 lr = upper_left_position + size_in_pixels;
            assert("rect extends off left edge" && (ul.x() >= 0));
            assert("rect extends off top edge" && (ul.y() >= 0));
            assert("rect extends off right edge" && (lr.x() <= cv_mat.cols));
            assert("rect extends off bottom edge" && (lr.y() <= cv_mat.rows));
        }
        return cv::Mat(cv_mat,
                       cv::Rect(upper_left_position.x(),
                                upper_left_position.y(),
                                size_in_pixels.x(),
                                size_in_pixels.y()));
    }
    
    // Perform occasional checks for user input (such as clicks, key presses,
    // command keys, such as to hide or unhide a GUI window). Does this via OpenCV's
    // waitKey() but takes care not to do too frequently. But if it does not call
    // cv::waitKey() it will at least do a yield().
    //
    // TODO
    //     maybe move this to Utilities.h ?
    //     perhaps have global hook which Texture can specialize for OpenCV ?
    static inline TimePoint time_of_last_user_input_check = TimeClock::now();
    static inline std::mutex check_user_input_mutex;
    static void checkForUserInput()
    {
        const std::lock_guard<std::mutex> lock(check_user_input_mutex);
        using namespace std::chrono_literals;
        TimeDuration since_last = TimeClock::now() - time_of_last_user_input_check;
        TimeDuration target_delay(250ms);
        if (since_last > target_delay)
        {
            // Pause for 1/1000 second (1 millisecond) return key pressed.
            int key = cv::waitKey(1);
            if (key > 0) { setLastKeyPushed(key); }
            if (key > 0) { debugPrint(key); }  // TODO TEMP
            time_of_last_user_input_check = TimeClock::now();
        }
        else
        {
            yield();
        }
    }

    
    
    
    static inline int last_key_read_;
    static int getLastKeyPushed() { return last_key_read_; }
    static void setLastKeyPushed(int key) { last_key_read_ = key; }
    // TODO maybe there should be a hook, a void() std::function, to handle
    // per-app key commands?
    
    // Global switch: should subclassed textures that use RandomSequence objects
    // seed them from consturctor args, or from a constant "random" value. The
    // latter case is used only in SimpleImageMatch ("to avoid huge differences
    // for tiny parameter mutation.").
    static bool getSeedFromHashedArgs() { return seed_from_hashed_args_; }
    static void setSeedFromHashedArgs(bool h) { seed_from_hashed_args_ = h; }

    class RasterizeHelper
    {
    public:
        RasterizeHelper(int size_) : RasterizeHelper(0, size_) {}
        RasterizeHelper(int j_, int size_) : RasterizeHelper(j_, size_, true) {}
        RasterizeHelper(int size_, bool disk_) : RasterizeHelper(0,size_,disk_){}
        RasterizeHelper(int j_, int size_, bool disk_)
            : j(j_), size(size_), disk(disk_)
        {
            odd = size % 2;                        // Is size (diameter) odd?
            half = size / 2;                       // Half size (radius) as int
            row_index = j + half + (odd ? 0 : -1); // Which row? [0, size-1]
            row_y = odd ? j : j - 0.5;             // Row center as float
            
            // Index (signed) of first and last pixel of disk on this row.
            last_pixel_index =
                (disk ?
                 (std::sqrt(sq(size * 0.5) - sq(row_y)) - (odd ? 0 : 0.5)) :
                 (odd ? half : half - 1));
            first_pixel_index = -last_pixel_index + (odd ? 0 : -1);
            
            // TODO should these be top / bot_row_index?
            bot_j = half;
            top_j = -bot_j + (odd ? 0 : 1);
            
            row_rect_x = half + first_pixel_index;
            row_rect_w = -first_pixel_index + last_pixel_index + 1;
            
            //std::cout << "RR:";
            //std::cout << " j=" << j;
            //std::cout << " size=" << size;
            //std::cout << " disk=" << disk;
            //std::cout << " odd=" << odd;
            //std::cout << " half=" << half;
            //std::cout << " row_index=" << row_index;
            //std::cout << " row_y=" << row_y;
            //std::cout << " first_pixel_index=" << first_pixel_index;
            //std::cout << " last_pixel_index=" << last_pixel_index;
            //std::cout << " top_j=" << top_j;
            //std::cout << " bot_j=" << bot_j;
            //std::cout << " row_rect_x=" << row_rect_x;
            //std::cout << " row_rect_w=" << row_rect_w;
            //std::cout << std::endl;
        }
        int j;
        int size;
        bool disk;
        bool odd;
        int half;
        int row_index;
        float row_y;
        int first_pixel_index;
        int last_pixel_index;
        int top_j;
        int bot_j;
        int row_rect_x;
        int row_rect_w;
    };

    // Static utility function to measure the "uniformity" of a cv::Mat.
    // Returns a float between 1 (when every pixel is identical) and 0 (when the
    // bounding box of all pixels covers the entire RGB gamut).
    // (TODO unimplemented optional arg "samples" says to examine only that many
    // pixels by random sampling, as in eg SimpleImageMatch::imageUniformity())
    static float matUniformity(const cv::Mat& cv_mat)
    {
        return matUniformity(cv_mat, 0);
    }

    static float matUniformity(const cv::Mat& cv_mat, int samples)
    {
        // TODO temp for debugging:
        assert(samples == 0);
        
        float r_max = 0;
        float g_max = 0;
        float b_max = 0;
        float r_min = 1;
        float g_min = 1;
        float b_min = 1;
        for (int y = 0; y < cv_mat.rows; y++)
        {
            for (int x = 0; x < cv_mat.cols; x++)
            {
                cv::Vec3b p = cv_mat.at<cv::Vec3b>(cv::Point(x, y));
                float r = p[2] / 255.0f;
                float g = p[1] / 255.0f;
                float b = p[0] / 255.0f;
                r_max = std::max(r, r_max);
                r_min = std::min(r, r_min);
                g_max = std::max(g, g_max);
                g_min = std::min(g, g_min);
                b_max = std::max(b, b_max);
                b_min = std::min(b, b_min);
            }
        }
        return 1 - std::max(r_max - r_min, std::max(g_max - g_min, b_max - b_min));
    };

    // Static utility function to read a pixel of a cv::Mat as a TexSyn Color.
    // Note: assumes CV_8UC3, but I'm reluctant to use assert for a pixel op.
    static Color matPixelRead(const cv::Mat& cv_mat, Vec2 pixel_pos)
    {
        cv::Point cv_point(pixel_pos.x(), pixel_pos.y());
        cv::Vec3b cv_vec3b = cv_mat.at<cv::Vec3b>(cv_point);
        Color color(cv_vec3b[2], cv_vec3b[1], cv_vec3b[0]);
        return color / 255;
    }
    
    // Static utility function to write a pixel to a cv::Mat from a Color.
    // Note: assumes CV_8UC3, but I'm reluctant to use assert for a pixel op.
    static void matPixelWrite(cv::Mat& cv_mat, Vec2 pixel_pos, Color color)
    {
        cv::Point cv_point(pixel_pos.x(), pixel_pos.y());
        color *= 255;
        cv::Vec3b cv_vec3b(color.b(), color.g(), color.r());
        cv_mat.at<cv::Vec3b>(cv_point) = cv_vec3b;
    }

    // Optional texture render timeout. Defaults to infinite.
    // Get/set max time, in seconds allowed for any texture render.
    static float getRenderMaxTime() { return render_max_time_; }
    static void setRenderMaxTime(float max_time) { render_max_time_ = max_time; }
    // Time in seconds since render began.
    static float elapsedRenderTime()
        { return time_diff_in_seconds(render_start_time_, TimeClock::now()); }
    // Has max render time been exceeded?
    static bool renderTimeOut()
        { return elapsedRenderTime() > getRenderMaxTime(); }
    // Return 2.2, TexSyn's default output gamma, intended to approximate
    // the nonlinearity of sRGB (digital display screen) color space.
    // (Made settable for testing/debugging.)
    static float defaultGamma() { return default_gamma_; }
    static void setDefaultGamma(float gamma) { default_gamma_ = gamma; }

private:
    static inline const int validity_key_ = 1234567890;
    static inline int invalid_instance_counter_ = 0;
    int valid_top_ = validity_key_;

    // Allocate a generic, empty, cv::Mat. Optionally used for rasterization.
    std::shared_ptr<cv::Mat> emptyCvMat() const
    {
        return std::make_shared<cv::Mat>();
    }

    std::shared_ptr<cv::Mat> raster_;
    // Optional cache of 100 colors randomly sampled in unit-diameter disk.
    std::vector<Color> cached_random_color_samples_;
    float cached_high_frequency_score_ = 0;
    // Global default render size.
    static inline int render_size_ = 511;
    // Global default "render as disk" flag: render disk if true, else square.
    static inline bool render_as_disk_ = true;
    // Global for one-thread-per-row rendering.
    static inline bool render_thread_per_row_ = true;

    // Global default pixel type for "raster_" -- set to CV_8UC3 -- 24 bit BGR.
    inline static int default_opencv_mat_type_ = CV_8UC3;

    // Max time, in seconds allowed for texture render, return black if exceeded.
    static inline float render_max_time_ = std::numeric_limits<float>::infinity();
    // TODO 20220522 this render start time SHOULD be per-instance, but stupidly
    // rasterizeToImageCache() is const.
    static inline TimePoint render_start_time_;
    // Start render timer.
    static void startRenderTimer() { render_start_time_ = TimeClock::now(); }
    // Global switch should subclassed textures that use RandomSequence objects
    // seed them from consturctor args, or from a constant "random" value. The
    // latter case is used only in SimpleImageMatch ("to avoid huge differences
    // for tiny parameter mutation.").
    // Count nesting of certain "expensive_to_nest" convolution operators.
    thread_local static inline int expensive_to_nest_ = 0;
    static inline int max_expensive_nest_ = 0;
    static inline bool seed_from_hashed_args_ = true;
    static inline int constructor_count_ = 0;
    static inline int destructor_count_ = 0;
    // Could be per-instance (non-static) but it has only been used for testing.
    static inline float default_gamma_ = 2.2;
    int valid_bot_ = validity_key_ / 2;
};
