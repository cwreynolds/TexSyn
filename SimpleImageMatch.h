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
//    // TODO this is the original "absolute fitness" version.
//    void run()
//    {
//        while (true)
//        {
//            // logFunctionUsageCounts(out);
//            updateGuiWindowTitle();
//            population_->evolutionStep([&]
//                                       (Individual* i)
//                                       { return fitnessFunction(i); });
//        }
//        // Delete Population instance.
//        population_ = nullptr;
//    }
    
    // TODO this is the new "relative (tournament) fitness" version.
    void run()
    {
        while (true)
        {
            // logFunctionUsageCounts(out);
            updateGuiWindowTitle();
            population_->evolutionStep([&]
                                       (TournamentGroup tg)
                                       { return tournamentFunction(tg); });
        }
        // Delete Population instance.
        population_ = nullptr;
    }

    // TODO new September 2, 2021
    TournamentGroup tournamentFunction(TournamentGroup tg)
    {
        // Helper class for Individual, Texture, Count of most similar pixels.
        class ITC
        {
        public:
            ITC(Individual* i) : ITC(i, GP::textureFromIndividual(i)) {}
            // TODO instead of this, maybe setup() should return a ITC instead
            //      of an Individual. In fact, try again to move setup() into
            //      the ITC constructor.
            ITC(Individual* i, Texture* t)
              : individual(i),
                texture(t),
                nonuniformity(1 - imageUniformity(texture->getCvMat())) {}
            Individual* individual = nullptr;
            Texture* texture = nullptr;
            float nonuniformity = 1;
//            void countUp() { count++; }
            int count = 0;
            cv::Mat wins;
        };
        // Ensure each Texture has been rendered
        auto setup = [&](TournamentGroupMember& tgm)
        {
            Individual* i = tgm.individual;
            Texture* texture = GP::textureFromIndividual(i);
            if (texture->getCvMat().cols == 0)
            {
                Timer t("Render");
                std::cout << "  ";
                int width = target_image_.cols;
                int height = target_image_.rows;
                texture->rasterizeToImageCache(width, false); // false → square.
                cv::Mat mat = texture->getCvMat();
                assert((mat.cols == width) && (mat.rows == height));
            }
            return i;
        };
        // Build collection of ITCs.
        std::vector<ITC> itcs;
        for (auto& tgm : tg.members()) { itcs.push_back({ setup(tgm) }); }
        // Lookup ITC for given Individual
        auto get_itc = [&](Individual* i)
        {
            ITC* r = &itcs.front() ;
            for (auto& itc : itcs) { if (i == itc.individual) { r = &itc; } }
            return r;
        };
        // Set win Mats to be all black
        for (auto& itc : itcs)
        {
            itc.wins = cv::Mat(target_image_.cols, target_image_.rows, CV_8UC3);
            itc.wins.setTo(cv::Scalar(0));
        }
        // For each pixel: score one point to the tournament group member whose
        // pixel color is most similar to the target image pixel color.
        for (int i = 0; i < target_image_.cols; i++)
        {
            for (int j = 0; j < target_image_.rows; j++)
            {
                float max_similarity = std::numeric_limits<float>::lowest();
                Individual* max_sim_ind = nullptr;
                for (auto& itc : itcs)
                {
                    Color a = getCvMatPixel(i, j, itc.texture->getCvMat());
                    Color b = getCvMatPixel(i, j, target_image_);
                    float similarity = Color::similarity(a, b);
                    if (max_similarity < similarity)
                    {
                        max_similarity = similarity;
                        max_sim_ind = itc.individual;
                    }
                }
                assert(max_sim_ind != nullptr);
                ITC* max_sim_itc = get_itc(max_sim_ind);
                // Increment score of Individual with max similarity this pixel.
                max_sim_itc->count++;
                // Draw a gray pixel on "wins" map for that Individual.
                int n = 255 * max_sim_itc->nonuniformity;
                cv::Vec3b gray(n, n, n);
                cv::Point position(j, i);
                max_sim_itc->wins.at<cv::Vec3b>(position) = gray;
            }
        }
        // Set "arbitrary" alt_fitness only for sake of display ordering in GUI.
        for (auto& itc : itcs)
        {
            Individual* individual = itc.individual;
            if (individual->alt_fitness < 0)
            {
                Texture* texture = itc.texture;
                cv::Mat mat = texture->getCvMat();
                float similarity = imageAvePixelSimilarity(mat, target_image_);
                //float similarity = imageAug3Similarlity(mat, target_image_);
                assert(similarity >= 0);
                individual->alt_fitness = similarity;
            }
        }
        // Write the ITC counts into tg "metric" field.
        tg.setAllMetrics([&](Individual* i)
        {
            float m = 0;
            for (auto& itc : itcs)
            {
                Individual* j = itc.individual;
                if (i == j)
                {
                    cv::Mat mat = GP::textureFromIndividual(i)->getCvMat();
                    m = int(itc.count * itc.nonuniformity);
                }
            }
            return m;
        });
        std::cout << "  counts:";
        for (auto& tgm : tg.members()) { std::cout << " " << tgm.metric; }
        std::cout << "  \"fitnesses\":";
        auto get_nonuni = [&](Individual* i){return get_itc(i)->nonuniformity;};
        bool fitness_in_order = true;
        float prev_fitness = std::numeric_limits<float>::lowest();
        for (auto& tgm : tg.members())
        {
            float n = get_nonuni(tgm.individual);
            float f = n * tgm.individual->alt_fitness;
            if (prev_fitness > f) fitness_in_order = false;
            prev_fitness = f;
            std::cout << " " << f;
        }
        if (!fitness_in_order) std::cout << " (ooo)";
        std::cout << " (nonuniformities:";
        for (auto& tgm : tg.members())
        {
            std::cout << " " << std::setprecision(2);
            std::cout << get_nonuni(tgm.individual);
            std::cout << std::setprecision(8);

        }
        std::cout << ")";
        std::cout << "  survivals:";
        for (auto& tgm : tg.members())
        {
            std::cout << " " << tgm.individual->getTournamentsSurvived();
        }
        std::cout << std::endl;
        // Collect pointers to all Individuals, sort by "alt_fitness".
        std::vector<Individual*> all_alt_fitnesses;
        population_->applyToAllIndividuals([&]
                                           (Individual* i)
                                           { all_alt_fitnesses.push_back(i); });
        // Sort with largest fitness Individuals at the front.
        std::sort(all_alt_fitnesses.begin(),
                  all_alt_fitnesses.end(),
                  [](Individual* a, Individual* b)
                  { return a->alt_fitness > b->alt_fitness; });
        // Draw.
        std::vector<const cv::Mat*> mats;
        cv::Mat blank(target_image_.cols, target_image_.rows, CV_8UC3);
        blank.setTo(cv::Scalar(127, 127, 127));
        mats.push_back(&target_image_);
        for (auto& tgm : tg.members())
        {
            Individual* i = tgm.individual;
            mats.push_back(&blank);
            mats.push_back(&(GP::textureFromIndividual(i)->getCvMat()));
            mats.push_back(&(get_itc(i)->wins));
        }
        // Two rows of highest alt_fitness.
        for (int a = 0; a < 20; a++)
        {
            Individual* i = all_alt_fitnesses.at(a);
            mats.push_back(&(GP::textureFromIndividual(i)->getCvMat()));
        }
        drawGuiForFitnessFunction(mats);
        drawLineUnderNthRowInGUI(1, Color(1));
        drawLineUnderNthRowInGUI(3, Color(1));
        gui().refresh();
        return tg;
    }
    
    float fitnessFunction(Individual* individual)
    {
        Texture& texture = *GP::textureFromIndividual(individual);
        texture.rasterizeToImageCache(getTargetImageSize().x(), false);
        cv::Mat mat = texture.getCvMat();
        drawGuiForFitnessFunction(mat, target_image_);
//        float similarity = imageOhDearGodSimilarity(mat, target_image_);
//        float similarity = imageTotalErrorSquared(mat, target_image_);
//        float similarity = imageYetAnotherSimilarlity(mat, target_image_);
//        float similarity = imageJuly30Similarlity(mat, target_image_);
//        float similarity = imageJuly31Similarlity(mat, target_image_);
//        float similarity = imageAug2Similarlity(mat, target_image_);
//        float similarity = imageAug3Similarlity(mat, target_image_);
//        float similarity = imageThresholdSimilarity(mat, target_image_);
        float similarity = imageCoarseToFineSimilarity(mat, target_image_);
        float nonuniformity = 1 - imageUniformity(mat);
        float fitness = similarity * nonuniformity;
//        float fitness = similarity;
        std::cout << "    fitness=" << fitness;
//        std::cout << " (oh_dear_god_similarity=" << similarity;
//        std::cout << " (imageTotalErrorSquared=" << similarity;
//        std::cout << " (imageYetAnotherSimilarlity=" << similarity;
//        std::cout << " (imageJuly30Similarlity=" << similarity;
//        std::cout << " (imageJuly31Similarlity=" << similarity;
//        std::cout << " (imageAug2Similarlity=" << similarity;
//        std::cout << " (imageAug3Similarlity=" << similarity;
//        std::cout << " (imageThresholdSimilarity=" << similarity;
//        std::cout << " nonuniformity=" << nonuniformity << ")" << std::endl;
//        std::cout << " (imageCoarseToFineSimilarity=" << similarity << ")";
//        std::cout << std::endl;
        std::cout << " (imageCoarseToFineSimilarity=" << similarity;
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
        
#if 1  // use ONLY the 16x16 down sampled version
        
        // TODO TEMP -- July 18, 2021 10:40-ish
        // use ONLY the 16x16 down sampled version
        
        // Index of the 1x1 image level in pyramid.
        size_t p = newest_pyramid.size() - 1;
        int step = 4;
        // debugPrint(newest_pyramid.at(p - step).cols);  ->  16 as expected
        return imageAvePixelSimilarity(newest_pyramid.at(p - step),
                                       target_pyramid_.at(p - step));

#else  // use ONLY the 16x16 down sampled version

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
#endif  // use ONLY the 16x16 down sampled version
    }

    // Returns a number on [0, 1]: the product of all pixel similarities.
    float imageProductPixelSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        float product_pixel_similarity = 1;
        similarityHelper(m0, m1, [&](float s){product_pixel_similarity *= s;});
//        similarityHelper(m0, m1, [&](float s){product_pixel_similarity *= sq(s);});
//        float min_value = 0.0000000001;  // TODO
        float min_value = std::numeric_limits<float>::min() * 1024;
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
    
    // Returns a number on [0, 1] measuring: 1 - pixel_error_square_average.
    // TODO July 18, remove squaring to keep things simple for single-level MIP.
    float imageAvePixelSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        float sum = 0;  // sum of per-pixel similarity squared
        similarityHelper(m0, m1, [&](float s){ sum += s; });
        return sum / (m0.cols * m0.rows);
    }
    
    // TODO found this on August 6, when I was about to implement some that
    // matches the comment and name, but not the body of the function. Decided
    // to overwrite this with the thing I want now.

//    // Returns a number on [0, 1]: fraction of pixels with at least "threshold"
//    // similarity.
//    float imageThresholdSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
//    {
//        float threshold = 0.8;
//        float sum = 0;  // sum of per-pixel similarities larger than threshold.
//        similarityHelper(m0,
//                         m1,
//                         [&](float s)
//                         {
//                            sum += remapIntervalClip(s, threshold, 1, 0, 1);
//                         });
//        return sum / (m0.cols * m0.rows);
//    }

//        // Returns a number on [0, 1]: fraction of pixels with at least "threshold"
//        // similarity.
//        float imageThresholdSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
//        {
//            float threshold = 0.9;
//            int count = 0;  // count per-pixel similarities larger than threshold.
//            similarityHelper(m0, m1, [&](float s){ if (s > threshold) count++; });
//            return count / float(m0.cols * m0.rows);
//        }

//        // Returns a number on [0, 1]: fraction of pixels with at least "threshold"
//        // similarity.
//        float imageThresholdSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
//        {
//    //        float threshold = 0.9;
//            float threshold = 0.98;
//    //        int count = 0;  // count per-pixel similarities larger than threshold.
//            float sum = 0;
//            similarityHelper(m0, m1, [&](float s) {sum += (s > threshold) ? 1 : s / 100;});
//            return sum / (m0.cols * m0.rows);
//        }
    
    // Returns a number on [0, 1]: fraction of pixels with at least "threshold"
    // similarity.
    float imageThresholdSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        float sum = 0;
        similarityHelper(m0, m1, [&](float s){ sum += ((s > 0.95) ?
                                                       1 :
                                                       ((s > 0.90) ?
                                                        0.5 :
                                                        s / 100));});
        return sum / (m0.cols * m0.rows);
    }

    // Returns a number on [0, 1]: one last try (I really mean it THIS time).
    // Realized that imageThresholdSimilarity()--after tweaks-- was very similar
    // to taking average of squared per-pixel similarity. This just generalizes
    // the square as an expentiation. (Trying 5 on July 19, 2021)
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
    
    // TODO July 22, 2021
    float imageTotalErrorSquared(const cv::Mat& m0, const cv::Mat& m1) const
    {
        double sum = 0;
        float q = 10;
        similarityHelper(m0, m1, [&](float s) { sum += sq(q * (1 - s)); });
        float max = sq(q) * m0.cols * m0.rows;
        return (max - sum) / max; // TODO fix inline tuning
    }
    
    // similar to imageProductPixelSimilarity()
    float imageYetAnotherSimilarlity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        float p = 1;
        similarityHelper(m0, m1, [&](float s){p *= remapInterval(s,0,1,0.999,1);});
        return p;
    }

    // TODO July 30 version
    float imageJuly30Similarlity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        // Use an n² downsampled version of the two textures
        // const int step = 4;  // n = 16
        const int step = 6;  // n = 64
        const int n = std::pow(2, step);
        // Build "MIP map like" resolution pyramid for newest evolved image.
        std::vector<cv::Mat> newest_pyramid;
        makeResolutionPyramid(m0, newest_pyramid);
        assert(newest_pyramid.size() == target_pyramid_.size()); // TODO temp
        size_t p = newest_pyramid.size() - 1;  // Index of 1x1 level in pyramid.
        assert(newest_pyramid.at(p - step).cols == n);
        return imageProductPixelSimilarity(newest_pyramid.at(p - step),
                                           target_pyramid_.at(p - step));
    }

    
    // TODO July 31 version
    float imageJuly31Similarlity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        assert(m0.cols == m1.cols);
        assert(m0.rows == m1.rows);
        float product_of_similarities = 1;
//        const int n = 400; // take n random samples
        const int n = 200; // take n random samples
        for (int i = 0; i < n; i++)
        {
            int x = LPRS().random2(0, m0.cols);
            int y = LPRS().random2(0, m0.rows);
            Color a = getCvMatPixel(x, y, m0);
            Color b = getCvMatPixel(x, y, m1);
            product_of_similarities *= Color::similarity(a, b);
        }
        return product_of_similarities;
    }
    
    // TODO Aug 2 version
    float imageAug2Similarlity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        float sum = 0;
        float max = std::numeric_limits<float>::min();
        float min = std::numeric_limits<float>::max();
        similarityHelper(m0,
                         m1,
                         [&](float s)
                         {
                             sum += s;
                             max = std::max(max, s);
                             min = std::min(min, s);
                         });
//        float ave = sum / (m0.cols * m0.rows);
//        std::cout << "    (ave, min, max = " << ave << ", "
//                  << min << ", " << max << ")" << std::endl;
//        return ave * min * max;
        std::cout << "    (min, max = " << min << ", " << max << ")";
        return min * max;
    }

    // TODO August 3 version
    float imageAug3Similarlity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        assert(m0.cols == m1.cols);
        assert(m0.rows == m1.rows);
        // Make 32x32 grid of 1024 sample points on size-x-size.
        int n = 32;
        int size = m0.cols;
        std::vector<Vec2> offsets;
        jittered_grid_NxN_in_square(n, size, LPRS(), offsets);
        float product_of_similarities = 1;
        for (auto v : offsets)
        {
            int x = v.x() + size / 2;
            int y = v.y() + size / 2;
            assert(between(x, 0, size - 1));
            assert(between(y, 0, size - 1));
            float similarity = Color::similarity(getCvMatPixel(x, y, m0),
                                                 getCvMatPixel(x, y, m1));
            // TODO 20210805 try this tweak, probably not significant
//            float min = 1 - (1.0 / 128.0);
            float min = 1 - (1.0 / 64);
            float remap_similarity = remapInterval(similarity, 0, 1, min, 1);
            product_of_similarities *= remap_similarity;
        }
        return product_of_similarities;
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
    
//        // Written on Aug 20 2021, started from imageJuly30Similarlity()
//        // TODO note that this incorrectly depends on m1 being target_image_.
//        float imageCoarseToFineSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
//        {
//            // Use (up to) "steps" levels of the MIP maps
//            const int steps = 6;  // n = 64
//
//            //  level  side   area
//            //  i      2^i    (2^i)^2    0.99^area
//            //  -      ---    -------    -----------------
//            //  0      1      1          0.99
//            //  1      2      4          0.96059601
//            //  2      4      16         0.851457771094876
//            //  3      8      64         0.525596487525562
//            //  4      16     256        0.076314983906594
//            //  5      32     1024       0.000033918705402
//
//            // Build "MIP map like" resolution pyramid for newest evolved image.
//            std::vector<cv::Mat> newest_pyramid;
//            makeResolutionPyramid(m0, newest_pyramid);
//            assert(newest_pyramid.size() == target_pyramid_.size()); // TODO temp
//            size_t p = newest_pyramid.size() - 1;  // Index of 1x1 level in pyramid.
//            assert(newest_pyramid.at(p - steps).cols == std::pow(2, steps));
//
//            // TODO maybe this should be a member function of its own, but for now
//            // the imageProductPixelSimilarity() name is already taken for a since
//            // modified ad hoc version.
//            //
//            // Returns a number on [0, 1]: the product of all pixel similarities.
//            auto product_of_pixel_similarities =
//            [&]
//            (const cv::Mat& m0, const cv::Mat& m1)
//            {
//                float product_pixel_similarity = 1;
//    //            similarityHelper(m0,m1,[&](float s){product_pixel_similarity *= s;});
//                similarityHelper(m0,
//                                 m1,
//                                 [&](float s){ product_pixel_similarity *= s; });
//                return product_pixel_similarity;
//            };
//
//            // Traverse levels of the two MIP maps in parallel, starting at the 1x1
//            // "coarse" end, compute score for each level based on product of all
//            // pixel similarlties. Return the average (weighted sum) of each level
//            // score as the final fitness for m0.
//            //
//            // TODO another way to structure this to enforce coarse-to-fine is to
//            // stop after any level where the score is too low (eg less than 0.8),
//            // so only the 1x1 level is considered until that is 80% similar, then
//            // we move on to 2x2...
//            //
//            float sum_of_per_level_scores = 0;
//            for (int i = 0; i < steps; i++)
//            {
//                cv::Mat a = newest_pyramid.at(p - i);
//                cv::Mat b = target_pyramid_.at(p - i);
//                float level_score = product_of_pixel_similarities(a, b);
//                sum_of_per_level_scores += level_score;
//
//                std::cout << "        ";
//                std::cout << i << " score=" << level_score;
//                std::cout << " total=" << sum_of_per_level_scores;
//                std::cout << " normed=" << sum_of_per_level_scores / steps;
//                std::cout << std::endl;
//
//    //            float min_level_score_to_continue = 0.3;
//                float min_level_score_to_continue = 0.05;
//                if (level_score < min_level_score_to_continue) break;
//            }
//            return sum_of_per_level_scores / steps;
//        }

//        // TODO note that this incorrectly depends on m1 being target_image_.
//        float imageCoarseToFineSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
//        {
//            // Use (up to) "steps" levels of the MIP maps
//            const int steps = 6;  // n = 64
//
//            // Build "MIP map like" resolution pyramid for newest evolved image.
//            std::vector<cv::Mat> newest_pyramid;
//            makeResolutionPyramid(m0, newest_pyramid);
//            assert(newest_pyramid.size() == target_pyramid_.size()); // TODO temp
//            size_t p = newest_pyramid.size() - 1;  // Index of 1x1 level in pyramid.
//            assert(newest_pyramid.at(p - steps).cols == std::pow(2, steps));
//
//            // TODO maybe this should be a member function of its own, but for now
//            // the imageProductPixelSimilarity() name is already taken for a since
//            // modified ad hoc version.
//            //
//            // Returns a number on [0, 1]: the product of all pixel similarities.
//    //        auto product_of_pixel_similarities =
//    //        [&]
//    //        (const cv::Mat& m0, const cv::Mat& m1)
//    //        {
//    //            float product_pixel_similarity = 1;
//    //            similarityHelper(m0,
//    //                             m1,
//    //                             [&](float s){ product_pixel_similarity *= s; });
//    //            return product_pixel_similarity;
//    //        };
//            auto product_of_pixel_similarities =
//            [&](const cv::Mat& m0, const cv::Mat& m1)
//            {
//                float product_pixel_similarity = 1;
//    //            similarityHelper(m0,
//    //                             m1,
//    //                             [&](float s){ product_pixel_similarity *= s; });
//
//                assert((m0.cols==m0.rows)&&(m0.cols==m1.cols)&&(m0.cols==m1.rows));
//                int area = m0.cols * m0.rows;
//                float min = 1 - (1.0 / std::max(1.0, area / 8.0));
//                auto pf = [&](float s)
//                {
//                    product_pixel_similarity *= remapInterval(s, 0, 1, min, 1);
//                };
//                similarityHelper(m0, m1, pf);
//
//                return product_pixel_similarity;
//            };
//
//            // Traverse levels of the two MIP maps in parallel, starting at the 1x1
//            // "coarse" end, compute score for each level based on product of all
//            // pixel similarlties. Return the average (weighted sum) of each level
//            // score as the final fitness for m0.
//            //
//            // TODO another way to structure this to enforce coarse-to-fine is to
//            // stop after any level where the score is too low (eg less than 0.8),
//            // so only the 1x1 level is considered until that is 80% similar, then
//            // we move on to 2x2...
//            //
//            float sum_of_per_level_scores = 0;
//            for (int i = 0; i < steps; i++)
//            {
//                cv::Mat a = newest_pyramid.at(p - i);
//                cv::Mat b = target_pyramid_.at(p - i);
//                float level_score = product_of_pixel_similarities(a, b);
//                sum_of_per_level_scores += level_score;
//
//                std::cout << "        ";
//                std::cout << i << " score=" << level_score;
//                std::cout << " total=" << sum_of_per_level_scores;
//                std::cout << " normed=" << sum_of_per_level_scores / steps;
//                std::cout << std::endl;
//
//    //    //            float min_level_score_to_continue = 0.3;
//    //                float min_level_score_to_continue = 0.05;
//    //                if (level_score < min_level_score_to_continue) break;
//            }
//            return sum_of_per_level_scores / steps;
//        }
    
//        // TODO try weighting the per-MIP-level-score to emphasize high frequencies
//        //
//        // TODO note that this incorrectly depends on m1 being target_image_.
//        float imageCoarseToFineSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
//        {
//            // Use (up to) "steps" levels of the MIP maps
//            const int steps = 6;  // n = 64
//
//            // Build "MIP map like" resolution pyramid for newest evolved image.
//            std::vector<cv::Mat> newest_pyramid;
//            makeResolutionPyramid(m0, newest_pyramid);
//            assert(newest_pyramid.size() == target_pyramid_.size()); // TODO temp
//            size_t p = newest_pyramid.size() - 1;  // Index of 1x1 level in pyramid.
//            assert(newest_pyramid.at(p - steps).cols == std::pow(2, steps));
//
//            // Returns a number on [0, 1]: the product of all pixel similarities.
//            //
//            // TODO maybe this should be a member function of its own, but for now
//            // the imageProductPixelSimilarity() name is already taken for a since
//            // modified ad hoc version.
//            auto product_of_pixel_similarities =
//            [&](const cv::Mat& m0, const cv::Mat& m1)
//            {
//                float product_pixel_similarity = 1;
//                assert((m0.cols==m0.rows)&&(m0.cols==m1.cols)&&(m0.cols==m1.rows));
//                int area = m0.cols * m0.rows;
//                float min = 1 - (1.0 / std::max(1.0, area / 8.0));
//                auto pf = [&](float s)
//                {
//                    product_pixel_similarity *= remapInterval(s, 0, 1, min, 1);
//                };
//                similarityHelper(m0, m1, pf);
//
//                return product_pixel_similarity;
//            };
//
//            // Traverse levels of the two MIP maps in parallel, starting at the 1x1
//            // "coarse" end, compute score for each level based on product of all
//            // pixel similarlties. Return the average (weighted sum) of each level
//            // score as the final fitness for m0.
//            //
//            // TODO 20210825 experiment with nonuniform weights to emphasize higher
//            //               frequences.
//            //
//            float sum_of_per_level_scores = 0;
//            float sum_of_per_level_weights = 0;
//            for (int i = 0; i < steps; i++)
//            {
//                cv::Mat a = newest_pyramid.at(p - i);
//                cv::Mat b = target_pyramid_.at(p - i);
//
//                // TODO so 0→0.5, 1→1, 2→1.5, 3→2, 4→2.5, 5→3.
//                float level_weight = (i + 1) * 0.5;
//                sum_of_per_level_weights += level_weight;
//
//                float level_score = product_of_pixel_similarities(a, b);
//    //            sum_of_per_level_scores += level_score;
//                sum_of_per_level_scores += level_score * level_weight;
//
//                std::cout << "        ";
//                std::cout << i << " score=" << level_score;
//    //            std::cout << " total=" << sum_of_per_level_scores;
//    //            std::cout << " normed=" << sum_of_per_level_scores / steps;
//                std::cout << " level_score=" << level_score;
//                std::cout << " level_weight=" << level_weight;
//                std::cout << std::endl;
//            }
//    //        return sum_of_per_level_scores / steps;
//            return sum_of_per_level_scores / sum_of_per_level_weights;
//        }


//        // TODO try weighting the per-MIP-level-score to emphasize high frequencies
//        //
//        // TODO note that this incorrectly depends on m1 being target_image_.
//        float imageCoarseToFineSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
//        {
//            // Use (up to) "steps" levels of the MIP maps
//            const int steps = 6;  // n = 64
//
//            // Build "MIP map like" resolution pyramid for newest evolved image.
//            std::vector<cv::Mat> newest_pyramid;
//            makeResolutionPyramid(m0, newest_pyramid);
//            assert(newest_pyramid.size() == target_pyramid_.size()); // TODO temp
//            size_t p = newest_pyramid.size() - 1;  // Index of 1x1 level in pyramid.
//            assert(newest_pyramid.at(p - steps).cols == std::pow(2, steps));
//
//            // Returns a number on [0, 1]: the product of all pixel similarities.
//            //
//            // TODO maybe this should be a member function of its own, but for now
//            // the imageProductPixelSimilarity() name is already taken for a since
//            // modified ad hoc version.
//            auto product_of_pixel_similarities =
//            [&](const cv::Mat& m0, const cv::Mat& m1)
//            {
//                float product_pixel_similarity = 1;
//                assert((m0.cols==m0.rows)&&(m0.cols==m1.cols)&&(m0.cols==m1.rows));
//                int area = m0.cols * m0.rows;
//                float min = 1 - (1.0 / std::max(1.0, area / 8.0));
//                auto pf = [&](float s)
//                {
//                    product_pixel_similarity *= remapInterval(s, 0, 1, min, 1);
//                };
//                similarityHelper(m0, m1, pf);
//
//                return product_pixel_similarity;
//            };
//
//            // Traverse levels of the two MIP maps in parallel, starting at the 1x1
//            // "coarse" end, compute score for each level based on product of all
//            // pixel similarlties. Return the average (weighted sum) of each level
//            // score as the final fitness for m0.
//            //
//            // TODO 20210825 experiment with nonuniform weights to emphasize higher
//            //               frequences.
//            //
//            float sum_of_per_level_scores = 0;
//            float sum_of_per_level_weights = 0;
//            for (int i = 0; i < steps; i++)
//            {
//                cv::Mat a = newest_pyramid.at(p - i);
//                cv::Mat b = target_pyramid_.at(p - i);
//
//    //                // TODO so 0→0.5, 1→1, 2→1.5, 3→2, 4→2.5, 5→3.
//    //                float level_weight = (i + 1) * 0.5;
//    //                sum_of_per_level_weights += level_weight;
//    //
//    //                float level_score = product_of_pixel_similarities(a, b);
//    //    //            sum_of_per_level_scores += level_score;
//    //                sum_of_per_level_scores += level_score * level_weight;
//    //
//    //                std::cout << "        ";
//    //                std::cout << i << " score=" << level_score;
//    //    //            std::cout << " total=" << sum_of_per_level_scores;
//    //    //            std::cout << " normed=" << sum_of_per_level_scores / steps;
//    //                std::cout << " level_score=" << level_score;
//    //                std::cout << " level_weight=" << level_weight;
//    //                std::cout << std::endl;
//
//    //            // TODO so 0→0.5, 1→1, 2→1.5, 3→2, 4→2.5, 5→3.
//    //            float level_weight = (i + 1) * 0.5;
//
//                // TODO so 0→1, 1→1.2, 2→1.4, 3→1.6, 4→1.8, 5→2.0
//                float level_weight = 1 + (i * 0.2);
//
//                sum_of_per_level_weights += level_weight;
//
//                float level_score = product_of_pixel_similarities(a, b);
//                sum_of_per_level_scores += level_score * level_weight;
//
//                std::cout << "        " << i ;
//                std::cout << " level_score=" << level_score;
//                std::cout << " level_weight=" << level_weight;
//                std::cout << std::endl;
//
//            }
//            return sum_of_per_level_scores / sum_of_per_level_weights;
//        }
    
//        // TODO sequentially ratchet up level-by-level (coarse to fine): when level
//        // N is below a score threshold of (say) 0.5 we do not go on to next step,
//        // then once it exceeds that threshold we just bump the level N score up to
//        // 100%, and go on to level N+1, etc. So once 1x1 works “pretty well” we
//        // assume it is perfect and fitness varies according to the the 2x2 level.
//        //
//        // TODO remove weighting per-MIP-level-score to emphasize high frequencies
//        //
//        // TODO note that this incorrectly depends on m1 being target_image_.
//        float imageCoarseToFineSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
//        {
//            // Use (up to) "steps" levels of the MIP maps
//            const int steps = 6;  // n = 64
//
//            // Build "MIP map like" resolution pyramid for newest evolved image.
//            std::vector<cv::Mat> newest_pyramid;
//            makeResolutionPyramid(m0, newest_pyramid);
//            assert(newest_pyramid.size() == target_pyramid_.size()); // TODO temp
//            size_t p = newest_pyramid.size() - 1;  // Index of 1x1 level in pyramid.
//            assert(newest_pyramid.at(p - steps).cols == std::pow(2, steps));
//
//            // Returns a number on [0, 1]: the product of all pixel similarities.
//            //
//            // TODO maybe this should be a member function of its own, but for now
//            // the imageProductPixelSimilarity() name is already taken for a since
//            // modified ad hoc version.
//            auto product_of_pixel_similarities =
//            [&](const cv::Mat& m0, const cv::Mat& m1)
//            {
//                float product_pixel_similarity = 1;
//                assert((m0.cols==m0.rows)&&(m0.cols==m1.cols)&&(m0.cols==m1.rows));
//                int area = m0.cols * m0.rows;
//                float min = 1 - (1.0 / std::max(1.0, area / 8.0));
//                auto pf = [&](float s)
//                {
//                    product_pixel_similarity *= remapInterval(s, 0, 1, min, 1);
//                };
//                similarityHelper(m0, m1, pf);
//
//                return product_pixel_similarity;
//            };
//
//            // Traverse levels of the two MIP maps in parallel, starting at the 1x1
//            // "coarse" end, compute score for each level based on product of all
//            // pixel similarlties. Return the average (weighted sum) of each level
//            // score as the final fitness for m0.
//            //
//            // TODO 20210825 experiment with nonuniform weights to emphasize higher
//            //               frequences.
//            //
//            float sum_of_per_level_scores = 0;
//    //        float sum_of_per_level_weights = 0;
//            for (int i = 0; i < steps; i++)
//            {
//                cv::Mat a = newest_pyramid.at(p - i);
//                cv::Mat b = target_pyramid_.at(p - i);
//
//    //            // TODO so 0→1, 1→1.2, 2→1.4, 3→1.6, 4→1.8, 5→2.0
//    //            float level_weight = 1 + (i * 0.2);
//    //            sum_of_per_level_weights += level_weight;
//
//    //            sum_of_per_level_weights++;
//
//                float level_score = product_of_pixel_similarities(a, b);
//    //            sum_of_per_level_scores += level_score * level_weight;
//
//                float level_completed_threshold = 0.33; // TODO just arbitrary guess
//                float adjusted_score = ((level_score > level_completed_threshold) ?
//                                        1 :
//                                        level_score);
//
//    //            if (level_score > level_completed_threshold) { level_score = 1; }
//    //            sum_of_per_level_scores += level_score;
//                sum_of_per_level_scores += adjusted_score;
//
//                std::cout << "        " << i ;
//                std::cout << " level_score=" << level_score;
//                std::cout << " adjusted_score=" << adjusted_score;
//                std::cout << std::endl;
//
//                // Do not advance to next finer level unless this one is completed.
//                if (adjusted_score < 1) break;
//            }
//    //        return sum_of_per_level_scores / sum_of_per_level_weights;
//            return sum_of_per_level_scores / steps;
//        }

    // TODO sequentially ratchet up level-by-level (coarse to fine): when level
    // N is below a score threshold of (say) 0.5 we do not go on to next step,
    // then once it exceeds that threshold we just bump the level N score up to
    // 100%, and go on to level N+1, etc. So once 1x1 works “pretty well” we
    // assume it is perfect and fitness varies according to the the 2x2 level.
    //
    // TODO remove weighting per-MIP-level-score to emphasize high frequencies
    //
    // TODO note that this incorrectly depends on m1 being target_image_.
    float imageCoarseToFineSimilarity(const cv::Mat& m0, const cv::Mat& m1) const
    {
        // Use (up to) "steps" levels of the MIP maps
        const int steps = 6;  // n = 64
        
        // Build "MIP map like" resolution pyramid for newest evolved image.
        std::vector<cv::Mat> newest_pyramid;
        makeResolutionPyramid(m0, newest_pyramid);
        assert(newest_pyramid.size() == target_pyramid_.size()); // TODO temp
        size_t p = newest_pyramid.size() - 1;  // Index of 1x1 level in pyramid.
        assert(newest_pyramid.at(p - steps).cols == std::pow(2, steps));
        
        // Returns a number on [0, 1]: the product of all pixel similarities.
        //
        // TODO maybe this should be a member function of its own, but for now
        // the imageProductPixelSimilarity() name is already taken for a since
        // modified ad hoc version.
        auto product_of_pixel_similarities =
        [&](const cv::Mat& m0, const cv::Mat& m1)
        {
            float product_pixel_similarity = 1;
            assert((m0.cols==m0.rows)&&(m0.cols==m1.cols)&&(m0.cols==m1.rows));
            int area = m0.cols * m0.rows;
            float min = 1 - (1.0 / std::max(1.0, area / 8.0));
            auto pf = [&](float s)
            {
                product_pixel_similarity *= remapInterval(s, 0, 1, min, 1);
            };
            similarityHelper(m0, m1, pf);

            return product_pixel_similarity;
        };

        // Traverse levels of the two MIP maps in parallel, starting at the 1x1
        // "coarse" end, compute score for each level based on product of all
        // pixel similarlties. If this exceeds level_completed_threshold
        // consider the level "good enough" give it full score, otherwise do not
        // continue to next finer level.
//        float level_completed_threshold = 0.33; // TODO just arbitrary guess
        float level_completed_threshold = 0.66; // TODO just arbitrary guess
        float sum_of_per_level_scores = 0;
        for (int i = 0; i < steps; i++)
        {
            cv::Mat a = newest_pyramid.at(p - i);
            cv::Mat b = target_pyramid_.at(p - i);
            float level_score = product_of_pixel_similarities(a, b);
            float adjusted_score = ((level_score > level_completed_threshold) ?
                                    1 :
                                    level_score);
            sum_of_per_level_scores += adjusted_score;
            std::cout << "        " << i ;
            std::cout << " level_score=" << level_score;
            std::cout << " adjusted_score=" << adjusted_score;
            std::cout << std::endl;
            // Do not advance to next finer level unless this one is completed.
            if (adjusted_score < 1) break;
        }
        return sum_of_per_level_scores / steps;
    }

    // Returns a number on [0, 1] measuring how uniform a CV Mat is.
    // TODO Is 10 tests good? Use some other RS?
//    float imageUniformity(const cv::Mat& mat) const
    static float imageUniformity(const cv::Mat& mat) // const
    {
        float uniformity = 1;
        // int tests = 10;
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
//    Color getCvMatPixel(int x, int y, const cv::Mat& mat) const
    static Color getCvMatPixel(int x, int y, const cv::Mat& mat) // const
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
        // Push the two args onto the beginning of a collection of cv::Mat*.
        std::vector<const cv::Mat*> mats;
        mats.push_back(&newest);
        mats.push_back(&target);
        drawGuiForFitnessFunction(mats);
    }
    
    
//    void drawGuiForFitnessFunction(const cv::Mat& newest, const cv::Mat& target)
    void drawGuiForFitnessFunction(std::vector<const cv::Mat*>& mats)
    {
//        // Push the two args onto the beginning of a collection of cv::Mat*.
//        std::vector<const cv::Mat*> mats;
//        mats.push_back(&newest);
//        mats.push_back(&target);
        // Now walk down the fitness sorted Population, pushing rendered cv_mats
        // of high fitness individuals onto the collection.
//        int mat_max = (gui_grid_cols_ * gui_grid_rows_) - 2;
        int mat_max = (gui_grid_cols_ * gui_grid_rows_) - int(mats.size());
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
        Vec2 stride_x(mats.at(0)->cols + margin(), 0);
        Vec2 stride_y(0, mats.at(0)->rows + margin());
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
        updateGuiWindowTitle();
    }
    
    void updateGuiWindowTitle()
    {
        gui().setWindowTitle("SimpleImageMatch -- " +
                             run_name_ +
                             " -- step " +
                             std::to_string(population_->getStepCount()));
        gui().refresh();
    }
    
    void drawLineUnderNthRowInGUI(int n, Color c)
    {
        Vec2 size(gui().getSize().x(), 1);
        float row_height = target_image_.rows + margin();
        Vec2 position(0, (row_height * n) + (margin() * 0.5) - 1);
        gui().drawRectangle(size, position, c);
    }
    
    void testFitnessOnNearMiss()
    {
        std::string dir =
            "/Users/cwr/Desktop/SimpleImageMatch/target_images/near_miss/";
        std::string t  = dir + "SIM_3812_target.jpeg";
        std::string g1 = dir + "SIM_3812_gray_1.jpeg";
        std::string g2 = dir + "SIM_3812_gray_2.jpeg";
        std::string g3 = dir + "SIM_3812_gray_3.jpeg";
        
        cv::Mat mat_t = cv::imread(t);
        cv::Mat mat_g1 = cv::imread(g1);
        cv::Mat mat_g2 = cv::imread(g2);
        cv::Mat mat_g3 = cv::imread(g3);
        
        // Build "MIP map like" resolution pyramid for target image.
        makeResolutionPyramid(mat_t, target_pyramid_);


//        debugPrint(imageThresholdSimilarity(mat_t, mat_t));
//        debugPrint(imageThresholdSimilarity(mat_g1, mat_t));
//        debugPrint(imageThresholdSimilarity(mat_g2, mat_t));
//        debugPrint(imageThresholdSimilarity(mat_g3, mat_t));
        debugPrint(imageCoarseToFineSimilarity(mat_t, mat_t));
        debugPrint(imageCoarseToFineSimilarity(mat_g1, mat_t));
        debugPrint(imageCoarseToFineSimilarity(mat_g2, mat_t));
        debugPrint(imageCoarseToFineSimilarity(mat_g3, mat_t));

        // gets:
        //
        // imageThresholdSimilarity(mat_t, mat_t) = 1
        // imageThresholdSimilarity(mat_g1, mat_t) = 0.779863
        // imageThresholdSimilarity(mat_g2, mat_t) = 0.512363
        // imageThresholdSimilarity(mat_g3, mat_t) = 0.200073
        //
        // So: t-vs-t is a perfect match, setting the leftmost ~1/4 to gray gets
        // about 3/4 similarlity, setting the leftmost ~1/2 to gray gets about
        // 1/2 similarlity, and setting the leftmost ~3/4 to gray gets about 1/4
        // similarlity. Its almost as if it is working correctly!
    }
    
    // Trying to figure out per-level scoring in imageCoarseToFineSimilarity().
    static void testScoreRanging()
    {
        RandomSequence rs(95628347);
        for (int i = 0; i < 6; i++)
        {
            int side = std::pow(2, i);
            int area = std::pow(side, 2);
            float pix_prod_high = 1;
            float pix_prod_midrange = 1;
            float pix_prod_random = 1;
            float min = 1 - (1.0 / std::max(1.0, area / 8.0));
            for (int j = 0; j < area; j++)
            {
                pix_prod_high *= remapInterval(0.99, 0, 1, min, 1);
                pix_prod_midrange *= remapInterval(0.5, 0, 1, min, 1);
                pix_prod_random *= remapInterval(rs.frandom01(), 0, 1, min, 1);
            }
            
            std::cout << "i=" << i;
            std::cout << "    side=" << side;
            std::cout << "    area=" << area;
            std::cout << "    min=" << min;
            std::cout << "    pix_prod_high=" << pix_prod_midrange;
            std::cout << "    pix_prod_midrange=" << pix_prod_midrange;
            std::cout << "    pix_prod_random=" << pix_prod_random;
            std::cout << std::endl;
        }
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
