//
//  GP.h
//  TexSyn
//
//  Created by Craig Reynolds on 9/20/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once
#include "../LazyPredator/LazyPredator.h"  // TODO use something more portable.
#include "GUI.h"
#include <fstream>

// Abbreviations for oft-repeated expressions below (used only in this file):
#define argFloat() tree.evalSubtree<float>(inc_tex_arg())
#define argVec2() tree.evalSubtree<Vec2>(inc_tex_arg())
#define argTexture() *tree.evalSubtree<Texture*>(inc_tex_arg())
#define evalTexture(body)                                \
[](GpTree& tree)                                         \
{                                                        \
    int i = 0;                                           \
    auto inc_tex_arg = [&](){ int j = i++; return j; };  \
    Texture* t = new body;                               \
    assert("evalTexture" && t->valid());                 \
    return std::any(t);                                  \
}

class GP
{
public:
    // LazyPredator FunctionSet coovering the "modern" TexSyn API.
    static const FunctionSet& fs() { return texsyn_gp_function_set; }
    static inline const FunctionSet texsyn_gp_function_set =
    {
        // GpTypes
        {
            {
                "Texture",
                [](std::any a)
                {
                    if (a.has_value())
                    {
                        Texture* t = std::any_cast<Texture*>(a);
                        if (t && t->valid()) delete t;
                    }
                }
            },
            { "Vec2" },
            { "Float_01", 0.0f, 1.0f },
            { "Float_02", 0.0f, 2.0f },
            { "Float_0_10", 0.0f, 10.0f },
            { "Float_m5p5", -5.0f, 5.0f }
        },
        // GpFunctions
        {
            {
                "Vec2", "Vec2", {"Float_m5p5", "Float_m5p5"},
                [](GpTree& tree)
                {
                    return std::any(Vec2(tree.evalSubtree<float>(0),
                                         tree.evalSubtree<float>(1)));
                }
            },
            
            {
                "Uniform", "Texture", {"Float_01", "Float_01", "Float_01"},
                evalTexture(Uniform(argFloat(), argFloat(), argFloat()))
            },
            
            {
                "Spot",
                "Texture",
                {"Vec2", "Float_02", "Texture", "Float_02", "Texture"},
                evalTexture(Spot(argVec2(),
                                 argFloat(),
                                 argTexture(),
                                 argFloat(),
                                 argTexture()))
            },
            {
                "Gradation", "Texture", {"Vec2", "Texture", "Vec2", "Texture"},
                evalTexture(Gradation(argVec2(),
                                      argTexture(),
                                      argVec2(),
                                      argTexture()))
            },
            {
                "Grating",
                "Texture",
                {"Vec2", "Texture", "Vec2", "Texture", "Float_01", "Float_01"},
                evalTexture(Grating(argVec2(),
                                    argTexture(),
                                    argVec2(),
                                    argTexture(),
                                    argFloat(),
                                    argFloat()))
            },
            {
                "SoftMatte", "Texture", {"Texture", "Texture", "Texture"},
                evalTexture(SoftMatte(argTexture(), argTexture(), argTexture()))
            },
            {
                "Add", "Texture", {"Texture", "Texture"},
                evalTexture(Add(argTexture(), argTexture()))
            },
            {
                "Subtract", "Texture", {"Texture", "Texture"},
                evalTexture(Subtract(argTexture(), argTexture()))
            },
            {
                "Multiply", "Texture", {"Texture", "Texture"},
                evalTexture(Multiply(argTexture(), argTexture()))
            },
            {
                "Max", "Texture", {"Texture", "Texture"},
                evalTexture(Max(argTexture(), argTexture()))
            },
            {
                "Min", "Texture", {"Texture", "Texture"},
                evalTexture(Min(argTexture(), argTexture()))
            },
            {"AbsDiff", "Texture", {"Texture", "Texture"},
                evalTexture(AbsDiff(argTexture(), argTexture()))
            },
            //{"NotEqual", "Texture", {"Texture", "Texture"}},
            //{"Noise", "Texture", {"Vec2", "Vec2", "Texture", "Texture"}},
            //{"Brownian", "Texture",
            //               {"Vec2", "Vec2", "Texture", "Texture"}},
            //{"Turbulence", "Texture",
            //               {"Vec2", "Vec2", "Texture", "Texture"}},
            //{"Furbulence", "Texture",
            //               {"Vec2", "Vec2", "Texture", "Texture"}},
            //{"Wrapulence", "Texture",
            //               {"Vec2", "Vec2", "Texture", "Texture"}},
            {
                "MultiNoise",
                "Texture",
                {"Vec2", "Vec2", "Texture", "Texture", "Float_01"},
                evalTexture(MultiNoise(argVec2(),
                                       argVec2(),
                                       argTexture(),
                                       argTexture(),
                                       argFloat()))
            },
            {
                "ColorNoise", "Texture", {"Vec2", "Vec2", "Float_01"},
                evalTexture(ColorNoise(argVec2(), argVec2(), argFloat()))
            },
            {
                "BrightnessToHue", "Texture", {"Float_01", "Texture"},
                evalTexture(BrightnessToHue(argFloat(), argTexture()))
            },
            {
                "Wrap", "Texture", {"Float_m5p5", "Vec2", "Vec2", "Texture"},
                evalTexture(Wrap(argFloat(), argVec2(), argVec2(), argTexture()))
            },
            {
                "StretchSpot",
                "Texture",
                {"Float_02", "Float_02", "Vec2", "Texture"},
                evalTexture(StretchSpot(argFloat(),
                                        argFloat(),
                                        argVec2(),
                                        argTexture()))
            },
            {
                "Stretch", "Texture", {"Vec2", "Vec2", "Texture"},
                evalTexture(Stretch(argVec2(), argVec2(), argTexture()))
            },
            {
                "SliceGrating", "Texture", {"Vec2", "Vec2", "Texture"},
                evalTexture(SliceGrating(argVec2(), argVec2(), argTexture()))
            },
            {
                "SliceToRadial", "Texture", {"Vec2", "Vec2", "Texture"},
                evalTexture(SliceToRadial(argVec2(), argVec2(), argTexture()))
            },
            {
                "SliceShear",
                "Texture",
                {"Vec2", "Vec2", "Texture", "Vec2", "Vec2", "Texture"},
                evalTexture(SliceShear(argVec2(),
                                       argVec2(),
                                       argTexture(),
                                       argVec2(),
                                       argVec2(),
                                       argTexture()))
            },
            {
                "Colorize", "Texture", {"Vec2", "Vec2", "Texture", "Texture"},
                evalTexture(Colorize(argVec2(),
                                     argVec2(),
                                     argTexture(),
                                     argTexture()))
            },
            {
                "MobiusTransform",
                "Texture",
                {"Vec2", "Vec2", "Vec2", "Vec2", "Texture"},
                evalTexture(MobiusTransform(argVec2(),
                                            argVec2(),
                                            argVec2(),
                                            argVec2(),
                                            argTexture()))
            },
            //{"Scale", "Texture", {"Float_m5p5", "Texture"}},
            //{"Rotate", "Texture", {"Float_m5p5", "Texture"}},
            //{"Translate", "Texture", {"Vec2", "Texture"}},
            {
                "Blur", "Texture", {"Float_01", "Texture"},
                evalTexture(Blur(argFloat(), argTexture()))
            },
            {
                "SoftThreshold", "Texture", {"Float_01", "Float_01", "Texture"},
                evalTexture(SoftThreshold(argFloat(), argFloat(), argTexture()))
            },
            {
                "EdgeDetect", "Texture", {"Float_01", "Texture"},
                evalTexture(EdgeDetect(argFloat(), argTexture()))
            },
            
            {
                "EdgeEnhance", "Texture", {"Float_01", "Float_02", "Texture"},
                evalTexture(EdgeEnhance(argFloat(), argFloat(), argTexture()))
            },
            {
                "AdjustHue", "Texture", {"Float_01", "Texture"},
                evalTexture(AdjustHue(argFloat(), argTexture()))
            },
            {
                "AdjustSaturation", "Texture", {"Float_01", "Texture"},
                evalTexture(AdjustSaturation(argFloat(), argTexture()))
            },
            {
                "AdjustBrightness", "Texture", {"Float_01", "Texture"},
                evalTexture(AdjustBrightness(argFloat(), argTexture()))
            },
            {
                "Twist",
                "Texture",
                {"Float_m5p5", "Float_m5p5", "Vec2", "Texture"},
                evalTexture(Twist(argFloat(),
                                  argFloat(),
                                  argVec2(),
                                  argTexture()))
            },
            {
                "BrightnessWrap", "Texture", {"Float_01", "Float_01", "Texture"},
                evalTexture(BrightnessWrap(argFloat(), argFloat(), argTexture()))
            },
            {
                "Mirror", "Texture", {"Vec2", "Vec2", "Texture"},
                evalTexture(Mirror(argVec2(), argVec2(), argTexture()))
            },
            {
                "Ring", "Texture", {"Float_0_10", "Vec2", "Vec2", "Texture"},
                evalTexture(Ring(argFloat(), argVec2(), argVec2(), argTexture()))
            },
            {
                "Row", "Texture", {"Vec2", "Vec2", "Texture"},
                evalTexture(Row(argVec2(), argVec2(), argTexture()))
            },
            //{"Shader", "Texture", {}},
            {
                "LotsOfSpots",
                "Texture",
                {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
                    "Texture", "Texture"},
                evalTexture(LotsOfSpots(argFloat(),
                                        argFloat(),
                                        argFloat(),
                                        argFloat(),
                                        argFloat(),
                                        argTexture(),
                                        argTexture()))
            },
            {
                "ColoredSpots",
                "Texture",
                {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
                    "Texture", "Texture"},
                evalTexture(ColoredSpots(argFloat(),
                                         argFloat(),
                                         argFloat(),
                                         argFloat(),
                                         argFloat(),
                                         argTexture(),
                                         argTexture()))
            },
            {
                "LotsOfButtons",
                "Texture",
                {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
                    "Vec2", "Texture", "Float_01", "Texture"},
                evalTexture(LotsOfButtons(argFloat(),
                                          argFloat(),
                                          argFloat(),
                                          argFloat(),
                                          argFloat(),
                                          argVec2(),
                                          argTexture(),
                                          argFloat(),
                                          argTexture()))
            },
            {
                "Gamma", "Texture", {"Float_0_10", "Texture"},
                evalTexture(Gamma(argFloat(), argTexture()))
            },
            {
                "RgbBox",
                "Texture",
                {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
                    "Float_01", "Texture"},
                evalTexture(RgbBox(argFloat(),
                                   argFloat(),
                                   argFloat(),
                                   argFloat(),
                                   argFloat(),
                                   argFloat(),
                                   argTexture()))
            },
            {
                "CotsMap",
                "Texture",
                {"Vec2", "Vec2", "Vec2", "Vec2", "Texture"},
                evalTexture(CotsMap(argVec2(),
                                    argVec2(),
                                    argVec2(),
                                    argVec2(),
                                    argTexture()))
            },
            {
                "Hyperbolic",
                "Texture",
                {"Vec2", "Float_0_10", "Float_0_10", "Float_0_10",
                    "Texture", "Texture"},
                evalTexture(Hyperbolic(argVec2(),
                                       argFloat(),
                                       argFloat(),
                                       argFloat(),
                                       argTexture(),
                                       argTexture()))
            },
            {
                "Affine", "Texture", {"Vec2", "Vec2", "Texture"},
                evalTexture(Affine(argVec2(), argVec2(), argTexture()))
            },
            {
                "HueOnly", "Texture", {"Float_01", "Float_01", "Texture"},
                evalTexture(HueOnly(argFloat(), argFloat(), argTexture()))
            }
        },
        // Crossover min_size, must be larger than: Uniform(r, g, b)
        5
    };
    
    // TODO move this elsewhere? Maybe static on Texture class?
    static Texture* textureFromIndividual(Individual* individual)
    {
        Texture* t = std::any_cast<Texture*>(individual->treeValue());
        assert(t);
        assert(t->valid());
        return t;
    };
};

#undef argFloat
#undef argVec2
#undef argTexture
#undef evalTexture

// Measures presence of high spatial frequencies ("confetti") in a texture.
// TODO 20201210 maybe move to new "Analyze" package?
// TODO 20201230 similar to Texture::highFrequencyScore()
//
// For these test textures:
//    Uniform black(0);
//    Uniform white(1);
//    Vec2 v0;
//    Vec2 v1(1, 1);
//    Noise n1(v0, v1, black, white);
//    Noise n2(v0, v1 / 10, black, white);
//    Noise n3(v0, v1 / 100, black, white);
// returns:
//    wiggliness(n1) = 0.00145902
//    wiggliness(n2) = 0.0150223
//    wiggliness(n3) = 0.130341
//
inline float wiggliness(const Texture& texture)
{
    float sum = 0;
    Color last_sample;
    float samples = 100;
    auto do_transect= [&](Vec2 v1, Vec2 v2)
    {
        for (int i = 0; i < samples; i++)
        {
            Vec2 sample_position = interpolate(i / samples, v1, v2);
            Color this_sample = texture.getColor(sample_position);
            float lum_diff = std::abs((last_sample - this_sample).luminance());
            if (i > 0) { sum += lum_diff; }
            last_sample = this_sample;
        }
        return sum / samples;
    };
    return (0.5 *
            (do_transect(Vec2(-0.1, -0.1), Vec2(+0.1, +0.1)) +
             do_transect(Vec2(-0.1, +0.1), Vec2(+0.1, -0.1))));
}


// TODO 20201205 a refactor of the LimitHue(Old) namespace as a class.
//      • should the whole run happen in the constructor?
//      • or should there be a separate run() function?
//      • should run() be a static that creates a instance on the fly?
//      • this is a prototype for somekind of base class for defining runs

class LimitHue
{
public:
    // Default constructor.
    LimitHue() : LimitHue(100, 100, 100, "") {}
    // Constructor with all parameters.
    LimitHue(int population_size,
             int max_init_tree_size,
             int evolution_steps,
             std::string path_for_saving_images)
      : population(population_size, max_init_tree_size, function_set),
        gui(guiSize(), Vec2(15, 15)),
        evolution_steps_(evolution_steps),
        path_for_saving_images_(path_for_saving_images) {}
    // Run the evolutionary computation for given number of steps.
    // TODO very generic, could be in base "run" class.
    void run()
    {
        Timer t(name() + " run");
        gui.drawText(gui_title_ + name(), 15, Vec2(20, 20), Color(0));
        gui.refresh();
        for (step = 0; step < evolution_steps_; step++)
        {
            population.evolutionStep([&](Individual* individual)
                                         { return fitnessFunction(individual); },
                                     function_set);
            if (per_step_hook_) per_step_hook_(population);
        }
        Individual* final_best = population.bestFitness();
        std::cout << "Final best in population:" << std::endl;
        std::cout << "fitness = " << final_best->getFitness() << std::endl;
        std::cout << final_best->tree().to_string() << std::endl;
        Texture* texture = GP::textureFromIndividual(final_best);
        std::string p = (path_for_saving_images_ + date_hours_minutes() +
                         "_" + name());
        Texture::displayAndFile(*texture, p, Texture::getDefaultRenderSize());
        // TODO need to be able to delete that specific window, or reuse.
        Texture::waitKey(1000);
    }
    // Determines fitness (on [0, 1]) for given Individual
    float fitnessFunction(Individual* individual)
    {
        Texture& texture = *GP::textureFromIndividual(individual);
        std::string pathname = "";
        Color average;
        const std::vector<Color>& samples = texture.cachedRandomColorSamples(LPRS());
        for (auto& color : samples) average += color;
        average *= 1.0 / samples.size();
        float brightness = average.luminance();
        float relative_distance_from_midrange = std::abs((brightness - 0.5) * 2);
        float closeness_to_midrange = remapIntervalClip(relative_distance_from_midrange,
                                                        0.8, 1, 1, 0);
        float average_saturation = average.getS();
        float enough_saturation = remapIntervalClip(average_saturation,
                                                    0, 0.4, 0.5, 1);
        std::cout << std::endl;
        float closeness_to_hue_constraint =
        measureHueConstraint(texture);
        // This one is just for the sake of logging, is that worthwhile?
        Individual* topind = population.bestFitness();
        Texture& toptex = *GP::textureFromIndividual(topind);
        measureHueConstraint(toptex);
        float wiggle_constraint = clip01(1 - wiggliness(texture));
        float fitness = (closeness_to_midrange *
                         closeness_to_hue_constraint *
                         enough_saturation *
                         wiggle_constraint);
        std::cout << "    fit=" << fitness;
        std::cout << " (hue=" << closeness_to_hue_constraint;
        std::cout << ", gray=" << closeness_to_midrange;
        std::cout << ", sat=" << enough_saturation;
        std::cout << ", wiggle=" << wiggle_constraint << ")";
        std::cout << std::endl << std::endl;
        updateGUI(individual, fitness);
        return fitness;
    }
    // A score on [0, 1] measuring how well given Texture meets the "LimitHue"
    // constraint. Hue is discretized into a 12 bucket histogram. Random samples
    // of Texture are inserted into hue histogram. The "ideal" pattern is for
    // all hue samples be in exactly four buckets, and none in any other. Also,
    // those four buckets should not be "adjacent" to each other in the modulo
    // 12 bucket ordering. Hue samples are only placed in the histogram if their
    // brightness is midrange and their saturation is sufficient. Score is based
    // on how well random texture samples match "ideal" pattern.
    float measureHueConstraint(Texture& texture)
    {
        int bucket_count = 12;
        int must_be_near_zero = 8;
        float min_saturation = 0.5;
        float min_brightness = 0.4;
        // Get random color samples from Texture, cached if previously generated.
        const std::vector<Color>& samples = texture.cachedRandomColorSamples(LPRS());
        // Set up histogram with "bucket_count" buckets
        class Bucket {public: int count; int index; };
        std::vector<Bucket> buckets(bucket_count);
        int k = 0;
        for (auto& b : buckets) { b.count = 0; b.index = k++;}
        // For each color sample, increment the corresponding histogram bucket.
        for (auto& color : samples)
        {
            // Transform into HSV metrics.
            float hue, saturation, value;
            color.getHSV(hue, saturation, value);
            // Increment hue bucket (unless dark or desaturated colors).
            if ((saturation > min_saturation) && (value > min_brightness))
            {
                int bucket_index = hue * bucket_count;
                if (bucket_index == bucket_count) bucket_index--;
                buckets.at(bucket_index).count++;
            }
        }
        // Sort buckets so biggest are at the front.
        std::sort(buckets.begin(), buckets.end(),
                  [](Bucket& a, Bucket& b){ return a.count > b.count; });
        // Find score (sum of abs error from target bucket size, neg for error)
        float score = 0;
        int big_buckets = bucket_count - must_be_near_zero;
        int target = int(samples.size()) / big_buckets;
        for (int i = 0; i < bucket_count; i++)
        {
            int ith_target = (i < big_buckets) ? target : 0;
            score -= sq(std::abs(buckets.at(i).count - ith_target));
        }
        // Adjacency_penalty: worse for each pair of adjacent "big buckets".
        float adjacency_penalty = 1;
        for (int i = 0; i < big_buckets; i++)
        {
            int i_index = buckets.at(i).index;
            for (int j = 0; j < big_buckets; j++)
            {
                int j_index = buckets.at(j).index;
                if (1 == modulo_abs_diff(i_index, j_index, bucket_count))
                {
                    adjacency_penalty *= 0.97;  // cost per adjacency
                }
            }
        }
        // Scale and shift score, adjust by adjacency_penalty.
        score = 1 + (score / 7500);
        score *= adjacency_penalty;
        // TODO debug print of score and buckets.
        std::cout << "    sorted hue buckets (";
        for (auto& b : buckets) std::cout << b.count << " ";
        std::cout << ") score=" << score << std::endl;
        return score;
    }
    
    // Display textures and text on GUI canvas. The current Individual and its
    // newly measured fitness is passed in. Also display "top ten" Individuals.
    void updateGUI(Individual* individual, float fitness)
    {
        Vec2 step_position(300, 20);
        gui.eraseRectangle(Vec2(200, gui_text_height_), step_position);
        std::string text = ("step " + std::to_string(step) +
                            " of " + std::to_string(evolution_steps_));
        gui.drawText(text, gui_text_height_, step_position, Color(1));
        Vec2 position(0, 50);
        drawTextureOnGui(individual, fitness, position);
        float row_spacing = gui_render_size_ + gui_text_height_ + gui_margin_;
        position += Vec2(0, row_spacing);
        // Draw the "top ten" textures in two rows of 5.
        for (int i = 0; i < 10; i++)
        {
            Individual* ind = population.nthBestFitness(i);
            float fit = ind->getFitness();
            if (ind->hasFitness()) drawTextureOnGui(ind, fit, position);
            position += Vec2(gui_margin_ + gui_render_size_, 0);
            if (i == 4) position = Vec2(0, position.y() + row_spacing);
        }
        gui.refresh();
    }

    // Render an Individual's Texture, above its numerical fitness (as percent).
    void drawTextureOnGui(Individual* individual,
                          float fitness,
                          const Vec2& upper_left_position)
    {
        Texture& texture = *GP::textureFromIndividual(individual);
        texture.rasterizeToImageCache(gui_render_size_, true);
        gui.drawTexture(texture, upper_left_position, gui_render_size_);
        Vec2 text_pos = upper_left_position + Vec2(0, gui_render_size_);
        std::string text = float_to_percent_fractional_digits(fitness, 1);
        Vec2 text_size(gui_render_size_, gui_text_height_);
        gui.eraseRectangle(text_size, text_pos);
        Vec2 center = text_pos + Vec2(gui_render_size_ / 2, 0);
        gui.drawTextHorizontalCenter(text, gui_text_height_, center, Color(1));
    }

    // Compute dimensions of the GUI window.
    Vec2 guiSize() const
    {
        int row_y = gui_render_size_ + gui_text_height_ + gui_margin_;
        return Vec2((gui_render_size_ * 5) + gui_margin_ * 6, row_y * 3 + 50);
    }
    // The generic name for this run.
    const std::string& name() const { return name_; }
        
    static void comparison(std::string pathname)
    {
        int runs = 4;
        int pop_size = 100;
        int tree_size = 100;
        int steps = 2000;
        Timer t("LimitHue::comparison run");
        std::vector<int> seeds =
        {
            444700671, 493213596, 840362786, 980307906, 261430925, 533548673,
            218546925, 633434603, 612577940, 999241686, 225521002, 597149672,
            207487440, 309365284, 110736518, 590685304, 949910212, 717531964,
            753082971, 176941139, 508919199, 562432923, 759317570, 473498701,
            207884788, 831877318, 393419179, 791381440, 723837506, 806703740,
            666704731, 291995111, 653300425, 449012055, 481843034, 515746916,
            166008386, 498372567, 169435341, 370407295, 255135461, 466952674
        };
        std::string logger_pathname;
        auto logger = [&](Population& p)
        {
            // Open file for append.
            std::ofstream outfile;
            outfile.open(logger_pathname, std::ios_base::app);
            assert(!outfile.fail());
            Individual* best_individual = p.bestFitness();
            outfile << best_individual->getFitness() << ",";
            outfile << p.averageFitness() << ",";
            outfile << p.averageTreeSize() << std::endl;
            outfile.close();
        };
        for (int run = 0; run < runs; run++)
        {
            std::string run_string = std::to_string(run + 1);
            std::string ab = "_a_";
            LPRS().setSeed(seeds.at(run));
            LimitHue lh1(pop_size, tree_size, steps, pathname + run_string + ab);
            logger_pathname = (pathname + run_string + ab +
                               date_hours_minutes() + ".txt");
            lh1.per_step_hook_ = logger;
            lh1.run();
            
            /*
            ab = "_b_";
            LPRS().setSeed(seeds.at(run));
            LimitHue lh2(pop_size, tree_size, steps, pathname + run_string + ab);
            logger_pathname = (pathname + run_string + ab +
                               date_hours_minutes() + ".txt");
            lh2.per_step_hook_ = logger;
            lh2.run();
             */
        }
    }
private:
    // TODO set this in default constructor?
    std::string name_ = "LimitHue";
    // Use the standard TexSyn FunctionSet.
    const FunctionSet& function_set = GP::fs();
    // GP population for this run.
    Population population;
    // Number of evolution steps (steady state population updates) in this run.
    int evolution_steps_;
    // GUI and parameters for it.
    int gui_render_size_ = 151;
    float gui_text_height_ = 15;
    float gui_margin_ = 10;
    std::string gui_title_ = "TexSyn/LazyPredator run: "; // TODO keep?
    GUI gui;
    int step;
    std::string path_for_saving_images_;
    std::function<void(Population&)> per_step_hook_ = nullptr;
};
