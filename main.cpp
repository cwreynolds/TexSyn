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
    if (!UnitTests::allTestsOK()) { return EXIT_FAILURE; }
    std::cout << "July 20, 2023" << std::endl;

    UnitTests::various_examples();
    
    std::cout << "Type any key to proceed." << std::endl;
    Texture::waitKey();
    Texture::invalidInstanceReport();
    Texture::leakCheck();
    Individual::leakCheck();
    abnormal_value_report();
    return EXIT_SUCCESS;
}
