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
