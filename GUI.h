//
//  GUI.h
//  texsyn
//
//  Created by Craig Reynolds on 12/4/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//
//------------------------------------------------------------------------------
// Simple graphical user interface for a LazyPredator evolution run using Texsyn
//------------------------------------------------------------------------------

// TODO 20210202 very temp, trying to use custom built OpenCV
#define TEMP_AVOID_TTF

#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#ifndef TEMP_AVOID_TTF
#include <opencv2/freetype.hpp>
#endif // TEMP_AVOID_TTF
#pragma clang diagnostic pop

#include "Texture.h"

class GUI
{
public:
    GUI() : GUI(Vec2(), Vec2(), "gui") {}
    // Note that OpenCV's coordinate system has zero in upper left
    GUI(Vec2 size_in_pixels,
        Vec2 upper_left_init_position,
        std::string window_name)
    {
        image_ = cv::Mat(size_in_pixels.y(), size_in_pixels.x(),
                         CV_8UC3, backgroundGray());
#ifndef TEMP_AVOID_TTF
        // TODO hardcoded, should be a settable path, with fall-back
        //      to using original highgui putText() if file not there.
        font = cv::freetype::createFreeType2();
        font->loadFontData("/opt/X11/share/fonts/TTF/Vera.ttf", 0);
#endif // TEMP_AVOID_TTF
        setWindowName(window_name);
        cv::namedWindow(getWindowName());
        cv::moveWindow(getWindowName(),
                       upper_left_init_position.x(),
                       upper_left_init_position.y());
    };
    virtual ~GUI() { cv::destroyWindow(getWindowName()); }
    void refresh() { cv::imshow(getWindowName(), image_); cv::waitKey(1); }
    void clear() { image_ = backgroundGray(); }
    void drawText(const std::string& text,
                  float font_height,
                  const Vec2& upper_left_position,
                  const Color& color)
    {
#ifndef TEMP_AVOID_TTF
        // TODO 20201204 this assumes .ttf file found
        font->putText(image_,
                      text,
                      cv::Point(upper_left_position.x(),
                                upper_left_position.y()),
                      font_height,
                      colorToCvScalar(color),
                      cv::FILLED,
                      cv::LINE_AA,
                      false);
#endif // TEMP_AVOID_TTF
    }
    // Draw given text horizontally centered around given position.
    // TODO 20201212 should centering be an arg to the main func?
    void drawTextHorizontalCenter(const std::string& text,
                                  float font_height,
                                  const Vec2& top_center_position,
                                  const Color& color)
    {
#ifndef TEMP_AVOID_TTF
        int bl = 0;
        cv::Size ts(font->getTextSize(text, font_height, cv::FILLED, &bl));
        Vec2 position = top_center_position - Vec2(ts.width / 2, 0);
        drawText(text, font_height, position, color);
#endif // TEMP_AVOID_TTF
    }
    
    // Draw a Texture, rendered at the given size, at given position on GUI.
    // TODO ideally this should clip the rectangle to be "not outside" image_.
    void drawTexture(const Texture& texture,
                     const Vec2& upper_left_position,
                     int size)
    {
        texture.rasterizeToImageCache(size, true);
        drawMat(texture.getCvMat(), upper_left_position);
    }

    // Draw a portion of a huge cv::Mat on whole GUI. TODO probably need a
    // version to draw a portion of the given Mat on a portion of the GUI.
    void drawMat(const cv::Mat& cv_mat,
                 const Vec2& upper_left_position,
                 const Vec2& size_in_pixels)
    {
        cv::Mat on_screen_part_of_given_mat(cv_mat,
                                            cv::Rect(upper_left_position.x(),
                                                     upper_left_position.y(),
                                                     size_in_pixels.x(),
                                                     size_in_pixels.y()));
        on_screen_part_of_given_mat.copyTo(image_);
    }
    
    // Draw a "small" cv::Mat on a portion of GUI at given location.
    void drawMat(const cv::Mat& cv_mat, const Vec2& upper_left_position)
    {
        cv::Mat part_of_gui_mat = cv::Mat(image_,
                                          cv::Rect(upper_left_position.x(),
                                                   upper_left_position.y(),
                                                   cv_mat.cols,
                                                   cv_mat.rows));
        cv_mat.copyTo(part_of_gui_mat);
    }

    // Get/set window name (its ID).
    const std::string& getWindowName() const { return window_name_; }
    void setWindowName(const std::string& name) { window_name_ = name; }
    // Set title (without changing ID).
    void setWindowTitle(const std::string& new_title)
    {
        cv::setWindowTitle(window_name_, new_title);
    }
    
    // TODO reconsider
    void eraseRectangle(Vec2 size_in_pixels, Vec2 upper_left_init_position)
    {
        cv::rectangle(image_,
                      vec2ToCvPoint(upper_left_init_position),
                      vec2ToCvPoint(upper_left_init_position + size_in_pixels),
                      backgroundGray(),
                      cv::FILLED,
                      cv::LINE_AA);
    }

    void drawRectangle(Vec2 size_in_pixels,
                       Vec2 upper_left_init_position,
                       Color color)
    {
        cv::rectangle(image_,
                      vec2ToCvPoint(upper_left_init_position),
                      vec2ToCvPoint(upper_left_init_position + size_in_pixels),
                      colorToCvScalar(color),
                      cv::FILLED,
                      cv::LINE_AA);
    }
    
    // Draw given circle (unfilled, fixed thickness, add more args later?)
    void drawCircle(float radius, Vec2 center, Color color)
    {
        cv::circle(image_, vec2ToCvPoint(center), radius, colorToCvScalar(color));
    }
    
    // Draw a circle as a dashed line on the GUI, with given xy "center" and
    // "diameter", both specified in pixel units. Optionally specify number of
    // steps, and the alternating colors of the dashes.
    void drawDashedCircle(Vec2 center, float diameter, bool draw_cross)
    {
        drawDashedCircle(center, diameter, 24, Color(0), Color(1), draw_cross);
    }
    void drawDashedCircle(Vec2 center,
                          float diameter,
                          int steps,
                          Color color0,
                          Color color1,
                          bool draw_cross)
    {
        Color color = color0;
        float prev_angle = 0;
        float radius = diameter / 2;
        for (int i = 0; i < steps; i++)
        {
            float next_angle = (i + 1) * 2 * pi / steps;
            Vec2 prev_point(std::sin(prev_angle), std::cos(prev_angle));
            Vec2 next_point(std::sin(next_angle), std::cos(next_angle));
            drawLine(center + (prev_point * radius),
                     center + (next_point * radius),
                     color);
            // Set color and angle for next line segment.
            color = (color == color0) ? color1 : color0;
            prev_angle = next_angle;
        }
        if (draw_cross)
        {
            float size = diameter * 0.3;
            Vec2 half_v(0, size / 2);
            Vec2 half_h(size / 2, 0);
            drawLine(center - half_v, center + half_v, color1);
            drawLine(center - half_h, center + half_h, color0);
        }
    }

    void drawLine(Vec2 endpoint0, Vec2 endpoint1, Color color)
    {
        cv::line(getCvMat(),
                 vec2ToCvPoint(endpoint0),
                 vec2ToCvPoint(endpoint1),
                 colorToCvScalar(color),
                 1,             // Line thickness
                 cv::LINE_AA);  // Line type: antialiased
    }

    
    // Return the entire cv::Mat for this GUI. (See also getCvMatRect())
    cv::Mat getCvMat() { return image_; }

    // Return a "submat"/"ROI" in the GUI's underlying cv::Mat.
    cv::Mat getCvMatRect(const Vec2& upper_left_position,
                         const Vec2& size_in_pixels)
    {
        return Texture::getCvMatRect(upper_left_position,
                                     size_in_pixels,
                                     image_);
    }
    
    // Get current size (in pixels of the cv::Mat underlying the GUI).
    Vec2 getSize() const { return Vec2(image_.cols, image_.rows); }
    // Reset size (in pixels of the cv::Mat underlying the GUI).
    void setSize(Vec2 size_in_pixels)
    {
        image_ = cv::Mat(size_in_pixels.y(),
                         size_in_pixels.x(),
                         CV_8UC3,
                         backgroundGray());
    }
    
    static cv::Scalar backgroundGray() { return cv::Scalar::all(127); }
    static cv::Scalar colorToCvScalar(const Color& c)
        { return CV_RGB(255 * c.r(), 255 * c.g(), 255 * c.b()); }
    static cv::Point vec2ToCvPoint(const Vec2& v)
        { return cv::Point(v.x(), v.y()); }
private:
    cv::Mat image_;
    std::string window_name_;
#ifndef TEMP_AVOID_TTF
    cv::Ptr<cv::freetype::FreeType2> font;
#endif // TEMP_AVOID_TTF
};
