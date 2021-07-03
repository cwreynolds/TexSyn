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
    // simple_image_match target_image             (pathname string)
    //                    output_directory         (pathname string)
    //                    random_seed              (int)
    //                    individuals              (int)
    //                    subpops                  (int)
    //                    max_init_tree_size       (int)
    //                    min_crossover_tree_size  (int)
    //                    max_crossover_tree_size  (int)

    SimpleImageMatch(const CommandLine& cmd)
      : target_image_pathname_(cmd.positionalArgument(1)),
        run_name_(generateRunName()),
        output_directory_(cmd.positionalArgument(2, ".")),
        output_directory_this_run_(runOutputDirectory()),
        random_seed_(cmd.positionalArgument(3, int(LPRS().defaultSeed()))),
        individuals_(cmd.positionalArgument(4, 120)),
        subpops_(cmd.positionalArgument(5, 6)),
        max_init_tree_size_(cmd.positionalArgument(6, 100)),
        min_crossover_tree_size_(max_init_tree_size_ * 0.5),
        max_crossover_tree_size_(max_init_tree_size_ * 1.5)
    {
        readTargetImage();
        gui_.setSize(getTargetImageSize());
        
        // log parameters for this run
        std::cout << "SimpleImageMatch parameters:" << std::endl;
        std::cout << "    "; debugPrint(target_image_pathname_)
        std::cout << "    "; debugPrint(getTargetImageSize());
        std::cout << "    "; debugPrint(run_name_);
        std::cout << "    "; debugPrint(output_directory_);
        std::cout << "    "; debugPrint(output_directory_this_run_);
        std::cout << "    "; debugPrint(random_seed_);
        std::cout << "    "; debugPrint(individuals_);
        std::cout << "    "; debugPrint(subpops_);
        std::cout << "    "; debugPrint(max_init_tree_size_);
        std::cout << "    "; debugPrint(min_crossover_tree_size_);
        std::cout << "    "; debugPrint(max_crossover_tree_size_);
    }

    // Run the evolution simulation.
    void run()
    {
        // Init GUI window.
        gui().setWindowName("SimpleImageMatch: " + run_name_);
        gui().drawMat(target_image_, Vec2());
        gui().refresh();

        Texture::waitKey();
    }
    
    // TODO should this be an initializer on the constructor?
    std::string generateRunName()
    {
        return std::filesystem::path(target_image_pathname_).stem();
    }

    // TODO should this be an initializer on the constructor?
    void readTargetImage()
    {
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
    // Pathname of target image file.
    const std::string target_image_pathname_;
    // cv::Mat containing the target image.
    cv::Mat target_image_;
    // Name of run. (Defaults to directory holding background image files.)
    const std::string run_name_;
    // Pathname of directory into which we can create a run log directory.
    const std::string output_directory_;
    // A subdirectory under output_directory_ for results from this run.
    const std::string output_directory_this_run_;
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
