//
//  Texture.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Texture.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// TODO temporary function to test image display.
void Texture::testImageDisplay()
{
    std::string name = "/Users/cwr/Desktop/test.jpg";
    cv::Mat image;              // Create Mat object.
    image = cv::imread(name);   // Read the file.
    cv::namedWindow("TexSyn");  // Create a window for display.
    imshow("TexSyn", image);    // Show our image inside it.
    cv::waitKey(0);             // Wait for a keystroke in the window.
}
