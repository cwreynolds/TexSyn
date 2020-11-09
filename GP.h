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

        // Given 3 Individuals and metric, find the one with lowest metric.
        auto lowest_average_metric = [&]
        (TournamentGroup group, std::function<float(const Color&)> color_metric)
        {
            group.setAllMetrics([&](Individual* i)
            {
                Texture* texture = GP::textureFromIndividual(i);
                return color_metric(GP::ygAverageColorOfTexture(texture));
            });
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


// “colorful, well exposed” evolution test
namespace CWE
{

// Global Population instance. (TODO maybe should be shared_ptr?)
// TODO refactor to share via lexical capture?
static inline Population* population = nullptr;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//static inline int render_size = 99;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Print log and render Texture.
void logger(TournamentGroup group)
{
    static int step_count = 0;
    Individual* best = group.bestIndividual();
    Individual* best2 = group.secondBestIndividual();
    Individual* worst = group.worstIndividual();

    std::cout << std::endl << "step " << step_count++ << std::endl;
    std::cout << "winner size ";
    std::cout << best->tree().size();
    std::cout << ", winner tournaments survived ";
    std::cout << best->getTournamentsSurvived();
    std::cout << std::endl;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    std::cout << std::endl << best->tree().to_string() << std::endl << std::endl;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    std::vector<Individual*> tops = population->nMostTournamentsSurvived(6);
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

float measureScalarHistogram(Individual* individual,
                             int bucket_count,
                             float min_metric,
                             float max_metric,
                             std::function<float(Color)> metric)
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Generate a jittered grid of Color samples
//    std::vector<Color> samples;
//    sampleColors(individual, samples);

    // Get random color samples from Texture, cached if previously needed.
    Texture& texture = *GP::textureFromIndividual(individual);
    const std::vector<Color>& samples = texture.cachedRandomColorSamples(LPRS());
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
    std::cout << "worstExposure: ";
    auto exposure = [](Color c){ return c.luminance(); };
//    return measureScalarHistogram(individual, 5, 0, 1, exposure);
    return measureScalarHistogram(individual, 10, 0, 1, exposure);
}

float measureSaturation(Individual* individual)
{
    std::cout << "worstSaturation: ";
    auto saturation = [](Color c){ return c.getS(); };
//    return measureScalarHistogram(individual, 3, 0, 1, saturation);
    return measureScalarHistogram(individual, 10, 0, 1, saturation);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//float measureSize(Individual* individual)
//{
//    return -(individual->tree().size());
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Score TournamentGroup according to histogram flatness for given metric.
TournamentGroup worstMetric(TournamentGroup group,
                            std::function<float(Individual*)> metric)
{
    group.setAllMetrics([&](Individual* i){ return metric(i); });
    return group;
}

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

// TODO maybe this skipping for all-too-small should happen elsewhere?
// Score TournamentGroup according to worst size.
TournamentGroup worstSize(TournamentGroup group)
{
//    int biggest_size = std::numeric_limits<int>::min();
//    auto metric = [&](Individual* i)
//    {
//        int tree_size = i->tree().size();
//        if (biggest_size < tree_size) biggest_size = tree_size;
//        return -tree_size;
//    };
    auto metric = [&](Individual* i) { return -(i->tree().size()); };
    TournamentGroup ranked = worstMetric(group, metric);
//    if (biggest_size < 100) // TODO need to make this value smarter
//    {
//        ranked.setNevermind();
//    }
//    else
//    {
        std::cout << "worstSize: ";
        for (auto& m : ranked.members()) std::cout << m.metric << " ";
        std::cout << std::endl;
//    }
    return ranked;
}



// Score TournamentGroup according to worst size.
TournamentGroup worstNoise(TournamentGroup group)
{
    auto metric = [&](Individual* i)
    {
        return -(GP::textureFromIndividual(i)->highFrequencyScore());
    };
    TournamentGroup ranked = worstMetric(group, metric);
    std::cout << "worstNoise: ";
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

// Hold tournament for 3 Individuals, scoring by various metrics.
TournamentGroup tournamentFunction(TournamentGroup group)
{
    TournamentGroup ranked_group;
    bool too_small = group.maxTreeSize() < 100; // TODO define threshold better
    float select = LPRS().frandom01();
//    if (select < 0.5)                        // 50% exposure control
    if (select < 0.4)                        // 40% exposure control
    {
        ranked_group = worstExposure(group);
    }
//    else if (select < 0.8)                   // 30% saturation control
    else if (select < 0.7)                   // 30% saturation control
    {
        ranked_group = worstSaturation(group);
    }
//    else if (select < 0.9)                   // 10% noise reduction
//    else if (select < 0.9)                   // 20% noise reduction
    else if (too_small || (select < 0.9))      // 20% noise reduction
    {
        ranked_group = worstNoise(group);
    }
    else                                      // 10% size control
    {
        ranked_group = worstSize(group);
    }
    logger(ranked_group);
    return ranked_group;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void run()
{
    Timer t("CWE test");
    LPRS().setSeed(20201101);
    const FunctionSet& function_set = GP::fs();
    int population_size = 100;
    int generation_equivalents = 50;
    int steps = population_size * generation_equivalents;
    int max_tree_size = 100;
    population = new Population(population_size, max_tree_size, function_set);
    population->run(steps, function_set, tournamentFunction);
    population = nullptr;
}

}

#undef argFloat
#undef argVec2
#undef argTexture
#undef evalTexture
