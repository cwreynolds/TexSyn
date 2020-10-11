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
};

#undef argFloat
#undef argVec2
#undef argTexture
#undef evalTexture
