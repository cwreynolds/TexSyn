//
//  main.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "TexSyn.h"
#include "GP.h"
#include "EvoCamoGame.h"

int main(int argc, const char * argv[])
{
    std::cout << texsyn_version_string << std::endl;
    UnitTests::allTestsOK();
    std::cout << "June 23, 2023" << std::endl;
    {
        // Draw a few test cases and UnitTests::instantiateAllTextureTypes().
        Timer t("Tests");
        Uniform red(1, 0, 0);
        Uniform blue(0, 0, 1);
        Uniform green(0, 1, 0);
        // Noise and spots.
        Texture::displayAndFile(ColorNoise(Vec2(), Vec2(0.3, 0.1), 0.65));
        Texture::displayAndFile(LotsOfSpots(0.7, 0.3, 0.3, 0.2, 0.02, red, green));
        //Texture::setDefaultGamma(1);
        Texture::displayAndFile(LotsOfSpots(0.7, 0.3, 0.3, 0.2, 0.02, red, green));
        //Texture::setDefaultGamma(2.2);
        // Demo for MobiusTransform, from Jan 28, 2020 (modernized)
        Texture::displayAndFile(MobiusTransform(Vec2(1,2), Vec2(0,.1),
                                                Vec2(.1,0), Vec2(1,-2),
                                                Plaid()));
        Texture::displayAndFile(MobiusTransform(Vec2(-0.958788, 1.64993),
                                                Vec2(-1.54534, -0.593485),
                                                Vec2(1.29155, -0.931471),
                                                Vec2(0.768266, 0.24665),
                                                Plaid()));
        // Randomly generated phasor noise pattern (see blog post from May 8,
        // 2021: https://cwreynolds.github.io/TexSyn/#20210508)
        Texture::displayAndFile
            (PhasorNoiseTextures
                 (0.966585,
                  0.575451,
                  BrightnessToHue(0.9796,
                                  Uniform(0.132475, 0.122944, 0.706272)),
                  Uniform(0.323459, 0.203465, 0.943876),
                  PhasorNoiseTextures(0.595613,
                                      0.0889437,
                                      Uniform(0.174884, 0.365976, 0.10978),
                                      Uniform(0.912784, 0.611449, 0.870163),
                                      Uniform(0.444307, 0.799821, 0.897265),
                                      Uniform(0.696277, 0.989898, 0.628882),
                                      Uniform(0.537117, 0.611029, 0.653296)),
                  Uniform(0.963422, 0.0551525, 0.181825),
                  Gamma(1.16942,
                        ColorNoise(Vec2(2.06214, 0.309379),
                                   Vec2(0.983534, 3.74675),
                                   0.00450438))));
        // Make one of each Operator type to make sure they all run OK.
        UnitTests::instantiateAllTextureTypes();
    }
    Texture::waitKey();
    
    Texture::invalidInstanceReport();
    Texture::leakCheck();
    Individual::leakCheck();
    abnormal_value_report();
    return EXIT_SUCCESS;
}
