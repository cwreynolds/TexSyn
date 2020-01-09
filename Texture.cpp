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

// Display this Texture in a pop-up OpenCV window.
void Texture::displayInWindow(int size) const
{
    // Make a 3-float OpenCV Mat instance
    cv::Mat opencv_image(size, size, CV_32FC3, cv::Scalar(0.5, 0.5, 0.5));
    int half = size / 2;
    // Reset statistics for debugging.
    resetStatistics();
    for (int i = -half; i <= half; i++)
    {
        for (int j = -half; j <= half; j++)
        {
            float radius = std::sqrt(sq(i) + sq(j));
            if (radius <= half)
            {
                // Read TexSyn Color from Texture.
                Vec2 position(i / float(half), j / float(half));
                Color color = getColorClipped(position);
                // Make OpenCV color, with reversed component order.
                cv::Vec3f opencv_color(color.b(), color.g(), color.r());
                // Make OpenCV location for pixel.
                cv::Point opencv_position(i + half, j + half);
                // Write corresponding OpenCV color to pixel:
                opencv_image.at<cv::Vec3f>(opencv_position) = opencv_color;
                // Collect statistics
                // collectStatistics(position, color);
            }
        }
    }
    // TODO temporary for debugging/testing reconsider a more permanent version.
    debugPrint(min_x);
    debugPrint(max_x);
    debugPrint(min_y);
    debugPrint(max_y);
    cv::namedWindow("TexSyn");       // Create a window for display.
    imshow("TexSyn", opencv_image);  // Show our image inside it.
    cv::waitKey(0);                  // Wait for a keystroke in the window.
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
