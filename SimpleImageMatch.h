//
//  SimpleImageMatch.h
//  TexSyn
//
//  Created by Craig Reynolds on 6/30/21.
//  Copyright © 2021 Craig Reynolds. All rights reserved.
//
//------------------------------------------------------------------------------
// TODO improve
// Evolve textures to match a given example. Uses simple "image similarity" for
// fitness. (Non-interactive, no tournament.) Hoping to to find interesting
// “stylizations” via evolutionary search. Prototype for co-evolutionary version
// of stylization search.
//------------------------------------------------------------------------------

#pragma once
#include "GP.h"

// TODO  could inherit from EvoCamoGame instead of stealing bit of its code.
// TODO  Or there could be a common base class.

class SimpleImageMatch
{
public:
    // TODO parameters
    // simple_image_match target_image output_directory
    
    
    // Constructor to get parameters from pre-parsed "unix style" command line.
//    SimpleImageMatch(const CommandLine& cmd) {}
    
    SimpleImageMatch(const CommandLine& cmd)
      : run_name_(runNameDefault(cmd)),
//        background_image_directory_(cmd.positionalArgument(1)),
    
        target_image_pathname_(cmd.positionalArgument(1)),
    
        output_directory_(cmd.positionalArgument(2, ".")),
        output_directory_this_run_(runOutputDirectory()),
//        background_scale_(cmd.positionalArgument(3, float(0.5))),
        random_seed_(cmd.positionalArgument(4, int(LPRS().defaultSeed()))),
//        gui_size_(cmd.positionalArgument(5, 1200),
//                  cmd.positionalArgument(6, 800)),
    
//        gui_(gui_size_, Vec2(), run_name_),
        gui_(Vec2(), Vec2(), run_name_),

        individuals_(cmd.positionalArgument(7, 120)),
        subpops_(cmd.positionalArgument(8, 6)),
        max_init_tree_size_(cmd.positionalArgument(9, 100)),
        min_crossover_tree_size_(max_init_tree_size_ * 0.5),
        max_crossover_tree_size_(max_init_tree_size_ * 1.5)
    {
        readTargetImage();
        gui_.setSize(getTargetImageSize());
    }

    // Run the evolution simulation.
    void run()
    {
//        gui_.refresh();
        
        // Init GUI window.
        gui().setWindowName(run_name_);
        gui().refresh();

        Texture::waitKey();
    }
    
    // TODO copied from EvoCamoGame
    // Get default run name from background_image_directory_.
    // (Provides consistent behavior with and without trailing "/".)
    std::string runNameDefault(const CommandLine& command_line)
    {
//        std::filesystem::path path = command_line.positionalArgument(1);
//        std::string fn = path.filename();
//        return (fn != "") ? fn : std::string(path.parent_path().filename());
        return "run name";
    }
    
    void readTargetImage()
    {
//        // Pathname of target image file.
//        const std::string target_image_pathname_;
//
//        // cv::Mat containing the target image.
//        cv::Mat target_image_;
        
        target_image_ = cv::imread(target_image_pathname_);


    }
    
    Vec2 getTargetImageSize()
    {
        return Vec2(target_image_.cols, target_image_.rows);
    }

    // TODO copied from EvoCamoGame
    // A subdirectory under output_directory_ for results from this run.
    std::string runOutputDirectory()
    {
        if (!std::filesystem::exists(output_directory_))
        {
            debugPrint(output_directory_);
            assert(!"output_directory_ does not exist.");
        }
        std::filesystem::path run_output_dir = output_directory_;
        run_output_dir /= (run_name_ + "_" + date_hours_minutes());
        return run_output_dir;
    }

    // Reference to GUI.
    GUI& gui() { return gui_; }

private:
    // TODO copied from EvoCamoGame

    
    // Name of run. (Defaults to directory holding background image files.)
    const std::string run_name_;
    
//    // Pathname of directory containing raw background image files.
//    const std::string background_image_directory_;
    // Pathname of target image file.
    const std::string target_image_pathname_;

    // Pathname of directory into which we can create a run log directory.
    const std::string output_directory_;
    // A subdirectory under output_directory_ for results from this run.
    const std::string output_directory_this_run_;
    
    // cv::Mat containing the target image.
    cv::Mat target_image_;

    
    // GUI object
    GUI gui_;
    // Seed for RandomSequence LPRS() to be used during this run
    int random_seed_ = LPRS().defaultSeed();
    // Default parameters for Population
    int individuals_ = 120;
    int subpops_ = 6;
    int max_init_tree_size_ = 100;
    int min_crossover_tree_size_ = 50;
    int max_crossover_tree_size_ = 150;


};
