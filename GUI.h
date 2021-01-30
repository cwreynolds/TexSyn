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

#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/freetype.hpp>
#pragma clang diagnostic pop

#include "Texture.h"

class GUI
{
public:
    // Note that OpenCV's coordinate system has zero in upper left
    GUI(Vec2 size_in_pixels, Vec2 upper_left_init_position)
    {
        image_ = cv::Mat(size_in_pixels.y(), size_in_pixels.x(),
                         CV_8UC3, backgroundGray());
        // TODO hardcoded, should be a settable path, with fall-back
        //      to using original highgui putText() if file not there.
        font = cv::freetype::createFreeType2();
        font->loadFontData("/opt/X11/share/fonts/TTF/Vera.ttf", 0);
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
    }
    // Draw given text horizontally centered around given position.
    // TODO 20201212 should centering be an arg to the main func?
    void drawTextHorizontalCenter(const std::string& text,
                                  float font_height,
                                  const Vec2& top_center_position,
                                  const Color& color)
    {
        int bl = 0;
        cv::Size ts(font->getTextSize(text, font_height, cv::FILLED, &bl));
        Vec2 position = top_center_position - Vec2(ts.width / 2, 0);
        drawText(text, font_height, position, color);
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

    const std::string& getWindowName() const { return window_name_; }
    void setWindowName(const std::string& name) { window_name_ = name; }
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
    
    // Return a "submat"/"ROI" in the GUI's underlying cv::Mat.
    cv::Mat getCvMatRect(const Vec2& upper_left_position,
                         const Vec2& size_in_pixels)
    {
        return cv::Mat(image_,
                       cv::Rect(upper_left_position.x(),
                                upper_left_position.y(),
                                size_in_pixels.x(),
                                size_in_pixels.y()));
        
    }

    static cv::Scalar backgroundGray() { return cv::Scalar::all(127); }
    static cv::Scalar colorToCvScalar(const Color& c)
        { return CV_RGB(255 * c.r(), 255 * c.g(), 255 * c.b()); }
    static cv::Point vec2ToCvPoint(const Vec2& v)
        { return cv::Point(v.x(), v.y()); }
private:
    cv::Mat image_;
    std::string window_name_ = "LimitHue run";
    cv::Ptr<cv::freetype::FreeType2> font;
};
