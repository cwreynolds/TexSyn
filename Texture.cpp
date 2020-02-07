//
//  Texture.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Texture.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#pragma clang diagnostic pop

// Display this Texture in a pop-up OpenCV window, wait for key, then close.
void Texture::displayInWindow(int size, bool wait) const
{
    // Make a 3-float OpenCV Mat instance
    cv::Mat opencv_image(size, size, CV_32FC3, cv::Scalar(0.5, 0.5, 0.5));
    // Reset statistics for debugging.
    resetStatistics();
    // For each pixel within the disk, get Texture's color, insert into cv::Mat.
    rasterizeDisk(size,
                  [&](int i, int j, Vec2 position)
                  {
                      // Read TexSyn Color from Texture.
                      Color color = getColorClipped(position);
                      // Make OpenCV color, with reversed component order.
                      cv::Vec3f opencv_color(color.b(), color.g(), color.r());
                      // Make OpenCV location for pixel.
                      cv::Point opencv_position((size / 2) + i, (size / 2) - j);
                      // Write corresponding OpenCV color to pixel:
                      opencv_image.at<cv::Vec3f>(opencv_position) = opencv_color;
                      // Collect statistics
                      // collectStatistics(position, color);
                  });
    // TODO temporary for debugging/testing reconsider a more permanent version.
//    debugPrint(min_x);
//    debugPrint(max_x);
//    debugPrint(min_y);
//    debugPrint(max_y);
    static int window_counter = 0;
    static int window_position = 0;
    std::string window_name = "TexSyn" + std::to_string(window_counter++);
    cv::namedWindow(window_name);       // Create a window for display.
    cv::moveWindow(window_name, window_position, window_position);
    window_position += 30;
    cv::imshow(window_name, opencv_image);  // Show our image inside it.
    if (wait) cv::waitKey(0);        // Wait for a keystroke in the window.
}

// Display a collection of Textures, each in a window, then wait for a char.
void Texture::displayInWindow(std::vector<const Texture*> textures,
                              int size,
                              bool wait)
{
    for (auto& t : textures) t->displayInWindow(size, false);
    // Wait for keystroke, close windows, exit function.
    if (wait) cv::waitKey(0);
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
// pixel raster, and the corresponding Vec2 in Texture space.
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
