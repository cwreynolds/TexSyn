# TexSyn
## Library for evolutionary texture synthesis.

**TexSyn** is a library for procedural texture synthesis. It is primarily intended for use by a _genetic programming_ (“GP”) system, a type of _genetic algorithm_. The GP system performs _simulated evolution_ on a _population_ of programs, according to a _fitness function_ (aka _fitness metric_, _utility function_, or _loss function_.) For a GP texture synthesis application, the programs are compositions of functions from this **TexSyn** library. When executed they describe a _color texture_—which is to say—an _image_.

For more information and many sample textures, see **TexSyn**'s [devo blog](https://cwreynolds.github.io/TexSyn/).

**TexSyn** is written in c++ (gnu++17) and structured as a “header only” library (as of version 2.1). All that is required to use it is to clone this git repository (or just copy the source files) and `#include "TexSyn.h"`. In addition there is an optional `main.cpp` which can be built to verify that the library is functioning as expected: it runs a unit test suite and displays some sample synthesized textures. You can also run these tests from your own code, e.g. `UnitTests::allTestsOK()`. Building the `main.cpp` test app can be done with with CMake (`CMakeLists.txt`) or macOS Xcode (`TexSyn.xcodeproj`).

**TexSyn**'s companion genetic programming system is [LazyPredator](https://github.com/cwreynolds/LazyPredator).

The research which **TexSyn** was built to support has been completed. It is described in the paper _Coevolution of Camouflage_ available as a [preprint on arXiv](https://arxiv.org/abs/2304.11793). That paper will be presented at, and published in the proceedings of, the [2023 Artificial Life conference](https://alife.org/conference/alife-2023/) in Sapporo, Japan, July 24-28, 2023. There will be a [poster](https://doi.org/10.1145/3588028.3603663) about this work at SIGGRAPH 2023 in Los Angeles, California, August 6-10.

In that research, **TexSyn** was used with **LazyPredator** to evolve artificial camouflage patterns which were “cryptic” against background images. The fitness metric that drives camouflage evolution is provided by survivial despite being hunted by a coevolving population of predators as maintained by a system called [PredatorEye](https://github.com/cwreynolds/PredatorEye).

On July 25, 2020 this was renamed to be “TexSyn version 2.0”. Changes to basic TexSyn have slowed to a trickle. The `EvoCamoGame` classes are still subject of active development, but really should be broken off into their own project/repository. This is a re-implementation and update to “TexSyn version 1”: the TextureSynthesisTest library as described in [Texture Synthesis Diary](http://www.red3d.com/cwr/texsyn/diary.html) and used as the basis of the 2011 paper [Interactive Evolution of Camouflage](https://www.red3d.com/cwr/iec/).

**Please note:** this has been open source code since the beginning. I am happy for you to look at it. However I treat this repository as a personal sandbox for experimentation. At any given time it might be broken or running a temporary experiment. If anyone out there actually wants to _use_ this code, please get in touch with me so I can make sure you get a clean version. Oh, and `main.cpp` is a mess, it is 6000 lines long, 99% of which is commented out test code.
