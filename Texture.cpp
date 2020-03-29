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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO prototyping for multi-threaded rasterization. Initially try simplifying
//      by removing rasterizeDisk() protocol, and going back to inline loops.

//    // Display this Texture in a pop-up OpenCV window, wait for key, then close.
//    void Texture::displayInWindow(int size, bool wait) const
//    {
//        // Make a 3-float OpenCV Mat instance
//        cv::Mat opencv_image(size, size, CV_32FC3, cv::Scalar(0.5, 0.5, 0.5));
//        // Reset statistics for debugging.
//        resetStatistics();
//        // For each pixel within the disk, get Texture's color, insert into cv::Mat.
//        rasterizeDisk(size,
//                      [&](int i, int j, Vec2 position)
//                      {
//                          // Read TexSyn Color from Texture.
//                          Color color = getColorClipped(position);
//                          // Make OpenCV color, with reversed component order.
//                          cv::Vec3f opencv_color(color.b(), color.g(), color.r());
//                          // Make OpenCV location for pixel.
//                          cv::Point opencv_position((size / 2) + i, (size / 2) - j);
//                          // Write corresponding OpenCV color to pixel:
//                          opencv_image.at<cv::Vec3f>(opencv_position) = opencv_color;
//                          // Collect statistics
//                          // collectStatistics(position, color);
//                      });
//        // TODO temporary for debugging/testing reconsider a more permanent version.
//    //    debugPrint(min_x);
//    //    debugPrint(max_x);
//    //    debugPrint(min_y);
//    //    debugPrint(max_y);
//        static int window_counter = 0;
//        static int window_position = 0;
//        std::string window_name = "TexSyn" + std::to_string(window_counter++);
//        cv::namedWindow(window_name);       // Create a window for display.
//        cv::moveWindow(window_name, window_position, window_position);
//        window_position += 30;
//        cv::imshow(window_name, opencv_image);  // Show our image inside it.
//        if (wait) cv::waitKey(0);        // Wait for a keystroke in the window.
//    }

//    // Display this Texture in a pop-up OpenCV window, wait for key, then close.
//    void Texture::displayInWindow(int size, bool wait) const
//    {
//        // Make a 3-float OpenCV Mat instance
//        cv::Mat opencv_image(size, size, CV_32FC3, cv::Scalar(0.5, 0.5, 0.5));
//        // Reset statistics for debugging.
//        resetStatistics();
//
//    //    // For each pixel within the disk, get Texture's color, insert into cv::Mat.
//    //    rasterizeDisk(size,
//    //                  [&](int i, int j, Vec2 position)
//    //                  {
//    //                      // Read TexSyn Color from Texture.
//    //                      Color color = getColorClipped(position);
//    //                      // Make OpenCV color, with reversed component order.
//    //                      cv::Vec3f opencv_color(color.b(), color.g(), color.r());
//    //                      // Make OpenCV location for pixel.
//    //                      cv::Point opencv_position((size / 2) + i, (size / 2) - j);
//    //                      // Write corresponding OpenCV color to pixel:
//    //                      opencv_image.at<cv::Vec3f>(opencv_position) = opencv_color;
//    //                      // Collect statistics
//    //                      // collectStatistics(position, color);
//    //                  });
//
//        // TODO TEMP -- not quite sure what to do in this case, but for now just
//        //              require that size is an odd number. Maybe only in the case
//        //              of rasterizing a disk
//        assert(size % 2 == 1 && "size needs to be odd (TODO)");
//
//        // For each pixel within the disk, get Texture's color, insert into cv::Mat.
//        int half = size / 2;
//
//    //    debugPrint(half);  // TODO TEMP
//
//    //    for (int i = -half; i <= half; i++)
//        for (int j = -half; j <= half; j++)
//        {
//
//            // TODO TEMP
//            int min_i = std::numeric_limits<int>::max();
//            int max_i = std::numeric_limits<int>::min();
//    //        std::cout << std::endl;
//
//    //        for (int j = -half; j <= half; j++)
//            for (int i = -half; i <= half; i++)
//            {
//                float radius = std::sqrt(sq(i) + sq(j));
//
//    //            // TODO TEMP
//    //            std::cout << ((radius <= half) ? "+" : "-") << " ";
//    //            std::cout << "i,j:" << i << "," << j << " r:" << radius << std::endl;
//
//                if (radius <= half)
//                {
//                    // TODO TEMP
//                    if (min_i > i) min_i = i;
//                    if (max_i < i) max_i = i;
//
//                    Vec2 position(i / float(half), j / float(half));
//
//                    // Read TexSyn Color from Texture.
//                    Color color = getColorClipped(position);
//                    // Make OpenCV color, with reversed component order.
//                    cv::Vec3f opencv_color(color.b(), color.g(), color.r());
//                    // Make OpenCV location for pixel.
//                    cv::Point opencv_position((size / 2) + i, (size / 2) - j);
//                    // Write corresponding OpenCV color to pixel:
//                    opencv_image.at<cv::Vec3f>(opencv_position) = opencv_color;
//                    // Collect statistics
//                    // collectStatistics(position, color);
//                }
//            }
//
//            // TODO TEMP
//    //        debugPrint(min_i);
//    //        debugPrint(max_i);
//    //        int xss = std::sqrt((half * half) - (j * j));
//    //        debugPrint(xss);
//
//            int xss = std::sqrt((half * half) - (j * j));
//            assert(xss == max_i && "error predicting x_start_stop");
//
//
//        }
//
//
//        static int window_counter = 0;
//        static int window_position = 0;
//        std::string window_name = "TexSyn" + std::to_string(window_counter++);
//        cv::namedWindow(window_name);       // Create a window for display.
//        //~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~
//    //    cv::moveWindow(window_name, window_position, window_position);
//    //    window_position += 30;
//
//        int tm = 23;  // TODO approximate top margin height
//        cv::moveWindow(window_name, window_position, window_position + size + tm);
//        window_position += tm;
//        //~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~
//        cv::imshow(window_name, opencv_image);  // Show our image inside it.
//        if (wait) cv::waitKey(0);        // Wait for a keystroke in the window.
//    }


// Display this Texture in a pop-up OpenCV window, wait for key, then close.
void Texture::displayInWindow(int size, bool wait) const
{
    // Make a 3-float OpenCV Mat instance
    cv::Mat opencv_image(size, size, CV_32FC3, cv::Scalar(0.5, 0.5, 0.5));
    
    // TODO TEMP -- not quite sure what to do in this case, but for now just
    //              require that size is an odd number. Maybe only in the case
    //              of rasterizing a disk
    assert(size % 2 == 1 && "size needs to be odd (TODO)");
    
    // For each pixel within the disk, get Texture's color, insert into cv::Mat.
    int half = size / 2;
    for (int j = -half; j <= half; j++)
    {
        
//        int x_start_stop = std::sqrt(sq(half) - sq(j));
//        for (int i = -half; i <= half; i++)
//        {
//            float radius = std::sqrt(sq(i) + sq(j));
//            if (radius <= half)
//            {
//                Vec2 position(i / float(half), j / float(half));
//
//                // Read TexSyn Color from Texture.
//                Color color = getColorClipped(position);
//                // Make OpenCV color, with reversed component order.
//                cv::Vec3f opencv_color(color.b(), color.g(), color.r());
//                // Make OpenCV location for pixel.
//                cv::Point opencv_position((size / 2) + i, (size / 2) - j);
//                // Write corresponding OpenCV color to pixel:
//                opencv_image.at<cv::Vec3f>(opencv_position) = opencv_color;
//                // Collect statistics
//                // collectStatistics(position, color);
//            }
//        }
        
//        rasterizeLineOfDisk(j, size, opencv_image);
        rasterizeRowOfDisk(j, size, opencv_image);
    }

    static int window_counter = 0;
    static int window_position = 0;
    std::string window_name = "TexSyn" + std::to_string(window_counter++);
    cv::namedWindow(window_name);       // Create a window for display.
    //~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~
//    cv::moveWindow(window_name, window_position, window_position);
//    window_position += 30;
    
    int tm = 23;  // TODO approximate top margin height
    cv::moveWindow(window_name, window_position, window_position + size + tm);
    window_position += tm;
    //~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~
    cv::imshow(window_name, opencv_image);  // Show our image inside it.
    if (wait) cv::waitKey(0);        // Wait for a keystroke in the window.

    
//    std::cout << "no window" << std::endl;
}

//    void Texture::rasterizeLineOfDisk(int j, int size, cv::Mat& opencv_image) const
//    {
//        int half = size / 2;
//        int x_limit = std::sqrt(sq(half) - sq(j));
//
//    //    cv::Mat line_image(1, size, CV_32FC3, cv::Scalar(0.5, 0.5, 0.5));
//        cv::Mat row_image(1, size, CV_32FC3, cv::Scalar(0.5, 0.5, 0.5));
//
//        for (int i = -x_limit; i <= x_limit; i++)
//        {
//            float radius = std::sqrt(sq(i) + sq(j));
//            if (radius <= half)
//            {
//                Vec2 position(i / float(half), j / float(half));
//
//                // Read TexSyn Color from Texture.
//                Color color = getColorClipped(position);
//                // Make OpenCV color, with reversed component order.
//                cv::Vec3f opencv_color(color.b(), color.g(), color.r());
//                // Make OpenCV location for pixel.
//    //            cv::Point opencv_position((size / 2) + i, (size / 2) - j);
//                cv::Point opencv_position((size / 2) + i, 0);
//                // Write corresponding OpenCV color to pixel:
//    //            opencv_image.at<cv::Vec3f>(opencv_position) = opencv_color;
//    //            line_image.at<cv::Vec3f>(opencv_position) = opencv_color;
//                row_image.at<cv::Vec3f>(opencv_position) = opencv_color;
//            }
//        }
//
//    //    cv::Rect rect(0, half - j, size, 1);
//    //    cv::Mat roi(opencv_image, rect);
//    //    line_image.copyTo(roi);
//
//    //    for (int i = 0; i < 1000000; i++)
//    //        line_image.copyTo(cv::Mat(opencv_image, cv::Rect(0, half - j, size, 1)));
//
//        // Copy line_image into the j-th line of opencv_image.
//        cv::Mat row_in_full_image(opencv_image, cv::Rect(0, half - j, size, 1));
//    //    line_image.copyTo(row_of_full_image);
//        row_image.copyTo(row_in_full_image);
//
//    }

//void Texture::rasterizeLineOfDisk(int j, int size, cv::Mat& opencv_image) const
void Texture::rasterizeRowOfDisk(int j, int size, cv::Mat& opencv_image) const
{
    // Half the rendering's size corresponds to the disk's center.
    int half = size / 2;
    // First and last pixels on j-th row of time
    int x_limit = std::sqrt(sq(half) - sq(j));
    cv::Mat row_image(1, size, CV_32FC3, cv::Scalar(0.5, 0.5, 0.5));
    for (int i = -x_limit; i <= x_limit; i++)
    {
        float radius = std::sqrt(sq(i) + sq(j));
        if (radius <= half)
        {
            // Read TexSyn Color from Texture at (i, j).
            Color color = getColorClipped(Vec2(i, j) / half);
            // Make OpenCV color, with reversed component order.
            cv::Vec3f opencv_color(color.b(), color.g(), color.r());
            // Write OpenCV color to corresponding pixel on row image:
            row_image.at<cv::Vec3f>(cv::Point(half + i, 0)) = opencv_color;
        }
    }
    // Copy line_image into the j-th line of opencv_image.
    cv::Mat row_in_full_image(opencv_image, cv::Rect(0, half - j, size, 1));
    row_image.copyTo(row_in_full_image);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Texture::total_pixels_rendered = 0;
int Texture::total_pixels_cached = 0;
int Texture::total_cache_lookups = 0;
size_t Texture::cache_size = 0;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
