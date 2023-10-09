# TexSyn
## Library for evolutionary texture synthesis.

**TexSyn** is a library for procedural texture synthesis. It is primarily intended for use by a _genetic programming_ (“GP”) system, a type of _genetic algorithm_. The GP system performs _simulated evolution_ on a _population_ of programs, according to a _fitness function_ (aka _fitness metric_, _utility function_, or _loss function_.) For a GP texture synthesis application, the programs are compositions of functions from this **TexSyn** library. When executed they describe a _color texture_ — which is to say — an _image_. As an example, this code:
```
    Noise(0.2, Vec2(1, 2),
          Furbulence(0.1, Vec2(3, -2), Uniform(1, 1, 0), Uniform(1, 0, 0)),
          Brownian(0.1, Vec2(-1, 5), Uniform(0, 0, 1), Uniform(0, 1, 1)));
```
produces a `Texture` object (two noise textures of two uniform colors each, modulated by a third noise texture) which can be rendered as in this image:

![rendered texture](https://cwreynolds.github.io/TexSyn/images/20200524_less_trivial.png)

For more information, including other operators, and many sample textures, see **TexSyn**'s [devo blog](https://cwreynolds.github.io/TexSyn/).

**TexSyn** is written in c++ (gnu++17) and structured as a “header only” library (as of version 2.1). All that is required to use it is to clone this git repository (or just copy the source files) and `#include "TexSyn.h"`. In addition there is an optional `main.cpp` which can be built to verify that the library is functioning as expected: it runs a unit test suite and displays some sample synthesized textures. You can also run these tests from your own code, e.g. `UnitTests::allTestsOK()`. Building the `main.cpp` test app can be done with with CMake (`CMakeLists.txt`) or macOS Xcode (`TexSyn.xcodeproj`).

**TexSyn**'s companion genetic programming system is [LazyPredator](https://github.com/cwreynolds/LazyPredator).

The research which **TexSyn** was built to support has been completed. It is described in the paper _Coevolution of Camouflage_ available as a [preprint on arXiv](https://arxiv.org/abs/2304.11793). That paper will be presented at, and published in the proceedings of, the [2023 Artificial Life conference](https://alife.org/conference/alife-2023/) in Sapporo, Japan, July 24-28, 2023. There will be a [poster](https://doi.org/10.1145/3588028.3603663) about this work at SIGGRAPH 2023 in Los Angeles, California, August 6-10.

In that research, **TexSyn** was used with **LazyPredator** to evolve artificial camouflage patterns which were “cryptic” against background images. The fitness metric that drives camouflage evolution is provided by survivial despite being hunted by a coevolving population of predators as maintained by a system called [PredatorEye](https://github.com/cwreynolds/PredatorEye).

The sequence of images below show disk-shaped prey (three in each image) whose camouflage improves over evolutionary time, in response to predation, to better blend in with the background (photos oak leaf litter in this run). See the [paper](https://arxiv.org/abs/2304.11793) for details.

![Camouflage improvement over evolutionary time](https://cwreynolds.github.io/TexSyn/images/20230804_coc_ri_small.jpg)

### Version history

| Version | Date | Description |
| :---: | :---: | :--- |
| 1.0 | 2009 | Preliminary version. Blog: [Texture Synthesis Diary](http://www.red3d.com/cwr/texsyn/diary.html). 2011 paper: [Interactive Evolution of Camouflage](https://www.red3d.com/cwr/iec/) |
| 2.0 | July 2022 | From January 2020 to July 2022: reimplement and expand version 1.0. Blog: [devo blog](https://cwreynolds.github.io/TexSyn/). 2023 paper preprint: [Coevolution of Camouflage](https://arxiv.org/abs/2304.11793)|
| 2.1 | July 2023 | Header only version. |

**Note:** the old `main.cpp` was moved to `docs/main.cpp.old` in case any of that old sample code is ever needed again. It was a 6800 line chronological collection of several years of test jigs and debugging code.

**Note:** still included in this repository are separate applications of TexSyn, which should be broken off into their own repository. `EvoCamoGame.h` contains a reimprementation of the 2010-2011 "Interactive Evolution of Camouflage" then a series of steps leading to the 2023 "Coevolution of Camouflage". Similarly `SimpleImageMatch.h` contains unfinished experiments on generation of image stylizations. 

## Usage notes for  running Coevolution of Camouflage from the command line.

Use this code to run “Coevolution of Camouflage” as described in the paper:

<ul>
Craig Reynolds. 2023. Coevolution of Camouflage. 2023 Artificial Life
Conference. MIT Press. https://doi.org/10.1162/isal_a_00583
</ul>

To run a simulation, two processes are launched, one to run camouflage
evolution using TexSyn and LazyPredator, the other to run predator evolution
with PredatorEye. I do this by opening two shell (Terminal on macOS) windows
and launching a command in each one. In one shell enter these commands:

[Note these pathnames are from my own laptop, you will need to adapt for your
own environemnt. Contact me (cwr@red3d.com) for help with that if needed.]

```
   cd ~/Documents/code/PredatorEye
   conda activate TensorFlow_GPU_M1
   python3 EvoCamoVsLearnPredPop.py 40
```
Then in the other window (typical usage):

```
    caffeinate texsyn ~/Pictures/camouflage_backgrounds/oak_leaf_litter ~/Desktop/TexSyn_temp/ 0.25 20230921 512 512 400 20
```

At this point both processes should run, trading images and other state via
the file system. To the extent possible, they execute in parallel.

"caffeinate" is a macOS utility to prevent the laptop from sleeping while the
command is running.

The parameters to the texsyn side:
```
  Requires at least one pathname parameter, others may be omitted from end:
    background_image_directory (required)
    output_directory (defaults to .)
    background_scale (defaults to 0.5)
    random_seed (else: default seed)
    window width (defaults to 1200)
    window height (defaults to 800)
    individuals (defaults to 120)
    subpopulations (defaults to 6)
    max_steps (defaults to 12000)
    max_init_tree_size (defaults to 100)
    min_crossover_tree_size (default max_init_tree_size_ * 0.5)
    max_crossover_tree_size (default max_init_tree_size_ * 1.5)
```

My habit is to use the date, as YYYYMMDD as the random number seed for unique
runs. Add a digit at the end if making multiple runs in a day. A directory
for each run is created under the "output_directory" named for the background
set, the date, and time.

Toward the end of CoC experiments 12000 steps was a typical run length, with
400 prey individuals and 40 predators. This is refered to in comments as a
“super heavy” run. Shorter runs and smaller populations were used earlier in
the research with mixed success. See section “Background Sets” and Figure 7
in https://doi.org/10.1162/isal_a_00583 for more detail.
