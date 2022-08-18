cmake_minimum_required(VERSION 3.10)

# Project name and version.
project(texsyn VERSION 2.0) # TODO needs to sync with texsyn_version_string

# Specify which C++ standard -- I think "17" here is same as "gnu++17" in Xcode 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Rewrite Version.h using Version.h.in as a template.
configure_file(Version.h.in Version.h)

# Add sourcfe files to the executable
add_executable(texsyn
               main.cpp
               Disk.cpp
               Operators.cpp
               Texture.cpp
               UnitTests.cpp
               Utilities.cpp)

# Configured Version.h file will be written into the binary tree, so add that
# directory to the list of paths to search for include files.
# (TODO Does this mean we end up with two copies of Version.h?)
target_include_directories(texsyn PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

# TODO 20220817 use hardcoded pathname for OpenCV headers.
target_include_directories(texsyn PUBLIC
                           "/opt/homebrew/opt/opencv/include/opencv4/")

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# # TODO 20220817 use hardcoded pathname for OpenCV compiled libs.
# target_link_libraries(texsyn PUBLIC "/opt/homebrew/opt/opencv/lib/")

# TODO 20220817 use hardcoded pathname for OpenCV compiled libs.
target_link_libraries(texsyn
    "/opt/homebrew/opt/opencv/lib/libopencv_gapi.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_stitching.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_alphamat.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_aruco.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_barcode.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_bgsegm.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_bioinspired.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_ccalib.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_dnn_objdetect.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_dnn_superres.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_dpm.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_face.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_freetype.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_fuzzy.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_hfs.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_img_hash.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_intensity_transform.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_line_descriptor.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_mcc.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_quality.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_rapid.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_reg.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_rgbd.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_saliency.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_sfm.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_stereo.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_structured_light.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_phase_unwrapping.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_superres.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_optflow.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_surface_matching.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_tracking.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_highgui.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_datasets.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_text.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_plot.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_videostab.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_videoio.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_viz.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_wechat_qrcode.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_xfeatures2d.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_shape.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_ml.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_ximgproc.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_video.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_xobjdetect.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_objdetect.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_calib3d.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_imgcodecs.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_features2d.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_dnn.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_flann.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_xphoto.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_photo.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_imgproc.a"
    "/opt/homebrew/opt/opencv/lib/libopencv_core.a"
    )
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
