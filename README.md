# TexSyn
## Library for evolutionary texture synthesis.

_(If you plan to build and run TexSyn, see notes at the bottom of this README.)_

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

The research which **TexSyn** was built to support is described in the paper _Coevolution of Camouflage_ available as a [preprint on arXiv](https://arxiv.org/abs/2304.11793). That paper was presented at (and published in the proceedings of) the [2023 Artificial Life conference](https://alife.org/conference/alife-2023/) in Sapporo, Japan, July 24-28, 2023. A [poster](https://doi.org/10.1145/3588028.3603663) about this work was presented at SIGGRAPH 2023 in Los Angeles, California, August 6-10.

In that research, **TexSyn** was used with **LazyPredator** to evolve artificial camouflage patterns which were “cryptic” against background images. The fitness metric that drives camouflage evolution is provided by survivial despite being hunted by a coevolving population of predators as maintained by a system called [PredatorEye](https://github.com/cwreynolds/PredatorEye).

The sequence of images below show disk-shaped prey (three in each image) whose camouflage improves over evolutionary time, in response to predation, to better blend in with the background (photos of oak leaf litter in this run). See the [paper](https://arxiv.org/abs/2304.11793) for details.

![Camouflage improvement over evolutionary time](https://cwreynolds.github.io/TexSyn/images/20230804_coc_ri_small.jpg)

### Version history

| Version | Date | Description |
| :---: | :---: | :--- |
| 1.0 | 2009 | Preliminary version. Blog: [Texture Synthesis Diary](http://www.red3d.com/cwr/texsyn/diary.html). 2011 paper: [Interactive Evolution of Camouflage](https://www.red3d.com/cwr/iec/) |
| 2.0 | July 2022 | From January 2020 to July 2022: reimplement and expand version 1.0. Blog: [devo blog](https://cwreynolds.github.io/TexSyn/). 2023 paper preprint: [Coevolution of Camouflage](https://arxiv.org/abs/2304.11793)|
| 2.1 | July 2023 | Header only version. |

**Note:** the old `main.cpp` was moved to `docs/main.cpp.old` in case any of that old sample code is ever needed again. It was a 6800 line chronological collection of several years of test jigs and debugging code.

**Note:** still included in this repository are separate applications of TexSyn, which should be broken off into their own repository. `EvoCamoGame.h` contains a reimplementation of the 2010-2011 "Interactive Evolution of Camouflage" then a series of steps leading to the 2023 "Coevolution of Camouflage". Similarly `SimpleImageMatch.h` contains unfinished experiments on generating image stylizations. 

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
own environment. Contact me (cwr@red3d.com) if you need help with that.]

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

## Building and running TexSyn with PredatorEye

(This section added January 1, 2025.)

**Note:** I built TexSyn only on macOS on my Macbook Pro (2021 M1 Max). It is unlikely to build on other platforms without changes. If you _have_ built TexSyn on other platforms, please let me know what changes you needed to make.

Install TexSyn from Github using (e.g.): `git clone https://github.com/cwreynolds/TexSyn.git`

**Size warning:** a fresh clone of TexSyn is 1.2 GB in size. Sorry for the large size. Half of that (0.60 GB) is the `docs` directory. Most of the rest (0.56 GB) is the `.git` directory. If you need to reduce that file size on your local machine, both of those directories can be deleted. The doc is available over the [web](https://cwreynolds.github.io/TexSyn/) and the `.git` directory is needed only if you want to submit Pull Requests, etc. Both folders could be deleted, then restored if needed by doing a fresh clone (requisite [xkcd citation](https://xkcd.com/1597/)).

Build the TexSyn app using Xcode or CMake. Then try to run it in parallel with the predator side (EvoCamoVsLearnPredPop.py) as described in the previous section. The python script will try to load a pre-trained “generalist predator” model in TensorFlow/Keras format called **20220321_1711_FCD6_rc4**. That model is publicly available in this [Google Drive folder](https://drive.google.com/drive/folders/1tJSHUhiqACmXipr-oVvZmCOyDogErVuq?usp=sharing).

You could download that model directory from Google Drive to your local machine. Or as I did, “mount” that Google Drive folder on your local file system using something like [Drive for desktop](https://support.google.com/drive/answer/12178485). The python script will be looking for `My Drive/PredatorEye/saved_models/20220321_1711_FCD6_rc4` so if you have copied/mounted elsewhere, you should edit the pathname in the python code.

From the intrepid researchers/developers who _have_ previously tried to build this software, I’ve heard several times about problems with the `withinEpsilon()` function in `Utiliites.h` file. Perhaps because any small numerical difference may lead to failing unit tests, probably via a `withinEpsilon()`. Again: let me know if you can shed any light.

**Select a background set for your first run**: a key input for a camouflage run is a set of background images. Prey will evolve to blend in with these backgrounds. Background images are usually photos of a natural texture. A background set is several images in a folder/directory. As discussed the [paper](https://arxiv.org/abs/2304.11793)'s section on _Background Sets_, it is “easier” to evolve camouflage for a background set which is more “stationary.” This means that any prey-sized sample of a background image will be “similar” to any other. They will have similar statistical distributions of color and spatial frequency. Anything “granular” will have this property: gravel, seeds, leaves, and so on. Try running the simulation on an “easy” background set before trying something more challenging.

Good luck and feel free to contact me (cwr@red3d.com).
