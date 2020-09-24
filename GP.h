//
//  GP.h
//  TexSyn
//
//  Created by Craig Reynolds on 9/20/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//

//  TODO experimenting with connection to LazyPredator as a heder-only library
//       and so defining a FunctionSet for directly evaluating TexSyn Texture
//       trees.

#pragma once
#include "../LazyPredator/LazyPredator.h"

//    // Nicknames for oft-repeated expressions:
//    #define argFloatq(n)   tree.evalSubtree<float>(n)
//    #define argVec2q(n)    tree.evalSubtree<Vec2>(n)
//    #define argTextureq(n) *tree.evalSubtree<Texture*>(n)
//
//    #define evalTexture(body)   \
//    [](const GpTree& tree)      \
//    {                           \
//        Texture* t = new body;  \
//        return std::any(t);     \
//    }


//    // TODO for prototyping, move elsewhere
//    class ArgIndex
//    {
//    public:
//        static void reset_arg_index() { arg_index = 0; }
//        static int inc_arg_index() { arg_index++; return arg_index - 1; }
//    private:
//        static inline int arg_index = 0;
//    };

//// Nicknames for oft-repeated expressions:
//#define argFloat()   tree.evalSubtree<float>(arg_index++)
//#define argVec2()    tree.evalSubtree<Vec2>(arg_index++)
//#define argTexture() *tree.evalSubtree<Texture*>(arg_index++)

//    // Nicknames for oft-repeated expressions:
//    #define argFloat()   tree.evalSubtree<float>(ArgIndex::inc_arg_index())
//    #define argVec2()    tree.evalSubtree<Vec2>(ArgIndex::inc_arg_index())
//    #define argTexture() *tree.evalSubtree<Texture*>(ArgIndex::inc_arg_index())
//
//
//    #define evalTexture(body)        \
//    [](const GpTree& tree)           \
//    {                                \
//        ArgIndex::reset_arg_index(); \
//        Texture* t = new body;       \
//        return std::any(t);          \
//    }



// TODO for prototyping, move elsewhere
class ArgIndex
{
public:
    ArgIndex(){}
//    static void reset_arg_index() { arg_index = 0; }
    int inc() { return arg_index++; }
private:
    int arg_index = 0;
};


// Nicknames for oft-repeated expressions:
#define argFloat()   tree.evalSubtree<float>(arg_index.inc())
#define argVec2()    tree.evalSubtree<Vec2>(arg_index.inc())
#define argTexture() *tree.evalSubtree<Texture*>(arg_index.inc())


//    #define evalTexture(body)        \
//    [](const GpTree& tree)           \
//    {                                \
//        ArgIndex arg_index;          \
//        Texture* t = new body;       \
//        arg_index.inc();  /* to silence warning */ \
//        return std::any(t);          \
//    }

#define evalTexture(body)        \
[](const GpTree& tree)           \
{                                \
    ArgIndex arg_index;          \
    Texture* t = new body;       \
    return std::any(t);          \
}


class TexSynFS
{
public:
    // Subset of TexSyn API
    static const FunctionSet& tinyTexSyn() { return tiny_texsyn; }
    // Covers "most" of TexSyn API.
    static const FunctionSet& fullTexSyn() { return full_texsyn; }

    static inline const FunctionSet tiny_texsyn =
    {
        {
            {"Texture"},
            {"Vec2"},
            {
                "Float_01",
                [](){ return std::any(LPRS().frandom01()); },
                any_to_string<float>
            },
            {
                "Float_02",
                [](){ return std::any(LPRS().frandom2(0, 2)); },
                any_to_string<float>
            },
            {
                "Float_0_10",
                [](){ return std::any(LPRS().frandom2(0, 10)); },
                any_to_string<float>
            },
            {
                "Float_m5p5",
//                [](){ return std::any(LPRS().frandom2(-5, 5)); },
                // TODO reality checking
                [](){ return std::any(LPRS().frandom2(-1, 1)); },
                any_to_string<float>
            }
        },
        {
            {
                "Vec2",
                "Vec2",
                {"Float_m5p5", "Float_m5p5"},
                [](const GpTree& tree)
                {
                    return std::any(Vec2(tree.evalSubtree<float>(0),
                                         tree.evalSubtree<float>(1)));
                }
            },
            
            {
                "Uniform", "Texture", {"Float_01", "Float_01", "Float_01"},
                [](const GpTree& tree)
                {
                    Texture* t = new Uniform(tree.evalSubtree<float>(0),
                                             tree.evalSubtree<float>(1),
                                             tree.evalSubtree<float>(2));
                    return std::any(t);
                }
            },
            
            {
                "Spot",
                "Texture",
//                {"Vec2", "Float_01", "Texture", "Float_01", "Texture"},
                {"Vec2", "Float_02", "Texture", "Float_02", "Texture"},
                [](const GpTree& tree)
                {
                    Texture* t = new Spot(tree.evalSubtree<Vec2>(0),
                                          tree.evalSubtree<float>(1),
                                          *tree.evalSubtree<Texture*>(2),
                                          tree.evalSubtree<float>(3),
                                          *tree.evalSubtree<Texture*>(4));
                    return std::any(t);
                }
            },
            
            
            {
                "Gradation",
                "Texture",
                {"Vec2", "Texture", "Vec2", "Texture"},
                [](const GpTree& tree)
                {
                    Texture* t = new Gradation(tree.evalSubtree<Vec2>(0),
                                               *tree.evalSubtree<Texture*>(1),
                                               tree.evalSubtree<Vec2>(2),
                                               *tree.evalSubtree<Texture*>(3));
                    return std::any(t);
                }
            },
            {
                "Grating",
                "Texture",
                {"Vec2", "Texture", "Vec2", "Texture", "Float_01", "Float_01"},
                [](const GpTree& tree)
                {
                    Texture* t = new Grating(tree.evalSubtree<Vec2>(0),
                                             *tree.evalSubtree<Texture*>(1),
                                             tree.evalSubtree<Vec2>(2),
                                             *tree.evalSubtree<Texture*>(3),
                                             tree.evalSubtree<float>(4),
                                             tree.evalSubtree<float>(5));
                    return std::any(t);
                }
            },
            {
                "SoftMatte",
                "Texture",
                {"Texture", "Texture", "Texture"},
                [](const GpTree& tree)
                {
                    Texture* t = new SoftMatte(*tree.evalSubtree<Texture*>(0),
                                               *tree.evalSubtree<Texture*>(1),
                                               *tree.evalSubtree<Texture*>(2));
                    return std::any(t);
                }
            },
            // texsyn6_
            {
                "Add",
                "Texture",
                {"Texture", "Texture"},
                [](const GpTree& tree)
                {
                    Texture* t = new Add(*tree.evalSubtree<Texture*>(0),
                                         *tree.evalSubtree<Texture*>(1));
                    return std::any(t);
                }
            },
            {
                "Subtract",
                "Texture",
                {"Texture", "Texture"},
                [](const GpTree& tree)
                {
                    Texture* t = new Subtract(*tree.evalSubtree<Texture*>(0),
                                              *tree.evalSubtree<Texture*>(1));
                    return std::any(t);
                }
            },
            
            {
                "Multiply",
                "Texture",
                {"Texture", "Texture"},
                [](const GpTree& tree)
                {
                    Texture* t = new Multiply(*tree.evalSubtree<Texture*>(0),
                                              *tree.evalSubtree<Texture*>(1));
                    return std::any(t);
                }
            },
            {
                "Max",
                "Texture",
                {"Texture", "Texture"},
                [](const GpTree& tree)
                {
                    Texture* t = new Max(*tree.evalSubtree<Texture*>(0),
                                         *tree.evalSubtree<Texture*>(1));
                    return std::any(t);
                }
            },
            {
                "Min",
                "Texture",
                {"Texture", "Texture"},
                [](const GpTree& tree)
                {
                    Texture* t = new Min(*tree.evalSubtree<Texture*>(0),
                                         *tree.evalSubtree<Texture*>(1));
                    return std::any(t);
                }
            },
            // texsyn10_
            {"AbsDiff", "Texture", {"Texture", "Texture"},
                [](const GpTree& tree)
                {
                    Texture* t = new AbsDiff(*tree.evalSubtree<Texture*>(0),
                                             *tree.evalSubtree<Texture*>(1));
                    return std::any(t);
                }
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
            
            // texsyn11_

            {
                "MultiNoise",
                "Texture",
                {"Vec2", "Vec2", "Texture", "Texture", "Float_01"},
                [](const GpTree& tree)
                {
                    Texture* t = new MultiNoise(tree.evalSubtree<Vec2>(0),
                                                tree.evalSubtree<Vec2>(1),
                                                *tree.evalSubtree<Texture*>(2),
                                                *tree.evalSubtree<Texture*>(3),
                                                tree.evalSubtree<float>(4));
                    return std::any(t);
                }
            },
            // TODO texsyn12_

            {
                "ColorNoise",
                "Texture",
                {"Vec2", "Vec2", "Float_01"},
                [](const GpTree& tree)
                {
                    Texture* t = new ColorNoise(tree.evalSubtree<Vec2>(0),
                                                tree.evalSubtree<Vec2>(1),
                                                tree.evalSubtree<float>(2));
                    return std::any(t);
                }
            },
            // TODO texsyn13_
            {
                "BrightnessToHue",
                "Texture",
                {"Float_01", "Texture"},
                [](const GpTree& tree)
                {
                    Texture* t = new
                        BrightnessToHue(tree.evalSubtree<float>(0),
                                        *tree.evalSubtree<Texture*>(1));
                    return std::any(t);
                }
            },
            
            // TODO texsyn14_
            {
                "Wrap",
                "Texture",
                {"Float_m5p5", "Vec2", "Vec2", "Texture"},
                [](const GpTree& tree)
                {
                    Texture* t = new Wrap(tree.evalSubtree<float>(0),
                                          tree.evalSubtree<Vec2>(1),
                                          tree.evalSubtree<Vec2>(2),
                                          *tree.evalSubtree<Texture*>(3));
                    return std::any(t);
                }
            },
                        
            
            // TODO texsyn15_
            {
                "StretchSpot",
                "Texture",
                {"Float_02", "Float_02", "Vec2", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new StretchSpot(argFloat(0),
//                                                 argFloat(1),
//                                                 argVec2(2),
//                                                 argTexture(3));
//                    return std::any(t);
//                }
                evalTexture(StretchSpot(argFloat(),
                                        argFloat(),
                                        argVec2(),
                                        argTexture()))
            },

            // TODO texsyn16_
            {
                "Stretch",
                "Texture",
                {"Vec2", "Vec2", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new Stretch(argVec2q(0),
//                                             argVec2q(1),
//                                             argTextureq(2));
//                    return std::any(t);
//                }
                evalTexture(Stretch(argVec2(),
                                    argVec2(),
                                    argTexture()))
            },
            
            {
                "SliceGrating",
                "Texture",
                {"Vec2", "Vec2", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new SliceGrating(argVec2q(0),
//                                                  argVec2q(1),
//                                                  argTextureq(2));
//                    return std::any(t);
//                }
                evalTexture(SliceGrating(argVec2(),
                                         argVec2(),
                                         argTexture()))
            },
            {
                "SliceToRadial",
                "Texture",
                {"Vec2", "Vec2", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new SliceToRadial(argVec2q(0),
//                                                   argVec2q(1),
//                                                   argTextureq(2));
//                    return std::any(t);
//                }
                evalTexture(SliceToRadial(argVec2(),
                                          argVec2(),
                                          argTexture()))
            },
            // TODO texsyn19_
            {
                "SliceShear",
                "Texture",
                {"Vec2", "Vec2", "Texture", "Vec2", "Vec2", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new SliceShear(argVec2q(0),
//                                                argVec2q(1),
//                                                argTextureq(2),
//                                                argVec2q(3),
//                                                argVec2q(4),
//                                                argTextureq(5));
//                    return std::any(t);
//                }
                evalTexture(SliceShear(argVec2(),
                                       argVec2(),
                                       argTexture(),
                                       argVec2(),
                                       argVec2(),
                                       argTexture()))
            },
            
            // TODO texsyn20_

            {
                "Colorize",
                "Texture",
                {"Vec2", "Vec2", "Texture", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new Colorize(argVec2(0),
//                                              argVec2(1),
//                                              argTexture(2),
//                                              argTexture(3));
//                    return std::any(t);
//                }
                
                evalTexture(Colorize(argVec2(),
                                     argVec2(),
                                     argTexture(),
                                     argTexture()))
            },
            // TODO texsyn21_
            {
                "MobiusTransform",
                "Texture",
                {"Vec2", "Vec2", "Vec2", "Vec2", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new MobiusTransform(argVec2(0),
//                                                     argVec2(1),
//                                                     argVec2(2),
//                                                     argVec2(3),
//                                                     argTexture(4));
//                    return std::any(t);
//                }
                evalTexture(MobiusTransform(argVec2(),
                                            argVec2(),
                                            argVec2(),
                                            argVec2(),
                                            argTexture()))
            },
            //{"Scale", "Texture", {"Float_m5p5", "Texture"}},
            //{"Rotate", "Texture", {"Float_m5p5", "Texture"}},
            //{"Translate", "Texture", {"Vec2", "Texture"}},
            
//            {
//                "Blur",
//                "Texture",
//                {"Float_01", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new Blur(argFloat(0),
//                                          argTexture(1));
//                    return std::any(t);
//                }
//            },
            
            // TODO texsyn23_
            {
                "SoftThreshold",
                "Texture",
                {"Float_01", "Float_01", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new SoftThreshold(argFloat(0),
//                                                   argFloat(1),
//                                                   argTexture(2));
//                    return std::any(t);
//                }
                evalTexture(SoftThreshold(argFloat(),
                                          argFloat(),
                                          argTexture()))
            },
            
            
//                {"EdgeDetect", "Texture", {"Float_01", "Texture"}},
//                {"EdgeEnhance", "Texture", {"Float_01", "Float_02", "Texture"}},
            
            {
                "AdjustHue",
                "Texture",
                {"Float_01", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new AdjustHue(argFloat(0),
//                                               argTexture(1));
//                    return std::any(t);
//                }
                evalTexture(AdjustHue(argFloat(),
                                      argTexture()))
            },
            {
                "AdjustSaturation",
                "Texture",
                {"Float_01", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new AdjustSaturation(argFloat(0),
//                                               argTexture(1));
//                    return std::any(t);
//                }
                evalTexture(AdjustSaturation(argFloat(),
                                             argTexture()))
            },
            // TODO texsyn28_
            {
                "AdjustBrightness",
                "Texture",
                {"Float_01", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new AdjustBrightness(argFloat(0),
//                                               argTexture(1));
//                    return std::any(t);
//                }
                evalTexture(AdjustBrightness(argFloat(),
                                             argTexture()))
            },
            // TODO texsyn29_
            {
                "Twist",
                "Texture",
                {"Float_m5p5", "Float_m5p5", "Vec2", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new Twist(argFloat(0),
//                                           argFloat(1),
//                                           argVec2(2),
//                                           argTexture(3));
//                    return std::any(t);
//                }
                evalTexture(Twist(argFloat(),
                                  argFloat(),
                                  argVec2(),
                                  argTexture()))
            },
            // TODO texsyn30_

//            {
//                "BrightnessWrap",
//                "Texture",
//                {"Float_01", "Float_01", "Texture"},
//                [](const GpTree& tree)
//                {
//                    Texture* t = new BrightnessWrap(argFloat(0),
//                                                    argFloat(1),
//                                                    argTexture(2));
//                    return std::any(t);
//                }
//            },

            {
                "BrightnessWrap",
                "Texture",
                {"Float_01", "Float_01", "Texture"},
                evalTexture(BrightnessWrap(argFloat(),
                                           argFloat(),
                                           argTexture()))
            },
            // TODO texsyn32_
            {
                "Mirror",
                "Texture",
                {"Vec2", "Vec2", "Texture"},
                evalTexture(Mirror(argVec2(),
                                   argVec2(),
                                   argTexture()))
            },
            // TODO texsyn33_
            {
                "Ring",
                "Texture",
                {"Float_0_10", "Vec2", "Vec2", "Texture"},
                evalTexture(Ring(argFloat(),
                                 argVec2(),
                                 argVec2(),
                                 argTexture()))
            },
            // TODO texsyn34_
            {
                "Row",
                "Texture",
                {"Vec2", "Vec2", "Texture"},
                evalTexture(Row(argVec2(),
                                argVec2(),
                                argTexture()))
            },
            //{"Shader", "Texture", {}},
            // TODO texsyn35_
            // TODO reconsider the ephemeral float ranges.
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
            // TODO texsyn36_
            // TODO reconsider the ephemeral float ranges.
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
            // TODO texsyn37_
            // TODO reconsider the ephemeral float ranges.
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
            
            
            // TODO texsyn38_
            {
                "Gamma",
                "Texture",
                {"Float_0_10", "Texture"},
                evalTexture(Gamma(argFloat(),
                                  argTexture()))
            },
            
            // TODO texsyn39_
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
            
            // TODO texsyn40_
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
            // TODO texsyn41_
            // TODO reconsider the ephemeral float ranges.
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
            // TODO texsyn42_
            {
                "Affine",
                "Texture",
                {"Vec2", "Vec2", "Texture"},
                evalTexture(Affine(argVec2(),
                                   argVec2(),
                                   argTexture()))
            },
            // TODO texsyn43_
            {
                "HueOnly",
                "Texture",
                {"Float_01", "Float_01", "Texture"},
                evalTexture(HueOnly(argFloat(),
                                    argFloat(),
                                    argTexture()))
            }
        }
    };

    static inline FunctionSet full_texsyn;

//    static FunctionSet& full()
//    {
//        static FunctionSet full =
//        {
//            {
//                {"Texture"},
//                {"Vec2"},
//                // TODO -- note these used to reference eg "fs.rs().frandom01()"
//                //         temporarily they are calling the global (non
//                //         RandomSequence) version, so repeatability is missing.
//                {"Float_01", [](){ return std::to_string(frandom01()); }},
//                {"Float_02", [](){ return std::to_string(frandom2(0, 2)); }},
//                {"Float_0_10", [](){ return std::to_string(frandom2(0, 10)); }},
//                {"Float_m5p5", [](){ return std::to_string(frandom2(-5, 5)); }}
//            },
//            {
//                // Functions
//                {"Vec2", "Vec2", {"Float_m5p5", "Float_m5p5"}},
//                {"Uniform", "Texture",  {"Float_01", "Float_01", "Float_01"}},
//                {"Spot", "Texture",
//                    {"Vec2", "Float_02", "Texture", "Float_02", "Texture"}},
//                {"Gradation", "Texture",
//                    {"Vec2", "Texture", "Vec2", "Texture"}},
//                {"Grating", "Texture",
//                    {"Vec2", "Texture", "Vec2", "Texture",
//                        "Float_01", "Float_01"}},
//                {"SoftMatte", "Texture", {"Texture", "Texture", "Texture"}},
//                {"Add", "Texture", {"Texture", "Texture"}},
//                {"Subtract", "Texture", {"Texture", "Texture"}},
//                {"Multiply", "Texture", {"Texture", "Texture"}},
//                {"Max", "Texture", {"Texture", "Texture"}},
//                {"Min", "Texture", {"Texture", "Texture"}},
//                {"AbsDiff", "Texture", {"Texture", "Texture"}},
//                //{"NotEqual", "Texture", {"Texture", "Texture"}},
//                //{"Noise", "Texture", {"Vec2", "Vec2", "Texture", "Texture"}},
//                //{"Brownian", "Texture",
//                //               {"Vec2", "Vec2", "Texture", "Texture"}},
//                //{"Turbulence", "Texture",
//                //               {"Vec2", "Vec2", "Texture", "Texture"}},
//                //{"Furbulence", "Texture",
//                //               {"Vec2", "Vec2", "Texture", "Texture"}},
//                //{"Wrapulence", "Texture",
//                //               {"Vec2", "Vec2", "Texture", "Texture"}},
//                {"MultiNoise", "Texture",
//                    {"Vec2", "Vec2", "Texture", "Texture", "Float_01"}},
//                {"ColorNoise", "Texture", {"Vec2", "Vec2", "Float_01"}},
//                {"BrightnessToHue", "Texture", {"Float_01", "Texture"}},
//                {"Wrap", "Texture", {"Float_m5p5", "Vec2", "Vec2", "Texture"}},
//                {"StretchSpot", "Texture",
//                    {"Float_02", "Float_02", "Vec2", "Texture"}},
//                {"Stretch", "Texture", {"Vec2", "Vec2", "Texture"}},
//                {"SliceGrating", "Texture", {"Vec2", "Vec2", "Texture"}},
//                {"SliceToRadial", "Texture", {"Vec2", "Vec2", "Texture"}},
//                {"SliceShear", "Texture",
//                    {"Vec2", "Vec2", "Texture", "Vec2", "Vec2", "Texture"}},
//                {"Colorize", "Texture", {"Vec2", "Vec2", "Texture", "Texture"}},
//                {"MobiusTransform", "Texture",
//                    {"Vec2", "Vec2", "Vec2", "Vec2", "Texture"}},
//                //{"Scale", "Texture", {"Float_m5p5", "Texture"}},
//                //{"Rotate", "Texture", {"Float_m5p5", "Texture"}},
//                //{"Translate", "Texture", {"Vec2", "Texture"}},
//                {"Blur", "Texture", {"Float_01", "Texture"}},
//                {"SoftThreshold", "Texture",
//                    {"Float_01", "Float_01", "Texture"}},
//                {"EdgeDetect", "Texture", {"Float_01", "Texture"}},
//                {"EdgeEnhance", "Texture", {"Float_01", "Float_02", "Texture"}},
//                {"AdjustHue", "Texture", {"Float_01", "Texture"}},
//                {"AdjustSaturation", "Texture", {"Float_01", "Texture"}},
//                {"AdjustBrightness", "Texture", {"Float_01", "Texture"}},
//                {"Twist", "Texture",
//                    {"Float_m5p5", "Float_m5p5", "Vec2", "Texture"}},
//                {"BrightnessWrap", "Texture",
//                    {"Float_01", "Float_01", "Texture"}},
//                {"Mirror", "Texture", {"Vec2", "Vec2", "Texture"}},
//                {"Ring", "Texture", {"Float_0_10", "Vec2", "Vec2", "Texture"}},
//                {"Row", "Texture", {"Vec2", "Vec2", "Texture"}},
//                //{"Shader", "Texture", {}},
//                // TODO reconsider the ephemeral float ranges.
//                {"LotsOfSpots", "Texture",
//                    {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
//                        "Texture", "Texture"}},
//                // TODO reconsider the ephemeral float ranges.
//                {"ColoredSpots", "Texture",
//                    {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
//                        "Texture", "Texture"}},
//                // TODO reconsider the ephemeral float ranges.
//                {"LotsOfButtons", "Texture",
//                    {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
//                        "Vec2", "Texture", "Float_01", "Texture"}},
//                {"Gamma", "Texture", {"Float_0_10", "Texture"}},
//                {"RgbBox", "Texture",
//                    {"Float_01", "Float_01", "Float_01", "Float_01", "Float_01",
//                        "Float_01", "Texture"}},
//                {"CotsMap", "Texture", {"Vec2", "Vec2", "Vec2", "Vec2", "Texture"}},
//                // TODO reconsider the ephemeral float ranges.
//                {"Hyperbolic", "Texture",
//                    {"Vec2", "Float_0_10", "Float_0_10", "Float_0_10",
//                        "Texture", "Texture"}},
//                {"Affine", "Texture", {"Vec2", "Vec2", "Texture"}},
//                {"HueOnly", "Texture", {"Float_01", "Float_01", "Texture"}}
//            }
//        };
//        return full;
//    }
};

#undef argFloat
#undef argVec2
#undef argTexture
#undef evalTexture
