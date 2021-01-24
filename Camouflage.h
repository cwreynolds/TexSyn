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
    
    // TODO specify directory to put run results into?
    
    Camouflage(std::string run_name,
               std::string background_image_directory,
               float background_scale)
      : run_name_(run_name),
        background_image_directory_(background_image_directory),
        background_scale_(background_scale)
    {}

    // Pathname of directory containing raw background image files.
    const std::string backgroundImageDirectory() const
        { return background_image_directory_; }

    // Collection of cv::Mat to be used as background image source material.
    const std::vector<cv::Mat>& backgroundImages() const
        { return background_images_; }
    // Add cv::Mat to collection of background images.
    void addBackgroundImage(cv::Mat& bg) { background_images_.push_back(bg); }

    // Read specified background image files, scale, and save as cv::Mats.
    void collectBackgroundImages()
    {
        // TODO very temp prototype, should read ("ls") these from directory.
        const std::vector<std::string> background_image_filename =
        {
            "IMG_6548.jpeg", "IMG_6549.jpeg", "IMG_6550.jpeg",
            "IMG_6551.jpeg", "IMG_6552.jpeg", "IMG_6553.jpeg"
        };

        for (auto& filename : background_image_filename)
        {
            // Compose absolute pathname for this background image file.
            std::string pathname = backgroundImageDirectory() + "/" + filename;
            // Read the image file into an OpenCV image.
            cv::Mat bg = cv::imread(pathname);
            // Adjust the size/resolution by "background_scale" parameter.
            cv::resize(bg, bg,
                       cv::Size(), backgroundScale(), backgroundScale(),
                       cv::INTER_CUBIC);
            // Add to collection of background images.
            addBackgroundImage(bg);
        }
        assert(!backgroundImages().empty());
    }

    // The size of background images is adjusted by this value. It is expected
    // to be less than 1, indicating that the input photographic images are
    // generally larger than the screen resolution. Assumes only one scale is
    // needed, that the user has curated the background images to be at the
    // same scale. (If there is need for the scale to be bigger than 1 some
    // adjustments may be needed.)
    float backgroundScale() const { return background_scale_; }
    
    void test()
    {
        // Read specified background image files, save as cv::Mats.
        collectBackgroundImages();
        
        Vec2 gui_size(1430, 850);
        GUI gui(gui_size, Vec2());
        gui.setWindowName(run_name_);
        gui.refresh();
        for (int i = 0; i < 100; i++)
        {
            const cv::Mat& bg = LPRS().randomSelectElement(backgroundImages());
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
    
    void run()
    {
        test();
    }

private:
    // Name of run, perhaps same as directory holding background image files.
    const std::string run_name_;
    // Pathname of directory containing raw background image files.
    const std::string background_image_directory_;
    // Collection of cv::Mat to be used as background image source material.
    std::vector<cv::Mat> background_images_;
    // The size of background images is adjusted by this value (usually < 1).
    const float background_scale_ = 1;
};
