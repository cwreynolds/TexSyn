################################################################################
#
# CMakeLists.txt
# CMake for TexSyn
#
################################################################################

cmake_minimum_required(VERSION 3.10)

# Project name and version.
project(texsyn VERSION 2.0) # TODO needs to sync with texsyn_version_string

# Specify which C++ standard -- I think "17" here is same as "gnu++17" in Xcode 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# C++ compiler flags. Most importantly "-Os" optimization to match Xcode build.
add_compile_options(-Os -Wall -Werror)

# Find the external OpenCV package.
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add main.cpp to the executable
add_executable(texsyn main.cpp)

# Link executable with libraries.
target_link_libraries(texsyn ${OpenCV_LIBS})
