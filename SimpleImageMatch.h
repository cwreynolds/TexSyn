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
        run_name_(std::filesystem::path(target_image_pathname_).stem()),
        output_directory_(cmd.positionalArgument(2, ".")),
        output_directory_this_run_(runOutputDirectory()),
        random_seed_(cmd.positionalArgument(3, int(LPRS().defaultSeed()))),
        individuals_(cmd.positionalArgument(4, 120)),
        subpops_(cmd.positionalArgument(5, 6)),
        max_init_tree_size_(cmd.positionalArgument(6, 100)),
        min_crossover_tree_size_(max_init_tree_size_ * 0.5),
        max_crossover_tree_size_(max_init_tree_size_ * 1.5),
        target_image_(cv::imread(target_image_pathname_))
    {
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
        
        assert((target_image_.cols > 0) &&
               (target_image_.rows > 0) &&
               "target image missing or empty");
        
        // TODO do this here, or in the initializers above, or in run()?
        std::cout << "Create initial population..." << std::endl;
        LPRS().setSeed(random_seed_);
        population_ = std::make_shared<Population>(individuals_,
                                                   subpops_,
                                                   max_init_tree_size_,
                                                   min_crossover_tree_size_,
                                                   max_crossover_tree_size_,
                                                   GP::fs());
        std::cout << "...done." << std::endl;
    }

    // Run the evolution simulation.
    void run()
    {
        // Init GUI window.
        gui().setWindowName("SimpleImageMatch: " + run_name_);
        gui().drawMat(target_image_, Vec2());
        gui().refresh();
        
        while (true)
        {
//            // Display step count in GUI title bar.
//            std::string step_string = " (step " + getStepAsString() + ")";
//            gui().setWindowTitle(run_name_ + step_string);
//            logFunctionUsageCounts(out);
            // Evolution step with wrapped EvoCamoGame::tournamentFunction().
            population_->evolutionStep([&]
                                       (Individual* i)
                                       { return fitnessFunction(i); });
        }
        // Delete Population instance.
        population_ = nullptr;
    }
    
    float fitnessFunction(Individual* individual) // const
    {
        Texture& texture = *GP::textureFromIndividual(individual);
        texture.rasterizeToImageCache(getTargetImageSize().x(), false);
        cv::Mat mat = texture.getCvMat();
        float similarity = imageSimilarity(mat, target_image_);
        float nonuniformity = 1 - imageUniformity(mat);
        gui().drawMat(mat, Vec2());
        gui().refresh();
        std::cout << "    fitness=" << similarity * nonuniformity;
        std::cout << " (similarity=" << similarity;
        std::cout << " nonuniformity=" << nonuniformity << ")" << std::endl;
        return similarity * nonuniformity;
    }
    
    // Returns a number on [0, 1] measuring how similar two images are.
    // TODO could be more efficient, but only only takes 0.0341692 for 511x511
    //      images so is just a tiny fraction of the time for a texture render.
    float imageSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        int m0w = m0.cols;
        int m0h = m0.rows;
        int m1w = m1.cols;
        int m1h = m1.rows;
        assert((m0w == m1w) && (m0h == m1h) && (m0w > 0) && (m0h > 0));
        //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
        bool similarity_squared = false;
        bool multiplicative = true;
//        bool similarity_squared = true;
//        bool multiplicative = false;
        bool averaging = !multiplicative;
        float similarlity = 0;
        if (multiplicative) similarlity = 1;
        //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
        for (int x = 0; x < m0w; x++)
        {
            for (int y = 0; y < m0h; y++)
            {
                float similar = Color::similarity(getCvMatPixel(x, y, m0),
                                                  getCvMatPixel(x, y, m1));
                assert (between(similar, 0, 1));
                //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
                if (averaging) similarlity += similar;
                if (multiplicative) similarlity *= remapInterval(similar,
                                                                 0, 1, 0.99, 1);
                //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
            }
        }
        //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
        if (averaging) similarlity /= m0w * m0h;
        if (similarity_squared) similarlity *= similarlity;
        //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
        return similarlity;
    }
    
    // Returns a number on [0, 1] measuring how uniform a CV Mat is.
    // TODO Is 10 tests good? Use some other RS?
    float imageUniformity(const cv::Mat& mat) const
    {
        float uniformity = 1;
        int tests = 10;
        for (int i = 0; i < tests; i++)
        {
            Color a = getCvMatPixel(LPRS().random2(0, mat.cols),
                                    LPRS().random2(0, mat.rows),
                                    mat);
            Color b = getCvMatPixel(LPRS().random2(0, mat.cols),
                                    LPRS().random2(0, mat.rows),
                                    mat);
            if (Color::similarity(a, b) < 0.95) uniformity /= 2;
        }
        return uniformity;
    }
    
    // Read a pixel as a Color value at given (x, y) in OpenCV Mat.
    Color getCvMatPixel(int x, int y, const cv::Mat& mat) const
    {
        cv::Vec3b bgrPixel = mat.at<cv::Vec3b>(x, y);
        float m = 255;
        return Color(bgrPixel[2] / m, bgrPixel[1] / m, bgrPixel[0] / m);
    };

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

    // TODO copied from EvoCamoGame,
    // TODO should this just be a member inside SimpleImageMatch instance?
    // Points to heap-allocated Population instance during run() function.
    std::shared_ptr<Population> population_ = nullptr;

};
