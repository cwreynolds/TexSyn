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
        "/Users/cwr/Pictures/camouflage backgrounds/oak leaf litter green brown";
    
    const std::vector<std::string> background_image_filename =
    {
        "IMG_6548.jpeg", "IMG_6549.jpeg", "IMG_6550.jpeg",
        "IMG_6551.jpeg", "IMG_6552.jpeg", "IMG_6553.jpeg"
    };
    
    const std::string run_name = "oak leaf litter green brown";
    
    // The size of background images is adjusted by this value. It is expected
    // to be less than 1, indicating that the input photographic images are
    // generally larger than the screen resolution. Assumes only one scale is
    // needed, that the user has curated the background images to be at the
    // same scale. (If there is need for the scale to be bigger than 1 some
    // adjustments may be needed.)
    const float background_scale = 0.5;
//    const float background_scale = 0.1;

    void test()
    {
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
        gui.setWindowName(run_name);
        gui.refresh();
        for (int i = 0; i < 100; i++)
        {
            const cv::Mat& bg = LPRS().randomSelectElement(background_images);
            int dx = std::max(0, int(bg.cols - gui_size.x()));
            int dy = std::max(0, int(bg.rows - gui_size.y()));
            // Is background image larger than GUI window?
            if ((dx > 0) && (dy > 0))
            {
                Vec2 random_position(LPRS().randomN(dx), LPRS().randomN(dy));
                gui.drawMat(bg, random_position, gui_size);
            }
            else
            {
                gui.eraseRectangle(gui_size, Vec2());
                gui.drawMat(bg, Vec2());
            }
            gui.refresh();
        }
        Texture::waitKey();
    }

private:
};
