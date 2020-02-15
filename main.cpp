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

bool run_unit_tests = false;

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
    if (run_unit_tests) UnitTests::allTestsOK();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // TODO TEMP testing display utility and generators.
//    Spot spot(Vec2(0, 0), 0.2, Color(1, 1, 1), 0.6, Color(0, 0, 0));
//    spot.displayInWindow();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Grating grating(Vec2(0.1, 0.1), Color(0, 0.8, 0),
//                    Vec2(0.5, 0.3), Color(0.85, 0.85, 0), 0.3);
//    grating.displayInWindow();
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200101_";
//    grating.writeToFile(511, path + "Grating");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Gradation gradation(Vec2(0.4, -0.2), Color(0.9, 0.0, 0.0),
//                        Vec2(-0.4, 0.2), Color(0.9, 0.9, 0.9));
//    gradation.displayInWindow();
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200101_";
//    gradation.writeToFile(511, path + "Gradation");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Spot sp(Vec2(0, 0), 0.2, Color(1, 1, 1), 0.8, Color(0, 0, 0));
//    Grating g1(Vec2(-0.2, 0), Color(0, 0, 0),
//               Vec2( 0.2, 0), Color(1, 1, 1), 1);
//    Grating g2(Vec2(0, -0.1), Color(1, 0, 1),
//               Vec2(0,  0.1), Color(0, 0, 1), 0.2);
//    SoftMatte sm(sp, g1, g2);
//    sm.displayInWindow();
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200101_";
//    sm.writeToFile(511, path + "SoftMatte");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Grating g1(Vec2(-0.2, 0), Color(0, 0, 0),
//               Vec2( 0.2, 0), Color(1, 1, 1), 1);
//    Grating g2(Vec2(0, -0.1), Color(1, 0, 1),
//               Vec2(0,  0.1), Color(0, 0, 1), 0.2);
//    Max mx(g1, g2);
//    Min mn(g1, g2);
//    Texture::displayInWindow({&mx, &mn});
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200101_";
//    mx.writeToFile(511, path + "Max");
//    mn.writeToFile(511, path + "Min");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Noise no(0.10, Vec2(3, 5), Color(0, 0, 1), Color(1, 1, 0));
//    no.displayInWindow();
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200103_";
//    no.writeToFile(511, path + "Noise");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Brownian br(0.20, Vec2(3, 5), Color(0, 1, 0), Color(0.3, 0.3, 0.3));
//    br.displayInWindow();
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200104_";
//    br.writeToFile(511, path + "Brownian");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Turbulence tu(0.3, Vec2(2, -5), Color(0.3, 0, 0.3), Color(1, 0.6, 0));
//    tu.displayInWindow();
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200106_";
//    tu.writeToFile(511, path + "Turbulence");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Furbulence fu(0.25, Vec2(-1, 2), Color(1, .1, .3), Color(.3, .1, 1));
//    fu.displayInWindow();
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200110_";
//    fu.writeToFile(511, path + "Furbulence");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    SoftMatte(Spot(Vec2(0, 0),
//                   0.1, Color(0, 0, 0),
//                   0.9, Color(1, 1, 1)),
//              Furbulence(0.1, Vec2(1, 2),
//                         Color(1, 1, 1), Color(.7, .7, 0)),
//              Turbulence(0.2, Vec2(-5, 7),
//                         Color(0, 0, 0), Color(.3, .3, 1))).displayInWindow();
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200110_";
//    SoftMatte(Spot(Vec2(0, 0),
//                   0.1, Color(0, 0, 0),
//                   0.9, Color(1, 1, 1)),
//              Furbulence(0.1, Vec2(1, 2),
//                         Color(1, 1, 1), Color(.7, .7, 0)),
//              Turbulence(0.2, Vec2(-5, 7),
//                         Color(0, 0, 0),
//                         Color(.3, .3, 1))).writeToFile(511,
//                                                        path + "comparison");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Wrapulence wr(0.9, Vec2(-2, -9), Color(0, 0, 0), Color(1, 0.6, 0));
//    wr.displayInWindow();
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200111_";
//    wr.writeToFile(511, path + "Wrapulence");

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
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200112_";
//        SoftMatte(spot(0.2),
//                  wrapulence,
//                  SoftMatte(spot(0.4),
//                            furbulence,
//                            SoftMatte(spot(0.6),
//                                      turbulence,
//                                      SoftMatte(spot(0.8),
//                                                brownian,
//                                                noise))))
//            .writeToFile(511, path + "MultiNoise");
//
//    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    //Grating g1(Vec2(0, 0), Color(0, 0, 0), Vec2(0.05, 0.2), Color(1, 1, 1), 1);
    //Grating g2(Vec2(0, 0), Color(0, 0, 0), Vec2(0.05, -0.2), Color(1, 1, 1), 1);
    //BrightnessToHue(0.1, Multiply(g1, g2)).displayInWindow();

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
//    Add gray_gratings(g0, s0);
//    BrightnessToHue bth(0.5, gray_gratings);
//    Texture::displayInWindow({&g0, &g1, &g2, &s0, &gray_gratings, &bth});
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200113_";
//    gray_gratings.writeToFile(511, path + "gray_gratings");
//    bth.writeToFile(511, path + "BrightnessToHue");

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
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200115_";
//    plaid.writeToFile(511, path + "plaid");
//    wrap1.writeToFile(511, path + "Wrap_1");
//    wrap2.writeToFile(511, path + "Wrap_2");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // Demo for StretchSpot, Jan 16
    Grating red_stripes(Vec2(0, 0), Color(1, 0, 0),
                        Vec2(0.1, 0.1), Color(0.3, 0, 0), 0.3);
    Grating green_stripes(Vec2(0, 0), Color(0, 1, 0),
                          Vec2(-0.1, 0.1), Color(0, 0.3, 0), 0.3);
    Add plaid(red_stripes, green_stripes);
    float radius = 0.8;
    float dist = 0.65;
    StretchSpot ss1(4.0, radius, Vec2(+dist, +dist), plaid);
    StretchSpot ss2(0.2, radius, Vec2(-dist, +dist), ss1);
    StretchSpot ss3(0.2, radius, Vec2(-dist, -dist), ss2);
    StretchSpot ss4(4.0, radius, Vec2(+dist, -dist), ss3);
    StretchSpot ss5(4.0, radius, Vec2(-dist, -dist), ss2);
    StretchSpot ss6(0.2, radius, Vec2(+dist, -dist), ss5);
    Texture::displayInWindow({ &plaid, &ss4, &ss6 });
    
    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200116_";
    ss4.writeToFile(511, path + "StretchSpot_1");
    ss6.writeToFile(511, path + "StretchSpot_2");


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

//    // Testing Texture::rasterizer() Jan 20, 2020
//    Noise n(0.2, Vec2(), Color(1, 0, 0), Color(1, 1, 0));
//    Grating g(Vec2(), Color(0, 1, 1), Vec2(0.1, 0.1), Color(0, 0, 1), 0.5);
//    texture_diff(n, g);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Demo for Stretch texture operator, Jan 21, 2020
//
//    ColorNoise cn(0.6, Vec2(5, -2), 0.6);
//    Stretch2009 st1(0.2, pi / 4, Vec2(0.5, 0.5), cn);
//
//    Stretch test0(Vec2(0.2, 0).rotate(pi / 4), Vec2(0.5, 0.5), cn);
//    // texture_diff(st1, test0);
//
//    float r_in = 0.33;
//    float r_out = 0.35;
//    Vec2 p1(0, 0.4);
//    Vec2 p2 = p1.rotate(2 * pi / 3);
//    Vec2 p3 = p2.rotate(2 * pi / 3);
//    Spot spot1(p1, r_in, Color(1, 1, 0), r_out, Color(0, 0, 0));
//    Spot spot2(p2, r_in, Color(0, 1, 1), r_out, Color(0, 0, 0));
//    Spot spot3(p3, r_in, Color(1, 0, 1), r_out, Color(0, 0, 0));
//    Add two_spots(spot1, spot2);
//    Add three_spots(spot3, two_spots);
//    Stretch2009 st2(2, (-2 * pi / 6) + (pi / 2), p2, three_spots);
//
//    Stretch test1((-p2).normalize() * 2, p2, three_spots);
//
//    // shows the effect of "center" argument
//    // Stretch2 test3((-p2).normalize() * 0.5, p2, three_spots);
//    // texture_diff(st2, test3);
//
//    Texture::displayInWindow({ &cn, &st1, &test0,
//                               &three_spots, &st2, &test1 });
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for SliceToRadial, Jan 24, 2020
//    ColorNoise cn(0.6, Vec2(5, -2), 0.6);
//    SliceGrating sg1(Vec2(1, 2) * 2.0, Vec2(0.5, 0.5), cn);
//    SliceGrating sg2(Vec2(1, 2) * 1.0, Vec2(0.5, 0.5), cn);
//    SliceGrating sg3(Vec2(1, 2) * 0.5, Vec2(0.5, 0.5), cn);
//    SliceGrating sg4(Vec2(2, 1) * 0.5, Vec2(0.5, 0.5), cn);
//    Texture::displayInWindow({ &cn, &sg1, &sg2, &sg3, &sg4 });
//
//    // std::string path = "/Users/cwr/Desktop/TexSyn_temp/";
//    // cn.writeToFile(511, path + "20200123_color_noise.jpg");
//    // sg1.writeToFile(511, path + "20200123_SliceGrating1.jpg");
//    // sg2.writeToFile(511, path + "20200123_SliceGrating2.jpg");
//    // sg3.writeToFile(511, path + "20200123_SliceGrating3.jpg");
//    // sg4.writeToFile(511, path + "20200123_SliceGrating4.jpg");
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for SliceToRadial, Jan 23, 2020
//    ColorNoise cn(0.6, Vec2(5, -2), 0.6);
//    SliceToRadial sr1(Vec2(1, 1), Vec2(0, 0), cn);
//    SliceToRadial sr2(Vec2(1, 1), Vec2(0.5, 0.5), cn);
//    SliceToRadial sr3(Vec2(1, 0), Vec2(0.5, 0.5), cn);
//    SliceToRadial sr4(Vec2(2, 0), Vec2(0.5, 0.5), cn);
//    Texture::displayInWindow({ &cn, &sr1, &sr2, &sr3, &sr4 });
//
//    // std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200124_";
//    // cn.writeToFile(511, path + "ColorNoise.jpg");
//    // sr1.writeToFile(511, path + "SliceToRadial1.jpg");
//    // sr2.writeToFile(511, path + "SliceToRadial2.jpg");
//    // sr3.writeToFile(511, path + "SliceToRadial3.jpg");
//    // sr4.writeToFile(511, path + "SliceToRadial4.jpg");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Demo for SliceShear, Jan 25, 2020
//        ColorNoise cn(0.6, Vec2(5, -2), 0.6);
//    //    Grating gr(Vec2(0, 0), Color(1, 0, 1), Vec2(0, 0.1), Color(1, 1, 0), 0.2);
//        Grating gr(Vec2(0, 0), Color(0.9, 0, 0.9),
//                   Vec2(0, 0.3), Color(0.9, 0.9, 0),
//                   0.2);

//    //    SliceShear ss1(Vec2(1, 1), Vec2(), cn, Vec2(0, 1), Vec2(), gr);
//        SliceShear ss1(Vec2(-.7, .7), Vec2(), cn, Vec2(0, 1), Vec2(), gr);
//        SliceShear ss2(Vec2(.1, .1), Vec2(), cn, Vec2(0, 1), Vec2(), gr);
//        SliceShear ss3(Vec2(0.5, 0), Vec2(), cn, Vec2(0, 0.7), Vec2(), gr);
//
//    //    SliceShear ss4(Vec2(1, -1), Vec2(1, 0), cn, Vec2(2, 0), Vec2(2, 1), ss3);
//    //    SliceShear ss4(Vec2(.5, -.5), Vec2(1, 0), cn, Vec2(2, 0), Vec2(2, 1), ss3);
//        SliceShear ss4(Vec2(1, -1), Vec2(1, 0), cn, Vec2(2, 0), Vec2(2, 1), ss3);
//
//    //    Texture::displayInWindow({ &cn, &gr, &ss1, &ss2, &ss3, &ss4 });
    
    
    
//        Spot sp(Vec2(), 0.2, Color(1, 0, 0), 0.5, Color(0, 0, 0));
//    //    Grating sine(Vec2(0, 0), Color(1, 1, 1), Vec2(0, 0.2), Color(0, 0, 0), 1);
//    //    SliceShear sssp1(Vec2(0, 1), Vec2(), sine, Vec2(0, 1), Vec2(), sp);
//        Grating sine(Vec2(0, 0), Color(1, 1, 1), Vec2(0.2, 0), Color(0, 0, 0), 1);
//        SliceShear sssp1(Vec2(1, 0), Vec2(), sine, Vec2(2, 0), Vec2(), sp);
//
//        Texture::displayInWindow({ &sp, &sine, &sssp1 });

    
//    ColorNoise cn(0.6, Vec2(5, -2), 0.6);
//    Grating gr(Vec2(0, 0), Color(0.9, 0, 0.9),
//               Vec2(0, 0.3), Color(0.9, 0.9, 0),
//               0.2);
//    SliceShear zz(Vec2(1, 1), Vec2(), cn, Vec2(0, 1), Vec2(), gr);
//    Texture::displayInWindow({ &cn, &gr, &zz });
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//        // Demo for MobiusTransform, Jan 28, 2020
//        Grating red_stripes(Vec2(0, 0), Color(1, 0, 0),
//                            Vec2(0.1, 0.1), Color(0.3, 0, 0), 0.3);
//        Grating green_stripes(Vec2(0, 0), Color(0, 1, 0),
//                              Vec2(-0.1, 0.1), Color(0, 0.3, 0), 0.3);
//        Add plaid(red_stripes, green_stripes);
//        srand(123456789);
//        auto rv = [](){ return Vec2::randomPointInUnitDiameterCircle() * 4; };
//        MobiusTransform mt0(Vec2(1,2), Vec2(0,.1), Vec2(.1,0), Vec2(1,-2), plaid);
//        MobiusTransform mt1(Vec2(-0.958788, 1.64993),
//                            Vec2(-1.54534, -0.593485),
//                            Vec2(1.29155, -0.931471),
//                            Vec2(0.768266, 0.24665),
//                            plaid);
//        rand();
//        MobiusTransform mt2(rv(), rv(), rv(), rv(), plaid);
//        rand();
//        MobiusTransform mt3(rv(), rv(), rv(), rv(), plaid);
//        MobiusTransform mt4(rv(), rv(), rv(), rv(), plaid);
//        MobiusTransform mt5(rv(), rv(), rv(), rv(), plaid);
//        Texture::displayInWindow({ &plaid, &mt0, &mt1, &mt2, &mt3, &mt4, &mt5 });
//
//    //    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200128_";
//    //    plaid.writeToFile(511, path + "plaid.jpg");
//    //    mt0.writeToFile(511, path + "MobiusTransform0.jpg");
//    //    mt1.writeToFile(511, path + "MobiusTransform1.jpg");
//    //    mt2.writeToFile(511, path + "MobiusTransform2.jpg");
//    //    mt3.writeToFile(511, path + "MobiusTransform3.jpg");
//    //    mt4.writeToFile(511, path + "MobiusTransform4.jpg");
//    //    mt5.writeToFile(511, path + "MobiusTransform5.jpg");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    return EXIT_SUCCESS;
}
