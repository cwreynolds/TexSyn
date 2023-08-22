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

## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ##
## TODO 20230822 set explicit optimization flag for c++ compilation: -Os

#    add_compile_options(
#    #       -Wall -Werror -Wno-error=maybe-uninitialized
#           -Wall -Werror
#    #       -Wno-error=uninitialized
#           -Wno-error=reorder-ctor
#           -Wno-error=unused-private-field
#
#    #       $<$<CONFIG:RELEASE>:-Ofast>
#    #       $<$<CONFIG:DEBUG>:-O0>
#
#    #       $<$<CONFIG:RELEASE>:-Os>
#    #       $<$<CONFIG:DEBUG>:-Os>
#
#            -Os
#
#    #       $<$<CONFIG:DEBUG>:-ggdb3>
#    )

add_compile_options(-Os
                    -Wall
                    -Werror
                    -Wno-error=reorder-ctor
                    -Wno-error=unused-private-field)

## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ##

# Find the external OpenCV package.
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add main.cpp to the executable
add_executable(texsyn main.cpp)

# Link executable with libraries.
target_link_libraries(texsyn ${OpenCV_LIBS})
