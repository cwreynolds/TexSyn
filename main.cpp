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
    auto exit_code = EXIT_FAILURE;
    std::cout << texsyn_version_string << std::endl;
    if (UnitTests::allTestsOK())
    {
        exit_code = EXIT_SUCCESS;
        std::cout << "September 22, 2023" << std::endl;
        auto cmd = CommandLine(argc, argv);
        // Run Coevolution of Camouflage, specifically "EvoCamoVsLearnPredPop",
        // the version which pits evolutionary camouflage against a population
        // of learning predators. (Code fragments below based on experiments run
        // between October 2022 and March 2023. See: https://bit.ly/3sV0bUt)
        bool use_static_quality_metric = false;
        if (use_static_quality_metric)
        {
            EvoCamoVsLppSqm(cmd).run();        // Include SQM, runs much slower
        }
        else
        {
            EvoCamoVsLearnPredPop(cmd).run();  // Normal COC run, about 6 hours.
        }
        // If no cmd line arguments, run various_examples() as "proof of life."
        if (cmd.parsedTokens().size() == 1)
        {
            exit_code = EXIT_FAILURE;
            std::cout << "Various examples as proof of life." << std::endl;
            UnitTests::various_examples();
            std::cout << "Type any key to proceed." << std::endl << std::endl;
            Texture::waitKey();
        }
        Texture::invalidInstanceReport();
        Texture::leakCheck();
        Individual::leakCheck();
        abnormal_value_report();
    }
    return exit_code;
}

//------------------------------------------------------------------------------
// Usage notes for running Coevolution of Camouflage from the command line.
//------------------------------------------------------------------------------
//
// Use this code to run “Coevolution of Camouflage” as described in the paper:
//
//     Craig Reynolds. 2023. Coevolution of Camouflage. 2023 Artificial Life
//     Conference. MIT Press. https://doi.org/10.1162/isal_a_00583
//         Preprint with supplemental materials and high resolution images:
//         https://arxiv.org/abs/2304.11793
//
// To run a simulation, two processes are launched, one to run camouflage
// evolution using TexSyn and LazyPredator, the other to run predator evolution
// with PredatorEye. I do this by opening two shell (Terminal on macOS) windows
// and launching a command in each one. In one shell enter these commands:
//
// [Note these pathnames are from my own laptop, you will need to adapt for your
// own environemnt. Contact me (cwr@red3d.com) for help with that if needed.]
//
//    cd ~/Documents/code/PredatorEye
//    conda activate TensorFlow_GPU_M1
//    python3 EvoCamoVsLearnPredPop.py 40
//
// Then in the other window (typical usage):
//
//     caffeinate texsyn ~/Pictures/camouflage_backgrounds/oak_leaf_litter ~/Desktop/TexSyn_temp/ 0.25 20230921 512 512 400 20
//
// At this point both processes should run, trading images and other state via
// the file system. To the extent possible, they execute in parallel.
//
// "caffeinate" is a macOS utility to prevent the laptop from sleeping while the
// command is running.
//
// The parameters to the texsyn side:
//   Requires at least one pathname parameter, others may be omitted from end:
//     background_image_directory (required)
//     output_directory (defaults to .)
//     background_scale (defaults to 0.5)
//     random_seed (else: default seed)
//     window width (defaults to 1200)
//     window height (defaults to 800)
//     individuals (defaults to 120)
//     subpopulations (defaults to 6)
//     max_steps (defaults to 12000)
//     max_init_tree_size (defaults to 100)
//     min_crossover_tree_size (default max_init_tree_size_ * 0.5)
//     max_crossover_tree_size (default max_init_tree_size_ * 1.5)
//
// My habit is to use the date, as YYYYMMDD as the random number seed for unique
// runs. Add a digit at the end if making multiple runs in a day. A directory
// for each run is created under the "output_directory" named for the background
// set, the date, and time.
//
// Toward the end of CoC experiments 12000 steps was a typical run length, with
// 400 prey individuals and 40 predators. This is refered to in comments as a
// “super heavy” run. Shorter runs and smaller populations were used earlier in
// the research with mixed success. See section “Background Sets” and Figure 7
// in https://doi.org/10.1162/isal_a_00583 for more detail.
//
//------------------------------------------------------------------------------
