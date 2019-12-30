//
//  main.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Texture.h"
#include "Generators.h"
#include "UnitTests.h"
#include "Utilities.h"

int main(int argc, const char * argv[])
{
    // TODO TEMP run unit tests
    UnitTests::allTestsOK();
    
    // TODO TEMP testing display utility and generators.
//    Spot spot(Vec2(0, 0), 0.2, Color(1, 1, 1), 0.6, Color(0, 0, 0));
//    spot.displayInWindow();
    Grating grating(Vec2(0.1, 0.1), Color(0, 0.8, 0),
                    Vec2(0.5, 0.3), Color(1, 1, 0), 0.3);
    grating.displayInWindow();

    return EXIT_SUCCESS;
}
