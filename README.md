# TexSyn
## Library for evolutionary texture synthesis.

**TexSyn** is a library for procedural texture synthesis. It is intended for use by a _genetic programming_ (“GP”) system, a type of _genetic algorithm_. The GP system performs _simulated evolution_ on a _population_ of programs, according to a _fitness function_ (aka _fitness metric_, _utility function_, or a _loss function_ in machine learning.) In a texture synthesis application, the programs are compositions of functions from this **TexSyn** library. When executed they describe a _color texture_—which is to say—an _image_.

**TexSyn**'s companion genetic programming system is [LazyPredator](https://github.com/cwreynolds/LazyPredator).

See **TexSyn**'s [blog / development diary / doc](https://cwreynolds.github.io/TexSyn/).

This is a re-implementation and update to the TextureSynthesisTest library as described in [Texture Synthesis Diary](http://www.red3d.com/cwr/texsyn/diary.html) and used as the basis of the 2011 paper [Interactive Evolution of Camouflage](https://www.red3d.com/cwr/iec/).

On July 25, 2020 this was renamed to be “TexSyn version 2.0”. Changes to basic TexSyn have slowed to a trickle. The `EvoCamoGame` classes are still subject of active development, but really should be broken off into their own project/repository.

**Please note:** this has been open source code since the beginning. I am happy for you to look at it. However I treat this repository as a personal sandbox for experimentation. At any given time it might be broken or running a temporary experiment. If anyone out there actually wants to _use_ this code, please get in touch with me so I can make sure you get a clean version. Oh, and `main.cpp` is a mess, it is 6000 lines long, 99% of which is commented out test code.

