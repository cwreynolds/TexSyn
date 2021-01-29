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
#include "Disk.h"

class Camouflage
{
public:
    
    // TODO specify directory to put run results into?
    
    Camouflage(std::string run_name,
               std::string background_image_directory,
               float background_scale)
      : run_name_(run_name),
        background_image_directory_(background_image_directory),
        background_scale_(background_scale),
        gui_(gui_size_, Vec2())
    {}

    // Read specified background image files, scale, and save as cv::Mats.
    void collectBackgroundImages()
    {
        // TODO very temp prototype, should read ("ls") these from directory.
        const std::vector<std::string> background_image_filename =
        {
            "IMG_6548.jpeg", "IMG_6549.jpeg", "IMG_6550.jpeg",
            "IMG_6551.jpeg", "IMG_6552.jpeg", "IMG_6553.jpeg"
        };
        
        std::cout << "Reading " << background_image_filename.size();
        std::cout << " background images:" << std::endl;
        for (auto& filename : background_image_filename)
        {
            // Compose absolute pathname for this background image file.
            std::string pathname = backgroundImageDirectory() + "/" + filename;
            std::cout << "    " << pathname << std::endl;
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
    
    void drawRandomBackground()
    {
        const cv::Mat& bg = LPRS().randomSelectElement(backgroundImages());
        int dx = std::max(0, int(bg.cols - guiSize().x()));
        int dy = std::max(0, int(bg.rows - guiSize().y()));
        // Is background image larger than GUI window?
        if ((dx > 0) && (dy > 0))
        {
            Vec2 random_position(LPRS().randomN(dx), LPRS().randomN(dy));
            gui().drawMat(bg, random_position, guiSize());
        }
        else
        {
            gui().eraseRectangle(guiSize(), Vec2());
            gui().drawMat(bg, Vec2());
        }
    }

    void run_setup()
    {
        // Read specified background image files, save as cv::Mats.
        collectBackgroundImages();        
        gui().setWindowName(run_name_);
        gui().refresh();
    }

    void run()
    {
        std::cout << "Create initial population." << std::endl;
        int individuals = 100;
        int subpops = 5;
        int max_tree_size = 100;
        Population population(individuals, subpops, max_tree_size, GP::fs());
        
        run_setup();
        for (int i = 0; i < 100; i++)
        {
            population.evolutionStep([&]
                                     (TournamentGroup tg)
                                     { return tournamentFunction(tg); });
        }
    }
    
    void setMouseCallbackForTournamentFunction()
    {
        auto mouse_callback =
        []
        (int event, int x, int y, int flags, void* userdata)
        {
            if (event != cv::EVENT_MOUSEMOVE)
            {
                if (event == cv::EVENT_LBUTTONDOWN)
                {
                    auto c = static_cast<Camouflage*>(userdata);
                    c->setLastMouseClick(Vec2(x, y));
                }
            }
        };
        cv::setMouseCallback(gui().getWindowName(), mouse_callback, this);
    }
    
    // TournamentFunction for "Interactive Evolution of Camouflage".
    TournamentGroup tournamentFunction(TournamentGroup tg)
    {
        // Restrict Texture disks to be completely inside a rectangle inset from
        // the window edge by 0.1 of a Texture's diameter. Rectangle is defined
        // by two diagonally opposite corners.
        float margin = textureSize() * 0.1;
        Vec2 rect_min = Vec2(margin, margin);
        Vec2 rect_max = guiSize() - rect_min;
        // Find non-overlapping positions for the Textures in TournamentGroup.
        float r = textureSize() / 2;
        std::vector<Disk> disks;
        disks = Disk::randomNonOverlappingDisksInRectangle(3, r, r, r,
                                                           rect_min, rect_max,
                                                           LPRS());
        // Draw a randomly selected background, then the 3 testures on top.
        drawRandomBackground();
        int p = 0;
        for (auto& tgm : tg.members())
        {
            Texture* texture = GP::textureFromIndividual(tgm.individual);
            texture->rasterizeToImageCache(textureSize(), true);
            Vec2 center_to_ul = Vec2(1, 1) * textureSize() / 2;
            Vec2 position = disks.at(p++).position - center_to_ul;
            gui().drawTexture(*texture, position, textureSize());
        }
        // Update the onscreen image. Wait for user to click on one texture.
        gui().refresh();
        setMouseCallbackForTournamentFunction();
        waitForMouseClick();
        // See if click was within a Texture disk, set "worst" Individual if so.
        p = 0;
        Individual* worst = nullptr;
        for (auto& tgm : tg.members())
        {
            Vec2 texture_center = disks.at(p++).position;
            float distance = (texture_center - getLastMouseClick()).length();
            if (distance <= (textureSize() / 2)) { worst = tgm.individual; }
        }
        // Designate clicked Texture's Individual as worst of TournamentGroup.
        tg.designateWorstIndividual(worst);
        // Clear GUI, return updated TournamentGroup.
        gui().clear();
        gui().refresh();
        return tg;
    }
    
    void waitForMouseClick()
    {
        wait_for_mouse_click_ = true;
        while(wait_for_mouse_click_)
        {
            Texture::waitKey(250);  // 1/4 second (250/1000)
        }
    }

    // The size of background images is adjusted by this value. It is expected
    // to be less than 1, indicating that the input photographic images are
    // generally larger than the screen resolution. Assumes only one scale is
    // needed, that the user has curated the background images to be at the
    // same scale. (If there is need for the scale to be bigger than 1 some
    // adjustments may be needed.)
    float backgroundScale() const { return background_scale_; }
    
    // Pathname of directory containing raw background image files.
    const std::string backgroundImageDirectory() const
    { return background_image_directory_; }
    
    // Collection of cv::Mat to be used as background image source material.
    const std::vector<cv::Mat>& backgroundImages() const
    { return background_images_; }
    // Add cv::Mat to collection of background images.
    void addBackgroundImage(cv::Mat& bg) { background_images_.push_back(bg); }
    
    // GUI size: drawable area in pixels.
    Vec2 guiSize() const { return gui_size_; }
    // Reference to GUI.
    GUI& gui() { return gui_; }
    
    // TODO very temp
    int textureSize() const { return 201; }
    
    // Get/set position of most recent mouse (left) click in GUI.
    Vec2 getLastMouseClick() const { return last_mouse_click_; }
    void setLastMouseClick(Vec2 mouse_pos)
    {
        last_mouse_click_ = mouse_pos;
        wait_for_mouse_click_ = false;
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
    // GUI size: drawable area in pixels.
    // TODO pick a better default (this is roughly screen size on my MBP).
    Vec2 gui_size_ = {1430, 850};
    // GUI object
    GUI gui_;
    // Store position of most recent mouse (left) click in GUI.
    Vec2 last_mouse_click_;
    // True during wait for user to select one texture on screen
    bool wait_for_mouse_click_;
};
