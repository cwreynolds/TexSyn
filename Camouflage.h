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
    std::string background_image_directory =
        "/Users/cwr/Pictures/camouflage backgrounds/oak leaves green brown";
    
    std::vector<std::string> background_image_filename =
    {
        "IMG_6548.jpeg", "IMG_6549.jpeg", "IMG_6550.jpeg",
        "IMG_6551.jpeg", "IMG_6552.jpeg", "IMG_6553.jpeg"
    };
    
    void test()
    {
        // TODO maybe want a "background scale" to relate source image size to
        // GUI screen size. Thinking there is just one of these and we assume
        // the user has curated the background images to be at the same scale.
        
        // Read specified background image files, save as cv::Mats.
        std::vector<cv::Mat> background_images;
        for (auto& filename : background_image_filename)
        {
            std::string pathname = background_image_directory + "/" + filename;
            background_images.push_back(cv::imread(pathname));
        }
        
        Vec2 gui_size(1430, 850);
        GUI gui(gui_size, Vec2());
        gui.refresh();
        for (int i = 0; i < 200; i++)
        {
            const cv::Mat& bg = LPRS().randomSelectElement(background_images);
            gui.drawMat(bg,
                        Vec2(LPRS().randomN(bg.cols - int(gui_size.x())),
                             LPRS().randomN(bg.rows - int(gui_size.y()))),
                        gui_size);
            gui.refresh();
        }
        Texture::waitKey();
    }

private:
};
