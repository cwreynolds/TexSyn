cmake_minimum_required(VERSION 3.10)

# Project name and version.
project(texsyn VERSION 2.0) # TODO needs to sync with texsyn_version_string

# Specify which C++ standard -- I think "17" here is same as "gnu++17" in Xcode 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

// Rewrite Version.h using Version.h.in as a template.
configure_file(Version.h.in Version.h)

# add the executable
add_executable(texsyn main.cpp)
