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
        setGuiSize();
        
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
        
        // Build "MIP map like" resolution pyramid for target image.
        makeResolutionPyramid(target_image_, target_pyramid_);

        
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
        while (true)
        {
//            logFunctionUsageCounts(out);
            // Evolution step with wrapped EvoCamoGame::tournamentFunction().
            population_->evolutionStep([&]
                                       (Individual* i)
                                       { return fitnessFunction(i); });
        }
        // Delete Population instance.
        population_ = nullptr;
    }
        
//    float fitnessFunction(Individual* individual)
//    {
//        Texture& texture = *GP::textureFromIndividual(individual);
//        texture.rasterizeToImageCache(getTargetImageSize().x(), false);
//        cv::Mat mat = texture.getCvMat();
//        drawGuiForFitnessFunction(mat, target_image_);
//        float mip_map_similarity = imageMipMapSimilarity(mat, target_image_);
//        float nonuniformity = 1 - imageUniformity(mat);
//        float fitness = mip_map_similarity * nonuniformity;
//        std::cout << "    fitness=" << fitness;
//        std::cout << " (mip_map_similarity=" << mip_map_similarity;
//        std::cout << " nonuniformity=" << nonuniformity << ")" << std::endl;
//        return fitness;
//    }

//    float fitnessFunction(Individual* individual)
//    {
//        Texture& texture = *GP::textureFromIndividual(individual);
//        texture.rasterizeToImageCache(getTargetImageSize().x(), false);
//        cv::Mat mat = texture.getCvMat();
//        drawGuiForFitnessFunction(mat, target_image_);
////        float mip_map_similarity = imageMipMapSimilarity(mat, target_image_);
//        float threshold_similarity = imageThresholdSimilarity(mat, target_image_);
//        float nonuniformity = 1 - imageUniformity(mat);
//        float fitness = threshold_similarity * nonuniformity;
//
//        float min_value = 0.0000000001;  // TODO
//        fitness = std::max(fitness, min_value);
//
//        std::cout << "    fitness=" << fitness;
//        std::cout << " (threshold_similarity=" << threshold_similarity;
//        std::cout << " nonuniformity=" << nonuniformity << ")" << std::endl;
//        return fitness;
//    }
  
    float fitnessFunction(Individual* individual)
    {
        Texture& texture = *GP::textureFromIndividual(individual);
        texture.rasterizeToImageCache(getTargetImageSize().x(), false);
        cv::Mat mat = texture.getCvMat();
        drawGuiForFitnessFunction(mat, target_image_);
        float similarity = imageOhDearGodSimilarity(mat, target_image_);
        float nonuniformity = 1 - imageUniformity(mat);
        float fitness = similarity * nonuniformity;
        std::cout << "    fitness=" << fitness;
        std::cout << " (oh_dear_god_similarity=" << similarity;
        std::cout << " nonuniformity=" << nonuniformity << ")" << std::endl;
        return fitness;
    }
    
    // Returns a number on [0, 1] by a MIP-map-ish approach operating at various
    // levels of resolution.
    //
    // TODO problem, this wants to compare with target_pyramid_, but currently
    // gets args passed in as two Mats (m0, m1) just assuming m0 is "newest" and
    // m1 is "target". Needs redesign.
    //
    //  0  64x64  4096
    //  1  32x32  1024
    //  2  16x16   256
    //  3   8x8     64
    //  4   4x4     16
    //  5   2x2      4
    //  6   1x1      1
    //
    float imageMipMapSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        // Build "MIP map like" resolution pyramid for newest evolved image.
        std::vector<cv::Mat> newest_pyramid;
        makeResolutionPyramid(m0, newest_pyramid);
        assert(newest_pyramid.size() == target_pyramid_.size()); // TODO temp
        
        
        // TODO TEMP -- July 18, 2021 10:40-ish
        // use ONLY the 16x16 down sampled version
        
        // Index of the 1x1 image level in pyramid.
        size_t p = newest_pyramid.size() - 1;
        int step = 4;
        // debugPrint(newest_pyramid.at(p - step).cols);  ->  16 as expected
        return imageAvePixelSimilarity(newest_pyramid.at(p - step),
                                       target_pyramid_.at(p - step));

        /*

        //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//        // TODO July 15, pure multiplicative
//        float score = 1;
        // TODO July 15, average of layers
        float score = 0;
        //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~

        
//        int steps = 7; // 64x64 = 4096 at highest resolution level
//        int steps = 5; // 16x16 = 256 at highest resolution level
//        int steps = 4; // 8x8 = 64 at highest resolution level
//        int steps = 3; // 4x4 = 16 at highest resolution level
//        int steps = 4; // 8x8 = 64 at highest resolution level (July 17)
//        int steps = 3; // 4x4 = 16 at highest resolution level
        int steps = 5; // 16x16 = 256 at highest resolution level (July 18)
        for (int step = 0; step < steps; step++)
        {
            // Index of the 1x1 image level in pyramid.
            size_t p = newest_pyramid.size() - 1;
            // Increment score by similarity at this pyramid level.
            //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//        // TODO July 15, pure multiplicative
//            score *= imageProductPixelSimilarity(newest_pyramid.at(p - step),
//                                                 target_pyramid_.at(p - step));
            score += imageAvePixelSimilarity(newest_pyramid.at(p - step),
                                             target_pyramid_.at(p - step));
            //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
        }
        //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//        // TODO July 15, pure multiplicative
//        return score;
        // TODO July 15, average of layers
        return score / steps;
        //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
         
         
         */
    }

    // Returns a number on [0, 1]: the product of all pixel similarities.
    float imageProductPixelSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        float product_pixel_similarity = 1;
//        similarityHelper(m0, m1, [&](float s){product_pixel_similarity *= s;});
        similarityHelper(m0, m1, [&](float s){product_pixel_similarity *= sq(s);});
        float min_value = 0.0000000001;  // TODO
        return std::max(product_pixel_similarity, min_value);
    }
    
    // Applies a given function to the float value from Color::similarity() for
    // each pair of corresponding pixels in the two given cv::Mat references.
    void similarityHelper(const cv::Mat& m0,
                          const cv::Mat& m1,
                          std::function<void(float)> pixel_similarity) const
    {
        assert((m0.cols == m1.cols) && (m0.rows == m1.rows) &&
               (m0.cols > 0) && (m0.rows > 0));
        for (int x = 0; x < m0.cols; x++)
        {
            for (int y = 0; y < m0.rows; y++)
            {
                float similar = Color::similarity(getCvMatPixel(x, y, m0),
                                                  getCvMatPixel(x, y, m1));
                assert (between(similar, 0, 1));
                pixel_similarity(similar);
            }
        }
    }
    
//    // Returns a number on [0, 1] measuring: 1 - pixel_error_square_average.
//    float imageAvePixelSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
//    {
//        assert((m0.cols == m1.cols) && (m0.rows == m1.rows) &&
//               (m0.cols > 0) && (m0.rows > 0));
//        float sum_pixel_similarity = 0;
//        for (int x = 0; x < m0.cols; x++)
//        {
//            for (int y = 0; y < m0.rows; y++)
//            {
//                float similar = Color::similarity(getCvMatPixel(x, y, m0),
//                                                  getCvMatPixel(x, y, m1));
//                assert (between(similar, 0, 1));
//                sum_pixel_similarity += similar;
//            }
//        }
//        return sum_pixel_similarity / (m0.cols * m0.rows);
//    }

    // Returns a number on [0, 1] measuring: 1 - pixel_error_square_average.
    // TODO July 18, remove squaring to keep things simple for single-level MIP.
    float imageAvePixelSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        float sum = 0;  // sum of per-pixel similarity squared
//        similarityHelper(m0, m1, [&](float s){ sum += sq(s); });
        similarityHelper(m0, m1, [&](float s){ sum += s; });
        return sum / (m0.cols * m0.rows);
    }

    // Returns a number on [0, 1]: fraction of pixels with at least "threshold"
    // similarity.
    float imageThresholdSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        float threshold = 0.8;
        float sum = 0;  // sum of per-pixel similarities larger than threshold.
//        similarityHelper(m0, m1, [&](float s){ if (s > threshold) sum += s; });
        
//        auto pixel_similarity =
//            [&](float s){ sum += remapIntervalClip(s, threshold, 1, 0, 1); };
//        similarityHelper(m0, m1, pixel_similarity);
        
        similarityHelper(m0,
                         m1,
                         [&](float s)
                         {
                            sum += remapIntervalClip(s, threshold, 1, 0, 1);
                         });
        return sum / (m0.cols * m0.rows);
    }
    
    // Returns a number on [0, 1]: one last try (I really mean it THIS time).
    // Realized that imageThresholdSimilarity()--after tweaks-- was very similar
    // to taking average of squared per-pixel similarity. This just generalizes
    // the square as an expentiation. (Trying 5 on July 19, 2021)
//    float imageOhDearGodSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
//    {
//        float sum = 0;  // sum of exponentiated per-pixel similarities.
//        similarityHelper(m0, m1, [&](float s){ sum += std::pow(s, 5); });
//        return sum / (m0.cols * m0.rows);
//    }
    // TODO OK one more tweak, higher power, a bit more at the low end, see:
    // https://www.wolframalpha.com/input/?i=plot++y+%3D+%28%28x+*+0.05%29+%2B+%280.95+*+x%5E10%29%29%2C+x%3D+0+to+1%2C+y+%3D+0+to+1
    float imageOhDearGodSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        float sum = 0;  // sum of exponentiated per-pixel similarities.
        similarityHelper(m0,
                         m1,
                         [&](float s){ sum += ((0.05 * s) +
                                               (0.95 * std::pow(s, 10))); });
        return sum / (m0.cols * m0.rows);
    }

    
    // Returns a number on [0, 1] measuring minimum-of-all-pixel-similarities.
    float imageMinPixelSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        int m0w = m0.cols;
        int m0h = m0.rows;
        int m1w = m1.cols;
        int m1h = m1.rows;
        assert((m0w == m1w) && (m0h == m1h) && (m0w > 0) && (m0h > 0));
        float min_pixel_similarity = std::numeric_limits<float>::max();
        for (int x = 0; x < m0w; x++)
        {
            for (int y = 0; y < m0h; y++)
            {
                float similar = Color::similarity(getCvMatPixel(x, y, m0),
                                                  getCvMatPixel(x, y, m1));
                assert (between(similar, 0, 1));
                if (min_pixel_similarity > similar)
                {
                    min_pixel_similarity = similar;
                }
            }
        }
        return min_pixel_similarity;
    }
    
//        // Returns a number on [0, 1] measuring how similar two images are.
//        // TODO could be more efficient, but only only takes 0.0341692 for 511x511
//        //      images so is just a tiny fraction of the time for a texture render.
//        float imageSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
//        {
//            int m0w = m0.cols;
//            int m0h = m0.rows;
//            int m1w = m1.cols;
//            int m1h = m1.rows;
//            assert((m0w == m1w) && (m0h == m1h) && (m0w > 0) && (m0h > 0));
//            //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//            bool similarity_squared = false;
//            bool multiplicative = true;
//    //        bool similarity_squared = true;
//    //        bool multiplicative = false;
//            bool averaging = !multiplicative;
//            float similarlity = 0;
//            if (multiplicative) similarlity = 1;
//            //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//            for (int x = 0; x < m0w; x++)
//            {
//                for (int y = 0; y < m0h; y++)
//                {
//                    float similar = Color::similarity(getCvMatPixel(x, y, m0),
//                                                      getCvMatPixel(x, y, m1));
//                    assert (between(similar, 0, 1));
//                    //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//                    if (averaging) similarlity += similar;
//                    if (multiplicative) similarlity *= remapInterval(similar,
//                                                                     0, 1, 0.99, 1);
//                    //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//                }
//            }
//            //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//            if (averaging) similarlity /= m0w * m0h;
//            if (similarity_squared) similarlity *= similarlity;
//            //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//            return similarlity;
//        }

    // Returns a number on [0, 1] measuring how uniform a CV Mat is.
    // TODO Is 10 tests good? Use some other RS?
    float imageUniformity(const cv::Mat& mat) const
    {
        float uniformity = 1;
//        int tests = 10;
        int tests = 100;
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
    
    // Construct something like a "MIP map" with a series of images, each half
    // as wide as the previous one. Returns MIP map in output argument levels.
    // This assumes square images in the resolution pyramid, so the first step
    // is to find a square image near the size of the input image ("cv_mat").
    // If "cv_mat" is not square this will streatch it to fit.
    void makeResolutionPyramid(const cv::Mat& cv_mat,
                               std::vector<cv::Mat>& levels) const
    {
        // Find a square image with about the same number of pixels as cv_mat
        // (the original image) then "round" it to the nearest power of 2.
        int res = nearest_power_of_2(std::sqrt(cv_mat.cols * cv_mat.rows));
        // "full" holds a reference to the original image or (while looping,
        // below) the previous scaled down level in the MIP map.
        cv::Mat full = cv_mat;
        cv::Mat half;
        levels.clear();
        // Until we get to a 1x1 image, scale down the current one ("full") by
        // half in each dimension (into "half"), save it in the "levels"
        // collection, and copy "half" into "full".
        while (res > 1)
        {
            res /= 2;
            cv::resize(full, half, cv::Size(res, res), 0, 0, cv::INTER_AREA);
            levels.push_back(half);
            full = half;
        }
    }
    
    // Read a pixel as a Color value at given (x, y) in OpenCV Mat.
    Color getCvMatPixel(int x, int y, const cv::Mat& mat) const
    {
        cv::Vec3b bgrPixel = mat.at<cv::Vec3b>(x, y);
        float m = 255;
        return Color(bgrPixel[2] / m, bgrPixel[1] / m, bgrPixel[0] / m);
    };

    Vec2 getTargetImageSize() const
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
    
    void setGuiSize()
    {
        Vec2 my_screen(2880, 1800);  // TODO Craig's screen: very unportable.
//        my_screen /= 2;              // TODO oops, use "pixels" not "points"
        int target_x = getTargetImageSize().x();
        int target_y = getTargetImageSize().y();
        gui_grid_cols_ = my_screen.x() / (target_x + margin());
        gui_grid_rows_ = my_screen.y() / (target_y + margin());
        gui_.setSize(Vec2((gui_grid_cols_ * (target_x + margin())) + margin(),
                          (gui_grid_rows_ * (target_y + margin())) + margin()));
    }
    
    // TODO move/rethink
    int gui_grid_cols_ = 0;
    int gui_grid_rows_ = 0;

    
    int margin() const { return 10; }
    
    void drawGuiForFitnessFunction(const cv::Mat& newest, const cv::Mat& target)
    {
        // TODO topmost experiment
        gui().makeTopmost();
        
        
        // Push the two args onto the beginning of a collection of cv::Mat*.
        std::vector<const cv::Mat*> mats;
        mats.push_back(&newest);
        mats.push_back(&target);
        // Now walk down the fitness sorted Population, pushing rendered cv_mats
        // of high fitness individuals onto the collection.
        int mat_max = (gui_grid_cols_ * gui_grid_rows_) - 2;
        for (int i = 0; i < mat_max; i++)
        {
            Individual* individual = population_->nthBestFitness(i);
            if (individual->getFitness() > 0)
            {
                Texture& texture = *GP::textureFromIndividual(individual);
                texture.rasterizeToImageCache(getTargetImageSize().x(), false);
                const cv::Mat& mat = texture.getCvMat();
                mats.push_back(&mat);
            }
        }
        // Draw collection of Mats as a grid on the GUI.
        int count = 0;
        Vec2 draw_point(margin(), margin());
        Vec2 stride_x(target.cols + margin(), 0);
        Vec2 stride_y(0, target.rows + margin());
        for (auto mat : mats)
        {
            // TODO cf drawTextureOnGui() function in GP.h
            gui().drawMat(*mat, draw_point);
            count++;
            draw_point += stride_x;
            if (0 == (count % gui_grid_cols_))
            {
                draw_point += stride_y;
                draw_point = Vec2(margin(), draw_point.y());
            }
        }
        gui().setWindowTitle("SimpleImageMatch -- " +
                             run_name_ +
                             " -- step " +
                             std::to_string(population_->getStepCount()));
        gui().refresh();
    }

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
    
    std::vector<cv::Mat> target_pyramid_;

    // TODO copied from EvoCamoGame,
    // TODO should this just be a member inside SimpleImageMatch instance?
    // Points to heap-allocated Population instance during run() function.
    std::shared_ptr<Population> population_ = nullptr;
};
