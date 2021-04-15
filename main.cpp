//
//  main.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "TexSyn.h"
#include "GP.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20210120 add Camouflage.h module
#include "Camouflage.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool run_unit_tests = true;

int main(int argc, const char * argv[])
{
    std::cout << texsyn_version_string << std::endl;
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
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200116_";
//    ss4.writeToFile(511, path + "StretchSpot_1");
//    ss6.writeToFile(511, path + "StretchSpot_2");
//
//
//    //    // TODO TEMP TEST related to size of inverse LUT
//    //    Grating vert_stripes(Vec2(0, 0), Color(1, 1, 1),
//    //                         Vec2(0.1, 0), Color(0, 0, 0), 0.2);
//    //    StretchSpot(0.5, 10, Vec2(-5, 0), vert_stripes).displayInWindow();
//
//    //    // TODO TEMP TEST for test for memory leaks
//    //    Color c(0, 0, 0);
//    //    int count = 10000;
//    //    for (int i = 0; i < count; i++)
//    //    {
//    //        StretchSpot ss(0.5, 10, Vec2(-5, 0), vert_stripes);
//    //        c = c + ss.getColor(Vec2());
//    //        if (((i + 1) % (count / 100)) == 0) debugPrint(c);
//    //    }
//    //    debugPrint(c);

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Demo for ColorNoise, Jan 20
//    ColorNoise cn1(1, Vec2(-7, 4), 0.0);
//    ColorNoise cn2(0.5, Vec2(-18, -20), 0.8);
//    ColorNoise cn3(0.08, Vec2(15, -12), 0.6);
//    Texture::displayInWindow({ &cn1, &cn2, &cn3 });
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200120_";
//    cn1.writeToFile(511, path + "ColorNoise_1");
//    cn2.writeToFile(511, path + "ColorNoise_2");
//    cn3.writeToFile(511, path + "ColorNoise_3");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Testing Texture::rasterizer() Jan 20, 2020
//    Noise n(0.2, Vec2(), Color(1, 0, 0), Color(1, 1, 0));
//    Grating g(Vec2(), Color(0, 1, 1), Vec2(0.1, 0.1), Color(0, 0, 1), 0.5);
//    Texture::diff(n, g);

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Demo for Stretch texture operator, Jan 21, 2020
//
//    ColorNoise cn(0.6, Vec2(5, -2), 0.6);
//    Stretch2009 old1(0.2, pi / 4, Vec2(0, 0), cn);
//    Stretch s1(Vec2(0.2, 0).rotate(pi / 4), Vec2(0, 0), cn);
//    // Texture::diff(old1, s1);
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
//    Stretch2009 old2(2, (-2 * pi / 6) + (pi / 2), p2, three_spots);
//    Stretch s2((-p2).normalize() * 2, p2, three_spots);
//    // shows the effect of "center" argument
//    // Stretch test3((-p2).normalize() * 0.5, p2, three_spots);
//    // Texture::diff(s2, test3);
//
//    Texture::displayInWindow({ &cn, &s1, &three_spots, &s2 });
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200121_";
//    cn.writeToFile(511, path + "ColorNoise");
//    s1.writeToFile(511, path + "Stretch_1");
//    three_spots.writeToFile(511, path + "three_spots");
//    s2.writeToFile(511, path + "Stretch_2");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for SliceGrating, Jan 23, 2020
//    ColorNoise cn(0.6, Vec2(5, -2), 0.6);
//    SliceGrating sg1(Vec2(1, 2) * 2.0, Vec2(0.5, 0.5), cn);
//    SliceGrating sg2(Vec2(1, 2) * 1.0, Vec2(0.5, 0.5), cn);
//    SliceGrating sg3(Vec2(1, 2) * 0.5, Vec2(0.5, 0.5), cn);
//    SliceGrating sg4(Vec2(2, 1) * 0.5, Vec2(0.5, 0.5), cn);
//    Texture::displayInWindow({ &cn, &sg1, &sg2, &sg3, &sg4 });
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200123_";
//    cn.writeToFile(511, path + "color_noise");
//    sg1.writeToFile(511, path + "SliceGrating1");
//    sg2.writeToFile(511, path + "SliceGrating2");
//    sg3.writeToFile(511, path + "SliceGrating3");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for SliceToRadial, Jan 24, 2020
//    ColorNoise cn(0.6, Vec2(5, -2), 0.6);
//    SliceToRadial sr1(Vec2(1, 1), Vec2(0, 0), cn);
//    SliceToRadial sr2(Vec2(1, 1), Vec2(0.5, 0.5), cn);
//    SliceToRadial sr3(Vec2(1, 0), Vec2(0.5, 0.5), cn);
//    SliceToRadial sr4(Vec2(2, 0), Vec2(0.5, 0.5), cn);
//    Texture::displayInWindow({ &cn, &sr1, &sr2, &sr3, &sr4 });
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200124_";
//    sr1.writeToFile(511, path + "SliceToRadial1");
//    sr2.writeToFile(511, path + "SliceToRadial2");
//    sr3.writeToFile(511, path + "SliceToRadial3");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Demo for SliceShear, Jan 25, 2020
    // Note: I returned to this on March 20. I left this unchanged but
    // made a copy at the bottom of the file for further experiments.
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
    
//    // Demo for MobiusTransform, Jan 28, 2020
//    Grating red_stripes(Vec2(0, 0), Color(1, 0, 0),
//                        Vec2(0.1, 0.1), Color(0.3, 0, 0), 0.3);
//    Grating green_stripes(Vec2(0, 0), Color(0, 1, 0),
//                          Vec2(-0.1, 0.1), Color(0, 0.3, 0), 0.3);
//    Add plaid(red_stripes, green_stripes);
//    srand(123456789);
//    auto rv = [](){ return Vec2::randomPointInUnitDiameterCircle() * 4; };
//    MobiusTransform mt0(Vec2(1,2), Vec2(0,.1), Vec2(.1,0), Vec2(1,-2), plaid);
//    MobiusTransform mt1(Vec2(-0.958788, 1.64993),
//                        Vec2(-1.54534, -0.593485),
//                        Vec2(1.29155, -0.931471),
//                        Vec2(0.768266, 0.24665),
//                        plaid);
//    rand();
//    MobiusTransform mt2(rv(), rv(), rv(), rv(), plaid);
//    rand();
//    MobiusTransform mt3(rv(), rv(), rv(), rv(), plaid);
//    MobiusTransform mt4(rv(), rv(), rv(), rv(), plaid);
//    MobiusTransform mt5(rv(), rv(), rv(), rv(), plaid);
//    //Texture::displayInWindow({ &plaid, &mt0, &mt1, &mt2, &mt3, &mt4, &mt5 });
//    Texture::displayInWindow({ &mt0, &mt5, &mt1, &mt2 });
//
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200128_";
//    mt0.writeToFile(511, path + "MobiusTransform_0");
//    mt1.writeToFile(511, path + "MobiusTransform_1");
//    mt2.writeToFile(511, path + "MobiusTransform_2");
//    mt5.writeToFile(511, path + "MobiusTransform_5");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for Scale, Feb 21, 2020
//    Spot spot1(Vec2(+0.2, 0), 0.38, Color(0.7, 0, 0), 0.4, Color());
//    Spot spot2(Vec2(-0.2, 0), 0.38, Color(0, 0, 0.7), 0.4, Color());
//    Add two_spots(spot1, spot2);
//    Scale scaled_spots(1.5, two_spots);
//    Rotate rotated_spots(pi / 4, two_spots);
//    Rotate scale_then_rotate(pi / 4, scaled_spots);
//    Translate translated_spots(Vec2(0, 0.3), two_spots);
//    Translate scale_rotate_translate(Vec2(0, 0.3), scale_then_rotate);
//    Texture::displayInWindow({
//        &two_spots,
//        &scaled_spots,
//        &rotated_spots,
//        &scale_then_rotate,
//        &translated_spots,
//        &scale_rotate_translate
//    });
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200223_";
//    two_spots.writeToFile(511, path + "two_spots");
//    scaled_spots.writeToFile(511, path + "scaled_spots");
//    scale_then_rotate.writeToFile(511, path + "scale_then_rotate");
//    scale_rotate_translate.writeToFile(511, path + "scale_rotate_translate");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Demo for Blur, Feb 23, 2020
//    Color b(0, 0, 0);
//    Color w(1, 1, 1);
//    //    SoftMatte(Spot(Vec2(), 0.5, w, 0.6, b),
//    //              Grating(Vec2(), w, Vec2(0.2, 0), b, 0.01),
//    //              Grating(Vec2(), w, Vec2(0, 0.2), b, 0.01)).displayInWindow();
//
//    //    SoftMatte(Spot(Vec2(), 0.6, w, 0.7, b),
//    //              Grating(Vec2(), w, Vec2(0.2, 0), b, 0.01),
//    //              Blur(0.2,
//    //                   Grating(Vec2(), w, Vec2(0, 0.2), b, 0.01))).displayInWindow();
//
//    Spot spot(Vec2(), 0.6, w, 0.7, b);
//    Grating grating1(Vec2(), w, Vec2(0.2, 0), b, 0.01);
//    Grating grating2(Vec2(), w, Vec2(0, 0.2), b, 0.01);
//    Blur blur(0.2, grating2);
//    SoftMatte no_blur(spot, grating1, grating2);
//    SoftMatte vs_blur(spot, grating1, blur);

//    auto start_time = std::chrono::high_resolution_clock::now();
//    vs_blur.displayInWindow(511, false);
//    auto finish_time = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double> elapsed = finish_time - start_time;
//    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    
//    Texture::displayInWindow({ &no_blur, &vs_blur});
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200225_";
//    no_blur.writeToFile(511, path + "no_blur");
//    vs_blur.writeToFile(511, path + "vs_blur_50_ss");

//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200226_";
//    vs_blur.writeToFile(511, path + "vs_blur");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for Colorize, February 27, 2020
//    ColorNoise for_slice(0.1, Vec2(2, 2), 0.6);
//    Brownian to_color(0.4, Vec2(), Color(0, 0, 0), Color(1, 1, 1));
//    Colorize colorized(Vec2(0, 1), Vec2(), for_slice, to_color);
//
//    Texture::displayInWindow({ &for_slice, &to_color, &colorized });
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200227_";
//    for_slice.writeToFile(511, path + "for_slice");
//    to_color.writeToFile(511, path + "to_color");
//    colorized.writeToFile(511, path + "colorized");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for SoftThreshold, February 28, 2020
//    Noise grays(0.4, Vec2(), Color(0, 0, 0), Color(1, 1, 1));
//    ColorNoise colors(0.6, Vec2(), 0);
//    SoftThreshold threshold_grays(0.2, 0.7, grays);
//    SoftThreshold threshold_colors(0.4, 0.75, colors);
//    Brownian brownian(0.08, Vec2(), Color(0, 0, 0), Color(1, 1, 1));
//    SoftThreshold composition_book_cover(0.5, 0.55, brownian);
//    ColorNoise cbc(0.15, Vec2(), 0);
//    SoftThreshold color_book_cover(0.55, 0.6, cbc);
//    Texture::displayInWindow({ &grays, &threshold_grays,
//                               &colors, &threshold_colors,
//                               &composition_book_cover,
//                               &color_book_cover });
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200228_";
//    grays.writeToFile(511, path + "grays");
//    threshold_grays.writeToFile(511, path + "threshold_grays");
//    colors.writeToFile(511, path + "colors");
//    threshold_colors.writeToFile(511, path + "threshold_colors");
//    composition_book_cover.writeToFile(511, path + "composition_book_cover");
//    color_book_cover.writeToFile(511, path + "color_book_cover");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for EdgeDetect and EdgeEnhance, February 29, 2020
//    // This was revisited/reimplemented, see March 23, 2020 below
//
//    Noise gray_noise(0.3, Vec2(-2, 1), Color(0, 0, 0), Color(1, 1, 1));
//    SoftThreshold gray_threshold(0.5, 0.55, gray_noise);
//    Uniform light_gray(Color::gray(0.8));
//    Uniform dark_gray(Color::gray(0.2));
//    SoftMatte grays(gray_threshold, dark_gray, light_gray);
//    EdgeDetect grays_edge_detect(0.2, grays);
//    EdgeEnhance grays_edge_enhance(0.2, grays);
//
//    //Spot spot1(Vec2(+0.4, 0), 0.39, Color(0.8, 0.8, 0), 0.41, Color());
//    //Spot spot2(Vec2(-0.4, 0), 0.39, Color(0, 0.8, 0.8), 0.41, Color());
//    //Add two_spots(spot1, spot2);
//    //EdgeDetect edge_spots(0.5, two_spots);
//
//    Noise n0(0.2, Vec2(-2, +1), Color(), Color::gray(1));
//    Noise n1(0.2, Vec2(+1, +3), Color(), Color::gray(1));
//    Noise n2(0.2, Vec2(-1, -4), Color(), Color::gray(1));
//    Rotate r0(1, n0);
//    Rotate r1(3, n1);
//    Rotate r2(5, n2);
//    SoftThreshold st0(0.60, 0.64, r0);
//    SoftThreshold st1(0.60, 0.64, r1);
//    SoftThreshold st2(0.60, 0.64, r2);
//    Uniform gray50(Color::gray(0.5));
//    Uniform yellow(Color(0.8, 0.8, 0));
//    Uniform magenta(Color(0.8, 0, 0.8));
//    Uniform cyan(Color(0, 0.8, 0.8));
//    SoftMatte sm0(st0, gray50, yellow);
//    SoftMatte sm1(st1, sm0, magenta);
//    SoftMatte colors(st2, sm1, cyan);
//    EdgeDetect color_edge_detect(0.2, colors);
//    EdgeEnhance color_edge_enhance(0.2, colors);
//
//    Texture::displayInWindow({
//        &grays,
//        &grays_edge_detect,
//        &grays_edge_enhance,
//        &colors,
//        &color_edge_detect,
//        &color_edge_enhance
//    });
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200301_";
//    grays.writeToFile(511, path + "grays");
//    grays_edge_detect.writeToFile(511, path + "grays_edge_detect");
//    grays_edge_enhance.writeToFile(511, path + "grays_edge_enhance");
//    colors.writeToFile(511, path + "colors");
//    color_edge_detect.writeToFile(511, path + "color_edge_detect");
//    color_edge_enhance.writeToFile(511, path + "color_edge_enhance");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for AdjustHue, March 2, 2020
//    Gradation grad(Vec2(0, -1), Color(1, 1, 1), Vec2(0, 1), Color());
//    MobiusTransform warp(Vec2(0.24665, 1.44486),
//                         Vec2(-0.184825, 1.64791),
//                         Vec2(0.391668, -1.24418),
//                         Vec2(1.04597, -0.412046),
//                         grad);
//    BrightnessToHue color1(0.7, warp);
//    AdjustHue color2(0.5, color1);
//    Texture::displayInWindow({ &color1, &color2 });
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200302_";
//    color1.writeToFile(511, path + "color1");
//    color2.writeToFile(511, path + "color2");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for AdjustSaturation, March 3, 2020
//
//    ColorNoise cn(1, Vec2(2, 3), 0.8);
//    AdjustSaturation as1(0.3, cn);
//    AdjustSaturation as2(3.0, cn);
//    Texture::displayInWindow({ &cn, &as1, &as2, });
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200303_";
//    cn.writeToFile(511, path + "ColorNoise");
//    as1.writeToFile(511, path + "AdjustSaturation_down");
//    as2.writeToFile(511, path + "AdjustSaturation_up");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for AdjustBrightness, March 4, 2020
//
//    ColorNoise cn(1, Vec2(2, 3), 0.8);
//    AdjustBrightness ab_down(0.5, cn);
//    AdjustBrightness ab_up(2.0, cn);
//    Texture::displayInWindow({ &cn, &ab_down, &ab_up, });
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200304_";
//    ab_down.writeToFile(511, path + "AdjustBrightness_down");
//    ab_up.writeToFile(511, path + "AdjustBrightness_up");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Experimenting with copying Texture arguments to Operators, March 5, 2020
//
//    Color greenish(0.2, 0.8, 0.2);
//    Color bluish(0.2, 0.2, 0.8);
//    Color white(1, 1, 1);
//    Color gray10(0.1, 0.1, 0.1);
//
//    // The "no subobjects" approach:
//    Grating grate(Vec2(-0.1, -0.1), greenish, Vec2(0.1, 0.1), bluish, 0.5);
//    Furbulence furb(0.2, Vec2(3, 4), gray10, white);
//    Multiply mult(grate, furb);
//
//    // The problematic case: temp Generator objects are "destroyed" after the
//    // Operator is constructed, having saved references to the Generators.
//    Multiply trouble(Grating(Vec2(-0.1, -0.1), greenish,
//                             Vec2(+0.1, +0.1), bluish,
//                             0.5),
//                     Furbulence(0.2, Vec2(3, 4),
//                                gray10, white));
//    // The new way: allows writing an arbitrarily nested expression of TexSyn
//    // constructors, whose lifetime extends across display and file operations.
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200305_";
//    Texture::displayAndFile
//    (Grating(Vec2(-0.1, -0.1), greenish, Vec2(0.1, 0.1), bluish, 0.5),
//     path + "grating");
//    Texture::displayAndFile
//    (Furbulence(0.2, Vec2(3, 4), gray10, white),
//     path + "furbulence");
//    Texture::displayAndFile
//    (Multiply(Grating(Vec2(-0.1, -0.1), greenish, Vec2(0.1, 0.1), bluish, 0.5),
//              Furbulence(0.2, Vec2(3, 4), gray10, white)),
//     path + "fuzzy_blue_green_stripes");
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for Twist, March 6, 2020
//
//    Vec2 center(0.9, 0);
//    Color yellow9(0.9, 0.9, 0);
//    Color orange3(0.3, 0.15, 0);
//    Grating grating(Vec2(0, -0.1), orange3, Vec2(0, +0.1), yellow9, 0.3);
//    SliceToRadial radial(Vec2(0, 0.318), center, grating);
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200306_";
//    Texture::displayAndFile(radial, path + "radial");
//    Texture::displayAndFile(Twist(1, 1, center, radial), path + "Twist_1_1");
//    Texture::displayAndFile(Twist(1, 9, center, radial), path + "Twist_1_9");
//    Texture::displayAndFile(Twist(7, 1, center, radial), path + "Twist_7_1");
//    Texture::displayAndFile(Twist(7, 9, center, radial), path + "Twist_7_9");
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for BrightnessWrap, March 7, 2020
//
//    Brownian gray_noise(0.3, Vec2(-1, -3), Color(1, 1, 1), Color(0, 0, 0));
//    ColorNoise color_noise(0.5, Vec2(-1, -3), 0.6);
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200308_";
//    Texture::displayAndFile(gray_noise,
//                            path + "gray_noise");
//    Texture::displayAndFile(BrightnessWrap(0.4, 0.6, gray_noise),
//                            path + "gray_BrightnessWrap");
//    Texture::displayAndFile(color_noise,
//                            path + "color_noise");
//    Texture::displayAndFile(BrightnessWrap(0.4, 0.6, color_noise),
//                            path + "color_BrightnessWrap");
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for Mirror, March 9, 2020
//
//    Grating grating(Vec2(0.1, 0), Color(1, 1, 1),
//                    Vec2(0.3, 0), Color(0, 0, 0), 1);
//    Multiply grating_squared(grating, grating);
//    Brownian gray_noise(0.3, Vec2(-1, -3), Color(1, 1, 1), Color(0, 0, 0));
//    Colorize squiggles(Vec2(1, 0), Vec2(), grating_squared, gray_noise);
//
//    ColorNoise color_noise(0.5, Vec2(-1, -3), 0.6);
//    Multiply color_squiggles(color_noise, squiggles);
//    Uniform white(Color(1, 1, 1));
//    Subtract test(white, color_squiggles);
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200309_";
//    Texture::displayAndFile(squiggles, path + "squiggles");
//    Texture::displayAndFile(test, path + "test");
//    Texture::displayAndFile(Mirror(Vec2(0, 1), Vec2(), test),
//                            path + "Mirror_y");
//    Texture::displayAndFile(Mirror(Vec2(1, 0),
//                                   Vec2(),
//                                   Mirror(Vec2(0, 1), Vec2(), test)),
//                            path + "Mirror_y_then_x");
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for using "lambda combination" to bind nested texture definitions
//    // for the duration of the demo code, March 10, 2020
//    Grating grating(Vec2(0.1, 0), Color(1, 1, 1),
//                    Vec2(0.3, 0), Color(0, 0, 0), 1);
//    []
//    (const Texture& test)
//    {
//        Texture::displayAndFile(test);
//        Texture::displayAndFile(Mirror(Vec2(0, 1), Vec2(), test));
//        Texture::displayAndFile(Mirror(Vec2(1, 0),
//                                       Vec2(),
//                                       Mirror(Vec2(0, 1), Vec2(), test)));
//        Texture::waitKey();
//    }
//    (Subtract(Uniform(Color(1, 1, 1)),
//              Multiply(ColorNoise(0.5, Vec2(-1, -3), 0.6),
//                       Colorize(Vec2(1, 0),
//                                Vec2(),
//                                Multiply(grating, grating),
//                                Brownian(0.3, Vec2(-1, -3),
//                                         Color(1, 1, 1),
//                                         Color(0, 0, 0))))));

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for Ring, March 11, 2020
//    Color white(1, 1, 1);
//    Color black(0, 0, 0);
//    Spot spot(Vec2(0, -0.05), 0.19, white, 0.20, black);
//    [&]
//    (const Texture& bg)
//    {
//        [&]
//        (const Texture& spots_and_bg)
//        {
//            std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200311_";
//            Texture::displayAndFile(spots_and_bg, path + "spots_and_bg");
//            Texture::displayAndFile(Ring(10.1,
//                                         Vec2(0, 1),
//                                         Vec2(0, -0.9),
//                                         spots_and_bg),
//                                    path + "Ring_1");
//            Texture::displayAndFile(Ring(-4.8,
//                                         Vec2(1, 1),
//                                         Vec2(-1, -1).normalize() / 2,
//                                         spots_and_bg),
//                                    path + "Ring_2");
//            Texture::waitKey();
//        }
//        (SoftMatte(Translate(Vec2(0, 0.1), spot),
//                   SoftMatte(spot, bg, AdjustBrightness(0.8, bg)),
//                   Uniform(Color::gray(0.8))));
//    }
//    (AdjustBrightness(0.7,
//                      AdjustSaturation(0.1,
//                                       ColorNoise(0.5, Vec2(-1, -3),  0.6))));

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for Row, March 12, 2020
//    Color white(1, 1, 1);
//    Color black(0, 0, 0);
//    Spot spot(Vec2(0, -0.05), 0.19, white, 0.20, black);
//    [&]
//    (const Texture& bg)
//    {
//        [&]
//        (const Texture& spots_and_bg)
//        {
//            std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200312_";
//            Texture::displayAndFile(spots_and_bg);
//            Texture::displayAndFile(Row(Vec2(0.4, 0),
//                                        Vec2(),
//                                        spots_and_bg),
//                                    path + "row_1");
//            Texture::displayAndFile(Row(Vec2(0.35, 0.35),
//                                        Vec2(),
//                                        spots_and_bg),
//                                    path + "row_2");
//            Texture::displayAndFile(Row(Vec2(0, 0.6),
//                                        Vec2(),
//                                        spots_and_bg),
//                                    path + "row_3");
//            Texture::displayAndFile(Row(Vec2(0.17, 0.52),
//                                        Vec2(),
//                                        Row(Vec2(-0.52, 0.17),
//                                            Vec2(),
//                                            spots_and_bg)),
//                                    path + "row_4");
//            Texture::waitKey();
//        }
//        (SoftMatte(Translate(Vec2(0, 0.1), spot),
//                   SoftMatte(spot, bg, AdjustBrightness(0.8, bg)),
//                   Uniform(Color::gray(0.8))));
//    }
//    (AdjustBrightness(0.7,
//                      AdjustSaturation(0.1,
//                                       ColorNoise(0.5, Vec2(-1, -3),  0.6))));

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // playing with spots, March 12, 2020
//    Color white(1, 1, 1);
//    Color black(0, 0, 0);
//    Texture::displayAndFile
//    (SoftThreshold(0.5, 0.6, Noise(0.05, Vec2(5, 2), white, black)));
//    Texture::displayAndFile
//    (SoftThreshold(0.5, 0.6, Brownian(0.05, Vec2(5, 2), white, black)));
//    Texture::displayAndFile
//    (SoftThreshold(0.50,
//                   0.55,
//                   Blur(0.1,
//                        Brownian(0.05, Vec2(5, 2), white, black))));
//    Texture::displayAndFile
//    (SoftThreshold(0.7,
//                   0.8,
//                   Max(Rotate(2, Noise(0.1, Vec2(5, 2), white, black)),
//                       Rotate(4, Noise(0.1, Vec2(2, 5), white, black)))));
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
    // cache for Blur? -- March 13, 2020
    // note: see "chrono" above for timing
    //
    Color white(1, 1, 1);
    Color black(0, 0, 0);
    Color yellow(1, 1, 0);
    Color blue(0, 0, 1);
    [&]
    (const Texture& blur_vs_not)
    {
        ColorGridCache cgc;
        bool present_in_cache = false;
        
        debugPrint(cgc.cached(1, 1));
        cgc.insert(1, 1, Color(1,2,3));
//        debugPrint(cgc.insert(1, 1, Color(1,2,3)));
        debugPrint(cgc.cached(1, 1));
        debugPrint(present_in_cache);
        debugPrint(cgc.lookup(1, 1, present_in_cache));
        debugPrint(present_in_cache);

        
        
        
        
        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200317_";
        
//        {
//            Timer blur_timer("Running blur_vs_not");
//            Texture::displayAndFile(blur_vs_not);
//        }

        
//        Texture::displayAndFile(Grating(Vec2(), white,
//                                        Vec2(0.2, 0.2), black,
//                                        0.01)
//                                // , path + "grating"
//                                );
        {
            Timer blur_timer("Running Blur(Grating(...))");
            Texture::total_pixels_rendered = 0;
            Texture::total_pixels_cached = 0;
            Texture::total_cache_lookups = 0;
            Texture::cache_size = 0;
            
            Texture::displayAndFile(Blur(0.2,
//            Texture::displayAndFile(Blur(0.5,
                                         Grating(Vec2(), yellow,
                                                 Vec2(0.2, 0.2), blue,
                                                 0.01))
//                                     , path + "Blur_newer_yb_02"
//                                    , path + "Blur_newer_yb_05"
//                                    , path + "Blur_old_yb_05"
//                                    , path + "Blur_old_yb_02_10x10"
//                                    , path + "Blur_old_yb_02_11x11"
//                                    , path + "Blur_old_yb_02_15x15"
                                    );
            
            debugPrint(Texture::total_pixels_rendered);
            debugPrint(Texture::total_pixels_cached);
            debugPrint(Texture::total_cache_lookups);
            debugPrint(Texture::cache_size);
        }

        Texture::displayAndFile(EdgeDetect(0.2,
//        Texture::displayAndFile(EdgeDetect(0.5,
                                           Grating(Vec2(), yellow,
                                                   Vec2(0.2, 0.2), blue,
                                                   0.01))
//                                , path + "EdgeD_newer_yb_02"
//                                , path + "EdgeD_newer_yb_05"
//                                , path + "EdgeD_old_yb_02"
                                );
        
        Texture::waitKey();
    }
    (SoftMatte(Spot(Vec2(), 0.6, white, 0.7, black),
               Grating(Vec2(), white, Vec2(0.2, 0), black, 0.01),
               Blur(0.2,
                    Grating(Vec2(), white, Vec2(0, 0.2), black, 0.01))));
    
*/
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Demo for hashing and RandomSequence, March 19, 2020

//    debugPrint(hash_float(0.000000));
//    debugPrint(hash_float(0.000001));
//    debugPrint(hash_float(100000.0));
//    debugPrint(hash_float(1123.456));
//    debugPrint(hash_float(123456.7));
//
//    debugPrint(Vec2(0, 0).hash());
//    debugPrint(Vec2(5, 7).hash());
//    debugPrint(Vec2(7, 5).hash());
//    debugPrint(Vec2(-8, 3).hash());
//    debugPrint(Vec2(3, -8).hash());
//
//    RandomSequence rs; for (int i = 0; i < 20; i++) debugPrint(rs.nextInt());
//
//    Color yellow(1, 1, 0);
//    Color blue(0, 0, 1);
//    Texture::diff(Blur(0.2, Grating(Vec2(), yellow, Vec2(0.2, 0.2), blue, 0.01)),
//                  Blur(0.2, Grating(Vec2(), yellow, Vec2(0.2, 0.2), blue, 0.01)));
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Demo for SliceShear, started Jan 25, 2020, restarted March 20, 2020
//    Color white(1, 1, 1);
//    Color black(0, 0, 0);
//    Color orange(1, 0.5, 0);
//    Color cyan_blue(0, 0.5, 1);
//    Color gray = Color::gray(0.3);
//
//    // Horizontal stripes -- to be sheared.
//    Grating to_shear(Vec2(), orange, Vec2(0, 0.25), cyan_blue, 0.4);
//
//    // For slice: noise in one direction, square wave in other direction
//    Brownian b1(0.1, Vec2(), black, gray);
//    SliceGrating sg1(Vec2(1, 0), Vec2(), b1);
//    Grating g1(Vec2(), black, Vec2(0, 0.1), gray, 0.2);
//    Add for_slice(sg1, g1);
//
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200321_";
//    Texture::displayAndFile(for_slice, path + "for_slice");
//    Texture::displayAndFile(to_shear, path + "to_shear");
//    Texture::displayAndFile(SliceShear(Vec2(1, 0), Vec2(), for_slice,
//                                       Vec2(0, 1), Vec2(), to_shear),
//                            path + "SliceShear_1");
//    Texture::displayAndFile(SliceShear(Vec2(0, 1), Vec2(), for_slice,
//                                       Vec2(0, 1), Vec2(), to_shear),
//                            path + "SliceShear_2");
//    Texture::displayAndFile(SliceShear(Vec2(1, 0), Vec2(), for_slice,
//                                       Vec2(1, 1), Vec2(), to_shear),
//                            path + "SliceShear_3");
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Demo for adding "strength" parameter to EdgeEnhance, March 23, 2020
//    // (starting from code dated February 29, 2020 above)
//    Color b(0, 0, 0);
//    Color w(1, 1, 1);
//    [&]
//    (const Texture& grays,
//     const Texture& colors)
//    {
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200323_";
//
//        // Texture::displayAndFile(grays);
//        Texture::displayAndFile(colors,
//                                path + "colors");
//        // Texture::displayAndFile(EdgeDetect(0.2, grays));
//        // Texture::displayAndFile(EdgeEnhance(0.2, grays));
//        // Texture::displayAndFile(EdgeDetect(0.2, colors));
//
//        // Filter width=0.2, for three strengths:
//        Texture::displayAndFile(EdgeEnhance(0.2, 0.66, colors),
//                                path + "EdgeEnhance_02_066");
//        Texture::displayAndFile(EdgeEnhance(0.2, 1.00, colors),
//                                path + "EdgeEnhance_02_100");
//        Texture::displayAndFile(EdgeEnhance(0.2, 1.33, colors),
//                                path + "EdgeEnhance_02_133");
//
//        // Filter width=0.1, for three strengths:
//        Texture::displayAndFile(EdgeEnhance(0.1, 0.66, colors),
//                                path + "EdgeEnhance_01_066");
//        Texture::displayAndFile(EdgeEnhance(0.1, 1.00, colors),
//                                path + "EdgeEnhance_01_100");
//        Texture::displayAndFile(EdgeEnhance(0.1, 1.33, colors),
//                                path + "EdgeEnhance_01_133");
//    }
//    (// grays:
//     SoftMatte(SoftThreshold(0.5, 0.55, Noise(0.3, Vec2(-2, 1), b, w)),
//               Uniform(0.2),
//               Uniform(0.8)),
//     // colors:
//     SoftMatte(SoftThreshold(0.60, 0.64,
//                             Rotate(5, Noise(0.2, Vec2(-1, -4), b, w))),
//               SoftMatte(SoftThreshold(0.60, 0.64,
//                                       Rotate(3, Noise(0.2, Vec2(+1, +3), b, w))),
//                         SoftMatte(SoftThreshold
//                                      (0.60, 0.64,
//                                       Rotate(1, Noise(0.2, Vec2(-2, +1), b, w))),
//                                   Uniform(0.5),
//                                   Uniform(Color(0.8, 0.8, 0))),
//                         Uniform(Color(0.8, 0, 0.8))),
//               Uniform(Color(0, 0.8, 0.8))));
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // Demo of large "strength" parameter to EdgeEnhance, March 24, 2020

//    []
//    (const Texture& grayish_color_noise)
//    {
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200324_";
//        Texture::displayAndFile(grayish_color_noise,
//                                path + "grayish_color_noise");
//        Texture::displayAndFile(EdgeEnhance(0.1, 5, grayish_color_noise),
//                                path + "EdgeEnhance_01_5");
//    }
//    (AdjustBrightness(0.7,
//                      AdjustSaturation(0.3,
//                                       ColorNoise(0.5, Vec2(5,7), 0.8))));
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // Demo for Shade, March 25, 2020
    
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200325_";
//    Texture::displayAndFile(ShadedSphereTest(Vec3(0, 0, -1)));
//    Texture::displayAndFile(ShadedSphereTest(Vec3(0, 0, 1)));
//    Texture::displayAndFile(ShadedSphereTest(Vec3(-1, 0.8, 0.5)));
//    Texture::displayAndFile(ShadedSphereTest(Vec3(-1, 0, 0)));
//    Texture::displayAndFile(ShadedSphereTest(Vec3(-2, 2, 4))
//                            , path + "no_moon");
    
//    [&]
//    (const Texture& color, const Texture& bump)
//    {
//        Texture::displayAndFile(color
//                                , path + "green_stripes");
//        Texture::displayAndFile(bump
//                                , path + "hemisphere");
//        Texture::displayAndFile(Shader(Vec3(-2, 2, 4), 0, color, bump)
//                                , path + "sphere_stripes");
//    }
//    (Grating(Vec2(), Color(0, 1, 0), Vec2(0, 0.1), Color(1,1,1), 0.2),
//     ShadedSphereTest(Vec3(0, 0, 1)));

    
//    // Demo for Shade, March 25, 2020
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200326_";
//    ColorNoise color_noise(0.6, Vec2(), 0.2);
//    Texture::displayAndFile(color_noise, path + "color_noise");
//    auto terrain = [&](std::string name, const Texture& bump)
//    {
//        Texture::displayAndFile(bump, path + name);
//        Texture::displayAndFile(Shader(Vec3(1, 3, 6), 0.3, color_noise, bump),
//                                path + name + "_shaded");
//    };
//    terrain("Spot",
//            Spot(Vec2(), 0.3, Color(1, 1, 1), 0.95, Color(0, 0, 0)));
//    terrain("Noise",
//            Noise(0.1, Vec2(9, -5), Color(0, 0, 0), Color(1, 1, 1)));
//    terrain("Brownian",
//            Brownian(0.3, Vec2(9, -5), Color(0, 0, 0), Color(1, 1, 1)));
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//        // Looking at multi-threading (esp for Blur) -- March 27, 2020
//
//    //    Blur::sqrt_of_subsample_count = 60;
//    //    Blur::sqrt_of_subsample_count = 15;
//
//        Color yellow(1, 1, 0);
//        Color blue(0, 0, 1);
//        Grating grating(Vec2(), yellow, Vec2(0.2, 0.2), blue, 0.01);
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200329_";
//        Texture::displayAndFile(grating);
//        {
//            Timer timer("Blur of Grating");
//            Texture::displayAndFile(Blur(0.2, grating));
//        }
//        Texture::displayAndFile(Blur(0.5, grating));
//        Texture::displayAndFile(Spot(Vec2(0.5, 0.5),
//                                     0.0, Color(1,0,0),
//                                     0.6, Color(1,1,1)));
//
//    //    // Test cached file writing, plus rarely used bg_color and margin params
//    //    Spot spot(Vec2(0.5, 0.5), 0.0, Color(1,0,0), 0.6, Color(1,1,1));
//    //    spot.writeToFile(511,
//    //                     "/Users/cwr/Desktop/TexSyn_temp/foobar",
//    //                     Color(0, 0, 1),
//    //                     50);
//
//        Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Testing Andrew Willmott's "incremental Halton sequence"
//    int size = 511 * 2;
//    std::string window_name = "TexSyn";
//    cv::Mat opencv_image(size, size, CV_8UC3, cv::Scalar(127, 127, 127));
//
//    cHaltonSequence3 seq;
//    for (int i = 0; i < 300; i++)
//    {
//        printf("%d = (%g, %g, %g)\n", i, seq.mX, seq.mY, seq.mZ);
//        cv::Scalar a(50, 100, 200);
//        cv::Scalar b(100, 200, 50);
//        cv::Scalar c(200, 50, 100);
//        cv::circle(opencv_image,
//                   cv::Point(seq.mX * size, seq.mY * size),
//                   (i < 100 ? 20 : (i < 200 ? 10 : 5)),
//                   (i < 100 ? a : (i < 200 ? b : c)),
//                   cv::FILLED,
//                   cv::LINE_AA);
//        seq.inc();
//    }
//    cv::namedWindow(window_name);       // Create a window for display.
//    cv::imshow(window_name, opencv_image);  // Show our image inside it.
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200401_";
//    cv::imwrite(path + "Halton.png", opencv_image);
//
//    Texture::waitKey();
//
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // Testing LotsOfSpots prototype, April 2, 2020

//    float density = 0.7;
//    float rlo = 0.1;
//    float rhi = 1.0;
//    float soft = 0.1;

//    {
//        Timer timer("LotsOfSpots(density, rlo, rhi, soft)");
//        LotsOfSpots(density, rlo, rhi, soft);
//    }
    
//    Texture::displayAndFile(Scale(0.2, LotsOfSpots(density, rlo, rhi, soft)));
//    Texture::displayAndFile(Scale(0.13, LotsOfSpots(density, rlo, rhi, soft)));

    
//        LotsOfSpots lots_of_spots(density, rlo, rhi, soft);
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200403_";
//        Texture::displayAndFile(lots_of_spots);
//        Texture::displayAndFile(Scale(0.13, lots_of_spots));
//
//
//    //    Texture::displayAndFile(lots_of_spots, path + "LotsOfSpots");
//    //    Texture::displayAndFile(Scale(0.13, lots_of_spots),
//    //                            path + "LotsOfSpots_zoomed_out");
//    //    Texture::displayAndFile(MobiusTransform(Vec2(-0.958788, 1.64993),
//    //                                            Vec2(-1.54534, -0.593485),
//    //                                            Vec2(1.29155, -0.931471),
//    //                                            Vec2(0.768266, 0.24665),
//    //                                            Translate(Vec2(-0.75, -0.75),
//    //                                                      Scale(0.13,
//    //                                                            lots_of_spots))),
//    //                            path + "LotsOfSpots_Mobized");
    
    
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200405_";
//    //                                             den  min  max  soft
//    Texture::displayAndFile(Scale(0.2, LotsOfSpots(0.5, 0.1, .15, 0.1)),
//                            ""); // path + "LotsOfSpots_d1");
//    Texture::displayAndFile(Scale(0.2, LotsOfSpots(0.7, 0.1, 1.0, 0.1)),
//                            ""); // path + "LotsOfSpots_d2");
//    Texture::displayAndFile(Scale(0.2, LotsOfSpots(0.9, 0.1, 1.0, 0.1)),
//                            ""); // path + "LotsOfSpots_d3");

//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200406_";
//    Color c(0, 0.8, 1);
//    Color m(0.8, 0, 0.8);
//    //                                  den  min   max  soft  colors
//    Texture::displayAndFile(LotsOfSpots(0.7, 0.02, 0.2, 0.01, c, m),
//                            ""); // path + "LotsOfSpots_with_color");

//    Texture::displayAndFile(Scale(0.2, ColorNoise(1.2, Vec2(3, 4), 0.2)));
//        Texture::displayAndFile(Scale(0.2,
//                                      ColoredSpots(0.5, 0.1, .15, 0.1,
//    //                                               ColorNoise(1.2, Vec2(3, 4), 0.2),
//                                                   ColorNoise(2.5, Vec2(3, 4), 0.2),
//    //                                               Color())));
//                                                   Color::gray(0.5))));

//        Texture::displayAndFile(ColoredSpots(0.7, 0.02, 0.2, 0.01,
//    //                                         ColorNoise(1.2, Vec2(3, 4), 0.2),
//    //                                         ColorNoise(0.24, Vec2(3, 4), 0.2),
//                                             ColorNoise(0.5, Vec2(3, 4), 0.2),
//                                             Color::gray(0.4)));

//        Texture::displayAndFile(Scale(0.2, ColorNoise(2.5, Vec2(3, 4), 0.2)));
//        Texture::displayAndFile(Scale(0.2,
//    //                                  ColoredSpots(0.5, 0.1, .15, 0.1,
//                                      ColoredSpots(0.5, 0.2, 0.3, 0.05,
//                                                   ColorNoise(2.5, Vec2(3, 4), 0.2),
//                                                   Color::gray(0.3))));

//    ColorNoise cn0(2.5, Vec2(3, 4), 0.2);
//    AdjustSaturation cn(1.2, cn0);
//    Texture::displayAndFile(Scale(0.2, cn)
//                            ); //, path + "ColorNoise");
//    Texture::displayAndFile(Scale(0.2,
//                                  ColoredSpots(0.5, 0.2, 0.3, 0.05,
//                                               cn,
//                                               Color::gray(0.5)))
//                            ); //, path + "ColoredSpots");

//    // LotsOfButtons, Apr 6
//    Color gray2(0.2, 0.2, 0.2);
//    Color gray3(0.3, 0.3, 0.3);
//    Color red(1, 0, 0);
//    [&]
//    (const Texture& twist)
//    {
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200406_";
//        Texture::displayAndFile(twist
//                                , path + "Twist");
//        Texture::displayAndFile(LotsOfButtons(0.79, 0.1, 0.6, 0.05,
//                                              Vec2(), twist, gray2)
//                                , path + "LotsOfButtons_1");
//        Texture::displayAndFile(LotsOfButtons(0.6, 0.05, 0.25, 0.025,
//                                              Vec2(), twist, gray2)
//                                , path + "LotsOfButtons_2");
//    }
//    (Twist(7, 9, Vec2(),
//           SliceToRadial(Vec2(0, 0.318), Vec2(),
//                         Grating(Vec2(0, -0.1), red,
//                                 Vec2(0, +0.1), gray3,
//                                 0.3))));
    
    
//    // testing soft_edge_width change Apr 7
//    Texture::displayAndFile(LotsOfSpots(0.79, 0.1, 0.6, 0.5,
//                                        Color(1,1,1), Color()));
//    Texture::displayAndFile(LotsOfSpots(0.9, 0.05, 1, 0.05,
//                                        Color(1,1,1), Color()));

    
    // testing button_center Apr 7
//        [&]
//        (const Texture& quarters)
//        {
//            Texture::displayAndFile(quarters);
//            Texture::displayAndFile(LotsOfButtons(0.8, 0.05, 0.5, 0.02,
//                                                  Vec2(0.5, 0.5),
//                                                  quarters,
//    //                                              Color(0.5, 0.5, 0.5)));
//                                                  Color(0.8, 0.2, 0.4)));
//        }
//    //    (Add(Gradation(Vec2(0.50, 0.50), Color(0, 1, 0),
//    //                   Vec2(0.55, 0.55), Color()),
//    //         Gradation(Vec2(0.50, 0.50), Color(0, 0, 1),
//    //                   Vec2(0.55, 0.45), Color())));
//    //    (Add(Gradation(Vec2(0.50, 0.50), Color::gray(0.2),
//    //                   Vec2(0.51, 0.51), Color::gray(0.4)),
//    //         Gradation(Vec2(0.50, 0.50), Color::gray(0.2),
//    //                   Vec2(0.51, 0.49), Color::gray(0.4))));
//    //    (Add(Gradation(Vec2(0.49, 0.49), Color::gray(0.2),
//    //                   Vec2(0.51, 0.51), Color::gray(0.4)),
//    //         Gradation(Vec2(0.51, 0.49), Color::gray(0.2),
//    //                   Vec2(0.49, 0.51), Color::gray(0.4))));
//        (Add(Gradation(Vec2(0.49, 0.49), Color(0, 0.3, 0),
//                       Vec2(0.51, 0.51), Color(0, 0.8, 0)),
//             Gradation(Vec2(0.51, 0.49), Color(0, 0, 0.3),
//                       Vec2(0.49, 0.51), Color(0, 0, 0.8))));
    
//            [&]
//            (const Texture& quarters)
//            {
//                Texture::displayAndFile(quarters);
//    //            Texture::displayAndFile(LotsOfButtons(0.8, 0.05, 0.5, 0.02,
//    //                                                  Vec2(0.5, 0.5),
//    //                                                  quarters,
//    //                                                  Color(0.3, 0.3, 0.3)));
//                Texture::displayAndFile(LotsOfButtons(0.75, 0.04, 0.4, 0.02,
//                                                      Vec2(0.5, 0.5),
//                                                      quarters,
//                                                      Color(0.3, 0.3, 0.3)));
//            }
//            (Add(Gradation(Vec2(0.49, 0.49), Color(0, 0.4, 0),
//                           Vec2(0.51, 0.51), Color(0.4, 0.4, 0.4)),
//                 Gradation(Vec2(0.51, 0.49), Color(0, 0, 0.4),
//                           Vec2(0.49, 0.51), Color(0.4, 0.4, 0.4))));

/*
    Vec2 p1(-0.01, 0);
    Vec2 p2(+0.01, 0);
    float a3 = 2 * pi / 3;
    [&]
    (const Texture& sixths)
    {
        //std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200407_";
        Texture::displayAndFile(sixths
                                ); // , path + "sixths");
        Texture::displayAndFile(LotsOfButtons(0.8, 0.04, 0.4, 0.02,
                                              Vec2(0.5, 0.5),
                                              sixths,
                                              0, // no rotation
                                              Color(0.3, 0.3, 0.3))
                                ); // , path + "LotsOfButtons_offset");
        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200408_";
        Texture::displayAndFile(LotsOfButtons(0.8, 0.04, 0.4, 0.02,
                                              Vec2(0.5, 0.5),
                                              sixths,
                                              1, // random rotation
                                              Color(0.3, 0.3, 0.3)),
                                ""); // path + "LotsOfButtons_random_rotate");

    }
    (Translate(Vec2(0.5, 0.5),
               Add(Gradation(p1.rotate(a3 * 2), Color(0.1, 0.1, 0.3),
                             p2.rotate(a3 * 2), Color(0.3, 0.3, 0.3)),
                   Add(Gradation(p1.rotate(a3), Color(0.1, 0.3, 0.1),
                                 p2.rotate(a3), Color(0.3, 0.3, 0.3)),
                       Gradation(p1,            Color(0.3, 0.1, 0.1),
                                 p2,            Color(0.3, 0.3, 0.3))))));

    
    Texture::waitKey();
*/
    
//    DiskOccupancyGrid dog(10, 10);
//    DiskOccupancyGrid dog(Vec2(-5, -5), Vec2(5, 5), 10);
//    Disk disk(1, Vec2(3,3));
//    Disk disk(4, Vec2(1, 1));

//    for (int i = 3; i <= 4; i++)
//    {
//        for (int j = 3; j <= 4; j++)
//        {
//            dog.insertSingle(i, j, &disk);
//        }
//    }
    
    /*
    dog.insertDisk(disk);
    
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            std::cout << dog.getSetFromGrid(i, j)->size();
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    auto printDiskCountOfCell = [&](int i, int j)
    {
        std::cout << dog.getSetFromGrid(i, j)->size();
        if (j == 9) std::cout << std::endl;
    };
    dog.applyToCellsInRect(0, 0, 9, 9, printDiskCountOfCell);
    
    std::set<Disk*> disks;
    dog.findNearbyDisks(Disk(100, Vec2(3, 3)), disks);
    debugPrint(disks.size());
    debugPrint(*disks.begin());
    debugPrint(&disk);
    
    dog.eraseDisk(disk);
    dog.findNearbyDisks(Disk(100, Vec2(3, 3)), disks);
    dog.applyToCellsInRect(0, 0, 9, 9, printDiskCountOfCell);
    debugPrint(disks.size());
    */
    
//    Color white(1, 1, 1);
//    Color black(0, 0, 0);
//    Texture::displayAndFile(LotsOfSpots(0.8, 0.04, 0.4, 0.02, white, black));
    
//    Color c(0.0, 0.8, 1.0);
//    Color m(0.8, 0.0, 0.8);
//    Texture::displayAndFile(LotsOfSpots(0.7, 0.02, 0.2, 0.01, c, m));

//    Color c(0.0, 0.8, 1.0);
//    Color m(0.8, 0.0, 0.8);
//    Texture::displayAndFile(Scale(0.13,
//                                  LotsOfSpots(0.7, 0.02, 0.2, 0.01, c, m)));
    
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200412_";
//    Texture::displayAndFile(Scale(0.13,
//                                  LotsOfSpots(0.7, 0.02, 0.2, 0.01, c, m))
//                            ); // , path + "oops2");
//
//
//    Texture::waitKey();

    
    
//    // April 13, 2020
//    std::cout << "April 13, 2020" << std::endl;
//    DiskOccupancyGrid grid(Vec2(-5, -5), Vec2(5, 5), 10);
//    Disk d1(1, Vec2(3, 3));
//    Disk d2(2, Vec2(-3, -3));
//    grid.insertDiskWrap(d1);
//    grid.insertDiskWrap(d2);
//    grid.applyToCellsInRect(0, 0, 9, 9,
//                            [&](int i, int j)
//                            {
//                                std::cout << grid.getSetFromGrid(i, j)->size();
//                                if (j == 9) std::cout << std::endl;
//                            });


//    Texture::displayAndFile(Scale(0.13,
//                                  LotsOfSpots(0.7, 0.02, 0.2, 0.01, c, m))
//                            );
//    Texture::waitKey();


//    std::cout << "April 15, 2020" << std::endl;
//    DiskOccupancyGrid grid(Vec2(-5, -5), Vec2(5, 5), 10);
//    std::cout << "insert d1 -- Disk d1(2, Vec2(2.5, 2.5))" << std::endl;
//    Disk d1(2, Vec2(2.5, 2.5));
//    grid.insertDiskWrap(d1);
//    grid.printCellCounts();
//    std::cout << "insert d2 -- Disk d2(1, Vec2(2.5, 2.5))" << std::endl;
//    Disk d2(1, Vec2(2.5, 2.5));
//    grid.insertDiskWrap(d2);
//    grid.printCellCounts();
//    std::cout << "erase d1" << std::endl;
//    grid.eraseDiskWrap(d1);
//    grid.printCellCounts();
//    std::cout << "insert d3 -- Disk d3(1, Vec2(0, 0))" << std::endl;
//    Disk d3(1, Vec2(0, 0));
//    grid.insertDiskWrap(d3);
//    grid.printCellCounts();
//    std::cout << "erase d2" << std::endl;
//    grid.eraseDiskWrap(d2);
//    grid.printCellCounts();
//    std::cout << "erase d3" << std::endl;
//    grid.eraseDiskWrap(d3);
//    grid.printCellCounts();
//    std::cout << "insert d4 -- Disk d4(0, Vec2(2, 4))" << std::endl;
//    Disk d4(0, Vec2(2, 4));
//    grid.insertDiskWrap(d4);
//    grid.printCellCounts();

//    std::cout << "April 17, 2020" << std::endl;
//    LotsOfSpots los1(0.7, 0.02, 0.2, 0.01, c, m);
//    Texture::displayAndFile(Scale(0.13, los1));
//    Texture::displayAndFile(los1);
//    LotsOfSpots los2(0.8, 0.02, 0.4, 0.02, c, m);
//    Texture::displayAndFile(Scale(0.13, los2));
//    Texture::displayAndFile(los2);

//    Color c(0.0, 0.8, 1.0);
//    Color m(0.8, 0.0, 0.8);
//
//    // std::cout << "April 18, 2020" << std::endl;
//    std::cout << "April 19, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200419_";
//    {
//        Timer timer("LOS timing test suite");
//        LotsOfSpots los1(0.7, 0.02, 0.2, 0.01, c, m);
//        Texture::displayAndFile(Scale(0.13, los1)
//                                ); // , path + "los1_scaled");
//        Texture::displayAndFile(los1
//                                ); // , path + "los1");
//        LotsOfSpots los2(0.8, 0.02, 0.4, 0.02, c, m);
//        Texture::displayAndFile(Scale(0.13, los2)
//                                ); // , path + "los2_scaled");
//        Texture::displayAndFile(los2
//                                ); // , path + "los2");
//        LotsOfSpots los3(0.8, 0.2, 0.2, 0.02, c, m);
//        Texture::displayAndFile(Scale(0.13, los3)
//                                ); // , path + "los3_scaled");
//        Texture::displayAndFile(los3
//                                ); // , path + "los3");
//    }

//    // Do these render identically?
//    // ...Suprisingly they do, I expected pointer-ordering to randomise them.
//    // Perhaps allocating them into a std::vector orders the pointers?
//    Texture::displayAndFile(LotsOfSpots(0.8, 0.02, 0.4, 0.02, c, m));
//    Texture::displayAndFile(LotsOfSpots(0.8, 0.02, 0.4, 0.02, c, m));
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    std::cout << "April 26, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200426_";
//    ColorNoise cn(0.3, Vec2(5, 3), 0.6);
//    //Texture::displayAndFile(Gamma(0.50, cn) ); //, path + "Gamma_0_50");
//    //Texture::displayAndFile(Gamma(0.66, cn) ); //, path + "Gamma_0_66");
//    //Texture::displayAndFile(Gamma(1.00, cn) ); //, path + "Gamma_1_00");
//    //Texture::displayAndFile(Gamma(1.50, cn) ); //, path + "Gamma_1_50");
//    //Texture::displayAndFile(Gamma(2.00, cn) ); //, path + "Gamma_2_00");
//
//    // testing April 28, 2020
//    Texture::displayAndFile(Gamma(0.20, cn) );
//    Texture::displayAndFile(Gamma(0.40, cn) );
//    Texture::displayAndFile(Gamma(1.00, cn) );
//    Texture::displayAndFile(Gamma(2.50, cn) );
//    Texture::displayAndFile(Gamma(5.00, cn) );
//
//
//    //debugPrint(pow(-2, 1.5));
//    //Texture::displayAndFile(Gamma(-1, cn));
//    //Texture::displayAndFile(Gamma(0, cn));
//    //Texture::displayAndFile(Gamma(1, AdjustBrightness(-1, cn)));
//    //Texture::diff(cn, Gamma(1, cn));  // identical
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    std::cout << "April 27, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200427_";
//    Uniform red(1, 0, 0);
//    Uniform green(0, 1, 0);
//    Uniform magenta(1, 0, 1);
//    Spot spot(Vec2(), 0.1, Color(1, 1, 1), 0.5, Color(0, 0, 0));
//    //[&]
//    //(const Texture& two_spots)
//    //{
//    //    Texture::displayAndFile(two_spots);//,               path+"gamma_1_00");
//    //    Texture::displayAndFile(Gamma(1/2.2, two_spots));//, path+"gamma_0_45");
//    //}
//    //(SoftMatte(Translate(Vec2(0.5, 0), spot),
//    //           SoftMatte(Translate(Vec2(-0.5, 0), spot), green, red),
//    //           magenta));

//    Texture::displayAndFile(SoftMatte(Translate(Vec2(0.5, 0), spot),
//                                      SoftMatte(Translate(Vec2(-0.5, 0), spot),
//                                                green, red),
//                                      magenta));

    
//    path = "/Users/cwr/Desktop/TexSyn_temp/20200429_";
//    Texture::final_gamma = 1;
//    Texture::displayAndFile(SoftMatte(Translate(Vec2(0.5, 0), spot),
//                                      SoftMatte(Translate(Vec2(-0.5, 0), spot),
//                                                green, red),
//                                      magenta),
//                            path + "lin_interp_not_alpha");
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//        // Bring back test from March 27, 2020 for correct gamma during blur
//        std::cout << "May 1, 2020" << std::endl;
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200501_";
//        // keep this for doc
//        Uniform red(1, 0, 0);
//        Uniform green(0, 1, 0);
//        Uniform magenta(1, 0, 1);
//        Spot spot(Vec2(), 0.1, Color(1, 1, 1), 0.5, Color(0, 0, 0));
//        // Repeat the definition of two_spot to get around caching the bad version:
//
//        Texture::use_linear = false;
//        Texture::displayAndFile(SoftMatte(Translate(Vec2(0.5, 0), spot),
//                                          SoftMatte(Translate(Vec2(-0.5, 0), spot),
//                                                    green, red),
//                                          magenta)
//    //                            , path + "two_spot_before"
//                                );
//        Texture::use_linear = true;
//        Texture::displayAndFile(SoftMatte(Translate(Vec2(0.5, 0), spot),
//                                          SoftMatte(Translate(Vec2(-0.5, 0), spot),
//                                                    green, red),
//                                          magenta)
//    //                            , path + "two_spot_after"
//                                );
//
//        Color yellow(1, 1, 0);
//        Color blue(0, 0, 1);
//        Grating grating(Vec2(), yellow, Vec2(0.2, 0), blue, 0.01);
//        Texture::displayAndFile(grating
//    //                            , path + "Grating"
//                                );
//        Texture::use_linear = false;
//        Texture::displayAndFile(Blur(0.1, grating)
//    //                            , path + "Blur_Grating_before"
//                                );
//        Texture::use_linear = true;
//        Texture::displayAndFile(Blur(0.1, grating)
//    //                            , path + "Blur_Grating_after"
//                                );
//
//        // keep this for doc
//        Color r(1, 0, 0);
//        Color g(0, 1, 0);
//        Texture::use_linear = false;
//        Texture::displayAndFile(LotsOfSpots(0.9, 0.02, 0.5, 0.1, r, g)
//    //                            , path + "LotsOfSpots_before"
//                                );
//        Texture::use_linear = true;
//        Texture::displayAndFile(LotsOfSpots(0.9, 0.02, 0.5, 0.1, r, g)
//    //                            , path + "LotsOfSpots_after"
//                                );
//
//    //    // May 1, 2020 -- these look identical -- LotsOfButtons random rot
//    //    // regression reality check from April 7
//
//    //    // May 1, 2020 -- these look identical -- "test" texture before and after.
//    //    // Demo for Mirror, March 9, 2020
//
//        Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // May 5, 2020 follow up on "Gamma angst: shouldn’t TexSyn use linear RGB?"
//    std::cout << "May 5, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200505_";
//    Color black(0, 0, 0);
//    Color white(1, 1, 1);
//    Color red(1, 0, 0);
//    Color green(0, 1, 0);
//    Color cyan(0, 1, 1);
//    Color orange(1, 0.8, 0);
//    [&]
//    (const Texture& stripes_rg,
//     const Texture& stripes_gr,
//     const Texture& stripes_co,
//     const Texture& brownian,
//     const Texture& color_noise,
//     const Texture& soft_matte)
//    {
////        Texture::displayAndFile(stripes_rg,                path + "lin_gam_a");
////        Texture::displayAndFile(Gamma(1/2.2, stripes_rg),  path + "lin_gam_b");
////        Texture::displayAndFile(stripes_gr,                path + "lin_gam_c");
////        Texture::displayAndFile(Gamma(1/2.2, stripes_gr),  path + "lin_gam_d");
////        Texture::displayAndFile(stripes_co,                path + "lin_gam_e");
////        Texture::displayAndFile(Gamma(1/2.2, stripes_co),  path + "lin_gam_f");
////        Texture::displayAndFile(brownian,                  path + "lin_gam_g");
////        Texture::displayAndFile(Gamma(1/2.2, brownian),    path + "lin_gam_h");
////        Texture::displayAndFile(color_noise,               path + "lin_gam_i");
////        Texture::displayAndFile(Gamma(1/2.2, color_noise), path + "lin_gam_j");
////        Texture::displayAndFile(soft_matte,                path + "lin_gam_k");
////        Texture::displayAndFile(Gamma(1/2.2, soft_matte),  path + "lin_gam_l");
//        Texture::displayAndFile(stripes_rg);
//        Texture::displayAndFile(stripes_gr);
//        Texture::displayAndFile(stripes_co);
//        Texture::displayAndFile(brownian);
//        Texture::displayAndFile(color_noise);
//        Texture::displayAndFile(soft_matte);
//    }
//    (Grating(Vec2(-0.2, 0), red, Vec2(0.2, 0), green, 1),
//     Grating(Vec2(-0.2, 0), green, Vec2(0.2, 0), red, 1),
//     Grating(Vec2(0, -0.2), cyan, Vec2(0, 0.2), orange, 1),
//     Brownian(0.2, Vec2(), red, green),
//     ColorNoise(0.15, Vec2(3, -5), 0),
//     SoftMatte(Spot(Vec2(), 0.2, white, 1, black),
//               Grating(Vec2(-0.2, 0), red, Vec2(0.2, 0), green, 1),
//               Grating(Vec2(0, -0.2), cyan, Vec2(0, 0.2), orange, 1)));
//    debugPrint(Color(0, 1, 1).luminance());
//    debugPrint(Color(1, 0.8, 0).luminance());
//
//
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // After gamma switch, May 7, 2020
//    std::cout << "May 7, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200507_";
//
//    // two_spot
//    Uniform red(1, 0, 0);
//    Uniform green(0, 1, 0);
//    Uniform magenta(1, 0, 1);
//    Spot spot(Vec2(), 0.1, Color(1, 1, 1), 0.5, Color(0, 0, 0));
//    Texture::displayAndFile(SoftMatte(Translate(Vec2(0.5, 0), spot),
//                                      SoftMatte(Translate(Vec2(-0.5, 0), spot),
//                                                green, red),
//                                      magenta)
//                            ); // , path + "two_spots");
//    // original
//    //float m = 0.1;
//    //float n = 0.3;
//    //float o = 0.3;
//    // revised for linear RGB:
//    float m = 0.005;
//    float n = 0.25;
//    float o = 0.07;
//
//    Vec2 p1(-0.01, 0);
//    Vec2 p2(+0.01, 0);
//    float a3 = 2 * pi / 3;
//    [&]
//    (const Texture& sixths)
//    {
//        Texture::displayAndFile(LotsOfButtons(0.8, 0.04, 0.4, 0.02,
//                                              Vec2(0.5, 0.5),
//                                              sixths,
//                                              1, // random rotation
//                                              Color(o, o, o))
//                              //, path + "LOB_original_code");
//                              //, path + "LOB_tweaked_code");
//                                );
//    }
//    (Translate(Vec2(0.5, 0.5),
//               Add(Gradation(p1.rotate(a3 * 2), Color(m, m, n),
//                             p2.rotate(a3 * 2), Color(n, n, n)),
//                   Add(Gradation(p1.rotate(a3), Color(m, n, m),
//                                 p2.rotate(a3), Color(n, n, n)),
//                       Gradation(p1,            Color(n, m, m),
//                                 p2,            Color(n, n, n))))));
//
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Duty cycle for Grating, May 8, 2020
//    std::cout << "May 10, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200510_";
//    Color c1(0, 0, 0);
//    Color c2(0, 1, 0);
//    Vec2 p1(-0.2, -0.05);
//    Vec2 p2(0.2, 0.05);
//    float soft = 1;
//    float hard = 0.2;
//
//    //    // old
//    //    Texture::displayAndFile(Grating(p1, c1, p2, c2, soft)
//    //                            ); // , path + "old_soft");
//    //    Texture::displayAndFile(Grating(p1, c1, p2, c2, hard)
//    //                            ); // , path + "old_hard");
//    // new
//    Texture::displayAndFile(Grating(p1, c1, p2, c2, soft, 0.8)
//                            ); // , path + "new_soft_0_8");
//    Texture::displayAndFile(Grating(p1, c1, p2, c2, soft, 0.5)
//                            ); // , path + "new_soft_0_5");
//    Texture::displayAndFile(Grating(p1, c1, p2, c2, soft, 0.2)
//                            ); // , path + "new_soft_0_2");
//    Texture::displayAndFile(Grating(p1, c1, p2, c2, hard, 0.8)
//                            ); // , path + "new_hard_0_8");
//    Texture::displayAndFile(Grating(p1, c1, p2, c2, hard, 0.5)
//                            ); // , path + "new_hard_0_5");
//    Texture::displayAndFile(Grating(p1, c1, p2, c2, hard, 0.2)
//                            ); // , path + "new_hard_0_2");
//
//    path = "/Users/cwr/Desktop/TexSyn_temp/20200511_";
//    Grating g(Vec2(), Color(1, 1, 1),
//              Vec2(0.04, 0.04), Color(0, 0, 1),
//              0.4, 0.8);
//    Noise n(0.3, Vec2(2, 7), Color(), Color(1, 1, 1));
//    Texture::displayAndFile(g); // , path + "Grating");
//    Texture::displayAndFile(Colorize(Vec2(1, 0), Vec2(), g,  n)
//                            ); // , path + "Colorize");
//
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Add margin to LotsOfSpotsBase, May 13, 2020
//    std::cout << "May 13, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200513_";
//    Color c1(0.7, 0.7, 0.0); // yellow
//    Color c2(0.9, 0.0, 0.0); // red
//    Noise noise(0.03, Vec2(), c1, c2);
//    Texture::displayAndFile(LotsOfSpots(0.8, 0.1, 0.3, 0.01, 0, c1, c2)
//                            ); // , path + "LOS_margin_0");
//    Texture::displayAndFile(LotsOfSpots(0.8, 0.1, 0.3, 0.01, 0.02, c1, c2)
//                            ); // , path + "LOS_margin_0_02");
//    Texture::displayAndFile(LotsOfSpots(0.8, 0.1, 0.3, 0.01, 0.04, c1, c2)
//                            ); // , path + "LOS_margin_0_04");
//    Texture::displayAndFile(ColoredSpots(0.8, 0.1, 0.3, 0.01, 0.04, noise, c2)
//                            ); // , path + "CS_margin_0_04");
//    Texture::displayAndFile(LotsOfButtons(0.8, 0.1, 0.3, 0.01, 0.04,
//                                          Vec2(), noise, 1, c2)
//                            ); // , path + "LOB_margin_0_04");
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Inherent matting, May 14, 2020
//    std::cout << "May 14, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200514_";
//
//    Color black(0, 0, 0);
//    Color white(1, 1, 1);
//
//    Grating white_cyan(Vec2(0, 0.2), white, Vec2(), Color(0, 1, 1), 0.1, 0.5);
//    Grating black_red(Vec2(0.1, 0), black, Vec2(), Color(1, 0, 0), 0.1, 0.5);
//
//    Texture::displayAndFile(LotsOfSpots(0.9, 0.1, 0.4, 0.08, 0.03,
//                                        Uniform(white),
//                                        Uniform(black))
//                            ); // , path + "LotsOfSpots_b_and_w");
//
//    Texture::displayAndFile(SoftMatte(LotsOfSpots(0.9, 0.1, 0.4, 0.08, 0.03,
//                                                  Uniform(white),
//                                                  Uniform(black)),
//                                      Uniform(Color(0.1, 0.1, 0.1)),
//                                      Uniform(Color(0.1, 0.6, 0.1)))
//                            ); // , path + "SoftMatte_of_Uniforms");
//
//    Texture::displayAndFile(SoftMatte(LotsOfSpots(0.9, 0.1, 0.4, 0.08, 0.03,
//                                                  Uniform(white),
//                                                  Uniform(black)),
//                                      white_cyan, black_red)
//                            ); // , path + "striped_spots");
//
//    Texture::displayAndFile(LotsOfSpots(0.9, 0.1, 0.4, 0.08, 0.03,
//                                        white_cyan, black_red)
//                            ); // , path + "striped_spots_new");
//
//    // Copy twist texture from https://cwreynolds.github.io/TexSyn/#20200406c
//    [&]
//    (const Texture& twist)
//    {
//        Texture::displayAndFile(LotsOfButtons(0.9, 0.1, 0.4, 0.08, 0.03,
//                                              Vec2(), twist, 1, white_cyan)
//                                ); // , path + "LotsOfButtons");
//    }
//    (Twist(7, 9, Vec2(),
//           SliceToRadial(Vec2(0, 0.318), Vec2(),
//                         Grating(Vec2(0, -0.1), Color(1, 0, 0),
//                                 Vec2(0, +0.1), black,
//                                 0.3, 0.5))));
//
//    Noise n1(0.3, Vec2(-3,2), Color(1, 1, 0), Color(0, 1, 0));
//    Noise n2(0.05, Vec2(3, 7), Color(0, 0, 1), Color(0.4, 0.4, 0.7));
//    Texture::displayAndFile(ColoredSpots(0.6, 0.05, 0.1, 0.03, 0.01, n1, n2)
//                            ); // , path + "ColoredSpots");
//
//    // testing BACKWARD_COMPATIBILITY May 15, 2020
//    ColorNoise cn0(2.5, Vec2(3, 4), 0.2);
//    AdjustSaturation cn(1.2, cn0);
//    Color gray50(0.25, 0.25, 0.25);
//    Texture::displayAndFile(Scale(0.2,
//                                  ColoredSpots(0.5, 0.2, 0.3, 0.05,
//                                               cn, gray50)));
//    [&]
//    (const Texture& twist)
//    {
//        Texture::displayAndFile(LotsOfButtons(0.79, 0.1, 0.6, 0.05,
//                                              Vec2(), twist, 1, Color()));
//    }
//    (Twist(7, 9, Vec2(),
//           SliceToRadial(Vec2(0, 0.318), Vec2(),
//                         Grating(Vec2(0, -0.1), Color(1, 0, 0),
//                                 Vec2(0, +0.1), black,
//                                 0.3, 0.5))));
//    Color c(0.0, 0.8, 1.0);
//    Color m(0.8, 0.0, 0.8);
//    Texture::displayAndFile(LotsOfSpots(0.7, 0.02, 0.2, 0.01, c, m));
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Inherent matting, part 2, May 17, 2020
//    std::cout << "May 17, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200517_";
//
//    Uniform black(Color(0, 0, 0));
//    Uniform white(Color(1, 1, 1));
//    Uniform red(Color(1, 0, 0));
//    Uniform cyan(Color(0, 1, 1));
//    Grating white_cyan(Vec2(0, 0.2), white, Vec2(), cyan, 0.1, 0.5);
//    Grating black_red(Vec2(0.1, 0), black, Vec2(), red, 0.1, 0.5);
//
//    Texture::displayAndFile(Spot(Vec2(), 0.2, white_cyan, 0.9, black_red)
//                            , path + "Spot");
//    Texture::displayAndFile(Gradation(Vec2(0.7, -0.7), white_cyan,
//                                      Vec2(-0.7, 0.7), black_red)
//                            , path + "Gradation");
//    Texture::displayAndFile(Grating(Vec2(0.1, 0), white_cyan,
//                                    Vec2(-0.1, 0), black_red,
//                                    0.2, 0.5)
//                            , path + "Grating_oops");
//    Texture::displayAndFile(Grating(Vec2(0.1, -0.2), white_cyan,
//                                    Vec2(-0.1, 0.2), black_red,
//                                    0.2, 0.3)
//                            , path + "Grating");
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // deGamma() for pattern generators? -- May 18, 2020
//    std::cout << "May 18, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200518_";
//
//    Uniform black(Color(0, 0, 0));
//    Uniform white(Color(1, 1, 1));
//
//    Texture::diff(Spot (Vec2(), 0.1, white, 1.0, black),
//                  Spot2(Vec2(), 0.1, white, 1.0, black)
//                  ); // , path + "Spot_diff");
//    Texture::diff(Gradation (Vec2(0.6, 0.6), black, Vec2(-0.6, -0.6), white),
//                  Gradation2(Vec2(0.6, 0.6), black, Vec2(-0.6, -0.6), white)
//                  ); // , path + "Gradation_diff");
//
//    // 11am: definitely looks "better" in that the gray transition seems better
//    //       aligned by the geometry of the transition, middle gray near center
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // clean up displayAndFile3() and windowPlacementTool() -- May 20, 2020
//    std::cout << "May 20, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200520_";
//
//    Uniform black(Color(0, 0, 0));
//    Uniform white(Color(1, 1, 1));
//
//    Texture::diff(Spot(Vec2(), 0.1, white, 1.0, black),
//                  Gradation(Vec2(0.6, 0.6), black, Vec2(-0.6, -0.6), white));
//    Texture::diff(Noise(0.2, Vec2(), Color(1, 0, 0), Color(1, 1, 0)),
//                  Grating(Vec2(), Color(0, 1, 1),
//                          Vec2(0.1, 0.1), Color(0, 0, 1), 0.5));
//    Texture::displayAndFile(Wrapulence(0.2, Vec2(7, 9),
//                                       Color(1, 0, 0), Color(0.8, 1, 0)));
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Inherent matting and gamma for noise textures -- May 20, 2020
//    std::cout << "May 20, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200520_";
//
//    Uniform black(Color(0, 0, 0));
//    Uniform white(Color(1, 1, 1));
//    Uniform red(Color(1, 0, 0));
//    Uniform cyan(Color(0, 1, 1));
//    Grating white_cyan(Vec2(0, 0.2), white, Vec2(), cyan, 0.1, 0.5);
//    Grating black_red(Vec2(0.1, 0), black, Vec2(), red, 0.1, 0.5);
//
//    Texture::diff(Noise (0.2, Vec2(), Color(1, 0, 0), Color(1, 1, 0)),
//                  Noise2(0.2, Vec2(), Color(1, 0, 0), Color(1, 1, 0)));
//    Texture::diff(Noise (0.2, Vec2(), Color(1, 1, 0), Color(1, 0, 0)),
//                  Noise2(0.2, Vec2(), Color(1, 1, 0), Color(1, 0, 0)));
//    Texture::diff(Noise (0.2, Vec2(), white_cyan, black_red),
//                  Noise2(0.2, Vec2(), white_cyan, black_red));
//    Texture::diff(Noise (0.2, Vec2(), black, white),
//                  Noise2(0.2, Vec2(), black, white));
//
//    Texture::displayAndFile(Noise(0.2, Vec2(), white_cyan, black_red)
//                            ); // , path + "inherent_matting");
//    Texture::displayAndFile(Noise2(0.2, Vec2(), white_cyan, black_red)
//                            ); // , path + "inherent_matting_gamma");
//    Texture::displayAndFile(SoftMatte(SoftThreshold(0.2,
//                                                    0.6,
//                                                    Noise(0.2,
//                                                          Vec2(-5, 3),
//                                                          white,
//                                                          black)),
//                                      white_cyan,
//                                      black_red)
//                            ); // , path + "in_my_mind");
//
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // gamma and noise -- May 21, 2020
//    std::cout << "May 20, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200521_";
//
//    Uniform black(Color(0, 0, 0));
//    Uniform white(Color(1, 1, 1));
//    Uniform red(Color(1, 0, 0));
//    Uniform cyan(Color(0, 1, 1));
//    Grating white_cyan(Vec2(0, 0.2), white, Vec2(), cyan, 0.1, 0.5);
//    Grating black_red(Vec2(0.1, 0), black, Vec2(), red, 0.1, 0.5);
//
//    Texture::displayAndFile(Noise(0.1, Vec2(), white, black));
//    Texture::displayAndFile(Noise(0.1, Vec2(), white_cyan, black_red));
//    setDefaultGamma(1);
//    Texture::displayAndFile(Noise(0.1, Vec2(), white, black));
//    Texture::displayAndFile(Noise(0.1, Vec2(), white_cyan, black_red)
//                            ); // , path + "Noise_1_0_im");

//    // for histogram test
//    setDefaultGamma(1);
//    Texture::displayAndFile(Noise(0.1, Vec2(), white, black)
//                            ); // , path + "Noise_1_0");
//    setDefaultGamma(2.2);
//    Texture::displayAndFile(Noise(0.1, Vec2(), white, black)
//                            ); // , path + "Noise_2_2");
//    Texture::displayAndFile(Noise2(0.1, Vec2(), white, black)
//                            ); // , path + "Noise_2_2_dg");
//    for (int i = 0; i < Texture::histogram.size(); i++)
//        std::cout << i << "," << Texture::histogram.at(i) << std::endl;

    
//    setDefaultGamma(2.2);
//    Texture::displayAndFile(Noise(0.1, Vec2(), white_cyan, black_red)
//                            , path + "Noise_2_2_im");
//    Texture::displayAndFile(Noise2(0.1, Vec2(), white_cyan, black_red)
//                            , path + "Noise2_2_2_im");

//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Noise range and gamma -- May 22, 2020
//    std::cout << "May 22, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200522_";
//
//    Uniform black(Color(0, 0, 0));
//    Uniform white(Color(1, 1, 1));
//    Uniform red(Color(1, 0, 0));
//    Uniform cyan(Color(0, 1, 1));
//    Grating white_cyan(Vec2(0, 0.2), white, Vec2(), cyan, 0.1, 0.5);
//    Grating black_red(Vec2(0.1, 0), black, Vec2(), red, 0.1, 0.5);
//
//    setDefaultGamma(1);
//    Texture::displayAndFile(Noise(0.1, Vec2(), black, white)
//                            ); // , path + "Noise_g_1_0_gray");
//
//    //setDefaultGamma(2.2);
//    //Texture::displayAndFile(Noise(0.1, Vec2(), black, white));
//    //for (int i = 0; i < Texture::histogram.size(); i++)
//    //    std::cout << i << "," << Texture::histogram.at(i) << std::endl;
//
//    setDefaultGamma(2.2);
//    Texture::displayAndFile(Noise(0.1, Vec2(), black, white)
//                            ); // , path + "Noise_g_2_2_gray");
//    Texture::displayAndFile(Noise(0.1, Vec2(), black_red, white_cyan)
//                            ); // , path + "Noise_g_2_2_im");
//    Texture::displayAndFile(Noise(0.1, Vec2(), Uniform(1, 1, 0), red)
//                            ); // , path + "Noise_g_2_2_colors");
//
//    //Texture::displayAndFile(Noise(0.1, Vec2(),
//    //                              Uniform(1, 0, 0), Uniform(0, 1, 0)));
//    //
//    //Texture::displayAndFile(Brownian(0.3, Vec2(),
//    //                                 Color(1, 1, 1), Color(0, 0, 0)));
//    //Texture::displayAndFile(SoftMatte(Brownian(0.3, Vec2(),
//    //                                           Color(1, 1, 1), Color(0, 0, 0)),
//    //                                  white_cyan,
//    //                                  black_red));
//    //Texture::displayAndFile(SoftMatte(Brownian(0.3, Vec2(),
//    //                                           Color(1, 1, 1), Color(0, 0, 0)),
//    //                                  Uniform(1, 1, 0),
//    //                                  Uniform(1, 0, 0)));
//    //Texture::displayAndFile(SoftMatte(Furbulence(0.3, Vec2(),
//    //                                             Color(1, 1, 1),
//    //                                             Color(0, 0, 0)),
//    //                                  Uniform(1, 1, 0),
//    //                                  Uniform(1, 0, 0)));
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // inherent matting for rest of noise textures -- May 24, 2020
//    std::cout << "May 24, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200524_";
//
//    Uniform black(Color(0, 0, 0));
//    Uniform white(Color(1, 1, 1));
//    Uniform red(Color(1, 0, 0));
//    Uniform cyan(Color(0, 1, 1));
//    Uniform yellow(Color(1, 1, 0));
//    Grating white_cyan(Vec2(0, 0.2), white, Vec2(), cyan, 0.1, 0.5);
//    Grating black_red(Vec2(0.1, 0), black, Vec2(), red, 0.1, 0.5);
//
//    Texture::displayAndFile(Noise(0.3, Vec2(), black_red, white_cyan)
//                            ); // , path + "Noise_stripes");
//    Texture::displayAndFile(Noise(0.3, Vec2(), yellow, red)
//                            ); // , path + "Noise_y_r");
//    Texture::displayAndFile(Brownian(0.3, Vec2(), black_red, white_cyan)
//                            ); // , path + "Brownian_stripes");
//    Texture::displayAndFile(Brownian(0.3, Vec2(), yellow, red)
//                            ); // , path + "Brownian_y_r");
//    Texture::displayAndFile(Turbulence(0.3, Vec2(), black_red, white_cyan)
//                            ); // , path + "Turbulence_stripes");
//    Texture::displayAndFile(Turbulence(0.3, Vec2(), yellow, red)
//                            ); // , path + "Turbulence_y_r");
//    Texture::displayAndFile(Furbulence(0.3, Vec2(), black_red, white_cyan)
//                            ); // , path + "Furbulence_stripes");
//    Texture::displayAndFile(Furbulence(0.3, Vec2(), yellow, red)
//                            ); // , path + "Furbulence_y_r");
//    Texture::displayAndFile(Wrapulence(0.3, Vec2(), black_red, white_cyan)
//                            ); // , path + "Wrapulence_stripes");
//    Texture::displayAndFile(Wrapulence(0.3, Vec2(), yellow, red)
//                            ); // , path + "Wrapulence_y_r");
//
//    Uniform blue(Color(0, 0, 1));
//    Texture::displayAndFile(Noise(0.2, Vec2(1, 2),
//                                  Furbulence(0.1, Vec2(3, -2), yellow, red),
//                                  Brownian(0.1, Vec2(-1, 5), blue, cyan))
//                            ); // , path + "less_trivial");
//
//    //    Texture::displayAndFile(MultiNoise(0.5, Vec2(), red, white, 0.0));
//    //    Texture::displayAndFile(MultiNoise(0.5, Vec2(), red, white, 0.2));
//    //    Texture::displayAndFile(MultiNoise(0.5, Vec2(), red, white, 0.4));
//    //    Texture::displayAndFile(MultiNoise(0.5, Vec2(), red, white, 0.6));
//    //    Texture::displayAndFile(MultiNoise(0.5, Vec2(), red, white, 0.8));
//
//    //    Texture::displayAndFile(ColorNoise(0.5, Vec2(), 0.0));
//    //    Texture::displayAndFile(ColorNoise(0.5, Vec2(), 0.2));
//    //    Texture::displayAndFile(ColorNoise(0.5, Vec2(), 0.4));
//    //    Texture::displayAndFile(ColorNoise(0.5, Vec2(), 0.6));
//    //    Texture::displayAndFile(ColorNoise(0.5, Vec2(), 0.8));
//
//    //    Texture::displayAndFile(Turbulence(0.3, Vec2(), black, white));
//    //    for (int i = 0; i < Texture::histogram.size(); i++)
//    //        std::cout << i << "," << Texture::histogram.at(i) << std::endl;
//
//    //    // Make histogram for 100 luminance levels, on [-1, 1] to test
//    //    // distribution of values from PerlinNoise::noise2d()
//    //    std::vector<int> histogram(100, 0);
//    //    RandomSequence rs(94548558);
//    //    for (int i = 0; i < 10000000; i++)
//    //    {
//    //        Vec2 p = rs.randomPointInUnitDiameterCircle() * 10;
//    //        float noise = PerlinNoise::noise2d(p);
//    //        assert(between(noise, -1, 1));
//    //        histogram.at((noise + 1) * 0.5 * (histogram.size() - 1))++;
//    //    }
//    //    for (int i = 0; i < histogram.size(); i++)
//    //    {
//    //        float x = interpolate(i * 0.01, -1.0, +1.0);
//    //        std::cout << x << "," << histogram.at(i) << std::endl;
//    //    }
//
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Switch back to row-by-row parallelism -- May 25, 2020
//    std::cout << "May 25, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200525_";
//
//    Uniform blue(0, 0, 1);
//    Uniform yellow(1, 1, 0);
//    Grating grating(Vec2(), yellow, Vec2(0.2, 0), blue, 0.01, 0.5);
//
//    //Texture::displayAndFile(grating);
//    //Texture::displayAndFile(Blur(0.1, grating));
//    //Blur::sqrt_of_subsample_count = 20;
//    //Texture::displayAndFile(Blur(0.1, grating));
//    //Texture::displayAndFile(Blur(0.2, grating));
//    //Texture::displayAndFile(Blur(0.4, grating));
//    ////Blur::sqrt_of_subsample_count = 30;
//    //Blur::sqrt_of_subsample_count = 11;
//    //Texture::displayAndFile(Blur(0.4, grating));
//
//    Texture::displayAndFile(grating           ); // , path + "grating");
//    Texture::displayAndFile(Blur(0.1, grating)); // , path + "Blur_0_1");
//    Texture::displayAndFile(Blur(0.2, grating)); // , path + "Blur_0_2");
//    Texture::displayAndFile(Blur(0.4, grating)); // , path + "Blur_0_4");
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        
//    // anti-aliasing -- May 26, 2020
//    std::cout << "May 26, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200526_";
//
//    float f = 1;
//    // float d = 0.3; // note this change, compensate for gamma change
//    float d = 0.1;    // note this change, compensate for gamma change
//    Uniform full_green(Color(f, 0, 0));
//    Uniform dark_green(Color(d, 0, 0));
//    Uniform full_red(Color(0, f, 0));
//    Uniform dark_red(Color(0, d, 0));
//    Grating red_stripes(Vec2(0, 0), full_red,
//                        Vec2(0.1, 0.1), dark_red, 0.3, 0.5);
//    Grating green_stripes(Vec2(0, 0), full_green,
//                          Vec2(-0.1, 0.1), dark_green, 0.3, 0.5);
//    Add plaid(red_stripes, green_stripes);
//
//    Vec2 p( 0.246782, -0.338772);
//    Vec2 q(-1.73553,  -0.969689);
//    Vec2 r( 0.535863, -1.75309);
//    Vec2 s(-0.201844, -0.394775);
//
//    Texture::displayAndFile(MobiusTransform(p, q, r, s, plaid)
//                            ); // , path + "mt_ss_1");
//    Texture::sqrt_of_aa_subsample_count = 5;
//    Texture::displayAndFile(MobiusTransform(p, q, r, s, plaid)
//                            ); // , path + "mt_ss_25");
//    Texture::sqrt_of_aa_subsample_count = 7;
//    Texture::displayAndFile(MobiusTransform(p, q, r, s, plaid)
//                            ); // , path + "mt_ss_49");
//    Texture::sqrt_of_aa_subsample_count = 10;
//    Texture::displayAndFile(MobiusTransform(p, q, r, s, plaid)
//                            ); // , path + "mt_ss_100");
//
//    //// Update: Wednesday morning, May 27
//    //Texture::sqrt_of_aa_subsample_count = 150; // 22500 samples, 273 seconds
//    //Texture::displayAndFile(MobiusTransform(p, q, r, s, plaid)
//    //                        ); // , path + "mt_ss_22500");
//
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        
//    // Use new jittered_grid_NxN_in_square() in Blur -- May 28, 2020
//    std::cout << "May 28, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200528_";
//
//    Uniform blue(0, 0, 1);
//    Uniform yellow(1, 1, 0);
//    Grating grating(Vec2(), yellow, Vec2(0.2, 0), blue, 0.01, 0.5);
//
//    Texture::displayAndFile(grating           ); // , path + "grating");
//    Texture::displayAndFile(Blur(0.1, grating)); // , path + "Blur_0_1");
//    Texture::displayAndFile(Blur(0.2, grating)); // , path + "Blur_0_2");
//    Texture::displayAndFile(Blur(0.4, grating)); // , path + "Blur_0_4");
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        
//    // Experiment with new "2 point" spec for noise textures -- May 31, 2020
//    std::cout << "May 31, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200531_";
//
//    Uniform white(1);
//    Uniform black(0);
//    Uniform gray(0.4);
//    Uniform magenta(0.8, 0, 1);
//
//    // Converts monochrome noise texture to threhsholded gray/magenta style.
//    auto style = [&](const Texture& texture) -> const SoftMatte&
//    {
//        // Note: "leaks" allocation of these textures. Used only in this test.
//        auto& st = *(new SoftThreshold(0.50, 0.53, texture));
//        return *(new SoftMatte(st, gray, magenta));
//    };
//
//    // First draw old style Noise
//    float baseline_scale = 0.4;
//    Noise2 bn1(baseline_scale, Vec2(), black, white);
//    Texture::displayAndFile(style(bn1) ); // , path + "old_Noise");
//
//    // Now draw several new style Noises, starting with equivalent to last one.
//    auto two_point_noise = [&] (Vec2 p1, Vec2 p2, std::string pathname = "")
//    {
//        Noise2 noise(p1, p2, black, white);
//        float spot_ir = 0.02;
//        float spot_or = 0.03;
//        Uniform red(1, 0, 0);
//        Uniform green(0, 1, 0);
//        Spot spot1(p2, spot_ir, green, spot_or, style(noise));
//        Spot spot2(p1, spot_ir, red, spot_or, spot1);
//        Texture::displayAndFile(spot2, pathname);
//    };
//    two_point_noise(Vec2(0, 0), Vec2(baseline_scale, 0)
//                    ); // , path + "new_Noise_1");
//    two_point_noise(Vec2(0.38, 0.13), Vec2(-0.38, -0.13)
//                    ); // , path + "new_Noise_2");
//    two_point_noise(Vec2(0.09, 0.03), Vec2(-0.09, -0.03)
//                    ); // , path + "new_Noise_3");
//    two_point_noise(Vec2(-0.3, -0.8), Vec2(0.2, 0.8)
//                    ); // , path + "new_Noise_4");
//    two_point_noise(Vec2(-0.3, 0.7), Vec2(-0.25, 0.68)
//                    ); // , path + "new_Noise_5");
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // New "2 point" spec for all noise textures -- June 2, 2020
//    std::cout << "June 2, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200602_";
//
//    Vec2 p1(10, 3);
//    Vec2 p2 = p1 + Vec2(-1, 3).normalize() * 0.4;
//    Vec2 p3(-1, 4);
//    Vec2 p4 = p3 + Vec2(4, 1).normalize() * 0.2;
//    Uniform red(1, 0, 0);
//    Uniform dark_blue(0, 0, 0.1);
//    Texture::displayAndFile(Brownian(p1, p2, red, dark_blue)
//                            ); // , path + "Brownian_1");
//    Texture::displayAndFile(Brownian(p3, p4, red, dark_blue)
//                            ); // , path + "Brownian_2");
//    Texture::displayAndFile(Turbulence(p1, p2, red, dark_blue)
//                            ); // , path + "Turbulence_1");
//    Texture::displayAndFile(Turbulence(p3, p4, red, dark_blue)
//                            ); // , path + "Turbulence_2");
//    Texture::displayAndFile(Furbulence(p1, p2, red, dark_blue)
//                            ); // , path + "Furbulence_1");
//    Texture::displayAndFile(Furbulence(p3, p4, red, dark_blue)
//                            ); // , path + "Furbulence_2");
//    Texture::displayAndFile(Wrapulence(p1, p2, red, dark_blue)
//                            ); // , path + "Wrapulence_1");
//    Texture::displayAndFile(Wrapulence(p3, p4, red, dark_blue)
//                            ); // , path + "Wrapulence_2");
//
//    // Do not save these for doc.
//    Uniform green(0, 1, 0);
//    Texture::displayAndFile(MultiNoise(p3, p4, red, green, 0.0));
//    Texture::displayAndFile(MultiNoise(p3, p4, red, green, 0.2));
//    Texture::displayAndFile(MultiNoise(p3, p4, red, green, 0.4));
//    Texture::displayAndFile(MultiNoise(p3, p4, red, green, 0.6));
//    Texture::displayAndFile(MultiNoise(p3, p4, red, green, 0.8));
//
//    Texture::displayAndFile(ColorNoise(p1, p2, 0.6)
//                            ); // , path + "ColorNoise_1");
//    Texture::displayAndFile(ColorNoise(p3, p4, 0.6)
//                            ); // , path + "ColorNoise_2");
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Experimental RgbBox -- June 3, 2020
//    std::cout << "June 3, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200603_";
//
//    Vec2 p1(2, 3);
//    Vec2 p2 = p1 + Vec2(-1, 3).normalize() * 0.3;
//    ColorNoise cn(p1, p2, 0.6);
//    Texture::displayAndFile(cn
//                            ); // , path + "cn");
//    Texture::displayAndFile(RgbBox(0.0, 0.3, 0.3, 0.6, 0.6, 0.9, cn)
//                            ); // , path + "RgbBox_1");
//    Texture::displayAndFile(RgbBox(0.5, 1.0, 0.2, 0.6, 0.0, 1.0, cn)
//                            ); // , path + "RgbBox_2");
//    Texture::displayAndFile(RgbBox(0.5, 1.0, 0.7, 1.0, 0.0, 0.2, cn)
//                            ); // , path + "RgbBox_3");
//    Texture::displayAndFile(RgbBox(0.2, 1.0, 0.0, 0.2, 0.0, 0.2, cn)
//                            ); // , path + "RgbBox_4");
//
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Merge Generators.h into Operators.h -- June 7, 2020
//    std::cout << "June 7, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200607_";
//
//    UnitTests::instantiateAllTextureTypes();
//
//    Texture::waitKey();
        
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // New texture interpolation utility -- June 10, 2020
//    std::cout << "June 10, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200610_";
//
//    Uniform b1(Color(0, 0, 1.0));
//    Uniform b2(Color(0, 0, 0.2));
//    Grating blues(Vec2(), b1, Vec2(-0.03, 0.09), b2, 0.2, 0.2);
//    Uniform g1(Color(0, 0.7, 0));
//    Uniform g2(Color(0, 0.05, 0));
//    Grating greens(Vec2(), g1, Vec2(0.03, 0.09), g2, 0.2, 0.2);
//    Add background(blues, greens);
//    Blur blurred(0.1, background);
//
//    Texture::displayAndFile(blues);
//    Texture::displayAndFile(greens);
//    Texture::displayAndFile(background
//                            ); // , path + "background");
//    Texture::displayAndFile(blurred
//                            ); // , path + "blurred");
//    Texture::displayAndFile(Spot(Vec2(), 0.15, blurred, 0.2, background)
//                            ); // , path + "Spot");
//    Texture::displayAndFile(Spot2(Vec2(), 0.15, blurred, 0.2, background));
//
//    Texture::waitKey();
        
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // New texture interpolation utility -- June 13, 2020
//    std::cout << "June 13, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200613_";
//
//    Vec2 p1(0, 0);
//    Vec2 p2(0.1, 0.2);
//    Uniform red(1, 0, 0);
//    Uniform blue(0, 0, 1);
//    Uniform green(0, 1, 0);
//    Uniform white(1, 1, 1);
//    Uniform black(0, 0, 0);
//    Noise noise1(p1, p2, red, blue);
//    Brownian noise2(p1, p2, blue, green);
//    Grating grate(p1, white, p2, black, 0.5, 0.5);
//
//    // original test
//    Texture::diff(SoftMatte(grate, noise1, noise2),
//                  SoftMatte2(grate, noise1, noise2)
//                  ); // , path + "a_few_bad_pixels");
//
//    // simplified test
//    Texture::diff(SoftMatte(white, noise1, noise2),
//                  SoftMatte2(white, noise1, noise2)
//                  ); // , path + "show_mismatches");
//
//    Texture::waitKey();
        
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // new interpolate(), clean up -- June 14, 2020
//    std::cout << "June 14, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200614_";
//
//    Vec2 p1(0, 0);
//    Vec2 p2(0.1, 0.2);
//    Uniform red(1, 0, 0);
//    Uniform blue(0, 0, 1);
//    Uniform green(0, 1, 0);
//    Uniform white(1, 1, 1);
//    Uniform black(0, 0, 0);
//    Noise noise1(p1, p2, red, blue);
//    Brownian noise2(p1, p2, blue, green);
//    Grating grate(p1, white, p2, black, 0.5, 0.5);
//    Grating bg(p1, blue, p2, green, 1, 0.5);
//
//    //Texture::diff(Brownian(p1, p2, white, green),
//    //              Brownian(p1, p2 * 1.1, white, green));
//    //Texture::diff(Noise(p1, p2 * 5, white, green),
//    //              Noise2(p1, p2 * 5, white, green));
//
//    //Texture::diff(LotsOfSpots(0.8, 0.05, 0.3, 0.05, 0.02,
//    //                          Blur(0.2, bg), white),
//    //              LotsOfSpots2(0.8, 0.05, 0.3, 0.05, 0.02,
//    //                           Blur(0.2, bg), white));
//
//    Texture::displayAndFile(LotsOfSpots(0.8, 0.05, 0.3, 0.05, 0.02,
//                                        Blur(0.2, bg), white));
//    Texture::displayAndFile(ColoredSpots(0.8, 0.05, 0.3, 0.05, 0.02,
//                                         Blur(0.2, bg), white));
//    Texture::displayAndFile(LotsOfButtons(0.8, 0.05, 0.3, 0.05, 0.02,
//                                          Vec2(), Blur(0.2, bg), 1, white));
//
//    //Texture::diff(LotsOfButtonsOld(0.8, 0.05, 0.3, 0.05, 0.02,
//    //                               Vec2(), Blur(0.2, bg), 1, white),
//    //              LotsOfButtons   (0.8, 0.05, 0.3, 0.05, 0.02,
//    //                               Vec2(), Blur(0.2, bg), 1, white));
//
//    Texture::waitKey();
        
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Prototyping COTS maps -- June 17, 2020
//    std::cout << "June 17, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200617_";
//
//    float s = 0.4;
//    float v = s * 0.8;
//    float th = s * 2.0;
//    float bh = s * 0.6;
//    float mv = -0.1;
//    Vec2 a(-th, mv + v);
//    Vec2 b(+th, mv + v);
//    Vec2 c(+bh, mv - v);
//    Vec2 d(-bh, mv - v);
//
//    CotsMap cots(a, b, c, d, Uniform(0.5));
//
//    float r1 = 0.03;
//    float r2 = 0.04;
//    float rs = 0.005;
//    Uniform white(1);
//    Uniform black(0);
//    Uniform red(1, 0, 0);
//    Uniform yellow(1, 1, 0);
//    Uniform green(0, 1, 0);
//    Uniform cyan(0, 1, 1);
//    Uniform blue(0, 0, 1);
//
//    Spot spot1(Vec2(), r1 - rs, white, r1, black);
//    Spot spot2(Vec2(), r2 - rs, white, r2, black);
//    Subtract sub(spot1, spot2);
//    Add ring(white, sub);
//
//    auto show_corners = [&](const Texture& t, const std::string& s)
//    {
//        Texture::displayAndFile
//        (SoftMatte(Translate(a, ring), red,
//                   SoftMatte(Translate(b, ring), yellow,
//                             SoftMatte(Translate(c, ring), green,
//                                       SoftMatte(Translate(d, ring), blue, t)))),
//         s);
//    };
//
//    //show_corners(SoftMatte(Translate(a, ring),
//    //                       red,
//    //                       SoftMatte(Translate(b, ring),
//    //                                 yellow,
//    //                                 SoftMatte(Translate(c, ring),
//    //                                           green,
//    //                                           SoftMatte(Translate(d, ring),
//    //                                                     blue, cots)))),
//    //             //, path + "COTS_UV_to_HS");
//    //             //, path + "COTS_UV_to_HS_inside");
//    //             //, path + "COTS_in_or_out");
//    //             "");
//
//
//    show_corners(CotsMap(a, b, c, d,
//                         Furbulence(Vec2(1, 3),
//                                    Vec2(1.1, 3.1),
//                                    Uniform(0.3, 0, 0),
//                                    Uniform(0, 0.5, 1))),
//                 path + "COTS_noise");
//
////    Grating grate1(Vec2(-0.05, 0), cyan, Vec2(+0.05, 0), white, 0.5, 0.5);
//    Uniform dark_cyan(0, 0.5, 0.5);
//    Uniform gray(0.6);
//    Grating grate1(Vec2(-0.05, 0), dark_cyan, Vec2(+0.05, 0), gray, 0.5, 0.5);
//    Grating grate2(Vec2(0, -0.1), black, Vec2(0, 0.1), white, 0.3, 0.1);
//    Multiply cwb_grid(grate1, grate2);
//    show_corners(CotsMap(a, b, c, d, cwb_grid),
//                 path + "COTS_grid");
//
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // More COTS maps -- June 18, 2020
//    std::cout << "June 18, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200618_";
//
//    Uniform red(1, 0, 0);
//    Uniform yellow(1, 1, 0);
//    Uniform green(0, 1, 0);
//    Uniform cyan(0, 1, 1);
//    Uniform blue(0, 0, 1);
//    Uniform white(1);
//    Uniform black(0);
//
//    float r1 = 0.03;
//    float r2 = 0.04;
//    float rd = 0.005;
//    Spot spot1(Vec2(), r1 - rd, white, r1, black);
//    Spot spot2(Vec2(), r2 - rd, white, r2, black);
//    Subtract sub(spot1, spot2);
//    Add ring(white, sub);
//
//    auto annote_cots = [&](Vec2 a, Vec2 b, Vec2 c, Vec2 d,
//                           const Texture& t, const std::string& s = "")
//    {
//        Texture::displayAndFile
//        (SoftMatte(Translate(a, ring), red,
//                   SoftMatte(Translate(b, ring), yellow,
//                             SoftMatte(Translate(c, ring), green,
//                                       SoftMatte(Translate(d, ring),
//                                                 blue,
//                                                 CotsMap(a, b, c, d, t))))),
//         s);
//    };
//
//    Uniform dark_cyan(0, 0.5, 0.5);
//    Uniform gray(0.6);
//    Grating grate1(Vec2(-0.05, 0), dark_cyan, Vec2(+0.05, 0), gray, 0.5, 0.5);
//    Grating grate2(Vec2(0, -0.1), black, Vec2(0, 0.1), white, 0.3, 0.1);
//    Multiply cwb_grid(grate1, grate2);
//
//
//    //int corner_count = 0;
//    //RandomSequence rs(229783476586);
//    //auto rc = [&]()
//    //{
//    //    Vec2 r = rs.randomUnitVector();
//    //    r = r * rs.frandom2(0.4, 0.9);
//    //    std::cout << corner_count++ << ": " << r << std::endl;
//    //    return r;
//    //};
//    //annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//
//
//    //    // TODO both of these come out all black
//    //    annote_cots(Vec2(-0.3, -0.2),
//    //                Vec2(-0.4, -0.5),
//    //                Vec2(0.6, 0.6),
//    //                Vec2(-0.4, -0.5),
//    //                cwb_grid);
//    //    annote_cots(Vec2(0.2, 0.2),
//    //                Vec2(),
//    //                Vec2(0.7, 0.7),
//    //                Vec2(),
//    //                cwb_grid);
//    //
//    //    annote_cots(Vec2(0.2, 0.2),
//    //                Vec2(0.5, -0.5),
//    //                Vec2(0.7, 0.7),
//    //                Vec2(),
//    //                cwb_grid);
//
//    auto cots_corner = [&](float x, float y)
//    {
//        // normalize and flip from Jarek's app
//        float scale = 800;
//        x /= scale;
//        y /= scale;
//        y = 1 - y;
//
//        // recenter on TexSyn's unit radius disk.
//        x *= 2;
//        y *= 2;
//        x -= 1;
//        y -= 1;
//        return Vec2(x, y);
//    };
//
//    path = "/Users/cwr/Desktop/TexSyn_temp/20200619_";
//
//    //// interactive edit
//    //annote_cots(cots_corner(396.80453, 55.000206),
//    //            cots_corner(378.0904, 773.7115),
//    //            cots_corner(393.3884, 575.8651),
//    //            cots_corner(413.66867, 773.59265),
//    //            cwb_grid
//    //            ); // , path + "test1");
//    //
//    //// tweaked
//    //annote_cots(cots_corner(400, 55.000206),
//    //            cots_corner(400, 770),
//    //            cots_corner(393.3884, 575.8651),
//    //            cots_corner(400, 770),
//    //            cwb_grid
//    //            ); // , path + "test2");
//    //
//    //annote_cots(cots_corner(400, 55.000206),
//    //            cots_corner(400, 770),
//    //            cots_corner(393.3884, 575.8651),
//    //            cots_corner(401, 770),
//    //            cwb_grid
//    //            ); // , path + "test3");
//    //
//    //annote_cots(cots_corner(400, 55.000206),
//    //            cots_corner(400, 770),
//    //            cots_corner(400, 575.8651),
//    //            cots_corner(401, 770),
//    //            cwb_grid
//    //            ); // , path + "test4");
//    //
//    //
//    //// Saturday Jun 20
//    //annote_cots(cots_corner(80.565414, 275.3029),
//    //            cots_corner(386.30826, 331.97156),
//    //            cots_corner(431.36285, 552.3875),
//    //            cots_corner(589.4879, 649.52625),
//    //            cwb_grid);
//    //annote_cots(cots_corner(400, 55.000206),
//    //            cots_corner(400, 770),
//    //            cots_corner(400, 575.8651),
//    //            cots_corner(400.1, 770),
//    //            cwb_grid);
//
//    int corner_count = 0;
//    //    RandomSequence rs(8475473559);
//    //    RandomSequence rs(939588689);
//    RandomSequence rs(29475656839);
//    auto rc = [&]()
//    {
//        Vec2 r = rs.randomUnitVector();
//        r = r * rs.frandom2(0.4, 0.9);
//        std::cout << corner_count++ << ": " << r << std::endl;
//        return r;
//    };
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//    //    annote_cots(rc(), rc(), rc(), rc(), cwb_grid);
//
//    Texture::displayAndFile(CotsMap(rc(), rc(), rc(), rc(),
//                                    CotsMap(rc(), rc(), rc(), rc(), cwb_grid)));
//    Texture::displayAndFile(CotsMap(rc(), rc(), rc(), rc(),
//                                    CotsMap(rc(), rc(), rc(), rc(), cwb_grid)));
//    Texture::displayAndFile(CotsMap(rc(), rc(), rc(), rc(),
//                                    CotsMap(rc(), rc(), rc(), rc(), cwb_grid)));
//    Texture::displayAndFile(CotsMap(rc(), rc(), rc(), rc(),
//                                    CotsMap(rc(), rc(), rc(), rc(), cwb_grid)));
//    Texture::displayAndFile(CotsMap(rc(), rc(), rc(), rc(),
//                                    CotsMap(rc(), rc(), rc(), rc(), cwb_grid)));
//    Texture::displayAndFile(CotsMap(rc(), rc(), rc(), rc(),
//                                    CotsMap(rc(), rc(), rc(), rc(), cwb_grid)));
//    Texture::displayAndFile(CotsMap(rc(), rc(), rc(), rc(),
//                                    CotsMap(rc(), rc(), rc(), rc(), cwb_grid)));
//    Texture::displayAndFile(CotsMap(rc(), rc(), rc(), rc(),
//                                    CotsMap(rc(), rc(), rc(), rc(), cwb_grid)));
//    Texture::displayAndFile(CotsMap(rc(), rc(), rc(), rc(),
//                                    CotsMap(rc(), rc(), rc(), rc(), cwb_grid)));
//    Texture::displayAndFile(CotsMap(rc(), rc(), rc(), rc(),
//                                    CotsMap(rc(), rc(), rc(), rc(), cwb_grid)));
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // More COTS maps -- June 20, 2020
//    std::cout << "June 20, 2021" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200620_";
//
//    Uniform black(0);
//    Uniform white(1);
//    Uniform gray(0.5);
//    Uniform blue(0, 0, 1);
//    Uniform green(0, 1, 0);
//    Uniform yellow(1, 1, 0);
//    Uniform magenta(1, 0, 1);
//    Wrapulence by_noise(Vec2(2, 3), Vec2(2.5, 3.5), black, yellow);
//    Grating grate0(Vec2(), blue, Vec2(0.1, 0), gray, 1, 0.5);
//    Grating grate1(Vec2(), green, Vec2(0, 0.1), gray, 1, 0.5);
//    Add bg_grid(grate0, grate1);
//    LotsOfSpots mg_los(0.8, 0.05, 0.05, 0.01, 0.02, magenta, gray);
//
//    Texture::displayAndFile(by_noise
//                            ); // , path + "noise0");
//    Texture::displayAndFile(CotsMap(Vec2(-0.4, -0.5),
//                                    Vec2(-0.5, -0.4),
//                                    Vec2(+0.2, +0.8),
//                                    Vec2(+0.8, -0.2),
//                                    by_noise)
//                            ); // , path + "noise1");
//    Texture::displayAndFile(bg_grid
//                            ); // , path + "grid0");
//    Texture::displayAndFile(CotsMap(Vec2(+0.1, -0.6),
//                                    Vec2(+0.7, +0.2),
//                                    Vec2(-0.5, +0.9),
//                                    Vec2(-0.8, -0.5),
//                                    bg_grid)
//                            ); // , path + "grid1");
//    Texture::displayAndFile(mg_los
//                            ); // , path + "los0");
//    Texture::displayAndFile(CotsMap(Vec2(+0.1, -0.2),
//                                    Vec2(+0.7, +0.8),
//                                    Vec2(-0.3, +0.4),
//                                    Vec2(-0.5, -0.6),
//                                    CotsMap(Vec2(-0.4, -0.5),
//                                            Vec2(-0.5, -0.4),
//                                            Vec2(+0.2, +0.8),
//                                            Vec2(+0.8, -0.2),
//                                            mg_los))
//                            ); // , path + "los1");
//    Texture::displayAndFile(CotsMap(Vec2(+0.2, +0.1),
//                                    Vec2(+0.4, +0.7),
//                                    Vec2(+0.5, +0.6),
//                                    Vec2(+0.7, +0.4),
//                                    CotsMap(Vec2(-0.7, -0.2),
//                                            Vec2(-0.5, -0.4),
//                                            Vec2(-0.6, -0.3),
//                                            Vec2(-0.1, -0.6),
//                                            mg_los))
//                            ); // , path + "los2");
//    Texture::displayAndFile(CotsMap(Vec2(+0.6, +0.0),
//                                    Vec2(+0.2, +0.8),
//                                    Vec2(+0.7, +0.4),
//                                    Vec2(+0.8, +0.6),
//                                    CotsMap(Vec2(-0.6, -0.2),
//                                            Vec2(-0.3, -0.3),
//                                            Vec2(-0.2, -0.5),
//                                            Vec2(-0.7, -0.6),
//                                            mg_los))
//                            ); // , path + "los3");
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // More COTS maps -- June 22, 2020
//    std::cout << "June 22, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200622_";
//
//    Vec2 p0(0, 0);
//    Vec2 p1(0.1, 0.1);
//    Vec2 p2(-0.1, -0.1);
//    Uniform black(0);
//    Uniform cyan(0, 1, 1);
//    Uniform magenta(1, 0, 1);
//    Grating grate_c(Vec2(), cyan, Vec2(0.1, 0), black, 0.2, 0.5);
//    Grating grate_m(Vec2(), magenta, Vec2(0.1, 0), black, 0.2, 0.5);
//
//    Texture::displayAndFile(Gradation(p0, cyan, p0, magenta)
//                            ); // , path + "degenerate_0");
//    Texture::displayAndFile(Gradation(p1, cyan, p2, magenta)
//                            ); // , path + "degenerate_1");
//    Texture::displayAndFile(Gradation(p0, grate_c, p0, grate_m)
//                            ); // , path + "degenerate_2");
//    Texture::displayAndFile(Gradation(p1, grate_c, p2, grate_m)
//                            ); // , path + "degenerate_3");
//    Texture::displayAndFile(Grating(p0, cyan, p0, magenta, 0.5, 0.5)
//                            ); // , path + "degenerate_4");
//    Texture::displayAndFile(Grating(p1, cyan, p2, magenta, 0.5, 0.5)
//                            ); // , path + "degenerate_5");
//    Texture::displayAndFile(Turbulence(p0, p0, grate_c, grate_m)
//                            ); // , path + "degenerate_6");
//    Texture::displayAndFile(Turbulence(p1, p2, grate_c, grate_m)
//                            ); // , path + "degenerate_7");
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Replace wellBehavedFractionalPart(), "temp" from 2009! -- June 23, 2020
//    std::cout << "June 23, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200623_";
//
//    // using the basic x-floor(x) for very small negative value produces 1 rather than 0
//    // XXX20091121 experiment
//    // a = -1.98682e-08
//    // floor(a) = -1
//    // a-floor(a) = 1
//    float a = -1.98682e-08;
//    debugPrint(a);
//    //debugPrint(floor(a));
//    //debugPrint(a-floor(a));
//    float b = -0.1;
//    debugPrint(b);
//
//    debugPrint(fmod_floor(a, 1));
//    debugPrint(fmod_floor(1 + a, 1));
//    debugPrint(fmod_floor(1 - a, 1));
//
//    debugPrint(fmod_floor(b, 1));
//    debugPrint(fmod_floor(1 + b, 1));
//    debugPrint(fmod_floor(1 - b, 1));
//
//    UnitTests::allTestsOK();
//
//    //Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Unit test "noise_ranges" faster, re-tune ranges -- June 24, 2020

//    std::cout << "June 24, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200624_";
//
//    Uniform black(0);
//    Uniform yellow(1, 1, 0);
//    Wrapulence by_noise(Vec2(2, 3), Vec2(2.5, 3.5), black, yellow);
//    Texture::displayAndFile(by_noise
//                            , path + "Wrapulence");
//    Vec2 p1(2, 3);
//    Vec2 p2 = p1 + Vec2(-1, 3).normalize() * 0.3;
//    Texture::displayAndFile(ColorNoise(p1, p2, 0.6)
//                            , path + "ColorNoise");
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // UnitTests avoid 106 disposableUniforms -- June 25, 2020
//    std::cout << "June 25, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200625_";
//
//    UnitTests::allTestsOK();
//
//    // Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // fmod_floor UnitTests -- June 26, 2020
//    std::cout << "June 25, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200625_";
//    
//    UnitTests::allTestsOK();
//    
//    // Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // StretchSpot, this time for sure -- July 2, 2020
//    std::cout << "July 2, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200702_";
//
//    float f = 1;
//    float d = 0.1;
//    Uniform full_green(Color(f, 0, 0));
//    Uniform dark_green(Color(d, 0, 0));
//    Uniform full_red(Color(0, f, 0));
//    Uniform dark_red(Color(0, d, 0));
//    Grating red_stripes(Vec2(0, 0), full_red,
//                        Vec2(0.1, 0.1), dark_red, 0.3, 0.5);
//    Grating green_stripes(Vec2(0, 0), full_green,
//                          Vec2(-0.1, 0.1), dark_green, 0.3, 0.5);
//    Add plaid(red_stripes, green_stripes);
//    
//    float radius = 0.9;
//    // Texture::displayAndFile(plaid);
//    //Texture::displayAndFile(StretchSpot(1.50, radius, Vec2(), plaid)
//    //                        ); // , path + "ss_old_150");
//    //Texture::displayAndFile(StretchSpot(0.67, radius, Vec2(), plaid)
//    //                        ); // , path + "ss_old_067");
//    //Texture::displayAndFile(StretchSpot(2.00, radius, Vec2(), plaid)
//    //                        ); // , path + "ss_old_200");
//    //Texture::displayAndFile(StretchSpot(0.50, radius, Vec2(), plaid)
//    //                        ); // , path + "ss_old_050");
//    //Texture::displayAndFile(StretchSpot(5.00, radius, Vec2(), plaid)
//    //                        ); // , path + "ss_old_500");
//    //Texture::displayAndFile(StretchSpot(0.20, radius, Vec2(), plaid)
//    //                        ); // , path + "ss_old_020");
//    
//    Texture::displayAndFile(StretchSpot(1.50, radius, Vec2(), plaid)
//                            ); // , path + "ss_new_150");
//    Texture::displayAndFile(StretchSpot(0.67, radius, Vec2(), plaid)
//                            ); // , path + "ss_new_067");
//    Texture::displayAndFile(StretchSpot(2.00, radius, Vec2(), plaid)
//                            ); // , path + "ss_new_200");
//    Texture::displayAndFile(StretchSpot(0.50, radius, Vec2(), plaid)
//                            ); // , path + "ss_new_050");
//    Texture::displayAndFile(StretchSpot(5.00, radius, Vec2(), plaid)
//                            ); // , path + "ss_new_500");
//    Texture::displayAndFile(StretchSpot(0.20, radius, Vec2(), plaid)
//                            ); // , path + "ss_new_020");
//    
//    //Texture::displayAndFile(NeoStretchSpot(-2.0, radius, Vec2(), plaid));
//    //Texture::displayAndFile(NeoStretchSpot(-0.5, radius, Vec2(), plaid));
//    //Texture::displayAndFile(NeoStretchSpot(0, radius, Vec2(), plaid));
//    //Texture::displayAndFile(NeoStretchSpot(2.0, -radius, Vec2(), plaid));
//    //Texture::displayAndFile(NeoStretchSpot(2.0, -0.1, Vec2(), plaid));
//
//    path = "/Users/cwr/Desktop/TexSyn_temp/20200704_";
//    Texture::displayAndFile(StretchSpot(0.20, radius, Vec2(),
//                                        ColoredSpots(0.8, 0.02, 0.02, 0.01, 0,
//                                                     plaid, Uniform(0)))
//                            ); // , path + std::string("ss_spots_020"));
//
//    // I cut-n-pasted this text into a spread sheet to visualize the curves.
//    //for (float x = 0; x <= 1; x += 0.01)
//    //{
//    //    std::cout << x;
//    //    std::cout << ",";
//    //    std::cout << sinusoid(x);
//    //    std::cout << ",";
//    //    std::cout << (x + sinusoid(x)) / 2;
//    //    std::cout << ",";
//    //    std::cout << interpolate(x, x, sinusoid(x));
//    //    std::cout << ",";
//    //    std::cout << interpolate(sinusoid(x), x, sinusoid(x));
//    //    std::cout << ",";
//    //    std::cout << interpolate(std::pow(x, 2), x, sinusoid(x));
//    //    std::cout << ",";
//    //    std::cout << interpolate(std::pow(x, 5), x, sinusoid(x));
//    //    std::cout << std::endl;
//    //}
//    
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // experimenting with hyperbolic projection -- July 6, 2020
//    std::cout << "July 6, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200706_";
//
//    Uniform black(0);
//    Uniform white(1);
//    Grating bw_stripes(Vec2(-0.05, 0), black, Vec2(0.05, 0), white, 0.1, 0.5);
//
//    float f = 1;
//    float d = 0.1;
//    Uniform full_green(Color(f, 0, 0));
//    Uniform dark_green(Color(d, 0, 0));
//    Uniform full_red(Color(0, f, 0));
//    Uniform dark_red(Color(0, d, 0));
//    Grating red_stripes(Vec2(0, 0), full_red,
//                        Vec2(0.1, 0.1), dark_red, 0.3, 0.5);
//    Grating green_stripes(Vec2(0, 0), full_green,
//                          Vec2(-0.1, 0.1), dark_green, 0.3, 0.5);
//    Add plaid(red_stripes, green_stripes);
//
//    Uniform dark_blue(0, 0, 0.3);
//    Uniform green(0, 1, 0);
//    LotsOfSpots los(0.85, 0.04, 0.17, 0.01, 0.02, green, dark_blue);
//
//    Texture::displayAndFile(bw_stripes); //  , path + "bw_stripes");
//    Texture::displayAndFile(plaid); //  , path + "plaid");
//    Texture::displayAndFile(los); //  , path + "los");
//
//    Texture::sqrt_of_aa_subsample_count = 20;
//
//    Texture::displayAndFile(Hyperbolic(Vec2(0, 0.75), 1.5, bw_stripes, white)
//                            ); //  , path + "Hyperbolic_bw_stripes");
//    Texture::displayAndFile(Hyperbolic(Vec2(), 0.9, plaid, black)
//                            ); //  , path + "Hyperbolic_plaid");
//    Texture::displayAndFile(Hyperbolic(Vec2(), 0.9, los, black)
//                            ); //  , path + "Hyperbolic_los");
//
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Affine -- July 8, 2020
//    std::cout << "July 8, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200708_";
//
//    Uniform black(0);
//    Uniform white(1);
//    Uniform gray(0.6);
//    Uniform red(1, 0, 0);
//    Uniform yellow(1, 1, 0);
//    Grating grate(Vec2(-0.05, 0), black, Vec2(0.05, 0), white, 0.1, 0.5);
//    Gradation grad(Vec2(0, 1), yellow, Vec2(0, -1), red);
//    Multiply color_grate(grate, grad);
//    Spot s1(Vec2(), 0.20, white, 0.22, black);
//    Spot s2(Vec2(), 0.18, white, 0.20, black);
//    Subtract ring(s1, s2);
//    SoftMatte test(ring, color_grate, white);
//
//    Vec2 o(0, 0);
//    Vec2 x1(1, 0);
//    Vec2 x2 = x1 * 2;
//    Vec2 xh = x1 * 0.5;
//    Vec2 d1 = Vec2(1, 1).normalize();
//    Vec2 d2 = d1 * 2;
//    Vec2 dh = d1 * 0.5;
//    Vec2 ph(0.5, 0.5);
//    Vec2 mh = ph * -1;
//    Texture::displayAndFile(test);
//    Texture::displayAndFile(Affine(o,  o  + x1, test));//, path + "Affine_o_x1");
//    Texture::displayAndFile(Affine(mh, mh + xh, test));//, path + "Affine_mh_xh");
//    Texture::displayAndFile(Affine(mh, mh + x1, test));//, path + "Affine_mh_x1");
//    Texture::displayAndFile(Affine(mh, mh + x2, test));//, path + "Affine_mh_x2");
//    Texture::displayAndFile(Affine(mh, mh + dh, test));//, path + "Affine_mh_dh");
//    Texture::displayAndFile(Affine(mh, mh + d1, test));//, path + "Affine_mh_d1");
//    Texture::displayAndFile(Affine(mh, mh + d2, test));//, path + "Affine_mh_d2");
//
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // add scale and exponent to hyperbolic projection -- July 11, 2020
//    std::cout << "July 11, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200711_";
//
//    Uniform black(0);
//    Uniform white(1);
//    Uniform dark_blue(0, 0, 0.3);
//    Uniform green(0, 1, 0);
//    LotsOfSpots los(0.85, 0.04, 0.17, 0.01, 0.02, green, dark_blue);
//
//    Texture::sqrt_of_aa_subsample_count = 20;
//    // Texture::sqrt_of_aa_subsample_count = 4;
//
//    Texture::displayAndFile(Hyperbolic(Vec2(), 0.9, 1, 1, los, black)
//                            , path + "Hyperbolic_10_10");
//    // Poincaré disk
//    Texture::displayAndFile(Hyperbolic(Vec2(), 0.9, 0.5, 2, los, black)
//                            , path + "Hyperbolic_05_20");
//    Texture::displayAndFile(Hyperbolic(Vec2(), 0.9, 0.3, 5, los, black)
//                            , path + "Hyperbolic_03_50");
//    Texture::displayAndFile(Hyperbolic(Vec2(), 0.9, 1,   5, los, black)
//                            , path + "Hyperbolic_10_50");
//    Texture::displayAndFile(Hyperbolic(Vec2(), 0.9, 3,   5, los, black)
//                            , path + "Hyperbolic_30_50");
//
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Tweak Texture::diff() -- July 12, 2020
//    std::cout << "July 12, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200712_";
//
//    Uniform black(0);
//    Uniform white(1);
//    Grating g1(Vec2(), black, Vec2(0.000, 0.200), white, 0.3, 0.8);
//    Grating g2(Vec2(), black, Vec2(0.005, 0.195), white, 0.3, 0.8);
//    Texture::diff(g1, g2);
//    Texture::diff(g1, g2, "", 471);
//    Texture::diff(g1, g2, path + "test", 471);
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // slides for UC Santa Cruz talk -- July 12, 2020
//    std::cout << "July 12, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200712_";
//
//    //    Uniform white(1);
//    //    Uniform black(0);
//    //    Uniform less_red(Color(0.7, 0, 0));
//    //    Uniform less_blue(Color(0, 0, 0.7));
//    //
//    //    Spot spot1(Vec2(+0.2, 0), 0.38, less_red, 0.4, black);
//    //    Spot spot2(Vec2(-0.2, 0), 0.38, less_blue, 0.4, black);
//    //    Add two_spots(spot1, spot2);
//    //
//    ////    Uniform less_yellow(Color(0.7, 0.7, 0));
//    ////    Uniform less_cyan(Color(0, 0.7, 0.7));
//    //    Uniform less_yellow(Color(1, 1, 0));
//    //    Uniform less_cyan(Color(0, 1, 1));
//    //    Spot spot3(Vec2(+0.2, 0), 0.38, less_yellow, 0.4, white);
//    //    Spot spot4(Vec2(-0.2, 0), 0.38, less_cyan, 0.4, white);
//    //    Multiply two_spots2(spot3, spot4);
//    //
//    //    //    Scale scaled_spots(1.5, two_spots);
//    //    //    Rotate rotated_spots(pi / 4, two_spots);
//    //    //    Rotate scale_then_rotate(pi / 4, scaled_spots);
//    //    //    Translate translated_spots(Vec2(0, 0.3), two_spots);
//    //    //    Translate scale_rotate_translate(Vec2(0, 0.3), scale_then_rotate);
//    //
//    //
//    ////    Texture::displayAndFile(white, path + "white");
//    ////    Texture::displayAndFile(less_red, path + "less_red");
//    ////    Texture::displayAndFile(less_blue, path + "less_blue");
//    ////    Texture::displayAndFile(two_spots, path + "two_spots");
//    //    Texture::displayAndFile(two_spots2); // , path + "two_spots2");
//    //
//    //    debugPrint(Color(1, 1, 0) * Color(0, 1, 1));
//
//    Uniform white(1);
//    Uniform black(0);
//    Uniform red(1, 0, 0);
//    Uniform green(0, 1, 0);
//    Uniform blue(0, 0, 1);
//
//    Vec2 p1(0.3, 0);
//    Vec2 p2 = p1.rotate(2 * pi / 3);
//    Vec2 p3 = p2.rotate(2 * pi / 3);
//
//    float ro = 0.6;
//    float ri = ro - 0.02;
//
//    //Spot rs(p1, ri, red, ro, black);
//    //Spot gs(p2, ri, green, ro, black);
//    //Spot bs(p3, ri, blue, ro, black);
//    //
//    //Texture::displayAndFile(red
//    //                        ); // , path + "red");
//    //Texture::displayAndFile(rs
//    //                        ); // , path + "red_spot");
//    //Texture::displayAndFile(Add(rs, Add(gs, bs))
//    //                        ); // , path + "additive");
//    //Texture::displayAndFile(Multiply(Subtract(white, rs),
//    //                                 Multiply(Subtract(white, gs),
//    //                                          Subtract(white, bs)))
//    //                        ); // , path + "subtractive");
//
//    Spot red_on_black(p1, ri, red, ro, black);
//    Spot green_on_black(p2, ri, green, ro, black);
//    Spot blue_on_black(p3, ri, blue, ro, black);
//    Subtract cyan_on_white(white, red_on_black);
//    Subtract magenta_on_white(white, green_on_black);
//    Subtract yellow_on_white(white, blue_on_black);
//
//    Texture::displayAndFile(Add(red_on_black,
//                                Add(green_on_black, blue_on_black)));
//    Texture::displayAndFile(Multiply(cyan_on_white,
//                                     Multiply(magenta_on_white,
//                                              yellow_on_white)));
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // for stackoverflow -- July 15, 2020
//    std::cout << "July 15, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200715_";
//
//    Uniform black(0);
//    Uniform white(1);
//    Uniform dark_blue(0, 0, 0.3);
//    Uniform green(0, 1, 0);
//    LotsOfSpots los(0.85, 0.04, 0.17, 0.01, 0.02, green, dark_blue);
//
//    Texture::sqrt_of_aa_subsample_count = 20;
//    //Texture::sqrt_of_aa_subsample_count = 4;
//
//    Texture::displayAndFile(Hyperbolic(Vec2(), 0.9, 1, 1, los, black)
//                            , path + "Hyperbolic_10_10");
//    // Poincaré disk
//    Texture::displayAndFile(Hyperbolic(Vec2(), 0.9, 0.5, 2, los, black)
//                            , path + "Hyperbolic_05_20");
//    Texture::displayAndFile(Hyperbolic(Vec2(), 0.9, 0.3, 5, los, black)
//                            , path + "Hyperbolic_03_50");
//    Texture::displayAndFile(Hyperbolic(Vec2(), 0.9, 1,   5, los, black)
//                            , path + "Hyperbolic_10_50");
//    Texture::displayAndFile(Hyperbolic(Vec2(), 0.9, 3,   5, los, black)
//                            , path + "Hyperbolic_30_50");
//
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // global flags for default render shape and resolution -- July 17, 2020
//    std::cout << "July 17, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200717_";
//
//    Vec2 p1(0, 0);
//    Vec2 p2(0.01, 0.03);
//    Uniform gray50(0.5);
//    Uniform red(1, 0, 0);
//
//    Texture::displayAndFile(Noise(p1, p2, gray50, red));
//    Texture::setDefaultRenderAsDisk(false);
//    Texture::displayAndFile(Noise(p1, p2, gray50, red));
//    Texture::setDefaultRenderAsDisk(true);
//    Texture::setDefaultRenderSize(701);
//    Texture::displayAndFile(Noise(p1, p2, gray50, red));
//    Texture::diff(Noise(p1, p2, gray50, red),
//                  Noise(p1, p2, red, gray50));
//    Texture::setDefaultRenderSize(333);
//    Texture::displayAndFile(Noise(p1, p2, gray50, red));
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // update UnitTests::instantiateAllTextureTypes() -- July 18, 2020
//    std::cout << "July 18, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200718_";
//
//    UnitTests::instantiateAllTextureTypes();
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//        // HsvBox -- July 18, 2020
//        std::cout << "July 18, 2020" << std::endl;
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200718_";
//
//
//    //    Texture::displayAndFile(ColorNoise(Vec2(1, 2), Vec2(1.1, 1.9), 0.8));
//    //    Texture::displayAndFile(ColorNoise(Vec2(1, 2), Vec2(1.1, 1.9), 0.7));
//    //    Texture::displayAndFile(ColorNoise(Vec2(1, 2), Vec2(1.2, 1.8), 0.7));
//    //    Texture::displayAndFile(ColorNoise(Vec2(1, 2), Vec2(1.15, 1.85), 0.7));
//    //    Texture::displayAndFile(ColorNoise(Vec2(3, 7), Vec2(3.15, 6.85), 0.7));
//
//    //    ColorNoise cn(Vec2(3, 7), Vec2(3.15, 6.85), 0.7);
//    //    ColorNoise cn(Vec2(3, 7), Vec2(3.15, 6.85), 0.2);
//    //    ColorNoise cn(Vec2(3, 7), Vec2(3.15, 6.85), 0);
//        ColorNoise cn(Vec2(3, 7), Vec2(3.10, 6.9), 0);
//        Texture::displayAndFile(cn);
//
//    //    Texture::displayAndFile(HsvBox(0.2, 0.4, 0.4, 0.6, 0.0, 1.0, cn));
//    //    Texture::displayAndFile(HsvBox(0.2, 0.4, 0.3, 0.5, 0.0, 0.5, cn));
//    //    Texture::displayAndFile(HsvBox(0.2, 0.4, 0.3, 0.5, 0.4, 0.6, cn));
//    //
//    //    Texture::displayAndFile(HsvBox(0.9, 0.5, 0.6, 0.8, 0.4, 1.0, cn));
//    //    Texture::displayAndFile(HsvBox(0.5, 0.9, 0.6, 0.8, 0.4, 1.0, cn));
//
//    //    Texture::displayAndFile(HsvBox(0.0, 1.0, 0.0, 1.0, 0.0, 1.0, cn));
//    //    Texture::displayAndFile(HsvBox(0.0, 0.5, 0.0, 1.0, 0.0, 1.0, cn));
//    //    Texture::displayAndFile(HsvBox(0.0, 1.0, 0.0, 0.5, 0.0, 1.0, cn));
//    //    Texture::displayAndFile(HsvBox(0.0, 1.0, 0.0, 1.0, 0.0, 0.5, cn));
//        Texture::displayAndFile(HsvBox(0.0, 1.0, 0.0, 1.0, 0.0, 1.0, cn));
//        Texture::displayAndFile(HsvBox(0.0, 0.33, 0.0, 1.0, 0.0, 1.0, cn));
//        Texture::displayAndFile(HsvBox(0.0, 1.0, 0.0, 0.33, 0.0, 1.0, cn));
//        Texture::displayAndFile(HsvBox(0.0, 1.0, 0.0, 1.0, 0.0, 0.33, cn));
//
//        Texture::displayAndFile(HueRing());
//    //    Texture::displayAndFile(HsvBox(0.0, 0.5, 0.0, 1.0, 0.0, 1.0, HueRing()));
//        Texture::displayAndFile(HsvBox(0.0, 0.33, 0.0, 1.0, 0.0, 1.0, HueRing()));
//
//        Texture::displayAndFile(HueIfAny(1, 1, cn));
//        Texture::displayAndFile(HueIfAny(0.8, 0.5, cn));
//        Texture::displayAndFile(HueIfAny(0.6, 0.8, cn));
//        Texture::displayAndFile(HueIfAny(0.8, 0.8, cn));
//        Texture::displayAndFile(HueIfAny(0.8, 1, cn));
//        Texture::displayAndFile(HueIfAny(0.6, 1, cn));
//        Texture::displayAndFile(HueIfAny(1, 0.7, cn));
//
//        Texture::displayAndFile(HueIfAny(1, 0.7,
//                                         Noise(Vec2(3, 7), Vec2(3.10, 6.9),
//                                               Uniform(0), Uniform(1))));
//
//    //    Texture::displayAndFile(HueIfAny(1, 0.7,
//    //                                     Spot(Vec2(1, 0), 0, cn, 1.6,
//    //                                          Noise(Vec2(3, 7), Vec2(3.10, 6.9),
//    //                                                Uniform(0), Uniform(1)))));
//    //    Texture::displayAndFile(Spot(Vec2(1, 0), 0, cn, 1.6,
//    //                                 Noise(Vec2(3, 7), Vec2(3.10, 6.9),
//    //                                       Uniform(0), Uniform(1))));
//        Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    //  Lueker circles -- July 20, 2020
//    // See: https://www.facebook.com/jonathan.lueker/posts/10221043878240079
//    std::cout << "July 20, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200720_";
//
//    Uniform black(0);
//    Uniform white(1);
//    Wrapulence noise(Vec2(1, 2), Vec2(1.1, 1.9), white, black);
//    SliceGrating hgrate(Vec2(0, 1), Vec2(), noise);
//    SliceGrating vgrate(Vec2(1, 0), Vec2(), noise);
//    Spot spot(Vec2(), 0.12, white, 0.13, black);
//    Row row0(Vec2(0.5, 0), Vec2(), spot);
//    Row spots(Vec2(0, 0.5), Vec2(), row0);
//
//    Texture::setDefaultRenderAsDisk(false);
//    Texture::displayAndFile(spots, path + "Lueker_1");
//    Texture::displayAndFile(hgrate, path + "Lueker_2");
//    Texture::displayAndFile(vgrate, path + "Lueker_3");
//    Texture::displayAndFile(SoftMatte(spots, hgrate, vgrate), path + "Lueker_4");
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // HueOnly -- July 20, 2020
//    std::cout << "July 20, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200720_";
//
//    ColorNoise cn(Vec2(3, 7), Vec2(3.10, 6.9), 0);
//    Texture::displayAndFile(cn,                    path + "cn");
//    Texture::displayAndFile(HueOnly(1, 1, cn),     path + "HueOnly_10_10");
//    Texture::displayAndFile(HueOnly(0.5, 0.5, cn), path + "HueOnly_05_05");
//    Texture::displayAndFile(HueOnly(0.5, 1, cn),   path + "HueOnly_05_10");
//    Texture::displayAndFile(HueOnly(1, 0.5, cn),   path + "HueOnly_10_05");
//
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // HsvBox for doc -- July 22, 2020
//    std::cout << "July 22, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200722_";
//
//    ColorNoise cn(Vec2(3, 7), Vec2(3.10, 6.9), 0);
//    Texture::displayAndFile(cn
//                            );  // , path + "cn");
//    Texture::displayAndFile(HsvBox(0, 0.33, 0, 1,    0, 1,    cn)
//                            );  // , path + "HsvBox_0_3_0_1_0_1");
//    Texture::displayAndFile(HsvBox(0, 0.33, 0, 0.33, 0, 1,    cn)
//                            );  // , path + "HsvBox_0_3_0_3_0_1");
//    Texture::displayAndFile(HsvBox(0, 0.33, 0, 0.33, 0, 0.33, cn)
//                            );  // , path + "HsvBox_0_3_0_3_0_3");
//
//    Texture::displayAndFile(HsvBox(0, 0.33, 0, 1,    0, 1,    cn)
//                            );  // , path + "HsvBox_unfolded");
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Move operator methods from .h to .cpp -- July 23, 2020
//    std::cout << "July 23, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200723_";
//
//    Uniform black(0);
//    Uniform white(1);
//    Vec2 p1(0.1, 0.1);
//    Vec2 p2(-0.1, -0.1);
//
//    Texture::displayAndFile(Grating(p1, black, p2, white, 0.0, 0.5));
//    Texture::displayAndFile(Grating(p1, black, p2, white, 0.2, 0.5));
//    Texture::displayAndFile(Grating(p1, black, p2, white, 0.4, 0.5));
//    Texture::displayAndFile(Grating(p1, black, p2, white, 0.6, 0.5));
//    Texture::displayAndFile(Grating(p1, black, p2, white, 0.8, 0.5));
//    Texture::displayAndFile(Grating(p1, black, p2, white, 1.0, 0.5));
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Experimental HsvFrom3 -- July 24, 2020
//    std::cout << "July 24, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200724_";
//
//    Uniform black(0);
//    Uniform white(1);
//    Uniform red(1, 0, 0);
//    Uniform green(0, 1, 0);
//    Uniform blue(0, 0, 1);
//    Vec2 p1;
//    Vec2 p2(1, -3);
//    Vec2 p3(-2, 4);
//    Vec2 o1 = Vec2::randomUnitVector() * 0.1;
//    Vec2 o2 = Vec2::randomUnitVector() * 0.2;
//    Vec2 o3 = Vec2::randomUnitVector() * 0.3;
//
//    Texture::displayAndFile(HsvFrom3(ColorNoise(p1, p1 + o2, 0.4),
//                                     ColorNoise(p2, p2 + o2, 0.8),
//                                     Brownian(p3, p3 + o2, white, black))
//                            );  // , path + "HsvFrom3_a");
//    Texture::displayAndFile(HsvFrom3(ColorNoise(p1, p1 + o2, 0.2),
//                                     ColorNoise(p2, p2 + o2, 0.4),
//                                     Turbulence(p3, p3 + o3, black, white))
//                            );  // , path + "HsvFrom3_b");
//    Texture::displayAndFile(HsvFrom3(Grating(p1, red, p1 + o1, green, 1, 0.5),
//                                     Wrapulence(p2, p2 + o3, blue, white),
//                                     Turbulence(p3, p3 + o3, black, white))
//                            );  // , path + "HsvFrom3_c");
//    Texture::waitKey();

//    UnitTests::instantiateAllTextureTypes();
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Move operator methods from .h to .cpp -- July 25, 2020
//    std::cout << "July 25, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200725_";
//
//    Uniform black(0);
//    Uniform white(1);
//    Vec2 p1(0.1, 0.1);
//    Vec2 p2(-0.1, -0.1);
//
//    //Texture::displayAndFile(Grating(p1, black, p2, white, 0.0, 0.5));
//    //Texture::displayAndFile(Grating(p1, black, p2, white, 0.2, 0.5));
//    //Texture::displayAndFile(Grating(p1, black, p2, white, 0.4, 0.5));
//    //Texture::displayAndFile(Grating(p1, black, p2, white, 0.6, 0.5));
//    //Texture::displayAndFile(Grating(p1, black, p2, white, 0.8, 0.5));
//    //Texture::displayAndFile(Grating(p1, black, p2, white, 1.0, 0.5));
//
//    Texture::displayAndFile(Grating(p1, black, p2, white, 0.2, 0.0));
//    Texture::displayAndFile(Grating(p1, black, p2, white, 0.2, 0.2));
//    Texture::displayAndFile(Grating(p1, black, p2, white, 0.2, 0.4));
//    Texture::displayAndFile(Grating(p1, black, p2, white, 0.2, 0.6));
//    Texture::displayAndFile(Grating(p1, black, p2, white, 0.2, 0.8));
//    Texture::displayAndFile(Grating(p1, black, p2, white, 0.2, 1.0));
//
//    //auto foo = [&](float dc)
//    //{
//    //    Texture::diff(Grating (p1, black, p2, white, 0.2, dc),
//    //                  Grating2(p1, black, p2, white, 0.2, dc));
//    //};
//    //foo(0.0);
//    //foo(0.2);
//    //foo(0.4);
//    //foo(0.6);
//    //foo(0.8);
//    //foo(1.0);
//
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // TwoPointTransform -- July 30, 2020
//    std::cout << "July 30, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200730_";
//
//    Uniform black(0);
//    Uniform white(1);
//    Vec2 p1(0.1, 0.1);
//    Vec2 p2(-0.1, -0.1);
//    Vec2 p3(0.6, 0.6);
//    Vec2 p4(-0.6, -0.6);
//    Vec2 p5(-0.56, -0.52);
//
//    Texture::diff(Grating (p1, black, p1, white, 0.2, 0.8),
//                  Grating2(p1, black, p1, white, 0.2, 0.8));
//    Texture::diff(Grating (p1, black, p2, white, 1.0, 0.8),
//                  Grating2(p1, black, p2, white, 1.0, 0.8),
//                  // path + "AbsDiff", Texture::getDiffSize(), false);
//                  "", Texture::getDiffSize(), false);
//
//    Texture::diff(Grating (p1, black, p2, white, 1.0, 0.8),
//                  Grating2(p1, black, p2, white, 1.0, 0.8),
//                  // path + "NotEqual", Texture::getDiffSize(), true);
//                  "", Texture::getDiffSize(), true);
//
//    Texture::diff(Gradation (p3, black, p3, white),
//                  Gradation2(p3, black, p3, white));
//    Texture::diff(Gradation (p3, black, p4, white),
//                  Gradation2(p3, black, p4, white),
//                  // path + "Gradation_AbsDiff", Texture::getDiffSize(), false);
//                  "", Texture::getDiffSize(), false);
//    Texture::diff(Gradation (p3, black, p4, white),
//                  Gradation2(p3, black, p4, white),
//                  // path + "Gradation_NotEqual", Texture::getDiffSize(), true);
//                  "", Texture::getDiffSize(), true);
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // TwoPointTransform part 2 -- July 31, 2020
//    std::cout << "July 31, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200731_";
//
//    Uniform black(0);
//    Uniform white(1);
//    Vec2 p1(0.1, 0.1);
//    Vec2 p2(-0.1, -0.1);
//    Vec2 p3(0.6, 0.6);
//    Vec2 p4(-0.6, -0.6);
//    Vec2 p5(-0.56, -0.52);
//
//    Texture::diff(Noise (p4, p4, black, white),
//                  Noise2(p4, p4, black, white));
//    Texture::diff(Noise (p4, p5, black, white),
//                  Noise2(p4, p5, black, white),
//                  // path + "Noise_AbsDiff", Texture::getDiffSize(), false);
//                  "", Texture::getDiffSize(), false);
//    Texture::diff(Noise (p4, p5, black, white),
//                  Noise2(p4, p5, black, white),
//                  // path + "Noise_NotEqual", Texture::getDiffSize(), true);
//                  "", Texture::getDiffSize(), true);
//
//    Uniform gray(0.5);
//    Gradation gg0(Vec2(-0.1, 0), black, Vec2(0.1, 0), gray);
//    Gradation gg1(Vec2(0, -0.1), black, Vec2(0, 0.1), gray);
//    Add gg(gg0, gg1);
//    Spot spot(Vec2(0.4, 0.4), 0.18, black, 0.2, gg);
//
//    // TODO OH, looks like a bug in current Affine for scale=0 transform.
//    Texture::diff(Affine (p1, p1, spot),
//                  Affine2(p1, p1, spot));
//    Texture::diff(Affine (p1, p2, spot),
//                  Affine2(p1, p2, spot),
//                  // path + "Affine_AbsDiff", Texture::getDiffSize(), false);
//                  "", Texture::getDiffSize(), false);
//    Texture::diff(Affine (p1, p2, spot),
//                  Affine2(p1, p2, spot),
//                  // path + "Affine_NotEqual", Texture::getDiffSize(), true);
//                  "", Texture::getDiffSize(), true);
//
//    Texture::displayAndFile(Grating(p1, black, p1, white, 1.0, 0.8));
//    Texture::displayAndFile(Grating(p1, black, p2, white, 1.0, 0.8));
//    Texture::displayAndFile(Gradation(p3, black, p3, white));
//    Texture::displayAndFile(Gradation(p3, black, p4, white));
//    Texture::displayAndFile(Noise(p4, p4, black, white));
//    Texture::displayAndFile(Noise(p4, p5, black, white));
//    Texture::displayAndFile(Affine(p1, p1, spot));
//    Texture::displayAndFile(Affine(p1, p2, spot));
//
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // clean up gamma, remove Texture::deGamma()/reGamma() -- August 2, 2020
//    std::cout << "August 2, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200802_";
//
//    Uniform red(1, 0, 0);
//    Uniform green(0, 1, 0);
//    Uniform magenta(1, 0, 1);
//    //Texture::sqrt_of_aa_subsample_count = 2;
//    Texture::displayAndFile(Spot(Vec2(0.5, 0),
//                                 0.1, red,
//                                 0.5, Spot(Vec2(-0.5, 0),
//                                           0.1, magenta,
//                                           0.5, green)));
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // visualizing some random programs from LazyPredator
//    std::cout << "August 15, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200815_";
//    int i = 0;
//    auto d = [&](const Texture& t)
//    {
//        std::string pathname = path + "random_program_" + std::to_string(i++);
//        Texture::displayAndFile(t, pathname);
//    };
//    // random_program_0
//    d(Twist(-1.5007, -3.12824, Vec2(0.389467, 0.832243), LotsOfButtons(0.862353, 0.326306, 0.00177993, 0.711357, 0.76274, Vec2(3.0448, 3.68129), Spot(Vec2(1.48987, -4.49347), 1.78005, ColorNoise(Vec2(-1.32046, 2.42863), Vec2(-1.91713, 0.940589), 0.84564), 0.764313, ColorNoise(Vec2(0.273154, -1.46148), Vec2(-1.18504, -4.00435), 0.724306)), 0.367926, ColorNoise(Vec2(-1.59627, -3.31247), Vec2(2.23131, -1.37167), 0.698217))));
//    // random_program_1
//    d(ColoredSpots(0.0873328, 0.904293, 0.0523186, 0.0906169, 0.854814, MobiusTransform(Vec2(3.11807, 1.18701), Vec2(-0.589404, 2.75448), Vec2(-3.33792, 2.08031), Vec2(-2.82157, -3.79023), Uniform(0.409955, 0.158851, 0.22622)), BrightnessWrap(0.0941196, 0.596546, EdgeDetect(0.985994, Max(MultiNoise(Vec2(-4.08933, -2.55314), Vec2(-3.52786, 3.43654), Uniform(0.547777, 0.71849, 0.51048), Uniform(0.0970927, 0.656361, 0.669388), 0.0278431), ColorNoise(Vec2(-0.665772, -3.61144), Vec2(3.66564, -4.4826), 0.695869))))));
//    // random_program_2
//    d(Ring(3.11535, Vec2(3.03757, 0.967017), Vec2(-1.42594, -1.81315), SliceShear(Vec2(-4.01977, -3.029), Vec2(-3.63676, 2.93948), Hyperbolic(Vec2(-4.63751, 3.72073), 8.21554, 9.18318, 4.19357, Uniform(0.642181, 0.261001, 0.428011), ColorNoise(Vec2(-4.05045, -1.65673), Vec2(-3.66899, 1.61807), 0.792373)), Vec2(-0.478021, 4.5042), Vec2(4.56995, 0.0112824), ColorNoise(Vec2(3.34685, 3.47989), Vec2(-0.249423, -0.262976), 0.175241))));
//    // random_program_3
//    d(Grating(Vec2(-1.47541, -0.86595), AdjustBrightness(0.0384087, BrightnessWrap(0.805273, 0.469073, BrightnessWrap(0.401083, 0.670738, HueOnly(0.281567, 0.0632304, Min(Uniform(0.427763, 0.769109, 0.859361), ColorNoise(Vec2(2.44752, -4.46087), Vec2(-2.27331, 4.02469), 0.0391472)))))), Vec2(-2.13785, -3.31639), Gamma(1.47854, SliceToRadial(Vec2(-3.44811, -2.35952), Vec2(-3.74908, 1.89859), ColorNoise(Vec2(-2.45512, 3.92944), Vec2(1.00124, 3.80463), 0.657286))), 0.965485, 0.55324));
//    // random_program_4
//     d(EdgeDetect(0.670139, ColoredSpots(0.7614, 0.427715, 0.698671, 0.885313, 0.0558514, Stretch(Vec2(-4.63756, -4.60256), Vec2(-0.0368361, -2.01205), AbsDiff(SoftMatte(Uniform(0.817255, 0.00670789, 0.326614), ColorNoise(Vec2(-1.51223, 2.41655), Vec2(-3.37003, -4.64803), 0.29183), Uniform(0.990324, 0.0578273, 0.848225)), Uniform(0.030859, 0.428472, 0.973189))), Add(SliceToRadial(Vec2(-0.793439, -2.48329), Vec2(4.03724, 2.45832), Uniform(0.442126, 0.573716, 0.949417)), LotsOfButtons(0.270409, 0.145435, 0.790308, 0.703025, 0.736794, Vec2(4.84287, 3.77388), ColorNoise(Vec2(-2.84685, -1.22626), Vec2(-1.97162, -3.88645), 0.157141), 0.0940529, Uniform(0.0183813, 0.9557, 0.00798232))))));
//
//    Texture::waitKey();
    
//    Texture::displayAndFile(Ring(3.11535, Vec2(3.03757, 0.967017), Vec2(-1.42594, -1.81315), SliceShear(Vec2(-4.01977, -3.029), Vec2(-3.63676, 2.93948), Hyperbolic(Vec2(-4.63751, 3.72073), 8.21554, 9.18318, 4.19357, Uniform(0.642181, 0.261001, 0.428011), ColorNoise(Vec2(-4.05045, -1.65673), Vec2(-3.66899, 1.61807), 0.792373)), Vec2(-0.478021, 4.5042), Vec2(4.56995, 0.0112824), ColorNoise(Vec2(3.34685, 3.47989), Vec2(-0.249423, -0.262976), 0.175241))));
//    Texture::displayAndFile(Ring(40, Vec2(3.03757, 0.967017), Vec2(0, 0), SliceShear(Vec2(-4.01977, -3.029), Vec2(-3.63676, 2.93948), Hyperbolic(Vec2(-4.63751, 3.72073), 8.21554, 9.18318, 4.19357, Uniform(0.642181, 0.261001, 0.428011), ColorNoise(Vec2(-4.05045, -1.65673), Vec2(-3.66899, 1.61807), 0.792373)), Vec2(-0.478021, 4.5042), Vec2(4.56995, 0.0112824), ColorNoise(Vec2(3.34685, 3.47989), Vec2(-0.249423, -0.262976), 0.175241))));
//
//    Texture::displayAndFile(Grating(Vec2(0.084042, -3.372581), Wrap(-1.211668, Vec2(4.022692, 3.877531), Vec2(-3.112230, 0.683885), ColorNoise(Vec2(2.435783, -4.575052), Vec2(4.331541, -2.095832), 0.196655)), Vec2(1.814379, -3.739372), LotsOfSpots(0.252883, 0.487457, 0.088388, 0.128435, 0.163529, ColorNoise(Vec2(4.333685, 2.229997), Vec2(4.410281, 1.619063), 0.044797), ColorNoise(Vec2(2.266673, 0.545833), Vec2(-2.298026, 4.235314), 0.004139)), 0.946295, 0.852064));
//
//    Texture::displayAndFile(Twist(0.705134, 1.189807, Vec2(2.909842, -4.278739), Wrap(-2.765690, Vec2(-2.950244, -4.747859), Vec2(2.744241, 2.461258), RgbBox(0.136500, 0.151909, 0.136924, 0.281066, 0.881210, 0.503134, Spot(Vec2(-3.338686, -3.287991), 0.746691, ColorNoise(Vec2(4.022692, 3.877531), Vec2(-2.298026, 4.235314), 0.817780), 0.412435, SliceGrating(Vec2(3.982192, -1.292202), Vec2(1.964732, 1.248409), Twist(1.57598, 0.535471, Vec2(2.429565, 3.697453), AdjustSaturation(0.810464, ColorNoise(Vec2(2.266673, 0.545833), Vec2(-2.298026, 4.235314), 0.004139)))))))));
//
//    Texture::waitKey();
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Try connecting to LazyPredator as a header-only library
//    std::cout << "September 20, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200920_";
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Fill out FunctionSet for TexSyn
//    std::cout << "September 22, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200922_";
//    // part two:
//    path = "/Users/cwr/Desktop/TexSyn_temp/20200923_";
//
//    const FunctionSet& fs = TexSynFS::tinyTexSyn();
//    std::cout << std::endl;
//    fs.print();
//    std::cout << std::endl;
//    LPRS().setSeed();
//    for (int i = 0; i < 30; i++)
//    {
//        GpTree gp_tree;
//        fs.makeRandomTree(100, gp_tree);
//        std::cout << std::endl << gp_tree.to_string() << std::endl;
//        std::cout << "size=" << gp_tree.size() << std::endl;
//        std::any result_as_any = gp_tree.eval();
//        Texture* result = std::any_cast<Texture*>(result_as_any);
//        Texture::displayAndFile(*result
//                                // , path + "texsyn43_" + std::to_string(i)
//                                );
//        Texture::waitKey();
//    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Fill out FunctionSet for TexSyn
//    std::cout << "September 24, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/202009224_";
//
//    const FunctionSet& fs = TexSynFS::tinyTexSyn();
//    fs.print();
//    LPRS().setSeed();
//    //for (int i = 0; i < 30; i++)
//    //{
//    //    GpTree gp_tree;
//    //    fs.makeRandomTree(100, gp_tree);
//    //    std::cout << std::endl << gp_tree.to_string() << std::endl;
//    //    std::cout << "size=" << gp_tree.size() << std::endl;
//    //    std::any result_as_any = gp_tree.eval();
//    //    Texture* result = std::any_cast<Texture*>(result_as_any);
//    //    Texture::displayAndFile(*result
//    //                            , path + "texsyn44_" + std::to_string(i)
//    //                            );
//    //    Texture::waitKey();
//    //}
//
//
//
//    Texture::displayAndFile
//    (EdgeEnhance(0.92443,
//                1.9135,
//                BrightnessToHue(0.420112,
//                                AdjustHue(0.0773456,
//                                          ColorNoise(Vec2(0.867493, -0.123083),
//                                                     Vec2(-0.729388, 0.0366988),
//                                                     0.508404)))),
//     path + "texsyn44_a");
//
//    Texture::displayAndFile
//    (EdgeDetect(0.931086,
//                AdjustBrightness(0.929502,
//                                 SliceToRadial(Vec2(0.277333, -0.691483),
//                                               Vec2(-0.216433, 0.97751),
//                                               Wrap(0.790288,
//                                                    Vec2(-0.572195, -0.432135),
//                                                    Vec2(0.145933, 0.402443),
//                                                    BrightnessToHue(0.307826,
//                                                                    Grating(Vec2(),
//                                                                            Uniform(0),
//                                                                            Vec2(0.1, 0.1),
//                                                                            Uniform(1),
//                                                                            1, 0.5)))))),
//     path + "texsyn44_b");
//
//    Texture::displayAndFile
//    (EdgeDetect(0.931086,
//                AdjustBrightness(0.929502,
//                                 SliceToRadial(Vec2(0.277333, -0.691483),
//                                               Vec2(-0.216433, 0.97751),
//                                               Wrap(0.790288,
//                                                    Vec2(-0.572195, -0.432135),
//                                                    Vec2(0.145933, 0.402443),
//                                                    BrightnessToHue(0.307826,
//                                                                    EdgeEnhance(0.92443,
//                                                                                1.9135,
//                                                                                BrightnessToHue(0.420112,
//                                                                                                AdjustHue(0.0773456,
//                                                                                                          ColorNoise(Vec2(0.867493, -0.123083),
//                                                                                                                     Vec2(-0.729388, 0.0366988),
//                                                                                                                     0.508404))))))))),
//     path + "texsyn44_c");
//    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//        // Experiment with Blur:
//        //     bias samples toward center of kernel
//        //     then just trade off speed with noise by reducing samples per pixel.
//        std::cout << "September 27, 2020" << std::endl;
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200927_";
//
//        Uniform yellow(1, 1, 0);
//        Uniform blue(0, 0, 1);
//        Grating grating(Vec2(), yellow, Vec2(0.2, 0), blue, 0.01, 0.5);
//        float filter_width = 0.1;
//
//        Texture::displayAndFile(grating, path + "grating");
//    //    Texture::displayAndFile(Blur(filter_width, grating));
//        Texture::displayAndFile(BlurN(11, filter_width, grating));
//        Texture::displayAndFile(BlurN(7, filter_width, grating));
//    //    Blur::sqrt_of_subsample_count = 8;
//    //    debugPrint(Blur::sqrt_of_subsample_count);
//    //    Texture::displayAndFile(Blur(filter_width, grating));
//    //    Blur::sqrt_of_subsample_count = 11;
//    //    debugPrint(Blur::sqrt_of_subsample_count);
//    //    Texture::displayAndFile(Blur2(filter_width, grating));
//    //    Texture::displayAndFile(Gradation(Vec2(0, 0.1),
//    //                                      Blur(filter_width, grating),
//    //                                      Vec2(0, -0.1),
//    //                                      Blur2(filter_width, grating)));
//        Texture::displayAndFile(Gradation(Vec2(0, 0.1),
//                                          BlurN(11, filter_width, grating),
//                                          Vec2(0, -0.1),
//                                          BlurN(7, filter_width, grating))
//                                , path + "blur_7_11_compare");
//        //Texture::displayAndFile(CotsMap(Vec2(),
//        //                                Vec2(0.4, 0),
//        //                                Vec2(0.2, 0.2),
//        //                                Vec2(0, 0.4),
//        //                                Blur2(filter_width * 10,
//        //                                      Scale(10, grating))));
//    //    Blur::sqrt_of_subsample_count = 40;
//    //    debugPrint(Blur::sqrt_of_subsample_count);
//        //Texture::displayAndFile(CotsMap(Vec2(),
//        //                                Vec2(0.4, 0),
//        //                                Vec2(0.2, 0.2),
//        //                                Vec2(0, 0.4),
//        //                                Blur2(filter_width * 10,
//        //                                      Scale(10, grating))));
//    //    Texture::displayAndFile(CotsMap(Vec2(),
//    //                                    Vec2(0.4, 0),
//    //                                    Vec2(0.2, 0.2),
//    //                                    Vec2(0, 0.4),
//    //                                    Blur(filter_width * 10,
//    //                                         Scale(10, grating))));
//        Texture::displayAndFile(CotsMap(Vec2(),
//                                        Vec2(0.4, 0),
//                                        Vec2(0.2, 0.2),
//                                        Vec2(0, 0.4),
//                                        BlurN(40,
//                                              filter_width * 10,
//                                              Scale(10, grating)))
//                                , path + "blur_40_spiral");
//    //    Blur::sqrt_of_subsample_count = 8;
//    //    debugPrint(Blur::sqrt_of_subsample_count);
//    //    Texture::displayAndFile(CotsMap(Vec2(),
//    //                                    Vec2(0.4, 0),
//    //                                    Vec2(0.2, 0.2),
//    //                                    Vec2(0, 0.4),
//    //                                    Blur(filter_width * 10,
//    //                                         Scale(10, grating))));
//    //    Blur::sqrt_of_subsample_count = 7;
//    //    debugPrint(Blur::sqrt_of_subsample_count);
//    //    Texture::displayAndFile(CotsMap(Vec2(),
//    //                                    Vec2(0.4, 0),
//    //                                    Vec2(0.2, 0.2),
//    //                                    Vec2(0, 0.4),
//    //                                    Blur(filter_width * 10,
//    //                                         Scale(10, grating))));
//        Texture::displayAndFile(CotsMap(Vec2(),
//                                        Vec2(0.4, 0),
//                                        Vec2(0.2, 0.2),
//                                        Vec2(0, 0.4),
//                                        BlurN(11,
//                                              filter_width * 10,
//                                              Scale(10, grating)))
//                                , path + "blur_11_spiral");
//        Texture::displayAndFile(CotsMap(Vec2(),
//                                        Vec2(0.4, 0),
//                                        Vec2(0.2, 0.2),
//                                        Vec2(0, 0.4),
//                                        BlurN(7,
//                                              filter_width * 10,
//                                              Scale(10, grating)))
//                                , path + "blur_7_spiral");
//        Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Clean up after experiments with Blur.
//    std::cout << "September 28, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20200928_";
//
//    //const FunctionSet& fs = TexSynFS::tinyTexSyn();
//    const FunctionSet& fs = GP::fs();
//    fs.print();
//    LPRS().setSeed();
//    //LPRS().setSeed(367459);
//    for (int i = 0; i < 30; i++)
//    {
//        GpTree gp_tree;
//        fs.makeRandomTree(100, gp_tree);
//        //fs.makeRandomTree(50, gp_tree);
//        std::cout << std::endl << gp_tree.to_string() << std::endl;
//        std::cout << "size=" << gp_tree.size() << std::endl;
//        std::any result_as_any = gp_tree.eval();
//        Texture* result = std::any_cast<Texture*>(result_as_any);
//        //Texture::displayAndFile(*result,
//        //                        path + "texsyn44_" + std::to_string(i));
//        Texture::displayAndFile(*result, "", 255);
//        //Texture::displayAndFile(*result);
//        Texture::waitKey();
//    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Test LazyPredator's new crossover.
//    std::cout << "October 3, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201003_";
//    path = "/Users/cwr/Desktop/TexSyn_temp/20201005_";
//
//    const FunctionSet& fs = GP::fs();
//    fs.print();
//    LPRS().setSeed(58138756); // curved dark brown/blue stripes, uniforms
//    int max_size = 100;
//    GpTree parent_0, parent_1;
//    fs.makeRandomTree(max_size, parent_0);
//    fs.makeRandomTree(max_size, parent_1);
//
//    for (int i = 0; i < 50; i++)
//    {
//        GpTree offspring;
//        fs.crossover(parent_0, parent_1, offspring);
//        std::cout << std::endl << offspring.to_string() << std::endl;
//        std::cout << "size=" << offspring.size() << std::endl;
//        std::any result_as_any = offspring.eval();
//        Texture* result = std::any_cast<Texture*>(result_as_any);
//        Texture::displayAndFile(*result
//                                // , path + "offspring_" + std::to_string(i)
//                                );
//        Texture::waitKey();
//    }
//
//    for (int i = 0; i < 30; i++) debugPrint(LPRS().random2(3, 8));
//    for (int i = 0; i < 30; i++) debugPrint(LPRS().random2(7, 7));
//    for (int i = 0; i < 30; i++) debugPrint(LPRS().random2(1.23f, 1.23f));
//
//    GpType gp_type("Test", 4, 6);
//    for (int i = 0; i < 30; i++)
//        debugPrint(std::any_cast<int>(gp_type.generateEphemeralConstant()));

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Test constructing LazyPredator Population from FunctionSet.
//    std::cout << "October 14, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201014_";
//
//    const FunctionSet& function_set = GP::fs();
//    int population_size = 30;
//    int max_tree_size = 60;
//    Population population(population_size, max_tree_size, function_set);
//    for (auto& individual : population.individuals())
//    {
//        std::any result_as_any = individual->tree().eval();
//        Texture* result = std::any_cast<Texture*>(result_as_any);
//        Texture::displayAndFile(*result);
//        Texture::waitKey();
//    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//        // Testing evolution runs, “yellow/green test”
//        // (was: Test constructing LazyPredator Population from FunctionSet.)
//        std::cout << "October 15-19, 2020" << std::endl;
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201015_";
//        path = "/Users/cwr/Desktop/TexSyn_temp/20201019_";
//
//        Timer t("evolution run");
//        const FunctionSet& function_set = GP::fs();
//    //    int population_size = 10;
//        int population_size = 50;
//        int generation_equivalents = 100;
//        int max_tree_size = 100;
//        Population population(population_size, max_tree_size, function_set);
//
//        for (int i = 0; i < (population_size * generation_equivalents); i++)
//        {
//            // TODO would it be any better to use the same samples for all three
//            //      textures?
//
//    //        auto texture_from_individual = [](Individual* i)
//            auto texture_from_individual = [](Individual* individual)
//            {
//    //            std::any result_as_any = i->treeValue();
//    //            Texture* result = std::any_cast<Texture*>(result_as_any);
//    //            return result;
//                return std::any_cast<Texture*>(individual->treeValue());
//            };
//
//
//    //            auto average_color_metric = []
//    //            (Texture* texture, std::function<float(const Color&)> color_metric)
//    //            {
//    //                int n = 10;
//    //                std::vector<Vec2> samples;
//    //    //            jittered_grid_NxN_in_square(10, 1.4, LPRS(), samples);
//    //                jittered_grid_NxN_in_square(n, 1.4, LPRS(), samples);
//    //                float sum = 0;
//    //                for (auto s : samples) sum += color_metric(texture->getColor(s));
//    //                return sum / sq(n);
//    //            };
//
//            // Added this several days after average_color_metric()
//            // Maybe that should be written in terms of this, or maybe not worth it?
//            auto average_color_of_texture = [] (Texture* texture)
//            {
//                int n = 10;
//                std::vector<Vec2> samples;
//                jittered_grid_NxN_in_square(n, 1.4, LPRS(), samples);
//                Color sum;
//    //            for (auto s : samples) sum += texture->getColor(s);
//                for (auto s : samples) sum += texture->getColor(s).clipToUnitRGB();
//                return sum / sq(n);
//            };
//
//            auto average_color_of_population = [&](Population& population)
//            {
//                Color sum;
//                for (auto individual : population.individuals())
//                {
//                    Texture* texture = texture_from_individual(individual);
//                    sum += average_color_of_texture(texture);
//                }
//                return sum / population.individuals().size();
//            };
//
//    //        auto average_color_metric = [&]
//    //        (Texture* texture, std::function<float(const Color&)> color_metric)
//    //        {
//    //            return color_metric(average_color_of_texture(texture));
//    //        };
//
//
//    //            auto texture_from_individual = [](Individual* i)
//    //            {
//    //    //            std::any result_as_any = i->tree().eval();
//    //                std::any result_as_any = i->treeValue();
//    //                Texture* result = std::any_cast<Texture*>(result_as_any);
//    //    //            assert(result->valid());
//    //                return result;
//    //            };
//
//            // TODO just for debugging
//            Texture* tournament_best;
//
//            auto lowest_average_metric = [&]
//            (Individual* a, Individual* b, Individual* c,
//             Texture* at, Texture* bt, Texture* ct,
//             std::function<float(const Color&)> color_metric)
//            {
//    //            float am = average_color_metric(at, color_metric);
//    //            float bm = average_color_metric(bt, color_metric);
//    //            float cm = average_color_metric(ct, color_metric);
//                float am = color_metric(average_color_of_texture(at));
//                float bm = color_metric(average_color_of_texture(bt));
//                float cm = color_metric(average_color_of_texture(ct));
//
//                // TODO just for debugging
//                tournament_best = ct;
//                if ((am > bm) && (am > cm)) tournament_best = at;
//                if ((bm > am) && (bm > cm)) tournament_best = bt;
//                if ((am < bm) && (am < cm)) return a;
//                if ((bm < am) && (bm < cm)) return b;
//                return c;
//            };
//
//            auto tournament_function = [&]
//            (Individual* a, Individual* b, Individual* c)
//            {
//                Texture* at = texture_from_individual(a);
//                Texture* bt = texture_from_individual(b);
//                Texture* ct = texture_from_individual(c);
//                if (LPRS().frandom01() < 0.5)
//                {
//                    auto high_green = [](const Color& c){ return c.green(); };
//                    return lowest_average_metric(a, b, c, at, bt, ct, high_green);
//                }
//                else
//                {
//                    auto low_blue = [](const Color& c)
//                        { return remapIntervalClip(c.blue(), 0, 1, 1, 0); };
//                    return lowest_average_metric(a, b, c, at, bt, ct, low_blue);
//                }
//            };
//
//            population.evolutionStep(tournament_function, function_set);
//
//            Individual* last_added = Population::last_individual_added;
//    //        assert(last_added->valid());
//    //        std::any result_as_any = last_added->tree().eval();
//            std::any result_as_any = last_added->treeValue();
//            Texture* result = std::any_cast<Texture*>(result_as_any);
//            // TODO just for debugging
//            result = tournament_best;
//            assert(result->valid());
//    // TODO super temp
//    //        Texture::displayAndFile(*result);
//            Texture::displayAndFile(*result, "", 99);
//    //        debugPrint(Population::last_individual_added->tree().size());
//    //        debugPrint(i);
//
//    //        debugPrint(average_color_of_population(population));
//    //        Color c = average_color_of_population(population);
//    //        std::cout << c.r() << "," << c.g() << "," << c.b() << std::endl;
//    //        population.findBestIndividual();
//
//
//
//            Color ac = average_color_of_population(population);
//            std::cout << ac.r() << "," << ac.g() << "," << ac.b() << ",";
//            Individual* best_individual = population.findBestIndividual();
//            Texture* best_texture = texture_from_individual(best_individual);
//            Color bc = average_color_of_texture(best_texture);
//            std::cout << bc.r() << "," << bc.g() << "," << bc.b() << std::endl;
//
//            Texture::waitKey(1);
//            Texture::closeAllWindows();
//
//
//
//        }
//    //    Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Improving evolution API, “yellow/green test”
//    std::cout << "October 22, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201022_";
//
//    GP::runYellowGreenTest();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Testing evolution runs, “colorful, well exposed”
//    std::cout << "October 24, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201024_";
//
//    CWE::run();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Hard limit on LotsOfSpotsBase constructor runtime.
//    std::cout << "November 4, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201104_";
//
//    // Use new default limit on number of spots
//    // LotsOfSpotsBase::max_spots_allowed = 5000;
//    [&](const Texture& spots)
//    {
//        Texture::displayAndFile(spots, path + "limit_spots_1_0");
//        Texture::displayAndFile(Scale(0.2, spots), path + "limit_spots_0_2");
//        Texture::displayAndFile(Scale(0.1, spots), path + "limit_spots_0_1");
//        Texture::displayAndFile(Scale(0.09, spots), path + "limit_spots_0_09");
//    }
//    (LotsOfSpots(0.95,  // density
//                 0.02,  // min radius
//                 0.05,  // max radius
//                 0.01,  // soft edge width
//                 0.01,  // margin
//                 Uniform(1, 1, 0),
//                 Uniform(0, 0, 0.8)));
//
//    // Allow "natural" density of spots given by these parameters.
//    LotsOfSpotsBase::max_spots_allowed = std::numeric_limits<int>::max();
//    [&](const Texture& spots)
//    {
//        Texture::displayAndFile(spots, path + "spots_1_0");
//        Texture::displayAndFile(Scale(0.2, spots), path + "spots_0_2");
//        Texture::displayAndFile(Scale(0.1, spots), path + "spots_0_1");
//        Texture::displayAndFile(Scale(0.09, spots), path + "spots_0_09");
//    }
//    (LotsOfSpots(0.95,  // density
//                 0.02,  // min radius
//                 0.05,  // max radius
//                 0.01,  // soft edge width
//                 0.01,  // margin
//                 Uniform(1, 1, 0),
//                 Uniform(0, 0, 0.8)));
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//        // Prototype simple Fourier transform
//        std::cout << "November 6, 2020" << std::endl;
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201106_";
//
//        Vec2 p05(0.05, 0);
//        Vec2 m05 = -p05;
//        Vec2 p02(0.02, 0);
//        Vec2 m02 = -p02;
//        Vec2 p2(0.2, 0);
//        Vec2 m2 = -p2;
//        [&](const Texture& texture)
//        {
//            Texture::displayAndFile(texture);
//            texture.fft_test();
//        }
//        (Grating(m05, Uniform(0), p05, Uniform(1), 1, 0.5)); // s=0.000108417
//    //    (Grating(m05, Uniform(0), p05, Uniform(1), 0, 0.5)); // s=8.04907e-05
//    //    (Noise(m05, p05, Uniform(0), Uniform(1))); // s=21.129
//    //    (Noise(m02, p02, Uniform(0), Uniform(1))); // s=41.122
//    //    (LotsOfSpots(0.85,0.1,0.3,0.050,0.05,Uniform(0),Uniform(1))); // s=88.3468
//    //    (LotsOfSpots(0.85,0.1,0.3,0.005,0.05,Uniform(0),Uniform(1))); // s=348.793
//    //    (Wrapulence(p2, m2, Uniform(0), Uniform(1))); // s=282.057
//
//        Texture::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Test absolute (non-relative) fitness evolution: "LimitHue"
//    std::cout << "November 13, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201113_";
//    //LPRS().setSeed(20201113);
//    //LPRS().setSeed(20201114);
//    //LPRS().setSeed(20201114 * 2);
//    //LPRS().setSeed(20201114 * 3);
//    LPRS().setSeed(20201116);
//    for (int i = 0; i < 100; i++)
//    {
//        LimitHue::run("/Users/cwr/Desktop/TexSyn_temp/");
//    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // Investigating Texture-related (?) memory leak.
//    std::cout << "November 19, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201119_";
//    
//    bool render = false;
//    auto test = [&]()
//    {
//        const FunctionSet& function_set = GP::fs();
//        int population_size = 10;
//        int max_tree_size = 100;
//        int generation_equivalents = 10;
//        Population population(population_size, max_tree_size, function_set);
//        Texture::leakCheck();
//        // TODO 20201121 try converting Population over to std::shared_ptr
//        //auto fitness = [](Individual& i)
//        auto fitness = [](std::shared_ptr<Individual> i)
//        {
//            //return GP::textureFromIndividual(&i)->getColor(Vec2()).getS();
//            return GP::textureFromIndividual(i)->getColor(Vec2()).getS();
//        };
//        for (int i = 0; i < population_size * generation_equivalents; i++)
//        {
//            if (render)
//            {
//                Texture::closeAllWindows();
//                // TODO 20201121 try converting Population over to std::shared_ptr
//                //Individual* best = population.findBestIndividual();
//                std::shared_ptr<Individual> best = population.findBestIndividual();
//                Texture::displayAndFile(*GP::textureFromIndividual(best));
//                Texture::leakCheck();
//                Texture::waitKey(1);
//            }
//            population.evolutionStep(fitness, function_set);
//        }
//        Texture::leakCheck();
//        if (render) Texture::waitKey();
//    };
//    test();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Investigating Texture-leak / double-delete problem.
//    std::cout << "November 22, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201122_";
//    for (int i = 0; i < 10; i++)
//    {
//        LimitHue::run("/Users/cwr/Desktop/TexSyn_temp/");
//    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Refactor Blur, EdgeDetect, EdgeEnhance.
//    std::cout << "November 23, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201123_";
//
//    // Block to contain lifetime of Textures:
//    {
//        float f = 1;
//        float d = 0.1;
//        Uniform full_green(Color(f, 0, 0));
//        Uniform dark_green(Color(d, 0, 0));
//        Uniform full_red(Color(0, f, 0));
//        Uniform dark_red(Color(0, d, 0));
//        Grating red_stripes(Vec2(0, 0), full_red,
//                            Vec2(0.1, 0.1), dark_red, 0.3, 0.5);
//        Grating green_stripes(Vec2(0, 0), full_green,
//                              Vec2(-0.1, 0.1), dark_green, 0.3, 0.5);
//        Add plaid(red_stripes, green_stripes);
//
//        float width = 0.05;
//        Texture::displayAndFile(plaid);
//        // Note to future self: ...Old versions were just rename of previous.
//        //                      See today's git commit to get that if needed.
//        //Texture::displayAndFile(BlurOld(width, plaid));
//        //Texture::displayAndFile(Blur(width, plaid));
//        //Texture::diff(BlurOld(width, plaid), Blur(width, plaid));
//        //Texture::diff(EdgeDetectOld(width, plaid), EdgeDetect(width, plaid));
//        //Texture::diff(EdgeEnhanceOld(width, 1, plaid),
//        //              EdgeEnhance(width, 1, plaid));
//        Texture::displayAndFile(Blur(width, plaid));
//        Texture::displayAndFile(EdgeDetect(width, plaid));
//        Texture::displayAndFile(EdgeEnhance(width, 2, plaid));
//        Texture::waitKey();
//    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Investigating Texture-leak / double-delete problem.
//    std::cout << "November 23, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201123_";
//
//    for (int i = 0; i < 10; i++)
//    {
//        LimitHue::run("/Users/cwr/Desktop/TexSyn_temp/");
//    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Problem with CotsMap?! Investigating Texture-leak/double-delete problem.
//    std::cout << "November 27, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201127_";
//
//    const FunctionSet& function_set = GP::fs();
//    int max_tree_size = 25;
//    for (int i = 0; i < 100; i++)
//    {
//        GpTree tree;
//        function_set.makeRandomTree(max_tree_size, tree);
//        if (tree.getFunction().name() == "CotsMap")
//        {
//            //debugPrint(tree.to_string(true, "... "));
//            //std::cout << tree.to_string() << std::endl << std::endl;
//            std::cout << tree.to_string(true) << std::endl << std::endl;
//            //std::cout << tree.to_string(true, "... ") << std::endl << std::endl;
//        }
//    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Testing fix to CotsMap.
//    std::cout << "November 28, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201128_";
//
//    for (int i = 0; i < 10; i++)
//    {
//        LimitHue::run("/Users/cwr/Desktop/TexSyn_temp/");
//    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Test memory usage of repeated construction/display of "pure OpenCV" Mat.
//    std::cout << "November 29, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201129_";
//
//    //    for (int i = 0; i < 1000; i++)
//    //    {
//    //        debugPrint(i);
//    //        int size = 800;
//    //        cv::Mat mat(size, size, CV_32FC3, cv::Scalar(1, 0, 0));
//    //        cv::circle(mat,
//    //                   cv::Point2i(rand() % size, rand() % size),
//    //                   50 + rand() % 150,
//    //                   cv::Scalar(0, 1, 1),
//    //                   cv::FILLED);
//    //        std::string window_name = "window";
//    //        cv::namedWindow(window_name);
//    //        cv::moveWindow(window_name, 200, 0);
//    //        cv::imshow(window_name, mat);
//    //        cv::waitKey(1);
//    //        cv::destroyWindow(window_name);
//    //    }
//    //    cv::waitKey();
//
//    std::string window_name = "window";
//    cv::namedWindow(window_name);
//    for (int i = 0; i < 1000; i++)
//    {
//        debugPrint(i);
//        int size = 800;
//        cv::Mat mat(size, size, CV_32FC3, cv::Scalar(1, 0, 0));
//        cv::circle(mat,
//                   cv::Point2i(rand() % size, rand() % size),
//                   50 + rand() % 150,
//                   cv::Scalar(0, 1, 1),
//                   cv::FILLED);
//        cv::imshow(window_name, mat);
//        cv::waitKey(1);
//    }
//    cv::destroyWindow(window_name);
//    cv::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Test/update Texture::windowPlacementTool() for OpenCV 4.5.0.
//    std::cout << "November 30, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201130_";
//
//    Texture::displayAndFile(ColorNoise(Vec2(), Vec2( 0.1,   0.2),  0.4));
//    Texture::displayAndFile(ColorNoise(Vec2(), Vec2( 0.04, -0.05), 0.4));
//    Texture::displayAndFile(ColorNoise(Vec2(), Vec2(-0.02, -0.01), 0.4));
//    for (int i = 0; i < 30; i++)
//    Texture::displayAndFile(ColorNoise(Vec2(), Vec2( 0.1,   0.2),  0));
//
//    //Texture::displayAndFile(ColorNoise(Vec2(), Vec2(0.1, 0.2),  0), "",  511);
//    //Texture::displayAndFile(ColorNoise(Vec2(), Vec2(0.1, 0.2),  0), "", 1023);
//
//    cv::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Test/update Texture::windowPlacementTool() for OpenCV 4.5.0.
//    std::cout << "December 1, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201201_";
//
//    LimitHue::run("");
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // OpenCV bug report: https://github.com/opencv/opencv/issues/19005
//    std::cout << "December 3, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201203_";
//
//    //compare_putTexts("/Users/cwr/Downloads/open-sans/OpenSans-Regular.ttf");
//
//    Vec2 v1(-0.8, 0);
//    Vec2 v2(+0.8, 0);
//    std::cout << "first render" << std::endl;
//    Texture::displayAndFile(Gradation(v1, Uniform(0), v2, Uniform(1)));
//    cv::waitKey(1);
//    std::cout << "second render" << std::endl;
//    Texture::setDefaultOpencvMatType(CV_8UC3);
//    Texture::displayAndFile(Gradation(v1, Uniform(0), v2, Uniform(1)));
//
//    cv::waitKey();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Prototyping GUI.
//    std::cout << "December 4, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201204_";
//
//    Texture::setDefaultOpencvMatType(CV_8UC3);
//    LimitHue::run("");

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Refactor LimitHue from namespace to class.
//    std::cout << "December 7, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201207_";
//
//    // LimitHueOld::run("");
//    //    LimitHue(10, 100, 100).run();
//    //    LimitHue(200, 50, 2000).run();
//    //    LimitHue(200, 20, 4000).run();
//    //    LimitHue(200, 100, 2000).run();
//
//    //    for (int i = 0; i < 20; i ++)
//    //        LimitHue(200, 60, 2000, "/Users/cwr/Desktop/TexSyn_temp/").run();
//
//    for (int i = 0; i < 20; i ++)
//        LimitHue(100, 100, 2000, "/Users/cwr/Desktop/TexSyn_temp/").run();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Refactor LimitHue from namespace to class.
//    std::cout << "December 9, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201209_";
//
//    //    debugPrint(modulo_abs_diff(5, 5, 12));
//    //    debugPrint(modulo_abs_diff(5, 6, 12));
//    //    debugPrint(modulo_abs_diff(6, 5, 12));
//    //    debugPrint(modulo_abs_diff(0, 11, 12));
//    //    debugPrint(modulo_abs_diff(11, 0, 12));
//    //    debugPrint(modulo_abs_diff(1, 11, 12));
//    //    debugPrint(modulo_abs_diff(11, 1, 12));
//    //
//    //    debugPrint(modulo_abs_diff(-1, 1, 12));
//    //    debugPrint(modulo_abs_diff(1, -1, 12));
//
//
//    //    for (int i = 0; i < 12; i++)
//    //    {
//    //        for (int j = 0; j < 12; j++)
//    //        {
//    //            std::cout << "modulo_abs_diff(" << i << ", " << j << ") = ";
//    //            std::cout << modulo_abs_diff(i, j, 12) << std::endl;
//    //        }
//    //    }
//
//    //    for (int i = 0; i < 20; i ++)
//    //        LimitHue(100, 50, 2000, "/Users/cwr/Desktop/TexSyn_temp/").run();
//
//    for (int i = 0; i < 20; i ++)
//        LimitHue(100, 50, 5000, "/Users/cwr/Desktop/TexSyn_temp/").run();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//        // Measure of high frequency in textures.
//        std::cout << "December 10, 2020" << std::endl;
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201210_";
//
//    //    Uniform black(0);
//    //    Uniform white(1);
//    //    Vec2 v0;
//    //    Vec2 v1(1, 1);
//    //    Noise n1(v0, v1, black, white);
//    //    Noise n2(v0, v1 / 10, black, white);
//    //    Noise n3(v0, v1 / 100, black, white);
//    //
//    //    Texture::displayAndFile(n1);
//    //    Texture::displayAndFile(n2);
//    //    Texture::displayAndFile(n3);
//    //
//    //    debugPrint(wiggliness(n1));
//    //    debugPrint(wiggliness(n2));
//    //    debugPrint(wiggliness(n3));
//
//    //    Texture::waitKey();
//
//    //    for (int i = 0; i < 20; i ++)
//    //        LimitHue(100, 50, 5000, "/Users/cwr/Desktop/TexSyn_temp/").run();
//
//        float p = std::numeric_limits<float>::infinity();
//        float q = std::numeric_limits<float>::quiet_NaN();
//        paper_over_abnormal_values(p);
//        paper_over_abnormal_values(q);
//
//        for (int i = 0; i < 20; i ++)
//            LimitHue(100, 30, 2000, "/Users/cwr/Desktop/TexSyn_temp/").run();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Try long run with large programs.
//    std::cout << "December 10, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201210_";
//
//    LimitHue(100, 200, 5000, "/Users/cwr/Desktop/TexSyn_temp/").run();
//    LimitHue(100, 201, 5000, "/Users/cwr/Desktop/TexSyn_temp/").run();
//
//    for (int i = 0; i < 20; i ++)
//    {
//        LimitHue(200, 100, 1000, "/Users/cwr/Desktop/TexSyn_temp/").run();
//        Texture::invalidInstanceReport();
//        Texture::leakCheck();
//        Individual::leakCheck();
//        abnormal_value_report();
//    }
//
//    LimitHue(100, 100, 100, "/Users/cwr/Desktop/TexSyn_temp/").run();
//
//    debugPrint(date_hours_minutes());
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // Set up hyperparameter comparison.
//    std::cout << "December 22, 2020" << std::endl;
//    std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201222_";
//
//    LimitHue::comparison("/Users/cwr/Desktop/TexSyn_temp/");
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//        // Cleaning up GP.h
//        std::cout << "December 25, 2020" << std::endl;
//        std::string path = "/Users/cwr/Desktop/TexSyn_temp/20201225_";
//
//        // TODO 20201226 experiment: measure all initial fitnesses
//
//    //    LimitHue(100, 100, 100, "/Users/cwr/Desktop/TexSyn_temp/").run();
//
//    //    LimitHue(100, 100, 1000, "/Users/cwr/Desktop/TexSyn_temp/").run();
//
//    //    Population::use_uniform_selection_for_absolute_fitness = true;
//    //    LimitHue(100, 100, 1000, "/Users/cwr/Desktop/TexSyn_temp/").run();
//
//    //    LimitHue::comparison("/Users/cwr/Desktop/TexSyn_temp/");
    
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//        // 20201227 build new "absolute fitness" step func based on tournament
//        std::cout << "December 27, 2020" << std::endl;
//        std::string temp_dir = "/Users/cwr/Desktop/TexSyn_temp/";
//        std::string path = temp_dir + "20201227_";
//
//    //    Population::use_uniform_selection_for_absolute_fitness = true;
//    //    LimitHue(100, 100, 100, "/Users/cwr/Desktop/TexSyn_temp/").run();
//    //    LimitHue(100, 100, 1000, "/Users/cwr/Desktop/TexSyn_temp/").run();
//    //    LimitHue(10, 10, 10, temp_dir).run();
//    //    LimitHue(100, 100, 1000, temp_dir).run();
//        LimitHue(100, 100, 100, temp_dir).run();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // 20210101 demes / subpopulations
//    std::cout << "January 1, 2021" << std::endl;
//    std::string temp_dir = "/Users/cwr/Desktop/TexSyn_temp/";
//    std::string path = temp_dir + "20210101_";
    
//    LimitHue(100, 100, 100, temp_dir).run();
    
//    for (int i = 0; i < 10; i++) debugPrint(i % 1);
//    for (int i = 0; i < 10; i++) debugPrint(LPRS().random2(0, 1));
//    for (int i = 0; i < 10; i++) debugPrint(LPRS().randomBool());

//    LimitHue(100, 4, 100, 1000, temp_dir).run();

//    LimitHue::comparison("/Users/cwr/Desktop/TexSyn_temp/");
    
//    LimitHue(100, 4, 100, 100, temp_dir).run();
//    LimitHue(500, 10, 100, 2000, temp_dir).run();
//    LimitHue(100, 4, 100, 100, temp_dir).run();
    
//    LimitHue(100, 4, 100, 2000, temp_dir).run();
    
//    LimitHue::comparison(temp_dir);
    
//    LimitHue(100, 4, 100, 1000, temp_dir).run();
//    LimitHue(101, 5, 100, 1000, temp_dir).run();
//    LimitHue(80, 5, 100, 1000, temp_dir).run();
//    LimitHue(120, 6, 100, 1000, temp_dir).run();
//    LimitHue(120, 7, 100, 1000, temp_dir).run();

//    Population::new_crossover = true;
//    LimitHue(120, 6, 100, 100, temp_dir).run();
//    LimitHue(120, 6, 100, 1000, temp_dir).run();
    
//    LimitHue::comparison(temp_dir);
    
    

//    Vec2 trans(0, 0.5);
//    float factor = 0.7;
//    int slices = 5;
//    ColorNoise cn(Vec2(), Vec2(1, 0), 0.8);
//    RgbBox recolor(0.5, 1.0,  0.4, 0.6,  0.0, 0.1,  cn);
//    Texture::displayAndFile(cn);
//    Texture::displayAndFile(recolor);
//    Texture::displayAndFile
//    (Ring(slices, Vec2(0, 1), Vec2(),
//          Translate(trans,
//                    Scale(factor,
//                          Ring(slices, Vec2(0, 1), Vec2(),
//                               Translate(trans,
//                                         Scale(factor,
//                                               Ring(slices, Vec2(0, 1), Vec2(),
//                                                    Translate(trans,
//                                                              Spot(Vec2(), 0, recolor, 0.4,
//                                                                   Uniform(0)))))))))));
//    Texture::waitKey();
    
//        Population::new_crossover = true;
//    //    LimitHue(120, 6, 100, 100, temp_dir).run();
//    //    LimitHue(20, 2, 100, 500, temp_dir).run();
//        LimitHue(100, 5, 100, 1000, temp_dir).run();

//    LimitHue::comparison(temp_dir);

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//        // 20210101 fix bug in new crossover min/max tree size limits.
//        std::cout << "January 18, 2021" << std::endl;
//        std::string temp_dir = "/Users/cwr/Desktop/TexSyn_temp/";
//        std::string path = temp_dir + "20210118_";
//
//
//    //    LimitHue(100, 5, 100, 1000, temp_dir).run();
//        LimitHue(100, 5, 100, 100, temp_dir).run();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // 20210121 prototyping Camouflage class
//    std::cout << "January 21, 2021" << std::endl;
//    std::string temp_dir = "/Users/cwr/Desktop/TexSyn_temp/";
//    std::string path = temp_dir + "20210121_";
//
//    std::string run_name = "oak leaf litter green brown";
//    std::string bg_dir_dir = "/Users/cwr/Pictures/camouflage backgrounds/";
//    std::string bg_image_dir = bg_dir_dir + run_name;
//    float bg_scale = 0.5;
//    Camouflage(run_name, bg_image_dir, bg_scale).run();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // 20210121 prototyping Camouflage class
//    std::cout << "January 31, 2021" << std::endl;
//    std::string temp_dir = "/Users/cwr/Desktop/TexSyn_temp/";
//    std::string path = temp_dir + "20210131_";
//
//    LimitHue(100, 5, 100, 100, temp_dir).run();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // 20210208 prototyping Camouflage class
//    std::cout << "Feburary 8, 2021" << std::endl;
//    std::string temp_dir = "/Users/cwr/Desktop/TexSyn_temp/";
//    std::string path = temp_dir + "20210208_";
//
//    Camouflage(argc, argv).run();
//
//    // Sample command line:
//    // texsyn /Users/cwr/Pictures/camouflage\ backgrounds/oak\ leaf\ litter\ green\ brown .
//    // texsyn /Users/cwr/Pictures/camouflage_backgrounds/oak_leaf_litter_green_brown .
//    // texsyn /Users/cwr/Pictures/camouflage_backgrounds/fungus_pores/ . 1.2 20210211

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // 20210215 prototyping CommandLine class
//    std::cout << "Feburary 15, 2021" << std::endl;
//
//    //    CommandLine cl(0, nullptr);
//    //    cl.test();
//
//    Camouflage(argc, argv).run();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // 20210307 prototyping Gabor Noise texture.
//    std::cout << "March 7, 2021" << std::endl;
//
//    {
//        Uniform c0(1, 0.5, 0);
//        Uniform c1(0, 0.5, 1);
//        Texture::displayAndFile(GaborNoiseTest(600,
//                                               0.1, 0.2,
//                                               0.01, 0.02,
//                                               0, pi / 2,
//                                               c0, c1));
//        Texture::displayAndFile(Scale(0.14,
//                                      GaborNoiseTest(600,
//                                                     0.1, 0.2,
//                                                     0.01, 0.02,
//                                                     0, pi / 2,
//                                                     c0, c1)) );

//        {
//            Timer t("GaborNoiseTestLinear");
//            Texture::displayAndFile(GaborNoiseTestLinear(1000,
//                                                         0.1, 0.2,
//                                                         0.01, 0.02,
//                                                         0, pi / 2,
//                                                         c0, c1));
//        }
//        {
//            Timer t("GaborNoiseTestGrid");
//            Texture::displayAndFile(GaborNoiseTestGrid(1000,
//                                                       0.1, 0.2,
//                                                       0.01, 0.02,
//                                                       0, pi / 2,
//                                                       c0, c1));
//        }

//            {
//                Timer t("GaborNoiseTestLinear");
//                Texture::displayAndFile(GaborNoiseTestLinear(500,
//                                                             0.2, 0.5,
//                                                             0.01, 0.02,
//                                                             0, pi / 2,
//                                                             c0, c1));
//            }
//            {
//                Timer t("GaborNoiseTestGrid");
//                Texture::displayAndFile(GaborNoiseTestGrid(500,
//                                                           0.2, 0.5,
//                                                           0.01, 0.02,
//                                                           0, pi / 2,
//                                                           c0, c1));
//            }
//            {
//                Timer t("TestGaborKernel");
//                Texture::displayAndFile(TestGaborKernel(500,
//                                                        0.2, 0.5,
//                                                        0.01, 0.02,
//                                                        0, pi / 2,
//                                                        c0, c1));
//            }
//            {
//                Timer t("TestGaborKernelTwo");
//                Texture::displayAndFile(TestGaborKernelTwo(500,
//                                                           0.2, 0.5,
//                                                           0.01, 0.02,
//                                                           0, pi / 2,
//                                                           c0, c1));
//            }
//            {
//                Timer t("GaborNoise");
//                Texture::displayAndFile(GaborNoise(500,
//    //                                               0.2, 0.5,
//    //                                               0.5, 1.0,
//    //                                               0.2, 0.8,
//    //                                               0.2, 0.3,
//    //                                               0.6, 0.8,
//                                                   0.3, 0.4,
//    //                                               0.01, 0.02,
//    //                                               0.01, 0.05,
//                                                   0.01, 0.01,
//    //                                               0, pi / 2,
//                                                   0, pi / 4,
//                                                   c0, c1));
//            }
//            {
//                Timer t("GaborNoise zoomed out");
//                Texture::displayAndFile(Scale(0.14,
//                                              GaborNoise(500,
//                                                         0.3, 0.4,
//                                                         0.01, 0.01,
//                                                         0, pi / 4,
//                                                         c0, c1)));
//            }
//
//            {
//                Timer t("GaborNoise very sparse");
//                std::cout << std::endl << "GaborNoise very sparse" << std::endl;
//                GaborNoise::max = 0;
//                GaborNoise::min = 0;
//
//    //            Texture::displayAndFile(GaborNoise(100,
//    //            Texture::displayAndFile(GaborNoise(150,
//                Texture::displayAndFile(GaborNoise(2,
//                                                   0.3, 0.4,
//    //                                               0.4, 0.6,
//                                                   0.01, 0.01,
//                                                   0, pi / 4,
//                                                   c0, c1));
//                debugPrint(GaborNoise::max);
//                debugPrint(GaborNoise::min);
//            }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    // 20210328 Gabor kernel noise experiment
//    std::cout << "March 28, 2021" << std::endl;
//    std::string temp_dir = "/Users/cwr/Desktop/TexSyn_temp/";
//    std::string path = temp_dir + "20210328_";
//    Uniform c0(1.0, 0.5, 0.1);
//    Uniform c1(0.1, 0.5, 1.0);
//    {
//        Timer t("GaborNoise A");
//        Texture::displayAndFile(GaborNoisePrototype(1000,
//                                                    0.3, 0.6,
//                                                    0.01, 0.03,
//                                                    0, pi / 4,
//                                                    c0, c1)
//                                );  // , path + "GaborNoise_test_a");
//    }
//    {
//        Timer t("GaborNoise B");
//        Texture::displayAndFile(GaborNoisePrototype(1000,
//                                                    0.2, 0.8,
//                                                    0.01, 0.03,
//                                                    pi * 0.33, pi * 0.66,
//                                                    c0, c1)
//                                );  // , path + "GaborNoise_test_b");
//    }
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // 20210316 tag the 0.9.4 "release"
//    std::cout << "March 16, 2021" << std::endl;
//
//    Camouflage(argc, argv).run();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
    // TODO 20210401 very temp test
    
    // 20210401 Gabor kernel noise experiment
    std::cout << "April 1, 2021" << std::endl;
    std::string temp_dir = "/Users/cwr/Desktop/TexSyn_temp/";
    std::string path = temp_dir + "20210401_";

    
    // Original
    Texture::displayAndFile(Mirror(Vec2(3.21816, -3.75822),
                                   Vec2(3.35559, -2.45618),
                                   LotsOfButtons(0.525498,
                                                 0.961501,
                                                 0.142252,
                                                 0.435903,
                                                 0.113291,
                                                 Vec2(1.57994, 4.01201),
                                                 Gradation(Vec2(1.80012, -2.66427),
                                                           Gamma(9.51904,
                                                                 Uniform(0.960211, 0.096008, 0.597687)),
                                                           Vec2(1.78565, 2.38711),
                                                           Blur(0.409173,
                                                                BrightnessWrap(0.271493,
                                                                               0.0877538,
                                                                               BrightnessToHue(0.55085,
                                                                                               AdjustSaturation(0.231542,
                                                                                                                EdgeDetect(0.598311,
                                                                                                                           Uniform(0.814141, 0.52608, 0.431994))))))),
                                                 0.754812,
                                                 Twist(2.92905,
                                                       1.55438,
                                                       Vec2(-2.18284, 4.04233),
                                                       BrightnessWrap(0.887923,
                                                                      0.72951,
                                                                      Affine(Vec2(-0.3471, 4.48348),
                                                                             Vec2(-3.05151, -0.422401),
                                                                             AdjustBrightness(0.635429,
                                                                                              Subtract(Uniform(0.165418, 0.545178, 0.589462),
                                                                                                       BrightnessWrap(0.940368,
                                                                                                                      0.6996,
                                                                                                                      Uniform(0.538748, 0.0987566, 0.542896)))))))))
                            );  //  , path + "original");
    
    Texture::displayAndFile(Mirror(Vec2(3.21816, -3.75822).normalize(),
                                   Vec2(3.35559, -2.45618),
                                   LotsOfButtons(0.525498,
                                                 0.961501,
                                                 0.142252,
                                                 0.435903,
                                                 0.113291,
                                                 Vec2(1.57994, 4.01201),
                                                 Gradation(Vec2(1.80012, -2.66427),
                                                           Gamma(9.51904,
                                                                 Uniform(0.960211, 0.096008, 0.597687)),
                                                           Vec2(1.78565, 2.38711),
                                                           Blur(0.409173,
                                                                BrightnessWrap(0.271493,
                                                                               0.0877538,
                                                                               BrightnessToHue(0.55085,
                                                                                               AdjustSaturation(0.231542,
                                                                                                                EdgeDetect(0.598311,
                                                                                                                           Uniform(0.814141, 0.52608, 0.431994))))))),
                                                 0.754812,
                                                 Twist(2.92905,
                                                       1.55438,
                                                       Vec2(-2.18284, 4.04233),
                                                       BrightnessWrap(0.887923,
                                                                      0.72951,
                                                                      Affine(Vec2(-0.3471, 4.48348),
                                                                             Vec2(-3.05151, -0.422401),
                                                                             AdjustBrightness(0.635429,
                                                                                              Subtract(Uniform(0.165418, 0.545178, 0.589462),
                                                                                                       BrightnessWrap(0.940368,
                                                                                                                      0.6996,
                                                                                                                      Uniform(0.538748, 0.0987566, 0.542896)))))))))
                            );  //  path + "normalized");

    //LotsOfButtons(<#float _spot_density#>, <#float _min_radius#>, <#float _max_radius#>, <#float _soft_edge_width#>, <#float _margin#>, <#Vec2 _button_center#>, <#const Texture &_button_texture#>, <#float _button_random_rotate#>, <#const Texture &_background_texture#>)
    Texture::displayAndFile(LotsOfButtons(0.525498,
                                          0.961501,
                                          0.142252,
                                          0.435903,
                                          0.113291,
                                          Vec2(1.57994, 4.01201),
                                          Gradation(Vec2(1.80012, -2.66427),
                                                    Gamma(9.51904,
                                                          Uniform(0.960211, 0.096008, 0.597687)),
                                                    Vec2(1.78565, 2.38711),
                                                    Blur(0.409173,
                                                         BrightnessWrap(0.271493,
                                                                        0.0877538,
                                                                        BrightnessToHue(0.55085,
                                                                                        AdjustSaturation(0.231542,
                                                                                                         EdgeDetect(0.598311,
                                                                                                                    Uniform(0.814141, 0.52608, 0.431994))))))),
                                          0.754812,
                                          Twist(2.92905,
                                                1.55438,
                                                Vec2(-2.18284, 4.04233),
                                                BrightnessWrap(0.887923,
                                                               0.72951,
                                                               Affine(Vec2(-0.3471, 4.48348),
                                                                      Vec2(-3.05151, -0.422401),
                                                                      AdjustBrightness(0.635429,
                                                                                       Subtract(Uniform(0.165418, 0.545178, 0.589462),
                                                                                                BrightnessWrap(0.940368,
                                                                                                               0.6996,
                                                                                                               Uniform(0.538748, 0.0987566, 0.542896))))))))
                            );  //  path + "buttons");
    
    Uniform red(1, 0, 0);
    Uniform blue(0, 0, 1);
    Texture::displayAndFile(Mirror(Vec2(3.21816, -3.75822),
                                   Vec2(3.35559, -2.45618),
                                   Spot(Vec2(), 0, red, 0.5, blue)));
    // debugPrint(Vec2(3.21816, -3.75822).length());
    // Vec2(3.21816, -3.75822).length() = 4.9478
    
    // Button texture:
    Texture::displayAndFile
    (Gradation(Vec2(1.80012, -2.66427),
               Gamma(9.51904,
                     Uniform(0.960211, 0.096008, 0.597687)),
               Vec2(1.78565, 2.38711),
               Blur(0.409173,
                    BrightnessWrap(0.271493,
                                   0.0877538,
                                   BrightnessToHue(0.55085,
                                                   AdjustSaturation(0.231542,
                                                                    EdgeDetect(0.598311,
                                                                               Uniform(0.814141, 0.52608, 0.431994))))))));
    
    // Background texture:
    Texture::displayAndFile(Twist(2.92905,
                                  1.55438,
                                  Vec2(-2.18284, 4.04233),
                                  BrightnessWrap(0.887923,
                                                 0.72951,
                                                 Affine(Vec2(-0.3471, 4.48348),
                                                        Vec2(-3.05151, -0.422401),
                                                        AdjustBrightness(0.635429,
                                                                         Subtract(Uniform(0.165418, 0.545178, 0.589462),
                                                                                  BrightnessWrap(0.940368,
                                                                                                 0.6996,
                                                                                                 Uniform(0.538748, 0.0987566, 0.542896))))))));
    
//    Texture::displayAndFile(Scale(0.14,
    Texture::displayAndFile(Scale(0.07,
                                  Mirror(Vec2(3.21816, -3.75822),
                                         Vec2(3.35559, -2.45618),
                                         LotsOfButtons(0.525498,
                                                       0.961501,
                                                       0.142252,
                                                       0.435903,
                                                       0.113291,
                                                       Vec2(1.57994, 4.01201),
                                                       Gradation(Vec2(1.80012, -2.66427),
                                                                 Gamma(9.51904,
                                                                       Uniform(0.960211, 0.096008, 0.597687)),
                                                                 Vec2(1.78565, 2.38711),
                                                                 Blur(0.409173,
                                                                      BrightnessWrap(0.271493,
                                                                                     0.0877538,
                                                                                     BrightnessToHue(0.55085,
                                                                                                     AdjustSaturation(0.231542,
                                                                                                                      EdgeDetect(0.598311,
                                                                                                                                 Uniform(0.814141, 0.52608, 0.431994))))))),
                                                       0.754812,
                                                       Twist(2.92905,
                                                             1.55438,
                                                             Vec2(-2.18284, 4.04233),
                                                             BrightnessWrap(0.887923,
                                                                            0.72951,
                                                                            Affine(Vec2(-0.3471, 4.48348),
                                                                                   Vec2(-3.05151, -0.422401),
                                                                                   AdjustBrightness(0.635429,
                                                                                                    Subtract(Uniform(0.165418, 0.545178, 0.589462),
                                                                                                             BrightnessWrap(0.940368,
                                                                                                                            0.6996,
                                                                                                                            Uniform(0.538748, 0.0987566, 0.542896)))))))))));

    Texture::waitKey();
 */
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // 20210403 refactor Camouflage constructor to take a CommandLine argument.
//    std::cout << "April 3, 2021" << std::endl;
//
//    // Obsolete:
//    // Camouflage(argc, argv).run();
//
//    // To build command:
//    // Camouflage(CommandLine(argc, argv)).run();
//
//    // For debugging from here inside Xcode.
//    CommandLine cmdlin({"evo_camo_game",
//                        "/Users/cwr/Pictures/camouflage_backgrounds/clover",
//                        "/Users/cwr/Desktop/TexSyn_temp/",
//                        "0.5",
//                        "63612358"});
//    Camouflage(cmdlin).run();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // 20210405 tag the evo_camo_game 0.9.5 "release"
//    std::cout << "April 5, 2021" << std::endl;
//
//    Camouflage(CommandLine(argc, argv)).run();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // 20210407 testing phasor noise.
//    std::cout << "April 11, 2021" << std::endl;
//    std::string temp_dir = "/Users/cwr/Desktop/TexSyn_temp/";
//    std::string path = temp_dir + "20210411_";
//
//    //    {
//    //        Timer timer("PhasorNoiseWrapper");
//    //        Texture::setDefaultRenderAsDisk(false);
//    //        // Texture::displayAndFile(PhasorNoiseWrapper());
//    //
//    //        Texture::displayAndFile(PhasorNoiseWrapper()
//    //                                ); // , path + "first_phasor");
//    //
//    //        // Each rendered pixel uses an NxN jittered grid of subsamples...
//    //        Texture::sqrt_of_aa_subsample_count = 3;
//    //
//    //        Texture::displayAndFile(PhasorNoiseWrapper()
//    //                                ); // , path + "first_phasor_3_3");
//    //    }
//
//    Uniform red_orange(1, 0.3, 0);
//    Uniform orange(1, 0.5, 0);
//    Uniform blue(0, 0, 1);
//    Uniform cyan_blue(0, 0.5, 1);
//
//    [&](const Texture& texture0, const Texture& texture1)
//    {
//        Timer timer("PhasorNoiseWrapper");
//        Texture::displayAndFile(PhasorNoisePrototype(texture0, texture1)
//                                );  // , path + "color0");
//    }
//    (LotsOfSpots(0.8, 0.2, 0.3, 0.04, 0.05, red_orange, orange),
//     Grating(Vec2(), blue, Vec2(0.1, 0.2), cyan_blue, 0.5, 0.3));
//
//    //    [&](const Texture& texture0, const Texture& texture1)
//    //    {
//    //        Timer timer("PhasorNoiseWrapper");
//    //        Texture::setDefaultRenderAsDisk(false);
//    //        Texture::displayAndFile(PhasorNoiseWrapper(texture0, texture1)
//    //                                );  // , path + "first_light");
//    //    }
//    //    (Uniform(0), Uniform(1));
//
//    Texture::waitKey();

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//    // 20210413 to launch run redwood_leaf_litter_20210413_1441
//    std::cout << "April 13, 2021" << std::endl;
//    Camouflage(CommandLine(argc, argv)).run();
    
    [&](const Texture& texture0, const Texture& texture1)
    {
        Timer timer("PhasorNoiseWrapper");
        Texture::setDefaultRenderAsDisk(false);
//        Texture::displayAndFile(PhasorNoisePrototype(texture0, texture1)
//                                );  // , path + "first_light");
        
        Texture::displayAndFile(PhasorNoisePrototype(1000,
                                                     0.2, 0.8,
                                                     0.01, 0.03,
                                                     pi * 0.33, pi * 0.66,
                                                     texture0, texture1));
    }
    (Uniform(0), Uniform(1));
    
    Texture::waitKey();


    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // 20210407 testing phasor noise.
    std::cout << "April 11, 2021" << std::endl;
    std::string temp_dir = "/Users/cwr/Desktop/TexSyn_temp/";
    std::string path = temp_dir + "20210411_";

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Texture::invalidInstanceReport();
    Texture::leakCheck();
    Individual::leakCheck();
    abnormal_value_report();
    return EXIT_SUCCESS;
}
