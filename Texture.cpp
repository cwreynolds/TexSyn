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
    // Function to be applied to each pixel in the rasterizer.
    auto f = [&](int i, int j, Vec2 position, bool inside_radius)
    {
        if (inside_radius)
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
        }
    };
    rasterizer(size, f);
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

// Utility for the wrapper used for rasterizing Textures. Given resolution/
// size of raster: do nested loops over rows and columns, applying the
// given function at each pixel.
void Texture::rasterizer(int size,
                         std::function<void(int i, int j,
                                            Vec2 position,
                                            bool inside_radius)> pixel_function)
{
    int half = size / 2;
    for (int i = -half; i <= half; i++)
    {
        for (int j = -half; j <= half; j++)
        {
            float radius = std::sqrt(sq(i) + sq(j));
            bool inside_radius = radius <= half;
            Vec2 position(i / float(half), j / float(half));
            pixel_function(i, j, position, inside_radius);
        }
    }
}
