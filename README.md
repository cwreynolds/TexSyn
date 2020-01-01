# TexSyn
## Library for evolutionary texture synthesis.

`TexSyn` is a library for procedural texture synthesis. It is intended for use by a _genetic programming_ (“GP”) system, a type of _genetic algorithm_. The GP system _evolves_ a _population_ of programs according to a _fitness function_ (aka _fitness metric_ analogous to a _loss function_ in machine learning.) In this texture synthesis application, the programs are compositions of functions from this `TexSyn` library. When executed they create a _color texture_, which is to say, an _image_.

This is a re-implementation and update to the TextureSynthesisTest library as described in [Texture Synthesis Diary](http://www.red3d.com/cwr/texsyn/diary.html) and used as the basis of the 2011 paper [Interactive Evolution of Camouflage](https://www.red3d.com/cwr/iec/).
