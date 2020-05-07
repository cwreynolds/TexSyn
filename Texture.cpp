//
//  Texture.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Texture.h"
#include <thread>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#pragma clang diagnostic pop

// Rasterize this texture into size² OpenCV image, display in pop-up window.
void Texture::displayInWindow(int size, bool wait) const
{
    rasterizeToImageCache(size, true);
    // Display in pop-up window.
    static int window_counter = 0;
    static int window_position = 0;
    std::string window_name = "TexSyn" + std::to_string(window_counter++);
    cv::namedWindow(window_name);       // Create a window for display.
    int tm = 23;  // TODO approximate top margin height
    cv::moveWindow(window_name, window_position, window_position + size + tm);
    window_position += tm;
    cv::imshow(window_name, *raster_);  // Show our image inside it.
    if (wait) waitKey();  // Wait for a keystroke in the window.
}

// Rasterize this texture into a size² OpenCV image. Arg "disk" true means
// draw a round image, otherwise a square. Run parallel threads for speed.
void Texture::rasterizeToImageCache(int size, bool disk) const
{
    Timer t("rasterizeToImageCache");
    // If size changed, including from initial value of 0x0, generate raster.
    // (TODO also ought to re-cache if "disk" changes. Issue ignored for now.)
    if ((size != raster_->rows) || (size != raster_->cols))
    {
        // Reset our OpenCV Mat to be (size, size) with 3 floats per pixel.
        raster_->create(size, size, CV_32FC3);
        // Code assumes disk center is at window center, so size must be odd. (TODO)
        assert(((!disk) || (size % 2 == 1)) && "For disk, size must be odd.");
        
#if 0  //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        // Synchronizes access to opencv_image by multiple row threads.
        std::mutex ocv_image_mutex;
        
        //~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
        // (TODO added Apr 23, even though this branch of the if def is wrongly
        //       turned off. LotsOfSpotsBase::adjustOverlappingSpots() has same
        //       change, and it was based on this.)
        //~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
//      // Collection of all row threads. (Use clear() to remove initial threads,
//      // see https://stackoverflow.com/a/38130584/1991373 )
//      std::vector<std::thread> all_threads(size);
//      all_threads.clear();
        //~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
        // Collection of all row threads.
        std::vector<std::thread> all_threads;
        //~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

        // Loop all image rows, bottom to top. For each, launch a thread running
        // rasterizeRowOfDisk() to compute pixels, write to image via mutex.
        for (int j = -(size / 2); j <= (size / 2); j++)
        {
            // This requires some unpacking. It creates a thread which is pushed
            // (using && move semantics, I think) onto the back of std::vector
            // all_row_threads. Because the initial/toplevel thread function is
            // member function of this instance, it is specified as two values,
            // a function pointer AND an instance pointer. The other four values
            // are args to rasterizeRowOfDisk(row, size, disk, image, mutex).
            all_threads.push_back(std::thread(&Texture::rasterizeRowOfDisk, this,
                                              j, size, disk,
                                              std::ref(*raster_),
                                              std::ref(ocv_image_mutex)));
        }
        // Wait for all row threads to finish.
        for (auto& t : all_threads) t.join();
#else  //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        // A type for the pixel data structure passed to the lambda below.
        typedef cv::Point3_<float> Pixel;
        // Default background color. (Should be arg but this is just speed test)
        *raster_ = cv::Vec3f(0.5, 0.5, 0.5);
        auto get_texture_color_for_pixel = [&](Pixel& pixel, const int coords[])
        {
            Vec2 flip(coords[1], size - coords[0]);
            Vec2 position = (flip / (size / 2)) - Vec2(1, 1);
            if (!disk || (position.length() <= 1))
            {
                //~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
//                Color color = getColor(position);
//                Color color = ((use_linear && !applyGammaAtEnd()) ?
//                               getColor(position) :
//                               getColorClipped(position).gamma(final_gamma));
                Color color = ((use_linear && !applyGammaAtEnd()) ?
                               getColor(position) :
                               reGamma(getColorClipped(position)));
                //~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
                pixel.x = color.b();
                pixel.y = color.g();
                pixel.z = color.r();
            }
        };
        raster_->forEach<Pixel>(get_texture_color_for_pixel);
#endif //~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    }
}

// Rasterize the j-th row of this texture into a size² OpenCV image. Expects
// to run in its own thread, uses mutex to synchonize access to the image.
void Texture::rasterizeRowOfDisk(int j, int size, bool disk,
                                 cv::Mat& opencv_image,
                                 std::mutex& ocv_image_mutex) const
{
    // Half the rendering's size corresponds to the disk's center.
    int half = size / 2;
    // First and last pixels on j-th row of time
    int x_limit = disk ? std::sqrt(sq(half) - sq(j)) : half;
    cv::Mat row_image(1, size, CV_32FC3, cv::Scalar(0.5, 0.5, 0.5));
    for (int i = -x_limit; i <= x_limit; i++)
    {
        //~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
        // Read TexSyn Color from Texture at (i, j).
//        Color color = getColorClipped(Vec2(i, j) / half);
//        Color color = ((use_linear && !applyGammaAtEnd()) ?
//                       getColorClipped(Vec2(i, j) / half) :
//                       getColorClipped(Vec2(i, j) / half).gamma(final_gamma));
        Color color = ((use_linear && !applyGammaAtEnd()) ?
                       getColorClipped(Vec2(i, j) / half) :
                       reGamma(getColorClipped(Vec2(i, j) / half)));

        //~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
        // Make OpenCV color, with reversed component order.
        cv::Vec3f opencv_color(color.b(), color.g(), color.r());
        // Write OpenCV color to corresponding pixel on row image:
        row_image.at<cv::Vec3f>(cv::Point(half + i, 0)) = opencv_color;
    }
    // Define a new image which is a "pointer" to j-th row of opencv_image.
    cv::Mat row_in_full_image(opencv_image, cv::Rect(0, half - j, size, 1));
    // Wait to grab lock for access to image. (Lock released at end of block)
    const std::lock_guard<std::mutex> lock(ocv_image_mutex);
    // Copy line_image into the j-th row of opencv_image.
    row_image.copyTo(row_in_full_image);
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
    // Ensure cached rendering of Texture is available. (TODO "disk" arg inline)
    rasterizeToImageCache(size, true);
    // Define a new image, a "pointer" to portion of opencv_image inside margin.
    cv::Mat render_target(opencv_image, cv::Rect(margin, margin, size, size));
    // Convert 3xfloat rendered raster to 3x8bit window inside opencv_image
    raster_->convertTo(render_target, CV_8UC3, 255);
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

// Reset statistics for debugging.
void Texture::resetStatistics() const
{
    // Clear bounds of sampled positions.
    min_x = std::numeric_limits<float>::infinity();
    max_x = -std::numeric_limits<float>::infinity();
    min_y = std::numeric_limits<float>::infinity();
    max_y = -std::numeric_limits<float>::infinity();
}

// Collect statistics for debugging.
void Texture::collectStatistics(Vec2 position, Color color) const
{
    // TODO "color" currently ignored
    // Update bounds of sampled positions.
    if (min_x > position.x()) min_x = position.x();
    if (max_x < position.x()) max_x = position.x();
    if (min_y > position.y()) min_y = position.y();
    if (max_y < position.y()) max_y = position.y();
}

float Texture::min_x = std::numeric_limits<float>::infinity();
float Texture::max_x = -std::numeric_limits<float>::infinity();
float Texture::min_y = std::numeric_limits<float>::infinity();
float Texture::max_y = -std::numeric_limits<float>::infinity();

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//float Texture::final_gamma = 1 / 2.2;

//bool Texture::use_linear = true;
// TODO test "Gamma angst: shouldn’t TexSyn use linear RGB?"
bool Texture::use_linear = false;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO move this back to Color.cpp after testing

// Override general interpolate() template with a version specific to Color,
// so it can correctly handle gamma.
Color interpolate(float alpha, const Color& x0, const Color& x1)
{
//    if (Texture::use_linear)
    if (Texture::use_linear && !applyGammaAtEnd())
    {
//            float g = 2.2;
            float g = defaultGamma();
            Color a = x0.gamma(g);
            Color b = x1.gamma(g);
            Color result = a + ((b - a) * alpha);
            return result.gamma(1 / g);
    }
    else
    {
        return x0 + ((x1 - x0) * alpha);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
