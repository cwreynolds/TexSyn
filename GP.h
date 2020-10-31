//
//  GP.h
//  TexSyn
//
//  Created by Craig Reynolds on 9/20/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

#pragma once
#include "../LazyPredator/LazyPredator.h"  // TODO use something more portable.

// Abbreviations for oft-repeated expressions below (used only in this file):
#define argFloat() tree.evalSubtree<float>(inc_tex_arg())
#define argVec2() tree.evalSubtree<Vec2>(inc_tex_arg())
#define argTexture() *tree.evalSubtree<Texture*>(inc_tex_arg())
#define evalTexture(body)                                \
[](const GpTree& tree)                                   \
{                                                        \
    int i = 0;                                           \
    auto inc_tex_arg = [&](){ int j = i++; return j; };  \
    Texture* t = new body;                               \
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
            {"Texture"},
            {"Vec2"},
            { "Float_01", 0.0f, 1.0f },
            { "Float_02", 0.0f, 2.0f },
            { "Float_0_10", 0.0f, 10.0f },
            { "Float_m5p5", -5.0f, 5.0f }
        },
        // GpFunctions
        {
            {
                "Vec2", "Vec2", {"Float_m5p5", "Float_m5p5"},
                [](const GpTree& tree)
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
                {"Vec2", "Vec2", "Vec2", "Vec2", "Texture", "Texture"},
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
        //~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
#ifndef USE_TOURNAMENT_GROUP
        // Print log and render Texture.
        auto logger = [&](Individual* a, Individual* b, Individual* c,
                          float am, float bm, float cm)
        {
            Individual* tournament_best = c;
            if ((am > bm) && (am > cm)) tournament_best = a;
            if ((bm > am) && (bm > cm)) tournament_best = b;
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
        };
#else // USE_TOURNAMENT_GROUP
//        // Print log and render Texture.
//        auto logger = [&](Individual* a, Individual* b, Individual* c,
//                          float am, float bm, float cm)
//        {
//            Individual* tournament_best = c;
//            if ((am > bm) && (am > cm)) tournament_best = a;
//            if ((bm > am) && (bm > cm)) tournament_best = b;
//            if (true)
//            {
//                std::cout << std::endl << "step " << step_count++ << std::endl;
//                std::cout << "winner size ";
//                std::cout << tournament_best->tree().size();
//                std::cout << ", winner tournaments survived ";
//                std::cout << tournament_best->getTournamentsSurvived();
//                std::cout << std::endl;
//                Texture* t = GP::textureFromIndividual(tournament_best);
//                Texture::displayAndFile(*t, "", 99);
//                Texture::waitKey(1);
//                Texture::closeAllWindows();
//            }
//            else
//            {
//                // for plotting:
//                step_count++;
//                Color ac = ygAverageColorOfPopulation(population);
//                std::cout << ac.r() << "," << ac.g() << "," << ac.b() << ",";
//                Individual* best_individual = population.findBestIndividual();
//                Texture* best_texture = GP::textureFromIndividual(best_individual);
//                Color bc = ygAverageColorOfTexture(best_texture);
//                std::cout << bc.r() << "," << bc.g() << "," << bc.b() << std::endl;
//            }
//        };
        
        // Print log and render Texture.
        auto logger = [&](TournamentGroup group)
        {
            Individual* tournament_best = group.bestIndividual();
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
                Individual* best_individual = population.findBestIndividual();
                Texture* bt = GP::textureFromIndividual(best_individual);
                Color bc = ygAverageColorOfTexture(bt);
                std::cout << bc.r()<<"," << bc.g()<<"," << bc.b() << std::endl;
            }
        };

#endif // USE_TOURNAMENT_GROUP
        //~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
        //~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
#ifndef USE_TOURNAMENT_GROUP
        // Given 3 Individuals and metric, find the one with lowest metric.
        auto lowest_average_metric = [&]
        (Individual* a, Individual* b, Individual* c,
         std::function<float(const Color&)> color_metric)
        {
            Texture* at = GP::textureFromIndividual(a);
            Texture* bt = GP::textureFromIndividual(b);
            Texture* ct = GP::textureFromIndividual(c);
            float am = color_metric(GP::ygAverageColorOfTexture(at));
            float bm = color_metric(GP::ygAverageColorOfTexture(bt));
            float cm = color_metric(GP::ygAverageColorOfTexture(ct));
            logger(a, b, c, am, bm, cm);
            if ((am < bm) && (am < cm)) return a;
            if ((bm < am) && (bm < cm)) return b;
            return c;
        };
#else // USE_TOURNAMENT_GROUP
        // Given 3 Individuals and metric, find the one with lowest metric.
        auto lowest_average_metric = [&]
        (TournamentGroup group, std::function<float(const Color&)> color_metric)
        {
//            Texture* at = GP::textureFromIndividual(a);
//            Texture* bt = GP::textureFromIndividual(b);
//            Texture* ct = GP::textureFromIndividual(c);
//            float am = color_metric(GP::ygAverageColorOfTexture(at));
//            float bm = color_metric(GP::ygAverageColorOfTexture(bt));
//            float cm = color_metric(GP::ygAverageColorOfTexture(ct));
            
            for (int i = 0; i < group.size(); i++)
            {
                Individual* individual = group.at(i).individual;
                Texture* texture = GP::textureFromIndividual(individual);
                float metric = color_metric(GP::ygAverageColorOfTexture(texture));
                group.setMetric(i, metric);
            }

            
            
//            logger(a, b, c, am, bm, cm);

//            logger(group.at(0).individual,
//                   group.at(1).individual,
//                   group.at(2).individual,
//                   group.at(0).metric,
//                   group.at(1).metric,
//                   group.at(2).metric);
            
            logger(group);

            
//            if ((am < bm) && (am < cm)) return a;
//            if ((bm < am) && (bm < cm)) return b;
//            return c;
            
            group.sort();

            return group;

        };
#endif // USE_TOURNAMENT_GROUP
        //~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
        //~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
#ifndef USE_TOURNAMENT_GROUP
        // Run tournament for 3 Individuals, random choice between 2 cases.
        auto tournament_function = [&]
        (Individual* a, Individual* b, Individual* c)
        {
            if (LPRS().frandom01() < 0.5)
            {
                auto high_green = [](const Color& c){ return c.green(); };
                return lowest_average_metric(a, b, c, high_green);
            }
            else
            {
                auto low_blue = [](const Color& c)
                { return remapIntervalClip(c.blue(), 0, 1, 1, 0); };
                return lowest_average_metric(a, b, c, low_blue);
            }
        };
        // Perform evolution run.
        population.run(steps, function_set, tournament_function);
#else // USE_TOURNAMENT_GROUP
        // Run tournament for 3 Individuals, random choice between 2 cases.
//        auto tournament_function = [&]
//        (Individual* a, Individual* b, Individual* c)
        auto tournament_function = [&](TournamentGroup group)
        {
            if (LPRS().frandom01() < 0.5)
            {
                auto high_green = [](const Color& c){ return c.green(); };
//                return lowest_average_metric(a, b, c, high_green);
                return lowest_average_metric(group, high_green);
            }
            else
            {
                auto low_blue = [](const Color& c)
                    { return remapIntervalClip(c.blue(), 0, 1, 1, 0); };
//                return lowest_average_metric(a, b, c, low_blue);
                return lowest_average_metric(group, low_blue);
            }
        };
    // Perform evolution run.
    population.run(steps, function_set, tournament_function);
#endif // USE_TOURNAMENT_GROUP
        //~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
    }
    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// “colorful, well exposed” evolution test

namespace CWE
{

static inline Population* POP;

// TODO is this computed corectly now that there are multiple fitness cases?
static inline Individual* tournament_best;

// Print log and render Texture.
void logger(const Population& population)
{
    static int step_count = 0;
    //Individual* best = population.findBestIndividual();
    Individual* best = tournament_best;
    std::cout << std::endl << "step " << step_count++ << std::endl;
    std::cout << "winner size ";
    std::cout << best->tree().size();
    std::cout << ", winner tournaments survived ";
    std::cout << best->getTournamentsSurvived();
    std::cout << std::endl;
    Texture* t = GP::textureFromIndividual(best);
    Texture::displayAndFile(*t, "", 99);
    Texture::window_x = 0;
    Texture::window_y = 150;

    Texture* b = GP::textureFromIndividual(POP->findBestIndividual());
    Texture::displayAndFile(*b, "", 99);
    
    Texture::waitKey(1);
    Texture::closeAllWindows();
};

void sampleColors(Individual* individual, std::vector<Color>& samples)
{
    int n = 10;
    samples.clear();
    std::vector<Vec2> positions;
    jittered_grid_NxN_in_square(n, 1.4, LPRS(), positions);
    Texture* texture = GP::textureFromIndividual(individual);
    for (auto& p : positions)
        samples.push_back(texture->getColor(p).clipToUnitRGB());
}

float measureScalarHistogram(Individual* individual,
                             int bucket_count,
                             float min_metric,
                             float max_metric,
                             std::function<float(Color)> metric)
{
    // Generate a jittered grid of Color samples
    std::vector<Color> samples;
    sampleColors(individual, samples);
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
    float score = 0;
    int target = int(samples.size()) / bucket_count;
    for (int b : buckets) score -= sq(std::abs(b - target));
    // TODO debug print of score and buckets.
    std::cout << "score = " << score << " (";
    for (int b : buckets) std::cout << b << " ";
    std::cout << ")" << std::endl;

    return score;
}

float measureExposure(Individual* individual)
{
    auto exposure = [](Color c){ return c.luminance(); };
    return measureScalarHistogram(individual, 5, 0, 1, exposure);
}

float measureSaturation(Individual* individual)
{
    auto saturation = [](Color c){ return c.getS(); };
    return measureScalarHistogram(individual, 3, 0, 1, saturation);
}

float measureSize(Individual* individual)
{
    return -(individual->tree().size());
}

//~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
#ifndef USE_TOURNAMENT_GROUP
typedef std::pair<float, Individual*> ScoredIndividual;

// Given 3 Individuals and scalar metric, return Individual with lowest metric.
Individual* worstMetric(Individual* a, Individual* b, Individual* c,
                        std::function<float(Individual*)> metric)
{
    // Create a vector of {metric, Individual*} pairs
    std::vector<ScoredIndividual> rankings = { {metric(a), a},
                                               {metric(b), b},
                                               {metric(c), c} };
    // Sort rankings by ascending score.
    std::sort(rankings.begin(), rankings.end(),
              [](const ScoredIndividual &a, const ScoredIndividual &b)
                  { return a.first < b.first; });
    // Primarily for the sake of debugging, remember the best:
    tournament_best = rankings.at(2).second;
    return rankings.at(0).second;
}
#else // USE_TOURNAMENT_GROUP
//typedef std::pair<float, Individual*> ScoredIndividual;

// Given 3 Individuals and scalar metric, return Individual with lowest metric.
//Individual* worstMetric(Individual* a, Individual* b, Individual* c,
//                        std::function<float(Individual*)> metric)
TournamentGroup worstMetric(TournamentGroup group,
                            std::function<float(Individual*)> metric)
{
//    // Create a vector of {metric, Individual*} pairs
//    std::vector<ScoredIndividual> rankings = { {metric(a), a},
//                                               {metric(b), b},
//                                               {metric(c), c} };
    
    for (int i = 0; i < group.size(); i++)
    {
        Individual* individual = group.at(i).individual;
        group.setMetric(i, metric(individual));
    }
    
    // Sort rankings by ascending score.
//    std::sort(rankings.begin(), rankings.end(),
//              [](const ScoredIndividual &a, const ScoredIndividual &b)
//                  { return a.first < b.first; });
    
    group.sort();
    
    // Primarily for the sake of debugging, remember the best:
//    tournament_best = rankings.at(2).second;
//    return rankings.at(0).second;
    tournament_best = group.bestIndividual();
    return group;
}
#endif // USE_TOURNAMENT_GROUP
//~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~

//~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
#ifndef USE_TOURNAMENT_GROUP
// Given 3 Individuals, return the one with the worse exposure histogram.
Individual* worstExposure(Individual* a, Individual* b, Individual* c)
{
    return worstMetric(a, b, c, measureExposure);
}

// Given 3 Individuals, return the one with the worse saturation histogram.
Individual* worstSaturation(Individual* a, Individual* b, Individual* c)
{
    return worstMetric(a, b, c, measureSaturation);
}
#else // USE_TOURNAMENT_GROUP
// TODO update doc
// Given 3 Individuals, return the one with the worse exposure histogram.
TournamentGroup worstExposure(TournamentGroup group)
{
    return worstMetric(group, measureExposure);
}

// TODO update doc
// Given 3 Individuals, return the one with the worse saturation histogram.
TournamentGroup worstSaturation(TournamentGroup group)
{
    return worstMetric(group, measureSaturation);
}
#endif // USE_TOURNAMENT_GROUP
//~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~

// TODO update doc
// Given 3 Individuals, return the one with the worse saturation histogram.
TournamentGroup worstSize(TournamentGroup group)
{
    TournamentGroup ranked = worstMetric(group, measureSize);
    std::cout << "worstSize: ";
    for (auto& m : ranked.members()) std::cout << m.metric << " ";
    std::cout << std::endl;
    return ranked;
}

//~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
#ifndef USE_TOURNAMENT_GROUP
// Hold tournament for 3 Individuals, return the worst performing one ("loser").
Individual* tournamentFunction(Individual* a, Individual* b, Individual* c)
{
    Individual* worst = nullptr;
    float select = LPRS().frandom01();
    if (select < 0.66)
    {
        worst = worstExposure(a, b, c);
        assert(worst);
    }
    else
    {
        worst = worstSaturation(a, b, c);
        assert(worst);
    }
    logger(*POP);
    return worst;
}
#else // USE_TOURNAMENT_GROUP

//    // Hold tournament for 3 Individuals, return the worst performing one ("loser").
//    //Individual* tournamentFunction(Individual* a, Individual* b, Individual* c)
//    TournamentGroup tournamentFunction(TournamentGroup group)
//    {
//    //    Individual* worst = nullptr;
//        TournamentGroup ranked_group;
//        float select = LPRS().frandom01();
//        if (select < 0.66)
//        {
//    //        worst = worstExposure(a, b, c);
//    //        assert(worst);
//            ranked_group = worstExposure(group);
//        }
//        else
//        {
//    //        worst = worstSaturation(a, b, c);
//    //        assert(worst);
//            ranked_group = worstSaturation(group);
//        }
//        logger(*POP);
//
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    //    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
//    //    ranked_group.print();
//    //    debugPrint(ranked_group.bestIndividual());
//    //    debugPrint(ranked_group.worstIndividual());
//    //    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//    //    return worst;
//        return ranked_group;
//    }

//    // Hold tournament for 3 Individuals, return the worst performing one ("loser").
//    //Individual* tournamentFunction(Individual* a, Individual* b, Individual* c)
//    TournamentGroup tournamentFunction(TournamentGroup group)
//    {
//        TournamentGroup ranked_group;
//        float select = LPRS().frandom01();
//        if (select < 0.65)                        // 65% exposure control
//        {
//            ranked_group = worstExposure(group);
//        }
//        else if (select < 0.95)                   // 30% saturation control
//        {
//            ranked_group = worstSaturation(group);
//        }
//        else                                      // 5% size control
//        {
//            ranked_group = worstSize(group);
//        }
//        logger(*POP);
//        return ranked_group;
//    }

//    // Hold tournament for 3 Individuals, return the worst performing one ("loser").
//    TournamentGroup tournamentFunction(TournamentGroup group)
//    {
//        TournamentGroup ranked_group;
//        float select = LPRS().frandom01();
//        if (select < 0.5)                        // 50% exposure control
//        {
//            ranked_group = worstExposure(group);
//        }
//        else if (select < 0.8)                   // 30% saturation control
//        {
//            ranked_group = worstSaturation(group);
//        }
//        else                                      // 20% size control
//        {
//            ranked_group = worstSize(group);
//        }
//        logger(*POP);
//        return ranked_group;
//    }

//// Hold tournament for 3 Individuals, return the worst performing one ("loser").
//TournamentGroup tournamentFunction(TournamentGroup group)
//{
//    TournamentGroup ranked_group;
//    float select = LPRS().frandom01();
//    if (select < 0.5)                        // 50% exposure control
//    {
//        ranked_group = worstExposure(group);
//    }
//    else if (select < 0.9)                   // 40% saturation control
//    {
//        ranked_group = worstSaturation(group);
//    }
//    else                                      // 10% size control
//    {
//        ranked_group = worstSize(group);
//    }
//    logger(*POP);
//    return ranked_group;
//}

// Hold tournament for 3 Individuals, return the worst performing one ("loser").
TournamentGroup tournamentFunction(TournamentGroup group)
{
    TournamentGroup ranked_group;
    float select = LPRS().frandom01();
    if (select < 0.6)                        // 60% exposure control
    {
        ranked_group = worstExposure(group);
    }
    else if (select < 0.9)                   // 30% saturation control
    {
        ranked_group = worstSaturation(group);
    }
    else                                      // 10% size control
    {
        ranked_group = worstSize(group);
    }
    logger(*POP);
    return ranked_group;
}

#endif // USE_TOURNAMENT_GROUP
//~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~


//void run()
//{
//    Timer t("CWE test");
//    const FunctionSet& function_set = GP::fs();
//    int population_size = 50;
//    // int population_size = 100;
//    int generation_equivalents = 100;
//    int steps = population_size * generation_equivalents;
////    int steps = 100;
//    int max_tree_size = 100;
//    Population population(population_size, max_tree_size, function_set);
//
//    POP = &population; // TODO for debugging
//
//    population.run(steps, function_set, tournamentFunction);
//}

void run()
{
    Timer t("CWE test");
    const FunctionSet& function_set = GP::fs();
//    int population_size = 50;
//    int generation_equivalents = 100;
    int population_size = 100;
    int generation_equivalents = 50;
    int steps = population_size * generation_equivalents;
    int max_tree_size = 100;
    Population population(population_size, max_tree_size, function_set);
    
    POP = &population; // TODO for debugging
    
    population.run(steps, function_set, tournamentFunction);
}

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#undef argFloat
#undef argVec2
#undef argTexture
#undef evalTexture
