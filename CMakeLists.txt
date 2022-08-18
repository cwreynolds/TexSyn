cmake_minimum_required(VERSION 3.10)

# Project name and version.
project(texsyn VERSION 2.0) # TODO needs to sync with texsyn_version_string

# Specify which C++ standard -- I think "17" here is same as "gnu++17" in Xcode 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Rewrite Version.h using Version.h.in as a template.
configure_file(Version.h.in Version.h)

# add the executable
add_executable(texsyn main.cpp)

# Configured Version.h file will be written into the binary tree, so add that
# directory to the list of paths to search for include files.
# (TODO Does this mean we end up with two copies of Version.h?)
target_include_directories(texsyn PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

# TODO 20220817 use hardcoded pathname for OpenCV headers.
target_include_directories(texsyn PUBLIC
                           "/opt/homebrew/opt/opencv/include/opencv4/")

# TODO 20220817 use hardcoded pathname for OpenCV compiled libs.
target_link_libraries(texsyn PUBLIC "/opt/homebrew/opt/opencv/lib")
