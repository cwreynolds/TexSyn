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
    // Make a 3-float OpenCV Mat instance
    cv::Mat opencv_image(size, size, CV_32FC3, cv::Scalar(0.5, 0.5, 0.5));
    // Rasterize this texture into OpenCV image.
    rasterizeToImage(size, true, opencv_image);
    // Display in pop-up window.
    static int window_counter = 0;
    static int window_position = 0;
    std::string window_name = "TexSyn" + std::to_string(window_counter++);
    cv::namedWindow(window_name);       // Create a window for display.
    int tm = 23;  // TODO approximate top margin height
    cv::moveWindow(window_name, window_position, window_position + size + tm);
    window_position += tm;
    cv::imshow(window_name, opencv_image);  // Show our image inside it.
    if (wait) waitKey();  // Wait for a keystroke in the window.
}

// Rasterize this texture into a size² OpenCV image. Arg "disk" true means
// draw a round image, otherwise a square. Run parallel threads for speed.
void Texture::rasterizeToImage(int size, bool disk, cv::Mat& opencv_image) const
{
    // Code assumes disk center is at window center, so size must be odd. (TODO)
    assert(((!disk) || (size % 2 == 1)) && "For disk, size must be odd.");
    // Synchronizes access to opencv_image by multiple row threads.
    std::mutex ocv_image_mutex;
    // Collection of all row threads. (Use clear() to remove initial threads,
    // see https://stackoverflow.com/a/38130584/1991373 )
    std::vector<std::thread> all_threads(size);
    all_threads.clear();
    // Loop bottom to top for all image rows. For each, launch a thread running
    // rasterizeRowOfDisk() to compute pixels, write them to image via mutex.
    for (int j = -(size / 2); j <= (size / 2); j++)
    {
        // This requires some unpacking. It creates a thread which is pushed
        // (using && move semantics, I think) onto the back of std::vector
        // all_row_threads. Because the initial/toplevel function of the thread
        // is a member function of this instance, it is specified as two values,
        // a function pointer AND an instance pointer. The other four values are
        // args to Texture::rasterizeRowOfDisk(row, size, disk, image, mutex).
        all_threads.push_back(std::thread(&Texture::rasterizeRowOfDisk, this,
                                          j, size, disk,
                                          std::ref(opencv_image),
                                          std::ref(ocv_image_mutex)));
    }
    // Wait for all row threads to finish.
    for (auto& t : all_threads) t.join();
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
        // Read TexSyn Color from Texture at (i, j).
        Color color = getColorClipped(Vec2(i, j) / half);
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
    // For each pixel within the disk, get Texture's color, insert into cv::Mat.
    rasterizeDisk(size,
                  [&](int i, int j, Vec2 position)
                  {
                      // Read TexSyn Color from Texture.
                      Color color = getColorClipped(position);
                      // Make 3x8b OpenCV color, with reversed component order.
                      cv::Vec3b opencv_color(std::round(255 * color.b()),
                                             std::round(255 * color.g()),
                                             std::round(255 * color.r()));
                      // Make OpenCV location for pixel.
                      cv::Point opencv_position((size / 2) + margin + i,
                                                (size / 2) + margin - j);
                      // Write corresponding OpenCV color to pixel:
                      opencv_image.at<cv::Vec3b>(opencv_position) = opencv_color;
                  });
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Texture::total_pixels_rendered = 0;
int Texture::total_pixels_cached = 0;
int Texture::total_cache_lookups = 0;
size_t Texture::cache_size = 0;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
