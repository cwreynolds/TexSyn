//
//  Camouflage.h
//  TexSyn
//
//  Created by Craig Reynolds on 1/20/21.
//  Copyright © 2021 Craig Reynolds. All rights reserved.
//
//------------------------------------------------------------------------------
//
// Early protoyping of an "Interactive Evolution of Camouflage" implementation
// on top of TexSyn and LazyPredator. Slightly updated version of the approach
// used in:
//         Craig Reynolds. 2011. Interactive Evolution of Camouflage.
//         Artificial Life 17(2). https://doi.org/10.1162/artl_a_00023
//
// Prototyping inside TexSyn, maybe should be its own library/git repository.
//
//------------------------------------------------------------------------------

#pragma once
#include "GP.h"

class Camouflage
{
public:
    
    // TODO very prototype
    const std::string background_image_directory =
        "/Users/cwr/Pictures/camouflage backgrounds/oak leaves green brown";
    
    const std::vector<std::string> background_image_filename =
    {
        "IMG_6548.jpeg", "IMG_6549.jpeg", "IMG_6550.jpeg",
        "IMG_6551.jpeg", "IMG_6552.jpeg", "IMG_6553.jpeg"
    };
    
    // The size of background images is adjusted by this value. It is expected
    // to be less than 1, indicating that the input photographic images are
    // generally larger than the screen resolution. (If there is need for it
    // to be bigger than 1 some adjustments may be needed.)
//    const float background_scale = 0.5;
//    const float background_scale = 0.25;
    const float background_scale = 0.4;

    void test()
    {
        // TODO maybe want a "background scale" to relate source image size to
        // GUI screen size. Thinking there is just one of these and we assume
        // the user has curated the background images to be at the same scale.
        
        // Read specified background image files, save as cv::Mats.
        std::vector<cv::Mat> background_images;
        for (auto& filename : background_image_filename)
        {
            // Compose absolute pathname for this background image file.
            std::string pathname = background_image_directory + "/" + filename;
            // Read the image file into an OpenCV image.
            cv::Mat bg = cv::imread(pathname);
            // Adjust the size/resolution by "background_scale" parameter.
            cv::resize(bg, bg,
                       cv::Size(), background_scale, background_scale,
                       cv::INTER_CUBIC);
            // Add to collection of background images.
            background_images.push_back(bg);
        }
        
        Vec2 gui_size(1430, 850);
        GUI gui(gui_size, Vec2());
        gui.refresh();
        for (int i = 0; i < 200; i++)
        {
            const cv::Mat& bg = LPRS().randomSelectElement(background_images);
            
            debugPrint(bg.cols);
            debugPrint(bg.rows);
            debugPrint(std::abs(bg.cols - int(gui_size.x())));
            debugPrint(std::abs(bg.rows - int(gui_size.y())));

//            gui.drawMat(bg,
//                        Vec2(LPRS().randomN(bg.cols - int(gui_size.x())),
//                             LPRS().randomN(bg.rows - int(gui_size.y()))),
//                        gui_size);

            gui.drawMat(bg,
//                        Vec2(LPRS().randomN(bg.cols - int(gui_size.x())),
//                             LPRS().randomN(bg.rows - int(gui_size.y()))),
                        Vec2(LPRS().randomN(std::abs(bg.cols -
                                                     int(gui_size.x()))),
                             LPRS().randomN(std::abs(bg.rows -
                                                     int(gui_size.y())))),
                        Vec2(std::min(int(gui_size.x()), int(bg.cols)),
                             std::min(int(gui_size.y()), bg.rows))
                        
                        
                        );
            gui.refresh();
        }
        Texture::waitKey();
    }

private:
};
