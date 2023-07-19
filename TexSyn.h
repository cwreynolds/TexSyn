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

// These are just defined here by hand.
static const int texsyn_version_major = 2;
static const int texsyn_version_minor = 1;
// TODO 20230719 I don't know how to get this automatically.
// static const std::string texsyn_architecture = "macOS-x86_64";
static const std::string texsyn_architecture = "macOS-arm64";

static const std::string texsyn_version_string =
    (std::string("TexSyn version ") +
     std::to_string(texsyn_version_major) + "." +
     std::to_string(texsyn_version_minor) + " " +
     texsyn_architecture);
