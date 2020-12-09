//
//  GP.h
//  TexSyn
//
//  Created by Craig Reynolds on 9/20/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once
#include "../LazyPredator/LazyPredator.h"  // TODO use something more portable.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20201204 start GUI package
#include "GUI.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
// TODO 20201201 prototyping new "one window" GUI
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// TODO 20201202 very experimental putText()
#include <opencv2/freetype.hpp>
#pragma clang diagnostic pop
//~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~

// Abbreviations for oft-repeated expressions below (used only in this file):
#define argFloat() tree.evalSubtree<float>(inc_tex_arg())
#define argVec2() tree.evalSubtree<Vec2>(inc_tex_arg())
#define argTexture() *tree.evalSubtree<Texture*>(inc_tex_arg())

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20201116 very experimental record result of eval() to
//               assist in deleting all of an Individual.
//
// changed all (const GpTree& t) to (/*const*/ GpTree& t)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    #define evalTexture(body)                                \
//    [](const GpTree& t)                                   \
//    {                                                        \
//        int i = 0;                                           \
//        auto inc_tex_arg = [&](){ int j = i++; return j; };  \
//        Texture* t = new body;                               \
//        return std::any(t);                                  \
//    }
#define evalTexture(body)                                \
[](GpTree& tree)                                   \
{                                                        \
    int i = 0;                                           \
    auto inc_tex_arg = [&](){ int j = i++; return j; };  \
    Texture* t = new body;                               \
    assert("evalTexture" && t->valid()); \
    return std::any(t);                                  \
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20201122
// experiment above to check validity of newly constructed Tex.
//     assert("evalTexture" && t->valid());
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class GP
{
public:
    // LazyPredator FunctionSet coovering the "modern" TexSyn API.
    static const FunctionSet& fs() { return texsyn_gp_function_set; }
    static inline const FunctionSet texsyn_gp_function_set =
    {
        // GpTypes
        {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // TODO use experimental "deleter" function. EG for Texture in TexSyn.
//            {"Texture"},
//            {"Vec2"},
            
//            // TODO gets EXC_BAD_ACCESS at end of deleting Population
//            //      Texture: constructions=2163, destructions=1995, leaked=168
//            { "Texture", [](std::any a)
//                {
//                    // TODO very temp
//                    std::cout << "In deleter for Texture GpType";
//                    if (a.has_value())
//                    {
//                        Texture* t = std::any_cast<Texture*>(a);
//                        std::cout << " t=" << t;
//
//                        // TODO 20201121 temporary for debugging
//                        t->validate();
//
//                        if (t) delete t;
//                    }
//                    std::cout << std::endl;
//                }
//            },
            
            { "Texture", [](std::any a)
                {
                    if (a.has_value())
                    {
                        Texture* t = std::any_cast<Texture*>(a);
                        if (t && t->valid()) delete t;
                    }
                }
            },

//            // TODO this runs to end but reports:
//            //      Texture: constructions=2163, destructions=321, leaked=1842
//            //      (oh, because it is deleting merely the Texture* pointer?)
//            { "Texture", [](std::any a) { a.reset(); } },
            
            { "Vec2" },
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            { "Float_01", 0.0f, 1.0f },
            { "Float_02", 0.0f, 2.0f },
            { "Float_0_10", 0.0f, 10.0f },
            { "Float_m5p5", -5.0f, 5.0f }
        },
        // GpFunctions
        {
            {
                "Vec2", "Vec2", {"Float_m5p5", "Float_m5p5"},
                [](/*const*/ GpTree& tree)
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
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // TODO 20201128 diagnosed bug last night, saw this error today
//                {"Vec2", "Vec2", "Vec2", "Vec2", "Texture", "Texture"},
                {"Vec2", "Vec2", "Vec2", "Vec2", "Texture"},
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
    
    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // Yellow-Green evolution test
    
    static Texture* textureFromIndividual(Individual* individual)
    {
        return std::any_cast<Texture*>(individual->treeValue());
    };
    static Texture* textureFromIndividual(std::shared_ptr<Individual> individual)
    {
        return std::any_cast<Texture*>(individual->treeValue());
    };

    // For yellow/green evolution test.
    static Color ygAverageColorOfTexture(Texture* texture)
    {
        int n = 10;
        std::vector<Vec2> samples;
        jittered_grid_NxN_in_square(n, 1.4, LPRS(), samples);
        Color sum;
        for (auto s : samples) sum += texture->getColor(s).clipToUnitRGB();
        return sum / sq(n);
    };

    // For yellow/green evolution test.
    static Color ygAverageColorOfPopulation(Population& population)
    {
        Color sum;
        for (auto individual : population.individuals())
        {
            Texture* texture = GP::textureFromIndividual(individual);
            sum += GP::ygAverageColorOfTexture(texture);
        }
        return sum / population.individuals().size();
    };
    
    static void runYellowGreenTest()
    {
        Timer t("runYellowGreenTest");
        const FunctionSet& function_set = GP::fs();
        int population_size = 50;
        //int generation_equivalents = 100;
        int generation_equivalents = 20;
        int steps = population_size * generation_equivalents;
        int max_tree_size = 100;
        Population population(population_size, max_tree_size, function_set);
        int step_count = 0;
        // Print log and render Texture.
        auto logger = [&](TournamentGroup group)
        {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // TODO 20201121 try converting Population over to std::shared_ptr
//            Individual* tournament_best = group.bestIndividual();
            std::shared_ptr<Individual> tournament_best = group.bestIndividual();
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            if (false)
            {
                std::cout << std::endl << "step " << step_count++ << std::endl;
                std::cout << "winner size ";
                std::cout << tournament_best->tree().size();
                std::cout << ", winner tournaments survived ";
                std::cout << tournament_best->getTournamentsSurvived();
                std::cout << std::endl;
                Texture* t = GP::textureFromIndividual(tournament_best);
                Texture::displayAndFile(*t, "", 99);
                Texture::waitKey(1);
                Texture::closeAllWindows();
            }
            else  // for plotting:
            {
                step_count++;
                Color ac = ygAverageColorOfPopulation(population);
                std::cout << ac.r() << "," << ac.g() << "," << ac.b() << ",";
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // TODO 20201121 try converting Population over to std::shared_ptr
//                Individual* best_individual = population.findBestIndividual();
                std::shared_ptr<Individual> best_individual = population.findBestIndividual();
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                Texture* bt = GP::textureFromIndividual(best_individual);
                Color bc = ygAverageColorOfTexture(bt);
                std::cout << bc.r()<<"," << bc.g()<<"," << bc.b() << std::endl;
            }
        };

        // Given 3 Individuals and metric, find the one with lowest metric.
        auto lowest_average_metric = [&]
        (TournamentGroup group, std::function<float(const Color&)> color_metric)
        {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // TODO 20201121 try converting Population over to std::shared_ptr
//            group.setAllMetrics([&](Individual* i)
            group.setAllMetrics([&](std::shared_ptr<Individual> i)
            {
                Texture* texture = GP::textureFromIndividual(i);
                return color_metric(GP::ygAverageColorOfTexture(texture));
            });
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            logger(group);
            return group;
        };

        auto tournament_function = [&](TournamentGroup group)
        {
            if (LPRS().frandom01() < 0.5)
            {
                auto high_green = [](const Color& c){ return c.green(); };
                return lowest_average_metric(group, high_green);
            }
            else
            {
                auto low_blue = [](const Color& c)
                    { return remapIntervalClip(c.blue(), 0, 1, 1, 0); };
                return lowest_average_metric(group, low_blue);
            }
        };
    // Perform evolution run.
    population.run(steps, function_set, tournament_function);
    }
};

#undef argFloat
#undef argVec2
#undef argTexture
#undef evalTexture

// “colorful, well exposed” evolution test
namespace CWE
{

// Global Population instance. (TODO maybe should be shared_ptr?)
// TODO refactor to share via lexical capture?
static inline Population* population = nullptr;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//static inline int render_size = 99;

std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Print log and render Texture.
void logger(TournamentGroup group)
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201121 try converting Population over to std::shared_ptr
//    //    static int step_count = 0;
//        Individual* best = group.bestIndividual();
//        Individual* best2 = group.secondBestIndividual();
//        Individual* worst = group.worstIndividual();
//        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
//    //    std::vector<Individual*> tops = population->nMostTournamentsSurvived(6);
//        std::vector<Individual*> tops = population->nTopFitness(6);
//        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

    std::shared_ptr<Individual> best = group.bestIndividual();
    std::shared_ptr<Individual> best2 = group.secondBestIndividual();
    std::shared_ptr<Individual> worst = group.worstIndividual();
    std::vector<std::shared_ptr<Individual>> tops = population->nTopFitness(6);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    //~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~

//    std::cout << std::endl << "step " << step_count++ << std::endl;
//    std::cout << "winner size ";
//    std::cout << best->tree().size();
//    std::cout << ", winner tournaments survived ";
//    std::cout << best->getTournamentsSurvived();
//    std::cout << std::endl;
    
    
//        std::chrono::time_point<std::chrono::high_resolution_clock>
//            now_time = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> elapsed_time = now_time - start_time;
//        start_time = now_time;
//
//        // 1527: t=0.7, winner size=85 won=7, pop ave size=107 won=2,
//        std::cout << step_count++ << ": ";
//        std::cout << "t=" << elapsed_time.count() << ", ";
//        std::cout << "winner size=" << best->tree().size();
//        std::cout << " won=" << best->getTournamentsSurvived() << ", ";
//        std::cout << "pop ave size=" << population->averageTreeSize();
//        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
//    //    std::cout << " won=" << population->averageTournamentsSurvived();
//        std::cout << " won=" << population->averageTournamentsSurvived() << ", ";
//
//        std::vector<float> fits;
//        for (auto i : tops) fits.push_back(i->getTournamentsSurvived());
//        std::cout << "pop best (" << vec_to_string(fits) << ")";
//        //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
//        std::cout << std::endl;

    Population::basicLogger(*population);
    //~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~

    //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
//    std::vector<Individual*> tops = population->nMostTournamentsSurvived(6);
    //~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
    std::string pathname = "";
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    int render_size = 99;
//    int render_size = 251;
    int render_size = 151;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Texture::window_x = 0;
    Texture::window_y = render_size * 2;
    Texture::displayAndFile3(*GP::textureFromIndividual(tops.at(3)),
                             *GP::textureFromIndividual(tops.at(4)),
                             *GP::textureFromIndividual(tops.at(5)),
                             pathname, render_size);
    Texture::window_x = 0;
    Texture::window_y = render_size;
    Texture::displayAndFile3(*GP::textureFromIndividual(tops.at(0)),
                             *GP::textureFromIndividual(tops.at(1)),
                             *GP::textureFromIndividual(tops.at(2)),
                             pathname, render_size);
    Texture::window_x = 0;
    Texture::window_y = 0;
    Texture::displayAndFile3(*GP::textureFromIndividual(best),
                             *GP::textureFromIndividual(best2),
                             *GP::textureFromIndividual(worst),
                             pathname, render_size);
    Texture::waitKey(1);
    Texture::closeAllWindows();
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void sampleColors(Individual* individual, std::vector<Color>& samples)
//{
//    int n = 10;
//    samples.clear();
//    std::vector<Vec2> positions;
//    jittered_grid_NxN_in_square(n, 1.4, LPRS(), positions);
//    Texture* texture = GP::textureFromIndividual(individual);
//    for (auto& p : positions)
//        samples.push_back(texture->getColor(p).clipToUnitRGB());
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    float measureScalarHistogram(Individual* individual,
//                             int bucket_count,
//                             float min_metric,
//                             float max_metric,
//                             std::function<float(Color)> metric)
//    {
//    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    // Generate a jittered grid of Color samples
//    //    std::vector<Color> samples;
//    //    sampleColors(individual, samples);
//
//    // Get random color samples from Texture, cached if previously needed.
//    Texture& texture = *GP::textureFromIndividual(individual);
//    const std::vector<Color>& samples = texture.cachedRandomColorSamples(LPRS());
//    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    // Set up histogram with "bucket_count" buckets
//    std::vector<int> buckets(bucket_count, 0);
//    // For each color sample, increment the corresponding histogram bucket.
//    for (auto& color : samples)
//    {
//        float value = remapInterval(metric(color), min_metric, max_metric, 0, 1);
//        int bucket_index = value * bucket_count;
//        if (bucket_index == bucket_count) bucket_index--;
//        buckets.at(bucket_index)++;
//    }
//    // Determine score (sum of abs error from target bucket size, neg for error)
//    float score = 0;
//    int target = int(samples.size()) / bucket_count;
//    for (int b : buckets) score -= sq(std::abs(b - target));
//    // TODO debug print of score and buckets.
//    std::cout << "score = " << score << " (";
//    for (int b : buckets) std::cout << b << " ";
//    std::cout << ")" << std::endl;
//
//    return score;
//    }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20201121 try converting Population over to std::shared_ptr
//float measureScalarHistogram(Individual* individual,
float measureScalarHistogram(std::shared_ptr<Individual> individual,
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                             int bucket_count,
                             int must_be_near_zero,
                             float min_metric,
                             float max_metric,
                             std::function<float(Color)> metric)
{
    assert(bucket_count > must_be_near_zero);
    // Get random color samples from Texture, cached if previously generated.
    Texture& texture = *GP::textureFromIndividual(individual);
    const std::vector<Color>& samples = texture.cachedRandomColorSamples(LPRS());
    // Set up histogram with "bucket_count" buckets
    std::vector<int> buckets(bucket_count, 0);
    // For each color sample, increment the corresponding histogram bucket.
    for (auto& color : samples)
    {
        float value = remapInterval(metric(color), min_metric, max_metric, 0, 1);
        int bucket_index = value * bucket_count;
        if (bucket_index == bucket_count) bucket_index--;
        buckets.at(bucket_index)++;
    }
    
    
//    // Determine score (sum of abs error from target bucket size, neg for error)
//    float score = 0;
//    int target = int(samples.size()) / bucket_count;
//    for (int b : buckets) score -= sq(std::abs(b - target));
    
    // Determine score (sum of abs error from target bucket size, neg for error)
    float score = 0;
//    int target = int(samples.size()) / bucket_count;
    int big_buckets = bucket_count - must_be_near_zero;
    int target = int(samples.size()) / big_buckets;
    if (must_be_near_zero == 0)
    {
        for (int b : buckets) score -= sq(std::abs(b - target));
    }
    else
    {
        auto biggest_first = [](int a, int b){ return a > b; };
        std::sort(buckets.begin(), buckets.end(), biggest_first);
        
        for (int i = 0; i < bucket_count; i++)
        {
            int ith_target = (i < big_buckets) ? target : 0;
            score -= sq(std::abs(buckets.at(i) - ith_target));
        }
    }
    // TODO debug print of score and buckets.
    std::cout << "score = " << score << " (";
    for (int b : buckets) std::cout << b << " ";
    std::cout << ")" << std::endl;
    
    return score;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    float measureExposure(Individual* individual)
//    {
//        std::cout << "worstExposure: ";
//        auto exposure = [](Color c){ return c.luminance(); };
//    //    return measureScalarHistogram(individual, 5, 0, 1, exposure);
//        return measureScalarHistogram(individual, 10, 0, 1, exposure);
//    }
//
//    float measureSaturation(Individual* individual)
//    {
//        std::cout << "worstSaturation: ";
//        auto saturation = [](Color c){ return c.getS(); };
//    //    return measureScalarHistogram(individual, 3, 0, 1, saturation);
//        return measureScalarHistogram(individual, 10, 0, 1, saturation);
//    }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20201121 try converting Population over to std::shared_ptr

//float measureExposure(Individual* individual)
float measureExposure(std::shared_ptr<Individual> individual)
{
    std::cout << "    brightness: ";
    auto exposure = [](Color c){ return c.luminance(); };
//    return measureScalarHistogram(individual, 10, 0, 1, exposure);
    return measureScalarHistogram(individual, 10, 0, 0, 1, exposure);
}

//float measureSaturation(Individual* individual)
float measureSaturation(std::shared_ptr<Individual> individual)
{
    std::cout << "    saturation: ";
    auto saturation = [](Color c){ return c.getS(); };
//    return measureScalarHistogram(individual, 10, 0, 1, saturation);
    return measureScalarHistogram(individual, 10, 0 , 0, 1, saturation);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20201121 try converting Population over to std::shared_ptr
//float measureHue(Individual* individual)
float measureHue(std::shared_ptr<Individual> individual)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{
    std::cout << "    hue: ";
    auto hue = [](Color c){ return c.getH(); };
//    return measureScalarHistogram(individual, 12, 4, 0, 1, hue);
    return measureScalarHistogram(individual, 12, 3, 0, 1, hue);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//float measureSize(Individual* individual)
//{
//    return -(individual->tree().size());
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Score TournamentGroup according to histogram flatness for given metric.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20201121 try converting Population over to std::shared_ptr
TournamentGroup worstMetric(TournamentGroup group,
//                            std::function<float(Individual*)> metric)
                            std::function<float(std::shared_ptr<Individual>)> metric)
{
//    group.setAllMetrics([&](Individual* i){ return metric(i); });
    group.setAllMetrics([&](std::shared_ptr<Individual> i){ return metric(i); });
    return group;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Score TournamentGroup according to worst "exposure" histogram.
TournamentGroup worstExposure(TournamentGroup group)
{
    return worstMetric(group, measureExposure);
}

// Score TournamentGroup according to worst "saturation" histogram.
TournamentGroup worstSaturation(TournamentGroup group)
{
    return worstMetric(group, measureSaturation);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Score TournamentGroup according to "hue" histogram.
TournamentGroup worstHue(TournamentGroup group)
{
    return worstMetric(group, measureHue);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//    // Score TournamentGroup according to worst size.
//    TournamentGroup worstSize(TournamentGroup group)
//    {
//        TournamentGroup ranked = worstMetric(group, measureSize);
//        std::cout << "worstSize: ";
//        for (auto& m : ranked.members()) std::cout << m.metric << " ";
//        std::cout << std::endl;
//        return ranked;
//    }

//    // TODO maybe this skipping for all-too-small should happen elsewhere?
//    // Score TournamentGroup according to worst size.
//    TournamentGroup worstSize(TournamentGroup group)
//    {
//        int biggest_size = std::numeric_limits<int>::min();
//        auto metric = [&](Individual* i)
//        {
//            int tree_size = i->tree().size();
//            if (biggest_size < tree_size) biggest_size = tree_size;
//            return -tree_size;
//        };
//        TournamentGroup ranked = worstMetric(group, metric);
//        if (biggest_size < 100) // TODO need to make this value smarter
//        {
//            ranked.setNevermind();
//        }
//        else
//        {
//            std::cout << "worstSize: ";
//            for (auto& m : ranked.members()) std::cout << m.metric << " ";
//            std::cout << std::endl;
//        }
//        return ranked;
//    }

//    // TODO maybe this skipping for all-too-small should happen elsewhere?
//    // Score TournamentGroup according to worst size.
//    TournamentGroup worstSize(TournamentGroup group)
//    {
//        int biggest_size = std::numeric_limits<int>::min();
//        auto metric = [&](Individual* i)
//        {
//            int tree_size = i->tree().size();
//            if (biggest_size < tree_size) biggest_size = tree_size;
//            return -tree_size;
//        };
//        TournamentGroup ranked = worstMetric(group, metric);
//        if (biggest_size < 100) // TODO need to make this value smarter
//        {
//            ranked.setNevermind();
//        }
//        else
//        {
//            std::cout << "worstSize: ";
//            for (auto& m : ranked.members()) std::cout << m.metric << " ";
//            std::cout << std::endl;
//        }
//        return ranked;
//    }

//    // TODO maybe this skipping for all-too-small should happen elsewhere?
//    // Score TournamentGroup according to worst size.
//    TournamentGroup worstSize(TournamentGroup group)
//    {
//    //    int biggest_size = std::numeric_limits<int>::min();
//    //    auto metric = [&](Individual* i)
//    //    {
//    //        int tree_size = i->tree().size();
//    //        if (biggest_size < tree_size) biggest_size = tree_size;
//    //        return -tree_size;
//    //    };
//        auto metric = [&](Individual* i) { return -(i->tree().size()); };
//        TournamentGroup ranked = worstMetric(group, metric);
//    //    if (biggest_size < 100) // TODO need to make this value smarter
//    //    {
//    //        ranked.setNevermind();
//    //    }
//    //    else
//    //    {
//            std::cout << "worstSize: ";
//            for (auto& m : ranked.members()) std::cout << m.metric << " ";
//            std::cout << std::endl;
//    //    }
//        return ranked;
//    }

// Score TournamentGroup according to worst size.
TournamentGroup worstSize(TournamentGroup group)
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201121 try converting Population over to std::shared_ptr
//    auto metric = [&](Individual* i) { return -(i->tree().size()); };
    auto metric = [&](std::shared_ptr<Individual> i) { return -(i->tree().size()); };
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    TournamentGroup ranked = worstMetric(group, metric);
//    std::cout << "worstSize: ";
    std::cout << "    size: ";
    for (auto& m : ranked.members()) std::cout << m.metric << " ";
    
    
//    std::cout << "  best tree size:" << ranked.bestIndividual()->tree().size();

    
    std::cout << std::endl;
    return ranked;
}



// Score TournamentGroup according to worst size.
TournamentGroup worstNoise(TournamentGroup group)
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201121 try converting Population over to std::shared_ptr
//    auto metric = [&](Individual* i)
    auto metric = [&](std::shared_ptr<Individual> i)
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    {
        return -(GP::textureFromIndividual(i)->highFrequencyScore());
    };
    TournamentGroup ranked = worstMetric(group, metric);
//    std::cout << "worstNoise: ";
    std::cout << "    noise: ";
    for (auto& m : ranked.members()) std::cout << m.metric << " ";
    std::cout << std::endl;
    return ranked;
}


//    // Hold tournament for 3 Individuals, scoring by various metrics.
//    TournamentGroup tournamentFunction(TournamentGroup group)
//    {
//        TournamentGroup ranked_group;
//        float select = LPRS().frandom01();
//        if (select < 0.6)                        // 60% exposure control
//    //    if (select < 0.55)                        // 55% exposure control
//    //    if (select < 0.58)                        // 58% exposure control
//        {
//            ranked_group = worstExposure(group);
//        }
//        else if (select < 0.9)                   // 30% saturation control
//    //    else if (select < 0.8)                   // 25% saturation control
//    //    else if (select < 0.86)                   // 28% saturation control
//        {
//            ranked_group = worstSaturation(group);
//        }
//        else                                      // 10% size control
//    //    else                                      // 20% size control
//    //    else                                      // 14% size control
//        {
//            ranked_group = worstSize(group);
//        }
//        logger(ranked_group);
//        return ranked_group;
//    }

//    // Hold tournament for 3 Individuals, scoring by various metrics.
//    TournamentGroup tournamentFunction(TournamentGroup group)
//    {
//        TournamentGroup ranked_group;
//        float select = LPRS().frandom01();
//    //    if (select < 0.5)                        // 50% exposure control
//        if (select < 0.4)                        // 40% exposure control
//        {
//            ranked_group = worstExposure(group);
//        }
//    //    else if (select < 0.8)                   // 30% saturation control
//        else if (select < 0.7)                   // 30% saturation control
//        {
//            ranked_group = worstSaturation(group);
//        }
//    //    else if (select < 0.9)                   // 10% noise reduction
//        else if (select < 0.9)                   // 20% noise reduction
//        {
//            ranked_group = worstNoise(group);
//        }
//        else                                      // 10% size control
//        {
//            ranked_group = worstSize(group);
//        }
//        logger(ranked_group);
//        return ranked_group;
//    }

//    // Hold tournament for 3 Individuals, scoring by various metrics.
//    TournamentGroup tournamentFunction(TournamentGroup group)
//    {
//        TournamentGroup ranked_group;
//        bool too_small = group.maxTreeSize() < 100; // TODO define threshold better
//        float select = LPRS().frandom01();
//    //    if (select < 0.5)                        // 50% exposure control
//        if (select < 0.4)                        // 40% exposure control
//        {
//            ranked_group = worstExposure(group);
//        }
//    //    else if (select < 0.8)                   // 30% saturation control
//        else if (select < 0.7)                   // 30% saturation control
//        {
//            ranked_group = worstSaturation(group);
//        }
//    //    else if (select < 0.9)                   // 10% noise reduction
//    //    else if (select < 0.9)                   // 20% noise reduction
//        else if (too_small || (select < 0.9))      // 20% noise reduction
//        {
//            ranked_group = worstNoise(group);
//        }
//        else                                      // 10% size control
//        {
//            ranked_group = worstSize(group);
//        }
//        logger(ranked_group);
//        return ranked_group;
//    }


//    // Hold tournament for 3 Individuals, scoring by various metrics.
//    TournamentGroup tournamentFunction(TournamentGroup group)
//    {
//        TournamentGroup ranked_group;
//    //    bool too_small = group.maxTreeSize() < 100; // TODO define threshold better
//        bool too_small = group.minTreeSize() < 100; // TODO define threshold better
//        float select = LPRS().frandom01();
//
//    //    if (select < 0.25)                          // 25% exposure control
//    //    {
//    //        ranked_group = worstExposure(group);
//    //    }
//    //    else if (select < 0.5)                      // 25% hue control
//    //    {
//    //        ranked_group = worstHue(group);
//    //    }
//    //    else if (select < 0.7)                      // 20% saturation control
//    //    {
//    //        ranked_group = worstSaturation(group);
//    //    }
//    //    else if (too_small || (select < 0.9))       // 20% noise reduction
//    //    {
//    //        ranked_group = worstNoise(group);
//    //    }
//    //    else                                        // 10% size control
//    //    {
//    //        ranked_group = worstSize(group);
//    //    }
//
//    //        if (select < 0.25)                          // 25% exposure control
//    //        {
//    //            ranked_group = worstExposure(group);
//    //        }
//    //        else if (select < 0.5)                      // 25% hue control
//    //        {
//    //            ranked_group = worstHue(group);
//    //        }
//    //    //    else if (select < 0.7)                      // 20% saturation control
//    //    //    {
//    //    //        ranked_group = worstSaturation(group);
//    //    //    }
//    //    //    else if (too_small || (select < 0.9))       // 20% noise reduction
//    //        else if (select < 0.7)                        // 20% noise reduction
//    //        {
//    //            ranked_group = worstNoise(group);
//    //        }
//    //    //    else if (select < 0.7)                      // 20% saturation control
//    //        else if (too_small || (select < 0.9))                      // 20% saturation control
//    //        {
//    //            ranked_group = worstSaturation(group);
//    //        }
//    //        else                                        // 10% size control
//    //        {
//    //            ranked_group = worstSize(group);
//    //        }
//
//        // TODO maybe cleaner to put worstSize() at the top, and the thing it defaults to just under it?
//
//
//    //    //    if (select < 0.25)                          // 25% exposure control
//    //        if (select < 0.3)                          // 30% exposure control
//    //        {
//    //            ranked_group = worstExposure(group);
//    //        }
//    //    //    else if (select < 0.5)                      // 25% hue control
//    //        else if (select < 0.5)                      // 20% hue control
//    //        {
//    //            ranked_group = worstHue(group);
//    //        }
//    //    //    else if (select < 0.7)                      // 20% noise reduction
//    //        else if (select < 0.6)                      // 10% noise reduction
//    //        {
//    //            ranked_group = worstNoise(group);
//    //        }
//    //    //    else if (too_small || (select < 0.9))       // 20% saturation control
//    //        else if (too_small || (select < 0.9))       // 30% saturation control
//    //        {
//    //            ranked_group = worstSaturation(group);
//    //        }
//    //        else                                        // 10% size control
//    //        {
//    //            ranked_group = worstSize(group);
//    //        }
//
//
//
//    //    //    if (!too_small && (select < 0.1))            // 10% size control
//    //        if (!too_small && (select < 0.15))            // 15% size control
//    //        {
//    //            ranked_group = worstSize(group);
//    //        }
//    //    //    if (select < 0.4)                           // 30% exposure control
//    //    //    if (select < 0.4)                           // 25% exposure control
//    //        else if (select < 0.4)                           // 25% exposure control
//    //        {
//    //            ranked_group = worstExposure(group);
//    //        }
//    //        else if (select < 0.6)                      // 20% hue control
//    //        {
//    //            ranked_group = worstHue(group);
//    //        }
//    //    //    else if (select < 0.9)                        // 30% saturation control
//    //        else if (select < 0.85)                        // 25% saturation control
//    //        {
//    //            ranked_group = worstSaturation(group);
//    //        }
//    //    //    else                                           // 10% noise reduction
//    //        else                                           // 15% noise reduction
//    //        {
//    //            ranked_group = worstNoise(group);
//    //        }
//
//
//    //    if (!too_small && (select < 0.15))            // 15% size control
//    //    {
//    //        ranked_group = worstSize(group);
//    //    }
//    //    else if (select < 0.4)                           // 25% exposure control
//    //    {
//    //        ranked_group = worstExposure(group);
//    //    }
//    //    else if (select < 0.6)                      // 20% hue control
//    //    {
//    //        ranked_group = worstHue(group);
//    //    }
//    //    else if (select < 0.85)                        // 25% saturation control
//    //    {
//    //        ranked_group = worstSaturation(group);
//    //    }
//    //    else                                           // 15% noise reduction
//    //    {
//    //        ranked_group = worstNoise(group);
//    //    }
//
//    //    //    if (!too_small && (select < 0.15))              // 15% size control
//    //        if (!too_small && (select < 0.2))                 // 20% size control
//    //        {
//    //            ranked_group = worstSize(group);
//    //        }
//    //    //    else if (select < 0.4)                          // 25% exposure control
//    //        else if (select < 0.4)                            // 20% exposure control
//    //        {
//    //            ranked_group = worstExposure(group);
//    //        }
//    //        else if (select < 0.6)                            // 20% hue control
//    //        {
//    //            ranked_group = worstHue(group);
//    //        }
//    //    //    else if (select < 0.85)                         // 25% saturation control
//    //        else if (select < 0.8)                            // 20% saturation control
//    //        {
//    //            ranked_group = worstSaturation(group);
//    //        }
//    //    //    else                                            // 15% noise reduction
//    //        else                                              // 20% noise reduction
//    //        {
//    //            ranked_group = worstNoise(group);
//    //        }
//
//        if (!too_small && (select < 0.15))                 // 15% size control
//        {
//            ranked_group = worstSize(group);
//        }
//        else if (select < 0.4)                             // 25% exposure control
//        {
//            ranked_group = worstExposure(group);
//        }
//        else if (select < 0.65)                            // 25% hue control
//        {
//            ranked_group = worstHue(group);
//        }
//        else if (select < 0.85)                            // 20% saturation control
//        {
//            ranked_group = worstSaturation(group);
//        }
//        else                                               // 15% noise reduction
//        {
//            ranked_group = worstNoise(group);
//        }
//
//
//        logger(ranked_group);
//        return ranked_group;
//    }

//    // Hold tournament for 3 Individuals, scoring by various metrics.
//    TournamentGroup tournamentFunction(TournamentGroup group)
//    {
//        TournamentGroup ranked_group;
//        bool too_small = group.minTreeSize() < 100; // TODO define threshold better
//        float select = LPRS().frandom01();
//        if (!too_small && (select < 0.15))                 // 15% size control
//        {
//            ranked_group = worstSize(group);
//        }
//        else if (select < 0.4)                             // 25% exposure control
//        {
//            ranked_group = worstExposure(group);
//        }
//        else if (select < 0.65)                            // 25% hue control
//        {
//            ranked_group = worstHue(group);
//        }
//        else if (select < 0.85)                            // 20% saturation control
//        {
//            ranked_group = worstSaturation(group);
//        }
//        else                                               // 15% noise reduction
//        {
//            ranked_group = worstNoise(group);
//        }
//        logger(ranked_group);
//        return ranked_group;
//    }


// Hold tournament for 3 Individuals, scoring by various metrics.
TournamentGroup tournamentFunction(TournamentGroup group)
{
    std::cout << std::endl;
    std::vector<TournamentGroup> ranked_groups =
    {
        worstExposure(group),
        worstSaturation(group),
        worstHue(group),
    };
    
    if (group.minTreeSize() > 100) ranked_groups.push_back(worstSize(group));
//    if (LPRS().frandom01() > 0.5) ranked_groups.push_back(worstNoise(group));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201121 try converting Population over to std::shared_ptr
//    auto combine = [&](Individual* individual)
    auto combine = [&](std::shared_ptr<Individual> individual)
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    {
        float score = 1;
        for (auto& group : ranked_groups)
        {
            int rank = group.rankOfIndividual(individual);
            assert((rank == 1) || (rank == 2) || (rank == 3));
            score *= 1.0 / rank;
        }
        return score;
    };
    
    TournamentGroup combined_result = group;
    combined_result.setAllMetrics(combine);
    
    std::cout << "    combined scores = ";
    for (auto& m : combined_result.members()) std::cout << m.metric << " ";
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201121 try converting Population over to std::shared_ptr
//    Individual* best = group.bestIndividual();
    std::shared_ptr<Individual> best = group.bestIndividual();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    std::cout << std::endl;
    std::cout << "    winner size=" << best->tree().size();
    std::cout << " won=" << best->getTournamentsSurvived();
    std::cout << std::endl << std::endl;

    logger(combined_result);
    return combined_result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void run()
{
    Timer t("CWE test");
    LPRS().setSeed(20201101);
    //LPRS().setSeed(20201109);
    const FunctionSet& function_set = GP::fs();
    int population_size = 100;
    int generation_equivalents = 50;
    int steps = population_size * generation_equivalents;
    int max_tree_size = 100;
    population = new Population(population_size, max_tree_size, function_set);
    population->setLoggerFunction(nullptr);
    population->run(steps, function_set, tournamentFunction);
    population = nullptr;
}

}

// "LimitHue" evolution test, uses traditional numeric (not relative) fitness.
//namespace LimitHue
namespace LimitHueOld
{

// Global Population instance. (TODO maybe should be shared_ptr?)
// TODO refactor to share via lexical capture?
static inline Population* population = nullptr;

//    float measureScalarHistogram(Individual* individual,
//                                 int bucket_count,
//                                 int must_be_near_zero,
//                                 float min_metric,
//                                 float max_metric,
//                                 std::function<float(Color)> metric)
//    {
//        assert(bucket_count > must_be_near_zero);
//        // Get random color samples from Texture, cached if previously generated.
//        Texture& texture = *GP::textureFromIndividual(individual);
//        const std::vector<Color>& samples = texture.cachedRandomColorSamples(LPRS());
//        // Set up histogram with "bucket_count" buckets
//        std::vector<int> buckets(bucket_count, 0);
//        // For each color sample, increment the corresponding histogram bucket.
//        for (auto& color : samples)
//        {
//            float value = remapInterval(metric(color), min_metric, max_metric, 0, 1);
//            int bucket_index = value * bucket_count;
//            if (bucket_index == bucket_count) bucket_index--;
//            buckets.at(bucket_index)++;
//        }
//
//        // Determine score (sum of abs error from target bucket size, neg for error)
//        float score = 0;
//        int big_buckets = bucket_count - must_be_near_zero;
//        int target = int(samples.size()) / big_buckets;
//    //    if (must_be_near_zero == 0)
//    //    {
//    //        for (int b : buckets) score -= sq(std::abs(b - target));
//    //    }
//    //    else
//    //    {
//            auto biggest_first = [](int a, int b){ return a > b; };
//            std::sort(buckets.begin(), buckets.end(), biggest_first);
//            for (int i = 0; i < bucket_count; i++)
//            {
//                int ith_target = (i < big_buckets) ? target : 0;
//                score -= sq(std::abs(buckets.at(i) - ith_target));
//            }
//    //    }
//    //    // TODO debug print of score and buckets.
//    //    std::cout << "score = " << score << " (";
//    //    for (int b : buckets) std::cout << b << " ";
//    //    std::cout << ")" << std::endl;
//
//        return score;
//    }

//    float measureScalarHistogram(Individual* individual,
//                                 int bucket_count,
//                                 int must_be_near_zero,
//                                 float min_metric,
//                                 float max_metric,
//                                 std::function<float(Color)> metric)
//    {
//        assert(bucket_count > must_be_near_zero);
//        // Get random color samples from Texture, cached if previously generated.
//        Texture& texture = *GP::textureFromIndividual(individual);
//        const std::vector<Color>& samples = texture.cachedRandomColorSamples(LPRS());
//        // Set up histogram with "bucket_count" buckets
//        std::vector<int> buckets(bucket_count, 0);
//        // For each color sample, increment the corresponding histogram bucket.
//        for (auto& color : samples)
//        {
//            float value = remapInterval(metric(color), min_metric, max_metric, 0, 1);
//            int bucket_index = value * bucket_count;
//            if (bucket_index == bucket_count) bucket_index--;
//            buckets.at(bucket_index)++;
//        }
//        // Determine score (sum of abs error from target bucket size, neg for error)
//        float score = 0;
//        int big_buckets = bucket_count - must_be_near_zero;
//        int target = int(samples.size()) / big_buckets;
//        auto biggest_first = [](int a, int b){ return a > b; };
//        std::sort(buckets.begin(), buckets.end(), biggest_first);
//        for (int i = 0; i < bucket_count; i++)
//        {
//            int ith_target = (i < big_buckets) ? target : 0;
//            score -= sq(std::abs(buckets.at(i) - ith_target));
//        }
//
//        // TODO debug print of score and buckets.
//    //    std::cout << "    score = " << score << " (";
//        std::cout << "    sorted hue buckets (";
//        for (int b : buckets) std::cout << b << " ";
//        std::cout << ")" << std::endl;
//
//        return score;
//    }

// TODO use multiplicative fitness (on [0,1]) for each bucket and final score.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20201121 try converting Population over to std::shared_ptr
float measureScalarHistogram(std::shared_ptr<Individual> individual,
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                             int bucket_count,
                             int must_be_near_zero,
                             float min_metric,
                             float max_metric,
                             std::function<float(Color)> metric)
{
    assert(bucket_count > must_be_near_zero);
    // Get random color samples from Texture, cached if previously generated.
    Texture& texture = *GP::textureFromIndividual(individual);
    const std::vector<Color>& samples = texture.cachedRandomColorSamples(LPRS());
    // Set up histogram with "bucket_count" buckets
    std::vector<int> buckets(bucket_count, 0);
    // For each color sample, increment the corresponding histogram bucket.
    for (auto& color : samples)
    {
        float value = remapInterval(metric(color), min_metric, max_metric, 0, 1);
        int bucket_index = value * bucket_count;
        if (bucket_index == bucket_count) bucket_index--;
        buckets.at(bucket_index)++;
    }
    // Determine score (sum of abs error from target bucket size, neg for error)
    // TODO after a lot of fiddling, back to previous version of score.
    float score = 0;
//    float score = 1;
    int big_buckets = bucket_count - must_be_near_zero;
    int target = int(samples.size()) / big_buckets;
    auto biggest_first = [](int a, int b){ return a > b; };
    std::sort(buckets.begin(), buckets.end(), biggest_first);
    for (int i = 0; i < bucket_count; i++)
    {
        int ith_target = (i < big_buckets) ? target : 0;
//        score -= sq(std::abs(buckets.at(i) - ith_target));
//        score *= 1 - (float(std::abs(buckets.at(i) - ith_target)) / samples.size());
//        float bucket_error = std::abs(buckets.at(i) - ith_target);
//        score *= 1 - (bucket_error / samples.size());
//        score *= 1 - sq(bucket_error / samples.size());
//        score *= 1 - (bucket_error / samples.size());
//        // TODO less severe
//        score *= remapInterval(bucket_error / samples.size(), 0, 1, 1, 0.5);
//        // map relative error on [0, 1] to score on [1, 0.9]
//        score *= remapInterval(bucket_error / samples.size(), 0, 1, 1, 0.9);
        
//        float bucket_error = std::abs(buckets.at(i) - ith_target);
//        // map relative error on [0, 1] to score on [1, 0.8]
//        score *= remapInterval(bucket_error / samples.size(), 0, 1, 1, 0.8);
        
        // TODO after a lot of fiddling, back to previous version of score.
        score -= sq(std::abs(buckets.at(i) - ith_target));
    }
    // TODO after a lot of fiddling, back to previous version of score.
    // TODO warning assumes these params (100 samples, 12 buckets, 8 near zeros)
    score = 1 + (score / 7500);

    
    
    // TODO debug print of score and buckets.
    std::cout << "    sorted hue buckets (";
    for (int b : buckets) std::cout << b << " ";
    std::cout << ") score=" << score << std::endl;

    return score;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    // TODO 20201121 try converting Population over to std::shared_ptr
//    //float fitness_function(Individual& individual)
//    float fitness_function(std::shared_ptr<Individual> individual)
//    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    {
//    //    Texture& texture = *GP::textureFromIndividual(&individual);
//        Texture& texture = *GP::textureFromIndividual(individual);
//        Texture::closeAllWindows();
//        int render_size = 151;
//        std::string pathname = "";
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        // TODO 20201121 try converting Population over to std::shared_ptr
//    //    std::vector<Individual*> tops = population->nTopFitness(9);
//        std::vector<std::shared_ptr<Individual>> tops = population->nTopFitness(9);
//
//        // TODO 20201122 Got EXC_BAD_ACCESS inside Texture::displayAndFile3
//        //               try checking all Textures for validity.
//
//        assert(texture.valid());
//        for (int i = 0; i < 9; i++)
//            assert(GP::textureFromIndividual(tops.at(i))->valid());
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        Texture::window_x = 0;
//        Texture::window_y = render_size * 3;
//        Texture::displayAndFile3(*GP::textureFromIndividual(tops.at(6)),
//                                 *GP::textureFromIndividual(tops.at(7)),
//                                 *GP::textureFromIndividual(tops.at(8)),
//                                 pathname, render_size);
//        Texture::window_x = 0;
//        Texture::window_y = render_size * 2;
//        Texture::displayAndFile3(*GP::textureFromIndividual(tops.at(3)),
//                                 *GP::textureFromIndividual(tops.at(4)),
//                                 *GP::textureFromIndividual(tops.at(5)),
//                                 pathname, render_size);
//        Texture::window_x = 0;
//        Texture::window_y = render_size;
//        Texture::displayAndFile3(*GP::textureFromIndividual(tops.at(0)),
//                                 *GP::textureFromIndividual(tops.at(1)),
//                                 *GP::textureFromIndividual(tops.at(2)),
//                                 pathname, render_size);
//        Texture::window_x = 0;
//        Texture::window_y = 0;
//        Texture::displayAndFile(texture, pathname, render_size);
//        Texture::waitKey(1);
//
//        Color average;
//        const std::vector<Color>& samples = texture.cachedRandomColorSamples(LPRS());
//        for (auto& color : samples) average += color;
//        average *= 1.0 / samples.size();
//        float brightness = average.luminance();
//        float relative_distance_from_midrange = std::abs((brightness - 0.5) * 2);
//        // TODO make flat near midrance, penalize only very bright or dark.
//    //    float closeness_to_midrange = 1 - relative_distance_from_midrange;
//        float closeness_to_midrange = remapIntervalClip(relative_distance_from_midrange,
//                                                        0.8, 1, 1, 0);
//
//
//        float average_saturation = average.getS();
//        float enough_saturation = remapIntervalClip(average_saturation,
//                                                    0, 0.5, 0.5, 1);
//        std::cout << std::endl;
//    //    float score = measureScalarHistogram(&individual, 12, 8, 0, 1,
//    //                                         [](Color c){ return c.getH(); });
//        float closeness_to_hue_constraint =
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        // TODO 20201121 try converting Population over to std::shared_ptr
//    //    measureScalarHistogram(&individual, 12, 8, 0, 1,
//    //                           [](Color c){ return c.getH(); });
//        measureScalarHistogram(individual, 12, 8, 0, 1,
//                               [](Color c){ return c.getH(); });
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        measureScalarHistogram(population->nTopFitness(1).at(0),
//                               12, 8, 0, 1,
//                               [](Color c){ return c.getH(); });
//    //    float closeness_to_hue_constraint = score;
//    //    float size_constraint = remapIntervalClip(individual.tree().size(),
//    //                                              100, 200, 1, 0.5);
//    //    float size_constraint = remapIntervalClip(individual.tree().size(),
//    //                                              150, 200, 1, 0.8);
//        float size_constraint = 1;
//        float fitness = (closeness_to_midrange *
//                         closeness_to_hue_constraint *
//                         enough_saturation *
//                         size_constraint);
//        std::cout << "    fit=" << fitness;
//        std::cout << " (hue=" << closeness_to_hue_constraint;
//        std::cout << ", gray=" << closeness_to_midrange;
//        std::cout << ", sat=" << enough_saturation;
//        std::cout << ", size=" << size_constraint << ")";
//        std::cout << std::endl << std::endl;
//        return fitness;
//    }

// TODO 20201201 prototyping new "one window" GUI
cv::Mat gui_image;
std::string window_name = "LimitHue run";

// TODO 20201202 very experimental putText()
cv::Ptr<cv::freetype::FreeType2> ft2;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int render_size = 151;
float text_height = 15;
float margin = 10;

// TODO 20201204 prototype GUI
GUI gui(Vec2((render_size * 5) + margin * 6,
             (render_size + text_height + margin) * 3 + 50),
        Vec2(100, 100));



//void drawIndividualsTextureWithFitness(std::shared_ptr<Individual> individual,
//                                       const Vec2& upper_left_position,
//                                       int size,
//                                       GUI& gui)
//{
//    Texture& texture = *GP::textureFromIndividual(individual);
//    texture.rasterizeToImageCache(size, true);
//    gui.drawTexture(texture, upper_left_position, size);
//    float text_height = 15;
//    Vec2 text_pos = upper_left_position + Vec2(0, size);
//    std::string text = std::to_string(individual->getFitness());
//    gui.eraseRectangle(Vec2(size, text_height), text_pos);
//    gui.drawText(text, text_height, text_pos, Color(1));
//}


void drawIndividualsTextureWithFitness(std::shared_ptr<Individual> individual,
                                       const Vec2& upper_left_position,
                                       GUI& gui)
{
    Texture& texture = *GP::textureFromIndividual(individual);
    texture.rasterizeToImageCache(render_size, true);
    gui.drawTexture(texture, upper_left_position, render_size);
    Vec2 text_pos = upper_left_position + Vec2(0, render_size);
    std::string text = std::to_string(individual->getFitness());
    gui.eraseRectangle(Vec2(render_size, text_height), text_pos);
    gui.drawText(text, text_height, text_pos, Color(1));
}

//    void updateGUI(std::shared_ptr<Individual> individual,
//                   const std::vector<std::shared_ptr<Individual>>& tops)
//    {
//        drawIndividualsTextureWithFitness(individual, Vec2(50, 50), gui);
//        gui.refresh();
//    }

void updateGUI(std::shared_ptr<Individual> individual)
{
    Vec2 position(0, 50);
    drawIndividualsTextureWithFitness(individual, position, gui);
    
//    position += Vec2(0, render_size + text_height + margin);
    float row_spacing = render_size + text_height + margin;
    position += Vec2(0, row_spacing);
    
    std::vector<std::shared_ptr<Individual>> tops = population->nTopFitness(10);

//    for (int i = 0; i < 5; i++)
//    {
//        drawIndividualsTextureWithFitness(tops.at(i), position, gui);
//        position += Vec2(margin + render_size, 0);
//    }
    
    for (int i = 0; i < 10; i++)
    {
        drawIndividualsTextureWithFitness(tops.at(i), position, gui);
        position += Vec2(margin + render_size, 0);
        if (i == 4) position = Vec2(0, position.y() + row_spacing);
    }
    gui.refresh();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20201121 try converting Population over to std::shared_ptr
//float fitness_function(Individual& individual)
float fitness_function(std::shared_ptr<Individual> individual)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{
//    Texture& texture = *GP::textureFromIndividual(&individual);
    Texture& texture = *GP::textureFromIndividual(individual);
//    Texture::closeAllWindows();
//    int render_size = 151;
    std::string pathname = "";
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201121 try converting Population over to std::shared_ptr
//    std::vector<Individual*> tops = population->nTopFitness(9);
    std::vector<std::shared_ptr<Individual>> tops = population->nTopFitness(9);
    
    // TODO 20201122 Got EXC_BAD_ACCESS inside Texture::displayAndFile3
    //               try checking all Textures for validity.
    
    assert(texture.valid());
    for (int i = 0; i < 9; i++)
        assert(GP::textureFromIndividual(tops.at(i))->valid());
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    Texture::window_x = 0;
//    Texture::window_y = render_size * 3;
//    Texture::displayAndFile3(*GP::textureFromIndividual(tops.at(6)),
//                             *GP::textureFromIndividual(tops.at(7)),
//                             *GP::textureFromIndividual(tops.at(8)),
//                             pathname, render_size);
//    Texture::window_x = 0;
//    Texture::window_y = render_size * 2;
//    Texture::displayAndFile3(*GP::textureFromIndividual(tops.at(3)),
//                             *GP::textureFromIndividual(tops.at(4)),
//                             *GP::textureFromIndividual(tops.at(5)),
//                             pathname, render_size);
//    Texture::window_x = 0;
//    Texture::window_y = render_size;
//    Texture::displayAndFile3(*GP::textureFromIndividual(tops.at(0)),
//                             *GP::textureFromIndividual(tops.at(1)),
//                             *GP::textureFromIndividual(tops.at(2)),
//                             pathname, render_size);
//    Texture::window_x = 0;
//    Texture::window_y = 0;
//    Texture::displayAndFile(texture, pathname, render_size);
    
    //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
//    // TODO 20201201 prototyping new "one window" GUI
//    int gui_w = (render_size + 10) * 5;
//    int gui_h = (render_size + 10) * 3;
////    gui_image = cv::Mat(gui_h, gui_w, CV_32FC3, cv::Scalar(1, 0, 0));
////    gui_image = cv::Mat(gui_h, gui_w, CV_32FC3, cv::Scalar(0.5, 0.5, 0.5));
////    gui_image = cv::Mat(gui_h, gui_w, CV_8UC3, cv::Scalar(127, 127, 127));
//    gui_image = cv::Mat(gui_h, gui_w, CV_8UC3, cv::Scalar::all(127));
//
//
//
//    int text_y = 0;
////    float text_scale = 0.8;
//    float text_scale = 1;
////    cv::putText(gui_image, "hello world", cv::Point(40, text_y += 40),
////                cv::FONT_HERSHEY_COMPLEX, text_scale, CV_RGB(1, 0.5, 0));
//
////    cv::putText(gui_image,
////                "hello world",
////                cv::Point(40, text_y += 40),
////                cv::FONT_HERSHEY_DUPLEX,
////                text_scale,
////                CV_RGB(1, 0.5, 0)
////                // , cv::FILLED
////                , 1
////                , cv::LINE_AA
////                );
//    cv::putText(gui_image,
//                "hello world",
//                cv::Point(40, text_y += 40),
//                cv::FONT_HERSHEY_SIMPLEX,
//                text_scale / 2,
//                CV_RGB(1, 0.5, 0),
//                1,
//                cv::LINE_AA);
//
//    cv::putText(gui_image,
//                "hello world",
//                cv::Point(40, text_y += 40),
//                cv::FONT_HERSHEY_SIMPLEX,
//                text_scale,
//                CV_RGB(1, 0.5, 0),
//                1,
//                cv::LINE_AA);
//
//    cv::putText(gui_image,
//                "hello world",
//                cv::Point(40, text_y += 80),
//                cv::FONT_HERSHEY_SIMPLEX,
//                text_scale * 2,
//                CV_RGB(1, 0.5, 0),
//                3,
//                cv::LINE_AA);
//
//
//    // TODO 20201202 very experimental putText()
//    ft2->putText(gui_image,
//                 "hello world",
//                 cv::Point(40, text_y += 20), // textOrg
//                 10, // fontHeight,
//                 cv::Scalar::all(255),
//                 cv::FILLED,  // thickness,
//                 cv::LINE_AA, // 8, // linestyle,
//                 true);
//
//    // TODO 20201202 very experimental putText()
//    ft2->putText(gui_image,
//                 "hello world",
//                 cv::Point(40, text_y += 40), // textOrg
//                 20, // fontHeight,
//                 cv::Scalar::all(255),
//                 cv::FILLED,  // thickness,
//                 cv::LINE_AA, // 8, // linestyle,
//                 true);
//
//    ft2->putText(gui_image,
//                 "hello world",
//                 cv::Point(40, text_y += 80), // textOrg
//                 60, // fontHeight,
//                 cv::Scalar::all(255),
//                 cv::FILLED,  // thickness,
//                 cv::LINE_AA, // 8, // linestyle,
//                 true);

//    cv::imshow(window_name, gui_image);
//    cv::moveWindow(window_name, 300, 300);
    //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~


    Texture::waitKey(1);

    Color average;
    const std::vector<Color>& samples = texture.cachedRandomColorSamples(LPRS());
    for (auto& color : samples) average += color;
    average *= 1.0 / samples.size();
    float brightness = average.luminance();
    float relative_distance_from_midrange = std::abs((brightness - 0.5) * 2);
    // TODO make flat near midrance, penalize only very bright or dark.
//    float closeness_to_midrange = 1 - relative_distance_from_midrange;
    float closeness_to_midrange = remapIntervalClip(relative_distance_from_midrange,
                                                    0.8, 1, 1, 0);

    
    float average_saturation = average.getS();
    float enough_saturation = remapIntervalClip(average_saturation,
                                                0, 0.5, 0.5, 1);
    std::cout << std::endl;
//    float score = measureScalarHistogram(&individual, 12, 8, 0, 1,
//                                         [](Color c){ return c.getH(); });
    float closeness_to_hue_constraint =
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201121 try converting Population over to std::shared_ptr
//    measureScalarHistogram(&individual, 12, 8, 0, 1,
//                           [](Color c){ return c.getH(); });
    measureScalarHistogram(individual, 12, 8, 0, 1,
                           [](Color c){ return c.getH(); });
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    measureScalarHistogram(population->nTopFitness(1).at(0),
                           12, 8, 0, 1,
                           [](Color c){ return c.getH(); });
//    float closeness_to_hue_constraint = score;
//    float size_constraint = remapIntervalClip(individual.tree().size(),
//                                              100, 200, 1, 0.5);
//    float size_constraint = remapIntervalClip(individual.tree().size(),
//                                              150, 200, 1, 0.8);
    float size_constraint = 1;
    float fitness = (closeness_to_midrange *
                     closeness_to_hue_constraint *
                     enough_saturation *
                     size_constraint);
    std::cout << "    fit=" << fitness;
    std::cout << " (hue=" << closeness_to_hue_constraint;
    std::cout << ", gray=" << closeness_to_midrange;
    std::cout << ", sat=" << enough_saturation;
    std::cout << ", size=" << size_constraint << ")";
    std::cout << std::endl << std::endl;
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201204 prototype GUI
    individual->setFitness(fitness);
//    drawIndividualsTextureWithFitness(individual, Vec2(50, 50), render_size,
//                                      gui);
    
//    updateGUI(individual, tops);
    updateGUI(individual);
    individual->setFitness(0);
//    gui.refresh();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    return fitness;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// TODO maybe instead: https://en.cppreference.com/w/cpp/io/manip/put_time
std::string hours_minutes()
{
    std::time_t t = std::time(nullptr);
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), "%Y%m%d_%I%M", std::localtime(&t));
    return std::string(mbstr);
}

void run(std::string path_for_saving)
{
    const FunctionSet& function_set = GP::fs();
//    int individuals = 100;
    int individuals = 10;
//    int generation_equivalents = 50;
//    int generation_equivalents = 20;
    int generation_equivalents = 10;
//    int generation_equivalents = 1;
    int steps = individuals * generation_equivalents;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201124
    // Wow! For 100 it stops on the 10 construction of an
    // Inidivdual. For 10, it completed a while run, with
    // 100 in the initial population and another 1000 offspring.
    //
    int max_tree_size = 100;
    // int max_tree_size = 10;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201204 prototype GUI
//    GUI gui(Vec2(500, 100), Vec2(100, 500));
    gui.drawText("TexSyn/LazyPredator GUI", 15, Vec2(20, 20), Color(0.5, 1, 0));
    gui.refresh();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    {
        Timer t("LimitHue test");
        population = new Population(individuals, max_tree_size, function_set);
//        population->run(steps, function_set, tournamentFunction);
        
        //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
        // TODO 20201201 prototyping new "one window" GUI
//        cv::namedWindow(window_name);
//        cv::moveWindow(window_name, 15, 15);
        
        
        // TODO 20201202 very experimental putText()
//        String text = "Funny text inside the box";
//        int fontHeight = 60;
//        int thickness = -1;
//        int linestyle = 8;
//        Mat img(600, 800, CV_8UC3, Scalar::all(0));
//        int baseline=0;
//        cv::Ptr<cv::freetype::FreeType2> ft2;
        ft2 = cv::freetype::createFreeType2();
        ft2->loadFontData("/opt/X11/share/fonts/TTF/Vera.ttf", 0);
//        ft2->loadFontData("/Users/cwr/Downloads/open-sans/OpenSans-Regular.ttf", 0);
//        Size textSize = ft2->getTextSize(text,
//                                         fontHeight,
//                                         thickness,
//                                         &baseline);
        
        //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~

        
        for (int i = 0; i < steps; i++)
        {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // TODO 20201122 -- before each step, lets validate the population:
            for (auto& individual : population->individuals())
                { assert(GP::textureFromIndividual(individual)->valid()); }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            
            // Run evolution step with given tournament and function set.
            population->evolutionStep(fitness_function, function_set);
            
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // TODO 20201122 -- after each step, lets validate the population:
            for (auto& individual : population->individuals())
                { assert(GP::textureFromIndividual(individual)->valid()); }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        }
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201121 try converting Population over to std::shared_ptr
//    Individual* final_best = population->nTopFitness(1).at(0);
    std::shared_ptr<Individual> final_best = population->nTopFitness(1).at(0);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    std::cout << "Final best in population:" << std::endl;
    std::cout << final_best->tree().to_string() << std::endl;
    
    Texture* texture = GP::textureFromIndividual(final_best);
    Texture::displayAndFile(*texture,
                            "",
                            // path_for_saving + hours_minutes() + "_abs_fit",
                            Texture::getDefaultRenderSize());
    final_best.reset();
    Texture::waitKey();
    Texture::leakCheck();
    Individual::leakCheck();

    //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
    // TODO 20201201 prototyping new "one window" GUI
//    cv::destroyWindow(window_name);
    //~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~

    delete population;
    population = nullptr;
    abnormal_value_report();
}

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
             int max_int_tree_size,
             int evolution_steps,
             std::string path_for_saving_images)
      : population(population_size, max_int_tree_size, function_set),
        gui(guiSize(), Vec2(15, 15)),
        evolution_steps_(evolution_steps),
        path_for_saving_images_(path_for_saving_images) {}
    // Run the evolutionary computation for given number of steps.
    // TODO very generic, could be in base "run" class.
    void run()
    {
        Timer t(name() + " run");
        gui.drawText(gui_title_, 15, Vec2(20, 20), Color(0.5, 1, 0));
        gui.refresh();
        Population::FitnessFunction fitness_function_wrapper =
            [&](std::shared_ptr<Individual> individual)
            { return fitness_function(individual); };
        for (step = 0; step < evolution_steps_; step++)
        {
            population.evolutionStep(fitness_function_wrapper, function_set);
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201208 penalize_adjacency

//        Texture::leakCheck();
//        Individual::leakCheck();
//        abnormal_value_report();
        
        std::shared_ptr<Individual> final_best = population.nTopFitness(1).at(0);
        std::cout << "Final best in population:" << std::endl;
        std::cout << final_best->tree().to_string() << std::endl;
        
        Texture* texture = GP::textureFromIndividual(final_best);
        Texture::displayAndFile(*texture,
                                (path_for_saving_images_ +
                                 LimitHueOld::hours_minutes() +
                                 "_" + name()),
                                Texture::getDefaultRenderSize());
        Texture::waitKey(1000);
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    }
    // Determines fitness (on [0, 1]) for given Individual
    float fitness_function(std::shared_ptr<Individual> individual)
    {
        Texture& texture = *GP::textureFromIndividual(individual);
        std::string pathname = "";
        Color average;
        const std::vector<Color>& samples = texture.cachedRandomColorSamples(LPRS());
        for (auto& color : samples) average += color;
        average *= 1.0 / samples.size();
        float brightness = average.luminance();
        float relative_distance_from_midrange = std::abs((brightness - 0.5) * 2);
        // TODO make flat near midrance, penalize only very bright or dark.
        //    float closeness_to_midrange = 1 - relative_distance_from_midrange;
        float closeness_to_midrange = remapIntervalClip(relative_distance_from_midrange,
                                                        0.8, 1, 1, 0);
        
        float average_saturation = average.getS();
        float enough_saturation = remapIntervalClip(average_saturation,
                                                    0, 0.5, 0.5, 1);
        std::cout << std::endl;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201208 penalize_adjacency
//        float closeness_to_hue_constraint =
//        measureScalarHistogram(individual, 12, 8, 0, 1,
//                               [](Color c){ return c.getH(); });
//
//        measureScalarHistogram(population.nTopFitness(1).at(0),
//                               12, 8, 0, 1,
//                               [](Color c){ return c.getH(); });
        float closeness_to_hue_constraint =
        measureScalarHistogram(individual, 12, 8, true, 0, 1,
                               [](Color c){ return c.getH(); });
        
        measureScalarHistogram(population.nTopFitness(1).at(0),
                               12, 8, true, 0, 1,
                               [](Color c){ return c.getH(); });
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        float size_constraint = 1;
        float fitness = (closeness_to_midrange *
                         closeness_to_hue_constraint *
                         enough_saturation *
                         size_constraint);
        std::cout << "    fit=" << fitness;
        std::cout << " (hue=" << closeness_to_hue_constraint;
        std::cout << ", gray=" << closeness_to_midrange;
        std::cout << ", sat=" << enough_saturation;
        std::cout << ", size=" << size_constraint << ")";
        std::cout << std::endl << std::endl;
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201204 for the sake of the GUI we want this individual to
        // already have its fitness set. Otherwise that is suppost to happen
        // in the caller of this fitnress function.
        individual->setFitness(fitness);
        updateGUI(individual);
        individual->setFitness(0);
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        return fitness;
    }
    
    // Assign a score on [0, 1] based on a histogram for a scalar (as defined
    // by "metric") property (of randomly sampled colors from the Texture).
    // TODO 20201208 maybe pass in Texture rather than Individual?
    float measureScalarHistogram(std::shared_ptr<Individual> individual,
                                 int bucket_count,
                                 int must_be_near_zero,
                                 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                 // TODO 20201208 penalize_adjacency
                                 bool penalize_adjacency,
                                 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                 float min_metric,
                                 float max_metric,
                                 std::function<float(Color)> metric)
    {
        assert(bucket_count > must_be_near_zero);
        // Get random color samples from Texture, cached if previously generated.
        Texture& texture = *GP::textureFromIndividual(individual);
        const std::vector<Color>& samples = texture.cachedRandomColorSamples(LPRS());
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201208 penalize_adjacency
//        // Set up histogram with "bucket_count" buckets
//        std::vector<int> buckets(bucket_count, 0);
        // Set up histogram with "bucket_count" buckets
        class Bucket {public: int count; int index; };
        std::vector<Bucket> buckets(bucket_count);
        int k = 0;
        for (auto& b : buckets) { b.count = 0; b.index = k++;}
        
//        for (int i = 0; i < bucket_count; i ++)
//        {
//            buckets.at(i).count = 0;
//            buckets.at(i).index = i;
//        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // For each color sample, increment the corresponding histogram bucket.
        for (auto& color : samples)
        {
            float value = remapInterval(metric(color), min_metric, max_metric, 0, 1);
            int bucket_index = value * bucket_count;
            if (bucket_index == bucket_count) bucket_index--;
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // TODO 20201208 penalize_adjacency
//            buckets.at(bucket_index)++;
            buckets.at(bucket_index).count++;
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        }
        // Determine score (sum of abs error from target bucket size, neg for error)
        float score = 0;
        int big_buckets = bucket_count - must_be_near_zero;
        int target = int(samples.size()) / big_buckets;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201208 penalize_adjacency
//        auto biggest_first = [](int a, int b){ return a > b; };
        auto biggest_first =
            [](Bucket& a, Bucket& b){ return a.count > b.count; };
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        std::sort(buckets.begin(), buckets.end(), biggest_first);
        for (int i = 0; i < bucket_count; i++)
        {
            int ith_target = (i < big_buckets) ? target : 0;
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // TODO 20201208 penalize_adjacency
//            score -= sq(std::abs(buckets.at(i) - ith_target));
            score -= sq(std::abs(buckets.at(i).count - ith_target));
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        }
        // TODO warning assumes these params (100 samples, 12 buckets, 8 near zeros)
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201208 penalize_adjacency
        
        float adjacency_penalty = 1;
        
        for (int i = 0; i < big_buckets; i++)
        {
            int i_index = buckets.at(i).index;
            for (int j = 0; j < big_buckets; j++)
            {
                int j_index = buckets.at(j).index;
//                if (1 == ((i_index - j_index) % bucket_count))
                if (1 == (std::abs(i_index - j_index) % bucket_count))
                {
//                    std::cout << "found one!" << std::endl;
                    adjacency_penalty *= 0.95;
                }
            }
        }
        
//        std::cout << "raw_score=" << score;
//        std::cout << ", normalized_score=" << 1 + (score / 7500);
//        std::cout << ", adjacency_penalty=" << adjacency_penalty;
//        std::cout << ", adjusted_score="<<adjacency_penalty*(1 + (score / 7500));
//        std::cout << std::endl;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        score = 1 + (score / 7500);
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201208 penalize_adjacency
        score *= adjacency_penalty;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // TODO debug print of score and buckets.
        std::cout << "    sorted hue buckets (";
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201208 penalize_adjacency
//        for (int b : buckets) std::cout << b << " ";
        for (auto& b : buckets) std::cout << b.count << " ";
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        std::cout << ") score=" << score << std::endl;
        
        return score;
    }
    // Display textures and text on GUI canvas.
    void updateGUI(std::shared_ptr<Individual> individual)
    {
        Vec2 step_position(300, 20);
        gui.eraseRectangle(Vec2(200, gui_text_height_), step_position);
        std::string text = ("step " + std::to_string(step) +
                            " of " + std::to_string(evolution_steps_));
        gui.drawText(text, gui_text_height_, step_position, Color(1, 0, 0));

        
        
        Vec2 position(0, 50);
        drawIndividualsTextureWithFitness(individual, position, gui);
        float row_spacing = gui_render_size_ + gui_text_height_ + gui_margin_;
        position += Vec2(0, row_spacing);
        
        std::vector<std::shared_ptr<Individual>> tops =
                                                     population.nTopFitness(10);
                
        for (int i = 0; i < 10; i++)
        {
            drawIndividualsTextureWithFitness(tops.at(i), position, gui);
            position += Vec2(gui_margin_ + gui_render_size_, 0);
            if (i == 4) position = Vec2(0, position.y() + row_spacing);
        }
        gui.refresh();
    }
    // Render an Individual's Texture, above its numerical fitness (as percent).
    void drawIndividualsTextureWithFitness(std::shared_ptr<Individual> individual,
                                           const Vec2& upper_left_position,
                                           GUI& gui)
    {
        Texture& texture = *GP::textureFromIndividual(individual);
        texture.rasterizeToImageCache(gui_render_size_, true);
        gui.drawTexture(texture, upper_left_position, gui_render_size_);
        Vec2 text_pos = upper_left_position + Vec2(0, gui_render_size_);
        float fitness = individual->getFitness();
        std::string text = float_to_percent_fractional_digits(fitness, 1);
        gui.eraseRectangle(Vec2(gui_render_size_, gui_text_height_), text_pos);
        gui.drawText(text, gui_text_height_, text_pos, Color(1));
    }
    // Compute dimensions of the GUI window.
    Vec2 guiSize() const
    {
        int row_y = gui_render_size_ + gui_text_height_ + gui_margin_;
        return Vec2((gui_render_size_ * 5) + gui_margin_ * 6, row_y * 3 + 50);
    }
    // The generic name for this run.
    const std::string& name() const { return name_; }
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
    std::string gui_title_ = "TexSyn/LazyPredator GUI"; // TODO keep?
    GUI gui;
    int step;
    
    std::string path_for_saving_images_;
};
