//
//  TexSyn.h
//  TexSyn
//
//  Created by Craig Reynolds on 8/4/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//
//  Top level header file for TexSyn library. Should provide all public API.

#pragma once
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20220816 working on CMake build -- very temp avoid includes
//#include "Operators.h"
//#include "UnitTests.h"
#include "Operators.h"
#include "UnitTests.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20220816 working on CMake build -- very temp avoid includes
//#include <string>
//#include <iostream>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20220814 working on CMake build -- version numbers

//    static std::string texsyn_version_string =
//    //    "TexSyn version 0.9.7 (alpha macOS-x86_64)";
//        "TexSyn version 2.0.0 (alpha macOS-arm64)";

#include "Version.h"

// Now (20220814) leans on CMake for version number, but still
// fragile because OS and architecture are specified inline below.
static std::string
    texsyn_version_string = (std::string("TexSyn version ") +
                             std::to_string(texsyn_VERSION_MAJOR) + "." +
                             std::to_string(texsyn_VERSION_MINOR) + " " +
                             // "macOS-x86_64"
                             "macOS-arm64");

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
