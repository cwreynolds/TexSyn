//
//  TexSyn.h
//  TexSyn
//
//  Created by Craig Reynolds on 8/4/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//
//  Top level header file for TexSyn library. Should provide all public API.

#pragma once
#include "Operators.h"
#include "UnitTests.h"
#include "Version.h"

// Now (20220814) leans on CMake for version number, but still
// fragile because OS and architecture are specified inline below.
static std::string
    texsyn_version_string = (std::string("TexSyn version ") +
                             std::to_string(texsyn_VERSION_MAJOR) + "." +
                             std::to_string(texsyn_VERSION_MINOR) + " " +
                             // "macOS-x86_64"
                             "macOS-arm64");
