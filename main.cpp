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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO needs to go somewhere else
// TODO originally had it in Texture but it wants to use Operators derived from that.

// Compare two Textures: compute average per-pixel color difference, display
// the two textures and the "distance" between them (abs diff?).
void texture_diff(const Texture& t0, const Texture& t1)
{
    AbsDiff abs_diff(t0, t1);
    int pixel_count = 0;
    Color total_color(0, 0, 0);
    int size = 511;
    Texture::rasterizeDisk(size,
                           [&](int i, int j, Vec2 position)
                           {
                               Color diff = abs_diff.getColor(position);
                               total_color += diff;
                               pixel_count++;
                           });
    debugPrint(pixel_count);
    debugPrint(total_color);
    debugPrint(total_color / pixel_count);
    Texture::displayInWindow({ &t0, &t1, &abs_diff }, 451);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

//    // Comparison of 5 noise varieties, test for MultiNoise. (Jan 12, 2020)
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
//    Vec2 basis(0, pi / frequency);
//    Vec2 basis1 = basis.rotate(+angle);
//    Vec2 basis2 = basis.rotate(-angle);
//    Color black(0, 0, 0);
//    Color white(1, 1, 1);
//    Color gray25(0.25, 0.25, 0.25);
//    Grating g1(-basis1, black, basis1, gray25, 1);
//    Grating g2(-basis2, black, basis2, gray25, 1);
//    Grating g0(Vec2(), black, Vec2(0, 2), Color(0.5, 0.5, 0.5), 1);
//    Add s0(g1, g2);
//    Add graygratings(g0, s0);
//    BrightnessToHue bth(0, graygratings);
//    Texture::displayInWindow({&g0, &g1, &g2, &s0, &graygratings, &bth});
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    Grating blue_stripes(Vec2(), Color(), Vec2(0.1, 0.2), Color(0, 0, 1), 0.8);
//    Gradation red_grad(Vec2(), Color(1, 0, 0), Vec2(0, 1), Color());
//    Add both(blue_stripes, red_grad);
//    Wrap wrap(5, Vec2(), Vec2(0, 1), both);
//    Texture::displayInWindow({&blue_stripes, &red_grad, &both, &wrap});
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for Wrap with arbitrary center and fixed_ray. Jan 15
//    Grating red_stripes(Vec2(0, 0), Color(1, 0, 0),
//                        Vec2(0.1, 0.1), Color(0.3, 0, 0), 0.3);
//    Grating green_stripes(Vec2(0, 0), Color(0, 1, 0),
//                          Vec2(-0.1, 0.1), Color(0, 0.3, 0), 0.3);
//    Add plaid(red_stripes, green_stripes);
//    // With identity transform.
//    Wrap wrap1(5, Vec2(), Vec2(0, 1), plaid);
//    // In this version, the discontinuity (-y,  opposite "fixed_ray") is along
//    // (-1, -1). Also noticable aliasing near "center".
//    Wrap wrap2(5, Vec2(0.2, 0.2), Vec2(1, 1), plaid);
//    Texture::displayInWindow({
//        &red_stripes,
//        &green_stripes,
//        &plaid,
//        &wrap1,
//        &wrap2});

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for StretchSpot, Jan 16
//    Grating red_stripes(Vec2(0, 0), Color(1, 0, 0),
//                        Vec2(0.1, 0.1), Color(0.3, 0, 0), 0.3);
//    Grating green_stripes(Vec2(0, 0), Color(0, 1, 0),
//                          Vec2(-0.1, 0.1), Color(0, 0.3, 0), 0.3);
//    Add plaid(red_stripes, green_stripes);
//    float radius = 0.8;
//    float dist = 0.65;
//    StretchSpot ss1(4.0, radius, Vec2(+dist, +dist), plaid);
//    StretchSpot ss2(0.2, radius, Vec2(-dist, +dist), ss1);
//    StretchSpot ss3(0.2, radius, Vec2(-dist, -dist), ss2);
//    StretchSpot ss4(4.0, radius, Vec2(+dist, -dist), ss3);
//    StretchSpot ss5(4.0, radius, Vec2(-dist, -dist), ss2);
//    StretchSpot ss6(0.2, radius, Vec2(+dist, -dist), ss5);
//    Texture::displayInWindow({ &plaid, &ss4, &ss6 });

//    // TODO TEMP TEST related to size of inverse LUT
//    Grating vert_stripes(Vec2(0, 0), Color(1, 1, 1),
//                         Vec2(0.1, 0), Color(0, 0, 0), 0.2);
//    StretchSpot(0.5, 10, Vec2(-5, 0), vert_stripes).displayInWindow();

//    // TODO TEMP TEST for test for memory leaks
//    Color c(0, 0, 0);
//    int count = 10000;
//    for (int i = 0; i < count; i++)
//    {
//        StretchSpot ss(0.5, 10, Vec2(-5, 0), vert_stripes);
//        c = c + ss.getColor(Vec2());
//        if (((i + 1) % (count / 100)) == 0) debugPrint(c);
//    }
//    debugPrint(c);

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Demo for ColorNoise, Jan 20
//    ColorNoise cn1(1, Vec2(-7, 4), 0.1);
//    ColorNoise cn2(0.5, Vec2(-18, -20), 0.8);
//    ColorNoise cn3(0.08, Vec2(15, -12), 0.6);
//    Texture::displayInWindow({ &cn1, &cn2, &cn3 });
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Testing Texture::rasterizer() Jan 20, 2020
    Noise n(0.2, Vec2(), Color(1, 0, 0), Color(1, 1, 0));
    Grating g(Vec2(), Color(0, 1, 1), Vec2(0.1, 0.1), Color(0, 0, 1), 0.5);
    texture_diff(n, g);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    return EXIT_SUCCESS;
}
