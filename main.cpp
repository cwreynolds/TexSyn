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
    
    // TODO TEMP testing display utility and generators.
//    Spot spot(Vec2(0, 0), 0.2, Color(1, 1, 1), 0.6, Color(0, 0, 0));
//    spot.displayInWindow();
    
//    Grating grating(Vec2(0.1, 0.1), Color(0, 0.8, 0),
//                    Vec2(0.5, 0.3), Color(1, 1, 0), 0.3);
//    grating.displayInWindow();
    
//    Gradation gradation(Vec2(0.4, -0.2), Color(0, 0.8, 0),
//                        Vec2(-0.4, 0.2), Color(1, 1, 1));
//    gradation.displayInWindow();
    
//    SoftMatte sm(Spot(Vec2(0, 0), 0.2, Color(1, 1, 1), 0.8, Color(0, 0, 0)),
//                 Grating(Vec2(-0.2, 0), Color(0, 0, 0),
//                         Vec2( 0.2, 0), Color(1, 1, 1), 1),
//                 Grating(Vec2(0, -0.1), Color(1, 0, 1),
//                         Vec2(0,  0.1), Color(0, 0, 1), 0.2));
//    sm.displayInWindow();

//    Max mx(Grating(Vec2(-0.2, 0), Color(0, 0, 0),
//                   Vec2( 0.2, 0), Color(1, 1, 1), 1),
//           Grating(Vec2(0, -0.1), Color(1, 0, 1),
//                   Vec2(0,  0.1), Color(0, 0, 1), 0.2));
//    mx.displayInWindow();

//    Noise no(0.10, Vec2(3, 5), Color(0, 0, 1), Color(1, 1, 0));
//    no.displayInWindow();
    
//    Brownian br(0.20, Vec2(3, 5), Color(0, 1, 0), Color(0.3, 0.3, 0.3));
//    br.displayInWindow();
    
//    Turbulence tu(0.3, Vec2(2, -5), Color(0.3, 0, 0.3), Color(1, 0.6, 0));
//    tu.displayInWindow();

//    Furbulence fu(0.25, Vec2(-1, 2), Color(1, .1, .3), Color(.3, .1, 1));
//    fu.displayInWindow();

    Wrapulence wr(0.9, Vec2(-2, -9), Color(0, 0, 0), Color(1, 0.6, 0));
    wr.displayInWindow();

    return EXIT_SUCCESS;
}
