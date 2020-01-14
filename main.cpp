//
//  main.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Texture.h"
#include "Generators.h"
#include "Operators.h"
#include "UnitTests.h"
#include "Utilities.h"

int main(int argc, const char * argv[])
{
    // TODO TEMP run unit tests
    UnitTests::allTestsOK();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // TODO TEMP testing display utility and generators.
//    Spot spot(Vec2(0, 0), 0.2, Color(1, 1, 1), 0.6, Color(0, 0, 0));
//    spot.displayInWindow();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Grating grating(Vec2(0.1, 0.1), Color(0, 0.8, 0),
//                    Vec2(0.5, 0.3), Color(1, 1, 0), 0.3);
//    grating.displayInWindow();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Gradation gradation(Vec2(0.4, -0.2), Color(0, 0.8, 0),
//                        Vec2(-0.4, 0.2), Color(1, 1, 1));
//    gradation.displayInWindow();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    SoftMatte sm(Spot(Vec2(0, 0), 0.2, Color(1, 1, 1), 0.8, Color(0, 0, 0)),
//                 Grating(Vec2(-0.2, 0), Color(0, 0, 0),
//                         Vec2( 0.2, 0), Color(1, 1, 1), 1),
//                 Grating(Vec2(0, -0.1), Color(1, 0, 1),
//                         Vec2(0,  0.1), Color(0, 0, 1), 0.2));
//    sm.displayInWindow();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Max mx(Grating(Vec2(-0.2, 0), Color(0, 0, 0),
//                   Vec2( 0.2, 0), Color(1, 1, 1), 1),
//           Grating(Vec2(0, -0.1), Color(1, 0, 1),
//                   Vec2(0,  0.1), Color(0, 0, 1), 0.2));
//    mx.displayInWindow();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Noise no(0.10, Vec2(3, 5), Color(0, 0, 1), Color(1, 1, 0));
//    no.displayInWindow();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Brownian br(0.20, Vec2(3, 5), Color(0, 1, 0), Color(0.3, 0.3, 0.3));
//    br.displayInWindow();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Turbulence tu(0.3, Vec2(2, -5), Color(0.3, 0, 0.3), Color(1, 0.6, 0));
//    tu.displayInWindow();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Furbulence fu(0.25, Vec2(-1, 2), Color(1, .1, .3), Color(.3, .1, 1));
//    fu.displayInWindow();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Wrapulence wr(0.9, Vec2(-2, -9), Color(0, 0, 0), Color(1, 0.6, 0));
//    wr.displayInWindow();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    SoftMatte(Spot(Vec2(0, 0),
//                   0.1, Color(0, 0, 0),
//                   0.9, Color(1, 1, 1)),
//              Furbulence(0.1, Vec2(1, 2),
//                         Color(1, 1, 1), Color(.3, .3, 0)),
//              Turbulence(0.2, Vec2(-5, 7),
//                         Color(0, 0, 1), Color(0, 0, 0))).displayInWindow();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Comparison of 5 noise varieties, test for MultiNoise.
//    {
//        float scale = 0.3;
//        Vec2 center(0, 0);
//        Color white(1, 1, 1);
//        Color black(0, 0, 0);
//        Color magenta(1, 0.3, 1);
//        Color red(1, 0.3, 0.3);
//        Color yellow(1, 1, 0.3);
//        Color green(0.3, 1, 0.3);
//        Color cyan(0.3, 1, 1);
//        // Noise noise(scale, center, black, magenta);
//        // Brownian brownian(scale, center, black, red);
//        // Turbulence turbulence(scale, center, black, yellow);
//        // Furbulence furbulence(scale, center, black, green);
//        // Wrapulence wrapulence(scale, center, black, cyan);
//        MultiNoise noise(scale, center, black, magenta, 0.0);
//        MultiNoise brownian(scale, center, black, red, 0.2);
//        MultiNoise turbulence(scale, center, black, yellow, 0.4);
//        MultiNoise furbulence(scale, center, black, green, 0.6);
//        MultiNoise wrapulence(scale, center, black, cyan, 0.8);
//        auto spot = [&](float r) { return Spot(center,r,black,r+0.05,white); };
//        SoftMatte(spot(0.2),
//                  wrapulence,
//                  SoftMatte(spot(0.4),
//                            furbulence,
//                            SoftMatte(spot(0.6),
//                                      turbulence,
//                                      SoftMatte(spot(0.8),
//                                                brownian,
//                                                noise)))).displayInWindow();
//    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    //Grating g1(Vec2(0, 0), Color(0, 0, 0), Vec2(0.05, 0.2), Color(1, 1, 1), 1);
//    //Grating g2(Vec2(0, 0), Color(0, 0, 0), Vec2(0.05, -0.2), Color(1, 1, 1), 1);
//    //BrightnessToHue(0.1, Multiply(g1, g2)).displayInWindow();
//
//    // Following graygratings and BrightnessToHue examples from 2009
//    float angle = pi *  0.6;
//    float frequency = 25;
//    Vec2 basis(0, 2 * pi / frequency);
//    Vec2 basis1 = basis.rotate(+angle);
//    Vec2 basis2 = basis.rotate(-angle);
//    Vec2 origin(0, 0);
//    Color black(0, 0, 0);
//    Color white(1, 1, 1);
//    Color gray25(0.25, 0.25, 0.25);
//    Grating g1(origin, black, basis1, gray25, 1);
//    Grating g2(origin, black, basis2, gray25, 1);
//    Grating g0(origin, black, Vec2(0, 2), Color(0.5, 0.5, 0.5), 1);
//    Add s0(g1, g2);
//    Add graygratings(g0, s0);
//    //graygratings.displayInWindow();
//    BrightnessToHue(0, graygratings).displayInWindow();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    Grating blue_stripes(Vec2(), Color(), Vec2(0.1, 0.2), Color(0, 0, 1), 0.8);
    Gradation red_grad(Vec2(), Color(1, 0, 0), Vec2(0, 1), Color());
    Add both(blue_stripes, red_grad);
    Wrap wrap(5, Vec2(), Vec2(0, 1), both);
    Texture::displayInWindow({&blue_stripes, &red_grad, &both, &wrap});
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    return EXIT_SUCCESS;
}
