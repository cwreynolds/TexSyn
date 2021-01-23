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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20210120 add Camouflage.h module

//    #pragma clang diagnostic push
//    #pragma clang diagnostic ignored "-Wdocumentation"
//    #include <opencv2/core/core.hpp>
//    #include <opencv2/highgui/highgui.hpp>
//    #include <opencv2/imgproc/imgproc.hpp>
//    // TODO 20201202 very experimental putText()
//    #include <opencv2/freetype.hpp>
//    #pragma clang diagnostic pop
//
//    //#include "Vec2.h"
//    //#include "Color.h"
//
//    #include "Texture.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/freetype.hpp>
#pragma clang diagnostic pop

#include "Texture.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
        cv::namedWindow(windowName());
        cv::moveWindow(windowName(),
                       upper_left_init_position.x(),
                       upper_left_init_position.y());
    };
    virtual ~GUI() { cv::destroyWindow(windowName()); }
    void refresh() { cv::imshow(windowName(), image_); cv::waitKey(1); }
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
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20210121 add API for drawing cv::Mat, maybe redo drawTexture to use?
    
    // TODO ideally this should clip the rectangle to be "not outside" image_.
    void drawTexture(const Texture& texture,
                     const Vec2& upper_left_position,
                     int size)
    {
        texture.rasterizeToImageCache(size, true);
        Vec2 ulp = upper_left_position;
        cv::Rect target_rect(ulp.x(), ulp.y(), size, size);
        cv::Mat submat = cv::Mat(image_, target_rect);
        texture.getCvMat().copyTo(submat);
    }
    
    // TODO 20210121 currently this is "draw a portion of a huge mat on image_"
    //               probably needs more parameters.
    void drawMat(const cv::Mat& cv_mat,
                 const Vec2& upper_left_position,
                 const Vec2& size_in_pixels)
    {
        debugPrint(upper_left_position);
        debugPrint(size_in_pixels);
        image_ = cv::Mat(cv_mat,
                         cv::Rect(upper_left_position.x(),
                                  upper_left_position.y(),
                                  size_in_pixels.x(),
                                  size_in_pixels.y()));
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    const std::string& windowName() const { return window_name_; }
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
