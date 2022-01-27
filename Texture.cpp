//
//  Texture.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Texture.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#include <thread>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>  // TODO temp(?) for fft_test()
#include <opencv2/highgui/highgui.hpp>
#pragma clang diagnostic pop

Texture::~Texture()
{
    assert("already invalid at top of ~Texture" && valid());
    markAsInvalid();
    destructor_count_++;
}

// Utility for getColor(), special-cased for when alpha is 0 or 1.
Color Texture::interpolatePointOnTextures(float alpha,
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
void Texture::displayInWindow(int size, bool wait) const
{
    rasterizeToImageCache(size, getDefaultRenderAsDisk());
    windowPlacementTool(*raster_);
    if (wait) waitKey();  // Wait for a keystroke in the window.
}

// Display cv::Mat in pop-up window. Stack diagonally from upper left.
void Texture::windowPlacementTool(cv::Mat& mat)
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

// Rasterize this texture into a size² OpenCV image. Arg "disk" true means
// draw a round image, otherwise a square. Run parallel threads for speed.
void Texture::rasterizeToImageCache(int size, bool disk) const
{
    //Timer t("rasterizeToImageCache");
    // If size changed, including from initial value of 0x0, generate raster.
    // (TODO also ought to re-cache if "disk" changes. Issue ignored for now.)
    if ((size != raster_->rows) || (size != raster_->cols))
    {
        // Reset our OpenCV Mat to be (size, size) at default depth.
        raster_->create(size, size, getDefaultOpencvMatType());
        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//        // TODO Code assumes disk center at window center, so size must be odd.
//        assert(((!disk) || (size % 2 == 1)) && "For disk, size must be odd.");
        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        // Synchronizes access to opencv_image by multiple row threads.
        std::mutex ocv_image_mutex;
        // Collection of all row threads.
        std::vector<std::thread> all_threads;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        int row_counter = 0;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Loop all image rows, bottom to top. For each, launch a thread running
        // rasterizeRowOfDisk() to compute pixels, write to image via mutex.
        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//        for (int j = -(size / 2); j <= (size / 2); j++)
            
        
//        bool odd_size = size % 2;
//        int last_j = size / 2;
//        int first_j = -last_j + (odd_size ? 0 : 1);
//        for (int j = first_j; j <= last_j; j++)
            
        // TODO more fiddling 20211107:

        RasterizeHelper rh(size, disk);
        for (int j = rh.top_j; j <= rh.bot_j; j++)

        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//            // This requires some unpacking. It creates a thread which is pushed
//            // (using && move semantics, I think) onto the back of std::vector
//            // all_row_threads. Because the initial/toplevel thread function is
//            // member function of this instance, it is specified as two values,
//            // a function pointer AND an instance pointer. The other four values
//            // are args to rasterizeRowOfDisk(row, size, disk, image, mutex).
//            all_threads.push_back(std::thread(&Texture::rasterizeRowOfDisk, this,
//                                              j, size, disk,
//                                              std::ref(*raster_),
//                                              std::ref(ocv_image_mutex)));
            // This requires some unpacking. It creates a thread which is pushed
            // (using && move semantics, I think) onto the back of std::vector
            // all_row_threads. Because the initial/toplevel thread function is
            // member function of this instance, it is specified as two values,
            // a function pointer AND an instance pointer. The other four values
            // are args to rasterizeRowOfDisk(row, size, disk, image, mutex).
            
            //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
            
//            all_threads.push_back(std::thread(&Texture::rasterizeRowOfDisk, this,
//                                              j, size, disk,
//                                              std::ref(*raster_),
//                                              std::ref(row_counter),
//                                              std::ref(ocv_image_mutex)));

//            if (render_thread_per_row)
            if (getParallelRender())
            {
                all_threads.push_back(std::thread(&Texture::rasterizeRowOfDisk,
                                                  this,
                                                  j, size, disk,
                                                  std::ref(*raster_),
                                                  std::ref(row_counter),
                                                  std::ref(ocv_image_mutex)));
            }
            else
            {
                rasterizeRowOfDisk(j, size, disk, *raster_,
                                   row_counter, ocv_image_mutex);
                checkForUserInput();
            }
            //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        }
        // Wait for all row threads to finish.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO very experimental, for SimpleImageMatch, Aug 7, 2021
        
//        for (auto& t : all_threads)
//        {
//            t.join();
//            yield();
//            occasional_sleep.sleepIfNeeded();
//            Texture::waitKey(1);
//        }

        
        
//        for (int i = 0; i < 100; i++)
//        {
//            for (auto& t : all_threads)
//            {
//                std::cout << i << " ";
//                debugPrint(t.joinable());
//            }
//            std::this_thread::sleep_for(std::chrono::seconds(1));
//        }
//        for (auto& t : all_threads) t.join();
        
        
        // Maybe I should use std::future ?
        // https://en.cppreference.com/w/cpp/thread/future
        // https://www.cppstories.com/2014/01/tasks-with-stdfuture-and-stdasync/
        
//        for (auto& t : all_threads)
//        {
//            debugPrint(row_counter);
//            t.join();
//        }
        
        
//        debugPrint(row_counter);
//        debugPrint(all_threads.size());
        

        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        
//        while (row_counter < all_threads.size())
//        {
//            checkForUserInput();
//        }
//
//        for (auto& t : all_threads) t.join();

//        if (render_thread_per_row)
        if (getParallelRender())
        {
            while (row_counter < all_threads.size()) { checkForUserInput(); }
            for (auto& t : all_threads) t.join();
        }
//        else
//        {
//        }
        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    }
}

// Rasterize the j-th row of this texture into a size² OpenCV image. Expects
// to run in its own thread, uses mutex to synchonize access to the image.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Texture::rasterizeRowOfDisk(int j, int size, bool disk,
                                 cv::Mat& opencv_image,
                                 int& row_counter,
                                 std::mutex& ocv_image_mutex) const
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{
    // Half the rendering's size corresponds to the disk's center.
    int half = size / 2;
    
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // TODO 20211101 testing new RasterizeRow helper class
    

//    debugPrint(half);
    RasterizeHelper rh(j, size, disk);


    // TODO investigating the "crash for even render size" bug
    // TODO this is the wrong solution, but at least indicates where it happens.
    // TODO does this "fix" work for render-as-disk?
    // TODO does this work when Texture::render_thread_per_row is true?
    
//    debugPrint(j);
//    debugPrint(half);
//    debugPrint(half - j);
//    debugPrint(size);

//    if (half - j == size)
//    {
//        std::cout << "aha!" << std::endl;
//        row_counter++;
//        return;
//    }
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // TODO 20211101 switching over to using RasterizeRowHelper
    // First and last pixels on j-th row of time
//    int x_limit = disk ? std::sqrt(sq(half) - sq(j)) : half;
    
//    debugPrint(x_limit);
    
//    if ((size % 2 == 0) && (x_limit == 0)) return;
    
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    cv::Scalar gray(127, 127, 127);  // Note: assumes CV_8UC3.
    cv::Mat row_image(1, size, getDefaultOpencvMatType(), gray);
    // TODO 20211101 switching over to using RasterizeRowHelper
//    for (int i = -x_limit; i <= x_limit; i++)
    for (int i = rh.first_pixel_index; i <= rh.last_pixel_index; i++)
    {
//        std::cout << "(" << i << "," << j << ") "; ///////////////////////////
        
        
        // Read TexSyn Color from Texture at (i, j).
//        Color color(0, 0, 0);
        Vec2 pixel_center = Vec2(i, j) / half;
        expensive_to_nest = 0;
//        if (sqrt_of_aa_subsample_count > 1) // anti-alaising?
//        {
//            float pixel_radius = 2.0 / size;
//            std::vector<Vec2> offsets;
//            RandomSequence rs(pixel_center.hash());
//            jittered_grid_NxN_in_square(sqrt_of_aa_subsample_count,
//                                        pixel_radius * 2, rs, offsets);
//            for (Vec2 offset : offsets)
//                color += getColorClipped(pixel_center + offset);
//            color = color / sq(sqrt_of_aa_subsample_count);
//        }
//        else
//        {
//            color = getColorClipped(pixel_center);
//        }
        Color color = getColorClippedAntialiased(pixel_center, size);
        
        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        // TODO 20211104 super temp debugging grid
        // if ((i + j) % 2) color *= 0.6;
        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

        
        // Adjust for display gamma.
        color = color.gamma(1 / defaultGamma());
        // Make OpenCV color, with reversed component order.
        cv::Vec3b opencv_color(color.b() * 255,  // Note: assumes CV_8UC3.
                               color.g() * 255,
                               color.r() * 255);
        // Write OpenCV color to corresponding pixel on row image:
        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        // TODO 20211031 ok, this fails for even sized textures
//        assert((half + i) < size);
        
        row_image.at<cv::Vec3b>(cv::Point(half + i, 0)) = opencv_color;
//        if ((half + i) < size)
//        {
//            row_image.at<cv::Vec3b>(cv::Point(half + i, 0)) = opencv_color;
//        }
        //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        // Near midpoint of rendering this Texture row, yield to other threads,
        // to avoid locking up the whole machine during a lengthy render run.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if (i == 0) { yield(); }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    
//    std::cout << std::endl; /////////////////////////////////////////////////

    
    // Define a new image which is a "pointer" to j-th row of opencv_image.
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//    debugPrint(size);
//    debugPrint(half - j);
//    debugPrint(cv::Rect(0, half - j, size, 1))
//    debugPrint(std::to_string(size) + " " + std::to_string(half - j));
//    assert("(half - j) > 0" && (half - j) > 0);
    

//    {
//        grabPrintLock();
//        cv::Mat row_in_full_image(opencv_image, cv::Rect(0, half - j, size, 1));
//    }
    
    
    cv::Mat row_in_full_image(opencv_image, cv::Rect(0, half - j, size, 1));
    //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // Wait to grab lock for access to image. (Lock released at end of block)
    const std::lock_guard<std::mutex> lock(ocv_image_mutex);
    // Copy line_image into the j-th row of opencv_image.
    row_image.copyTo(row_in_full_image);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    row_counter++;
//    debugPrint(row_counter);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

// Copies disk-shaped portion of image cache onto given background cv::Mat.
// Normally "bg" is a CV "ROI", a "submat" of a presumably larger cv::Mat.
void Texture::matteImageCacheDiskOverBG(int size, cv::Mat& bg)
{
    // Ensure the Texture has been rendered to image cache as disk.
    rasterizeToImageCache(size, true);
    // Matte cached disk image over bg.
    matteImageCacheDiskOverBG(*raster_, bg);
}

// Copies disk-shaped portion of one cv::Mat onto a background cv::Mat.
void Texture::matteImageCacheDiskOverBG(const cv::Mat& disk, cv::Mat& bg)
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
bool Texture::isDiskSymmetric(const cv::Mat& mat)
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

Color Texture::getColorClippedAntialiased(Vec2 position, float size) const
{
    //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
    // TODO experimental 20220123
    position /= secret_render_scale_factor_;
    //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
    // Read TexSyn Color from Texture at (i, j).
    Color color(0, 0, 0);
    expensive_to_nest = 0;
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

// Display a collection of Textures, each in a window, then wait for a char.
void Texture::displayInWindow(std::vector<const Texture*> textures,
                              int size,
                              bool wait)
{
    for (auto& t : textures) t->displayInWindow(size, false);
    // Wait for keystroke, close windows, exit function.
    if (wait) waitKey();
}

// Writes Texture to a file using cv::imwrite(). Generally used with JPEG
// codec, but pathname's extension names the format to be used. Converts to
// "24 bit" image (8 bit unsigned values for each of red, green and blue
// channels) because most codecs do not support 3xfloat format.
void Texture::writeToFile(int size,
                          const std::string& pathname,
                          Color bg_color,
                          int margin,
                          const std::string& file_type) const
{
    // Make OpenCV Mat instance of type CV_8UC3 (3 by unsigned 8 bit primaries).
    cv::Mat opencv_image(size + margin * 2,
                         size + margin * 2,
                         CV_8UC3,
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

// Combines display on screen and writing file, but primary benefit is that
// this allows writing an arbitrarily nested expression of TexSyn
// constructors, whose lifetime extends across both operations.
void Texture::displayAndFile(const Texture& texture,
                             std::string pathname,
                             int size)
{
    texture.displayInWindow(size, false);
    if (pathname != "") texture.writeToFile(size, pathname);
}

void Texture::waitKey()
{
    cv::waitKey(0);
}

void Texture::waitKey(int delay_in_milliseconds)
{
    cv::waitKey(delay_in_milliseconds);
}

// close the window
void Texture::closeAllWindows()
{
    //cv::destroyWindow(name);
    //cvReleaseImage(&images[i]);
    
    cv::destroyAllWindows();
    window_counter = 0;
    window_x = 0;
    window_y = 0;
}

// Utilities for rasterizing a Texture to tiling of pixels, with versions
// for a square and a disk of pixels. Each require a "size" (width of the
// square or diameter of the disk) and a function to be applied at each
// pixel. The function's parameters are i/j (column/row) indexes of the
// pixel raster, and the corresponding Vec2 in Texture space. [DEPRECATED]
void Texture::rasterizeSquare(int size, PixelFunction pixel_function)
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
void Texture::rasterizeDisk(int size, PixelFunction pixel_function)
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

// Allocate a generic, empty, cv::Mat. Optionally used for rasterization.
std::shared_ptr<cv::Mat> Texture::emptyCvMat() const
{
    return std::make_shared<cv::Mat>();
}

// Special utility for Texture::diff() maybe refactor to be more general?
void Texture::displayAndFile3(const Texture& t1,
                              const Texture& t2,
                              const Texture& t3,
                              std::string pathname,
                              int size)
{
    // Make OpenCV Mat instance of type CV_8UC3 which is size*3 x size pixels.
    cv::Mat mat(size, size * 3, CV_8UC3);
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

void Texture::fft_test() // const
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

float Texture::highFrequencyScore()
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
const std::vector<Color>& Texture::cachedRandomColorSamples(RandomSequence& rs)
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

// Global default pixel type for "raster_" -- set to CV_8UC3 -- 24 bit BGR.
int Texture::default_opencv_mat_type_ = CV_8UC3;

// Return a "submat"/"ROI" reference into a portion of a given cv::Mat.
cv::Mat Texture::getCvMatRect(const Vec2& upper_left_position,
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO
//     maybe move this to Utilities.h ?
//     perhaps have global hook which Texture can specialize for OpenCV ?

TimePoint time_of_last_user_input_check = TimeClock::now();

//    void Texture::checkForUserInput()
//    {
//        using namespace std::chrono_literals;
//
//        TimeDuration since_last = TimeClock::now() - time_of_last_user_input_check;
//        if (since_last > TimeDuration(150ms))
//        {
//            // Pause for 1/1000 second (1 millisecond) return key pressed.
//            int key = cv::waitKey(1);;
//            if (key > 0) { setLastKeyPushed(key); }
//            if (key > 0) { debugPrint(key); }  // TODO TEMP
//            time_of_last_user_input_check = TimeClock::now();
//        }
//        else
//        {
//            yield();
//        }
//    }

//    std::mutex check_user_input_mutex;
//
//    void Texture::checkForUserInput()
//    {
//        const std::lock_guard<std::mutex> lock(check_user_input_mutex);
//
//        using namespace std::chrono_literals;
//
//        TimeDuration since_last = TimeClock::now() - time_of_last_user_input_check;
//        TimeDuration target_delay(150ms);
//        if (since_last > target_delay)
//        {
//    //        // Pause for 1/1000 second (1 millisecond) return key pressed.
//    //        int key = cv::waitKey(1);
//
//            // xxx
//    //        int key = cv::waitKey(1000 * since_last.count());
//            int key = cv::waitKey(100);
//
//
//
//            if (key > 0) { setLastKeyPushed(key); }
//            if (key > 0) { debugPrint(key); }  // TODO TEMP
//            time_of_last_user_input_check = TimeClock::now();
//        }
//        else
//        {
//            yield();
//
//
//    //        float seconds_left = since_last.count();
//    //        int ms_left = 1000 * seconds_left;
//
//
//
//        }
//    }


std::mutex check_user_input_mutex;

//    void Texture::checkForUserInput()
//    {
//        const std::lock_guard<std::mutex> lock(check_user_input_mutex);
//
//        using namespace std::chrono_literals;
//
//        TimeDuration since_last = TimeClock::now() - time_of_last_user_input_check;
//    //    TimeDuration target_delay(150ms);
//        TimeDuration target_delay(250ms);
//        if (since_last > target_delay)
//    //    if (since_last > TimeDuration(10ms))
//        {
//            // Pause for 1/1000 second (1 millisecond) return key pressed.
//            int key = cv::waitKey(1);
//    //        // xxx
//    //        int key = cv::waitKey(240);
//
//            if (key > 0) { setLastKeyPushed(key); }
//            if (key > 0) { debugPrint(key); }  // TODO TEMP
//            time_of_last_user_input_check = TimeClock::now();
//        }
//        else
//        {
//            yield();
//        }
//    }

void Texture::checkForUserInput()
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

//void Texture::checkForUserInput()
//{
//    yield();
//}

//void Texture::checkForUserInput()
//{
//    // Pause for 1/1000 second (1 millisecond) return key pressed.
//    int key = cv::waitKey(1);
//    if (key > 0) { setLastKeyPushed(key); }
//    if (key > 0) { debugPrint(key); }  // TODO TEMP
//}

//    void Texture::checkForUserInput()
//    {
//    //    const std::lock_guard<std::mutex> lock(check_user_input_mutex);
//        // Pause for 1/10 second (100 millisecond) return key pressed.
//        int key = cv::waitKey(100);
//        if (key > 0) { setLastKeyPushed(key); }
//        if (key > 0) { debugPrint(key); }  // TODO TEMP
//    //    yield();
//    }


/*
 
 // WITHOUT calls to Texture::checkForUserInput()
 
 1: t=9.37e+05, pop ave size=81 fit=0.000783486, pop best (0.06 0.034 0 0 0 0 0 0 0 0)
 2: t=1.13, pop ave size=81 fit=0.00128124, pop best (0.06 0.052 0.034 0.0078 0 0 0 0 0 0)
 3: t=5.91, pop ave size=81 fit=0.00178439, pop best (0.06 0.052 0.034 0.031 0.03 0.0078 0 0 0 0)
 4: t=12.4, pop ave size=81 fit=0.00202186, pop best (0.06 0.052 0.034 0.031 0.03 0.015 0.013 0.0078 0 0)
 5: t=12.7, pop ave size=81 fit=0.00250596, pop best (0.06 0.052 0.034 0.034 0.031 0.03 0.024 0.015 0.013 0.0078)
 6: t=59.5, pop ave size=81 fit=0.00333497, pop best (0.072 0.06 0.052 0.034 0.034 0.031 0.03 0.027 0.024 0.015)
 7: t=37.4, pop ave size=81 fit=0.00341107, pop best (0.072 0.06 0.052 0.034 0.034 0.031 0.03 0.027 0.024 0.015)
 8: t=0.887, pop ave size=82 fit=0.00412081, pop best (0.072 0.069 0.06 0.052 0.034 0.034 0.031 0.03 0.027 0.024)
 9: t=0.962, pop ave size=82 fit=0.00417767, pop best (0.072 0.069 0.06 0.052 0.034 0.034 0.031 0.03 0.027 0.024)
 10: t=17, pop ave size=82 fit=0.00466689, pop best (0.072 0.069 0.06 0.052 0.044 0.034 0.034 0.031 0.03 0.028)

 // WITH calls to Texture::checkForUserInput()
 
 1: t=9.37e+05, pop ave size=81 fit=0.000783486, pop best (0.06 0.034 0 0 0 0 0 0 0 0)
 2: t=1.1, pop ave size=81 fit=0.00128124, pop best (0.06 0.052 0.034 0.0078 0 0 0 0 0 0)
 3: t=5.93, pop ave size=81 fit=0.00178439, pop best (0.06 0.052 0.034 0.031 0.03 0.0078 0 0 0 0)
 4: t=12.5, pop ave size=81 fit=0.00202186, pop best (0.06 0.052 0.034 0.031 0.03 0.015 0.013 0.0078 0 0)
 5: t=12.8, pop ave size=81 fit=0.00250596, pop best (0.06 0.052 0.034 0.034 0.031 0.03 0.024 0.015 0.013 0.0078)
 6: t=58.9, pop ave size=81 fit=0.00333497, pop best (0.072 0.06 0.052 0.034 0.034 0.031 0.03 0.027 0.024 0.015)
 7: t=37.2, pop ave size=81 fit=0.00341107, pop best (0.072 0.06 0.052 0.034 0.034 0.031 0.03 0.027 0.024 0.015)
 8: t=0.864, pop ave size=82 fit=0.00412081, pop best (0.072 0.069 0.06 0.052 0.034 0.034 0.031 0.03 0.027 0.024)
 9: t=0.948, pop ave size=82 fit=0.00417767, pop best (0.072 0.069 0.06 0.052 0.034 0.034 0.031 0.03 0.027 0.024)
 10: t=16.7, pop ave size=82 fit=0.00466689, pop best (0.072 0.069 0.06 0.052 0.044 0.034 0.034 0.031 0.03 0.028)

 */



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
// TODO experimental 20220127

// Returns a measure of non-unifornity, a float on [0, 1].
// TODO Didn't I write this before somewhere?
// TODO Assumes image is "24 bit RGB" (well BGR since it is OpenCV).

// a static function something like `Texture::matUniformity(const cv::Mat& mat, int samples = 0)` — returns float — **0** for mat that spans the RGB gamut to **1** for all pixels equal — where `samples==0` means look at all pixels.


float Texture::matUniformity(const cv::Mat& cv_mat, int samples)
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

//~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
