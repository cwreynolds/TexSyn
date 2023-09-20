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
    std::cout << "September 19, 2023" << std::endl;
    auto cmd = CommandLine(argc, argv);
    if (cmd.parsedTokens().size() == 1)
    {
        // When command given with no arguments, just run various_examples().
        UnitTests::various_examples();
    }
    else
    {
        // Otherwise run Coevolution of Camouflage, specifically the version
        // that pits evolutionary camouflage against a population of predators
        // ("EvoCamoVsLearnPredPop").
        //
        // As used in earlier experiments. See here: https://bit.ly/3sV0bUt
        EvoCamoVsLearnPredPop(cmd).run();  // Normal COC run.
        // EvoCamoVsLppSqm(cmd).run();     // Include SQM, runs much slower
    }
    std::cout << "Type any key to proceed." << std::endl;
    Texture::waitKey();
    Texture::invalidInstanceReport();
    Texture::leakCheck();
    Individual::leakCheck();
    abnormal_value_report();
    return EXIT_SUCCESS;
}


// Notes on running Coevolution of Camouflage from the command line:
// requires at least one pathname parameter, others may be omitted from the end:
//    background_image_directory (required)
//    output_directory (defaults to .)
//    background_scale (defaults to 0.5)
//    random_seed (else: default seed)
//    window width (defaults to 1200)
//    window height (defaults to 800)
//    individuals (defaults to 120)
//    subpopulations (defaults to 6)
//    max_init_tree_size (defaults to 100)
//    min_crossover_tree_size (default max_init_tree_size_ * 0.5)
//    max_crossover_tree_size (default max_init_tree_size_ * 1.5)

// See doc in EvoCamoGame.h and in https://cwreynolds.github.io/TexSyn/
// Typical usage is something like:
//     caffeinate texsyn ~/Pictures/camouflage_backgrounds/oak_leaf_litter
//                       ~/Desktop/TexSyn_temp/
//                       0.25
//                       20230308
//                       512 512
//                       400
//                       20

