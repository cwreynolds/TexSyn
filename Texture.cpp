//
//  Texture.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Texture.h"
#include <thread>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <opencv2/core/core.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO temp(?) for fft_test()
#include <opencv2/imgproc/imgproc.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <opencv2/highgui/highgui.hpp>
#pragma clang diagnostic pop

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Texture::~Texture()
{
    // TODO deja vu?
    assert("already invalid at top of ~Texture" && valid());
    
//    // TODO I suspect both of these are superfluous:
//    raster_->release();
//    raster_.reset();
//    // TODO this too, right?
//    raster_ = nullptr;
    
    // TODO 20201122 temporary for debugging
    markAsInvalid();
    
    destructor_count_++;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Utility for getColor(), special-cased for when alpha is 0 or 1.
Color Texture::interpolatePointOnTextures(float alpha,
                                          Vec2 position0,
                                          Vec2 position1,
                                          const Texture& t0,
                                          const Texture& t1) const
{
    return ((alpha == 0) ?
            // For alpha==0 evaluate only t0.
            t0.getColor(position0) :
            ((alpha == 1) ?
             // For alpha==1 evaluate only t1.
             t1.getColor(position1) :
             // Otherwise evaluate both and interpolate between them.
             interpolate(alpha,
                         t0.getColor(position0),
                         t1.getColor(position1))));
}

// Rasterize this texture into size² OpenCV image, display in pop-up window.
void Texture::displayInWindow(int size, bool wait) const
{
    rasterizeToImageCache(size, getDefaultRenderAsDisk());
    windowPlacementTool(*raster_);
    if (wait) waitKey();  // Wait for a keystroke in the window.
}

// Display cv::Mat in pop-up window. Stack diagonally from upper left.
void Texture::windowPlacementTool(cv::Mat& mat)
{
    std::string window_name = "TexSyn" + std::to_string(window_counter++);
    cv::namedWindow(window_name);       // Create a window for display.
    int tm = 23;  // TODO approximate top margin height
    cv::moveWindow(window_name, window_x, window_y);
    window_x += tm;
    window_y += tm;
    cv::imshow(window_name, mat);  // Show our image inside it.
    // TODO pure hack, assumes 511x511, screen size of my MacBook Pro (Mid 2014)
    if ((window_counter % 15) == 0) window_y =0 ;
}

// Rasterize this texture into a size² OpenCV image. Arg "disk" true means
// draw a round image, otherwise a square. Run parallel threads for speed.
void Texture::rasterizeToImageCache(int size, bool disk) const
{
    //Timer t("rasterizeToImageCache");
    // If size changed, including from initial value of 0x0, generate raster.
    // (TODO also ought to re-cache if "disk" changes. Issue ignored for now.)
    if ((size != raster_->rows) || (size != raster_->cols))
    {
        // Reset our OpenCV Mat to be (size, size) with 3 floats per pixel.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201203 experiment change default type from CV_32FC3 to CV_8UC3
//        raster_->create(size, size, CV_32FC3);
        raster_->create(size, size, getDefaultOpencvMatType());
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO Code assumes disk center at window center, so size must be odd.
        assert(((!disk) || (size % 2 == 1)) && "For disk, size must be odd.");
        // Synchronizes access to opencv_image by multiple row threads.
        std::mutex ocv_image_mutex;
        // Collection of all row threads.
        std::vector<std::thread> all_threads;
        // Loop all image rows, bottom to top. For each, launch a thread running
        // rasterizeRowOfDisk() to compute pixels, write to image via mutex.
        for (int j = -(size / 2); j <= (size / 2); j++)
        {
            // This requires some unpacking. It creates a thread which is pushed
            // (using && move semantics, I think) onto the back of std::vector
            // all_row_threads. Because the initial/toplevel thread function is
            // member function of this instance, it is specified as two values,
            // a function pointer AND an instance pointer. The other four values
            // are args to rasterizeRowOfDisk(row, size, disk, image, mutex).
            all_threads.push_back(std::thread(&Texture::rasterizeRowOfDisk, this,
                                              j, size, disk,
                                              std::ref(*raster_),
                                              std::ref(ocv_image_mutex)));
        }
        // Wait for all row threads to finish.
        for (auto& t : all_threads) t.join();
    }
}

// Rasterize the j-th row of this texture into a size² OpenCV image. Expects
// to run in its own thread, uses mutex to synchonize access to the image.
void Texture::rasterizeRowOfDisk(int j, int size, bool disk,
                                 cv::Mat& opencv_image,
                                 std::mutex& ocv_image_mutex) const
{
    // Half the rendering's size corresponds to the disk's center.
    int half = size / 2;
    // First and last pixels on j-th row of time
    int x_limit = disk ? std::sqrt(sq(half) - sq(j)) : half;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201203 experiment change default type from CV_32FC3 to CV_8UC3
//    cv::Mat row_image(1, size, CV_32FC3, cv::Scalar(0.5, 0.5, 0.5));
    cv::Mat row_image(1, size, getDefaultOpencvMatType(),
//                      cv::Scalar::all(127));
                      cv::Scalar::all((getDefaultOpencvMatType() == CV_32FC3) ?
                                      0.5 : 127));
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    for (int i = -x_limit; i <= x_limit; i++)
    {
        // Read TexSyn Color from Texture at (i, j).
        Color color(0, 0, 0);
        Vec2 pixel_center = Vec2(i, j) / half;
        expensive_to_nest = 0;
        if (sqrt_of_aa_subsample_count > 1) // anti-alaising?
        {
            float pixel_radius = 2.0 / size;
            std::vector<Vec2> offsets;
            RandomSequence rs(pixel_center.hash());
            jittered_grid_NxN_in_square(sqrt_of_aa_subsample_count,
                                        pixel_radius * 2, rs, offsets);
            for (Vec2 offset : offsets)
                color += getColorClipped(pixel_center + offset);
            color = color / sq(sqrt_of_aa_subsample_count);
        }
        else
        {
            color = getColorClipped(pixel_center);
        }
        // Adjust for display gamma.
        color = color.gamma(1 / defaultGamma());
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201203 experiment change default type from CV_32FC3 to CV_8UC3
//        // Make OpenCV color, with reversed component order.
//        cv::Vec3f opencv_color(color.b(), color.g(), color.r());
//        // Write OpenCV color to corresponding pixel on row image:
//        row_image.at<cv::Vec3f>(cv::Point(half + i, 0)) = opencv_color;
        if (getDefaultOpencvMatType() == CV_32FC3)
        {
            // Make OpenCV color, with reversed component order.
            cv::Vec3f opencv_color(color.b(), color.g(), color.r());
            // Write OpenCV color to corresponding pixel on row image:
            row_image.at<cv::Vec3f>(cv::Point(half + i, 0)) = opencv_color;
        }
        else
        {
            // TODO 20201203 experiment case for CV_8UC3
            
            // Make OpenCV color, with reversed component order.
//            cv::Vec3f opencv_color(color.b(), color.g(), color.r());
            cv::Vec3b opencv_color(color.b() * 255,
                                   color.g() * 255,
                                   color.r() * 255);
            // Write OpenCV color to corresponding pixel on row image:
//            row_image.at<cv::Vec3f>(cv::Point(half + i, 0)) = opencv_color;
            row_image.at<cv::Vec3b>(cv::Point(half + i, 0)) = opencv_color;

        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    // Define a new image which is a "pointer" to j-th row of opencv_image.
    cv::Mat row_in_full_image(opencv_image, cv::Rect(0, half - j, size, 1));
    // Wait to grab lock for access to image. (Lock released at end of block)
    const std::lock_guard<std::mutex> lock(ocv_image_mutex);
    // Copy line_image into the j-th row of opencv_image.
    row_image.copyTo(row_in_full_image);
}

// Display a collection of Textures, each in a window, then wait for a char.
void Texture::displayInWindow(std::vector<const Texture*> textures,
                              int size,
                              bool wait)
{
    for (auto& t : textures) t->displayInWindow(size, false);
    // Wait for keystroke, close windows, exit function.
    if (wait) waitKey();
}

// Writes Texture to a file using cv::imwrite(). Generally used with JPEG
// codec, but pathname's extension names the format to be used. Converts to
// "24 bit" image (8 bit unsigned values for each of red, green and blue
// channels) because most codecs do not support 3xfloat format.
void Texture::writeToFile(int size,
                          const std::string& pathname,
                          Color bg_color,
                          int margin,
                          const std::string& file_type) const
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201203 experiment change default type from CV_32FC3 to CV_8UC3
    // need anything here?
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Make OpenCV Mat instance of type CV_8UC3 (3 by unsigned 8 bit primaries).
    cv::Mat opencv_image(size + margin * 2,
                         size + margin * 2,
                         CV_8UC3,
                         cv::Scalar(255 * bg_color.b(),
                                    255 * bg_color.g(),
                                    255 * bg_color.r()));
    // Ensure cached rendering of Texture is available.
    rasterizeToImageCache(size, getDefaultRenderAsDisk());
    // Define a new image, a "pointer" to portion of opencv_image inside margin.
    cv::Mat render_target(opencv_image, cv::Rect(margin, margin, size, size));
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20201203 experiment change default type from CV_32FC3 to CV_8UC3
    // need anything here?
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Convert 3xfloat rendered raster to 3x8bit window inside opencv_image
    raster_->convertTo(render_target, CV_8UC3, 255);
    bool ok = cv::imwrite(pathname + file_type, opencv_image);
    std::cout << (ok ? "OK " : "bad") << " write Texture: size=" << size;
    std::cout << ", margin=" << margin << ", bg_color=" << bg_color;
    std::cout << ", path=\"" << pathname + file_type << "\", " << std::endl;
}

// Combines display on screen and writing file, but primary benefit is that
// this allows writing an arbitrarily nested expression of TexSyn
// constructors, whose lifetime extends across both operations.
void Texture::displayAndFile(const Texture& texture,
                             std::string pathname,
                             int size)
{
    texture.displayInWindow(size, false);
    if (pathname != "") texture.writeToFile(size, pathname);
}

void Texture::waitKey()
{
    cv::waitKey(0);
}

void Texture::waitKey(int delay_in_milliseconds)
{
    cv::waitKey(delay_in_milliseconds);
}

// close the window
void Texture::closeAllWindows()
{
    //cv::destroyWindow(name);
    //cvReleaseImage(&images[i]);
    
    cv::destroyAllWindows();
    window_counter = 0;
    window_x = 0;
    window_y = 0;
}

// Utilities for rasterizing a Texture to tiling of pixels, with versions
// for a square and a disk of pixels. Each require a "size" (width of the
// square or diameter of the disk) and a function to be applied at each
// pixel. The function's parameters are i/j (column/row) indexes of the
// pixel raster, and the corresponding Vec2 in Texture space. [DEPRECATED]
void Texture::rasterizeSquare(int size, PixelFunction pixel_function)
{
    int half = size / 2;
    for (int i = -half; i <= half; i++)
    {
        for (int j = -half; j <= half; j++)
        {
            pixel_function(i, j, Vec2(i / float(half), j / float(half)));
        }
    }
}
void Texture::rasterizeDisk(int size, PixelFunction pixel_function)
{
    int half = size / 2;
    for (int i = -half; i <= half; i++)
    {
        for (int j = -half; j <= half; j++)
        {
            float radius = std::sqrt(sq(i) + sq(j));
            if (radius <= half)
            {
                pixel_function(i, j, Vec2(i / float(half), j / float(half)));
            }
        }
    }
}

// Allocate a generic, empty, cv::Mat. Optionally used for rasterization.
std::shared_ptr<cv::Mat> Texture::emptyCvMat() const
{
    return std::make_shared<cv::Mat>();
}

// Special utility for Texture::diff() maybe refactor to be more general?
void Texture::displayAndFile3(const Texture& t1,
                              const Texture& t2,
                              const Texture& t3,
                              std::string pathname,
                              int size)
{
    // Make OpenCV Mat instance of type CV_8UC3 which is size*3 x size pixels.
    cv::Mat mat(size, size * 3, CV_8UC3);
    // Function to handle each Texture.
    auto subwindow = [&](const Texture& t, int x)
    {
        // Render Texture to its raster_ cv::Mat.
        t.rasterizeToImageCache(size, getDefaultRenderAsDisk());
        // Define a size*size portion of "mat" whose left edge is at "x".
        cv::Mat submat = cv::Mat(mat, cv::Rect(x, 0, size, size));
        // Copy into submat while conveting from rgb float to rgb uint8_t
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20201203 experiment change default type from CV_32FC3 to CV_8UC3
        // need anything here?
//
        
        if (getDefaultOpencvMatType() == CV_8UC3)
        {
            t.raster_->convertTo(submat, CV_8UC3, 1);
        }
        else
        {
            t.raster_->convertTo(submat, CV_8UC3, 255);
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    };
    subwindow(t1, 0);
    subwindow(t2, size);
    subwindow(t3, size * 2);
    // Write "mat" to file if non-empty "pathname" given.
    std::string file_type = ".png";  // Maybe should be an optional parameter?
    if (pathname != "") cv::imwrite(pathname + file_type, mat);
    // Display "mat" in the TexSyn fashion.
    windowPlacementTool(mat);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//    //CV_32FC3
//
//    void Texture::fft_test() const
//    {
//        // Read image from file
//        // Make sure that the image is in grayscale
//    //    cv::Mat img = cv::imread("lena.JPG",0);
//    //    cv::Mat& img = *raster_;
//    //    cv::Mat img = *raster_;
//    //    cv::Mat img(*raster_);
//        cv::Mat img;
//
//    //    img.convertTo(img, CV_32F);
//        raster_->convertTo(img, CV_32F);
//
//    //    cv::cvtColor(img, *raster_, cv::COLOR_BGR2GRAY);
//        cv::cvtColor(*raster_, img, cv::COLOR_BGR2GRAY);
//
//        debugPrint(raster_->cols);
//        debugPrint(raster_->rows);
//        debugPrint(img.cols);
//        debugPrint(img.rows);
//        debugPrint(cv::Mat_<float>(img).cols);
//        debugPrint(cv::Mat_<float>(img).rows);
//
//        cv::imshow("img", img);
//    //    cv::imshow("cv::Mat_<float>(img)", cv::Mat_<float>(img));
//    //    Texture::waitKey();
//
//        //Complex plane to contain the DFT coefficients {[0]-Real,[1]-Img}
//    //    cv::Mat planes[] = {cv::Mat_<float>(img), cv::Mat::zeros(img.size(), CV_32F)};
//    //    cv::Mat complexI;
//    //    cv::merge(planes, 2, complexI);
//    //    std::vector<cv::Mat> planes =
//    //        { cv::Mat_<float>(img), cv::Mat::zeros(img.size(), CV_32F) };
//        std::vector<cv::Mat> planes = { img, cv::Mat::zeros(img.size(), CV_32F) };
//        cv::Mat complexI;
//        cv::merge(planes, complexI);
//
//        // Applying DFT
//        cv::dft(complexI, complexI);
//
//        // Reconstructing original image from the DFT coefficients
//        cv::Mat invDFT, invDFTcvt;
//        // Applying IDFT
//        cv::idft(complexI, invDFT, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT );
//
//    //    invDFT.convertTo(invDFTcvt, CV_8U);
//        invDFT.convertTo(invDFTcvt, CV_8U, 256);
//        cv::imshow("Output", invDFTcvt);
//
//
//        // Split the image into different channels
//        std::vector<cv::Mat> fftChannels(2);
//        split(complexI, fftChannels);
//
//
//    //    cv::imshow("complexI[0]", complexI[0]);
//        cv::imshow("fftChannels[0]", fftChannels[0]);
//        cv::imshow("fftChannels[1]", fftChannels[1]);
//
//
//    //    //show the image
//    //    cv::imshow("Original Image", img);
//
//    //    // Wait until user press some key
//    //    cv::waitKey(0);
//    //    return 0;
//    }

void Texture::fft_test() // const
{
//    Texture::rasterizeToImageCache(251, false);
//    Texture::rasterizeToImageCache(101, false);
    Texture::rasterizeToImageCache(201, false);

    
//    cv::Mat img;
    cv::Mat monochrome;
//    raster_->convertTo(img, CV_32F);
//    cv::cvtColor(*raster_, img, cv::COLOR_BGR2GRAY);
    cv::cvtColor(*raster_, monochrome, cv::COLOR_BGR2GRAY);

//    debugPrint(raster_->cols);
//    debugPrint(raster_->rows);
//    debugPrint(img.cols);
//    debugPrint(img.rows);
//    debugPrint(cv::Mat_<float>(img).cols);
//    debugPrint(cv::Mat_<float>(img).rows);
//    cv::imshow("img", img);
    cv::imshow("monochrome", monochrome);

    // Complex plane to contain the DFT coefficients {[0]-Real,[1]-Img}
//    std::vector<cv::Mat> planes = { img, cv::Mat::zeros(img.size(), CV_32F) };
//    std::vector<cv::Mat> planes = { monochrome, cv::Mat::zeros(monochrome.size(), CV_32F) };
    cv::Mat zeros = cv::Mat::zeros(monochrome.size(), CV_32F);
    std::vector<cv::Mat> planes = { monochrome, zeros };
    cv::Mat complexI;
    cv::merge(planes, complexI);

    // Applying DFT
    cv::dft(complexI, complexI);

    // Reconstructing original image from the DFT coefficients
//    cv::Mat invDFT, invDFTcvt;
    cv::Mat invDFT;
    // Applying IDFT
    cv::idft(complexI, invDFT, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT );
    
//    invDFT.convertTo(invDFTcvt, CV_8U, 256);
//    cv::imshow("Output", invDFTcvt);
//    cv::imshow("Output", invDFT);
    cv::imshow("DFT-iDFT reconstruction", invDFT);

    // Split the image into different channels
    std::vector<cv::Mat> fftChannels(2);
    split(complexI, fftChannels);
//    cv::imshow("fftChannels[0]", fftChannels[0]);
//    cv::imshow("fftChannels[1]", fftChannels[1]);
    cv::imshow("DFT real part", fftChannels[0]);
    cv::imshow("DFT imginary part", fftChannels[1]);
    
    cv::Mat& real = fftChannels[0];
    int width = real.rows;
    int y = width / 2;

//        for (int x = real.cols / 2; x < real.cols; x++)
//        {
//    //        std::cout << real.at<float>(y, x) << " ";
//    //        int v = (real.at<float>(y, x) * 10) + 50;
//            int v = real.at<float>(y, x) * 10;
//            std::cout << v << " ";
//        }
    for (int x = width / 2; x < width; x++)
    {
        std::cout << real.at<float>(y, x) << " ";
    }
    std::cout << std::endl << std::endl;
    for (int x = width / 2; x < width; x++)
    {
        std::cout << int(10 * real.at<float>(y, x)) << " ";
    }
    std::cout << std::endl << std::endl;
    
    
//        float score = 0;
//        for (int x = width / 2; x < width; x++)
//        {
//            float real_part = real.at<float>(y, x);
//    //        float weight = float(x - (real.cols / 2)) / (real.cols / 2);
//    //        float weight = remapInterval(x, width / 2, width, 0, 1);
//            float weight = sq(remapInterval(x, width / 2, width, 0, 1));
//    //        score += real_part * weight;
//            score += std::abs(real_part * weight);
//        }
//        debugPrint(score);

    debugPrint(highFrequencyScore());
}

 
//    float Texture::highFrequencyScore() const
//    //{
//    //    return highFrequencyScore(getDefaultRenderSize());
//    //}
//    //float Texture::highFrequencyScore(int render_size) const
//    {
//    //    // TODO no this wastes a lot of re-rendering time during GP run
//    //    // but how to make sure there is a correctly-sized raster?
//    //    // Render this texture to monochrome (square image, 201x201).
//    //    Texture::rasterizeToImageCache(201, false);
//    //    if (raster_->empty()) Texture::rasterizeToImageCache(201, false);
//
//        // TODO, no, still wrong, some images displayed with square shape.
//        // Render this texture to monochrome, at given render_size, square shape.
//    //    Texture::rasterizeToImageCache(render_size, false);
//    //    Texture::rasterizeToImageCache(render_size, getDefaultRenderAsDisk());
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        Timer t("....................................Texture::highFrequencyScore");
//        cv::Mat temp = *raster_;
//        Texture::rasterizeToImageCache(101, false);
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//        cv::Mat monochrome;
//        cv::cvtColor(*raster_, monochrome, cv::COLOR_BGR2GRAY);
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        *raster_ = temp;
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//        // Complex plane to contain the DFT coefficients {[0]-Real,[1]-Img}
//        cv::Mat complexI;
//        cv::Mat zeros = cv::Mat::zeros(monochrome.size(), CV_32F);
//        std::vector<cv::Mat> planes = { monochrome, zeros };
//        cv::merge(planes, complexI);
//
//        // Applying DFT
//        cv::dft(complexI, complexI);
//
//        // Split the image into different channels
//        std::vector<cv::Mat> fftChannels(2);
//        split(complexI, fftChannels);
//
//        cv::Mat& real = fftChannels[0];
//        int width = real.rows;
//        int y = width / 2;
//        float score = 0;
//        for (int x = width / 2; x < width; x++)
//        {
//            float real_part = real.at<float>(y, x);
//            float weight = sq(remapInterval(x, width / 2, width, 0, 1));
//            score += std::abs(real_part * weight);
//        }
//        return score;
//    }

//    float Texture::highFrequencyScore() // const
//    {
//        // TODO maybe cache the rendered image used here, or just case the score?
//        Timer t("....................................Texture::highFrequencyScore");
//        float score = cached_high_frequency_score_;
//        if (score == 0)
//        {
//            // Render this texture to monochrome (square image, size x size).
//            int size = 101;
//            cv::Mat temp = *raster_;  // Save raster_
//            Texture::rasterizeToImageCache(size, false);
//            cv::Mat monochrome;
//            cv::cvtColor(*raster_, monochrome, cv::COLOR_BGR2GRAY);
//            // restore raster_
//            *raster_ = temp;
//
//            // Complex plane to contain the DFT coefficients {[0]-Real,[1]-Img}
//            cv::Mat complexI;
//            cv::Mat zeros = cv::Mat::zeros(monochrome.size(), CV_32F);
//            std::vector<cv::Mat> planes = { monochrome, zeros };
//            cv::merge(planes, complexI);
//
//            // Applying DFT
//            cv::dft(complexI, complexI);
//
//            // Split the image into different channels
//            std::vector<cv::Mat> fftChannels(2);
//            split(complexI, fftChannels);
//
//            cv::Mat& real = fftChannels[0];
//            int width = real.rows;
//            for (int x = width / 2; x < width; x++)
//            {
//                for (int y = width / 2; y < width; y++)
//                {
//                    float real_part = real.at<float>(y, x);
//                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                    // TODO I think this is left over form the 1d version several days ago
//    //                float weight = sq(remapInterval(x, width / 2, width, 0, 1));
//                    Vec2 offset = Vec2(x, y) - Vec2(width / 2, width / 2);
//                    float weight = sq(offset.length() / (width * 0.5));
//                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                    score += std::abs(real_part * weight);
//                }
//            }
//            cached_high_frequency_score_ = score;
//        }
//        return score;
//    }

// TODO rewrite to score entire real plane (all four quadrants)
//      wondered if this is why slice gratings at a diagonal were so popular

//    float Texture::highFrequencyScore()
//    {
//        // TODO maybe cache the rendered image used here, or just case the score?
//        Timer t("    highFrequencyScore");
//        float score = cached_high_frequency_score_;
//        if (score == 0)
//        {
//            // Render this texture to monochrome (square image, size x size).
//            int size = 101;
//            cv::Mat temp = *raster_;  // Save raster_
//            Texture::rasterizeToImageCache(size, false);
//            cv::Mat monochrome;
//            cv::cvtColor(*raster_, monochrome, cv::COLOR_BGR2GRAY);
//            // restore raster_
//            *raster_ = temp;
//
//            // Complex plane to contain the DFT coefficients {[0]-Real,[1]-Img}
//            cv::Mat complexI;
//            cv::Mat zeros = cv::Mat::zeros(monochrome.size(), CV_32F);
//            std::vector<cv::Mat> planes = { monochrome, zeros };
//            cv::merge(planes, complexI);
//
//            // Applying DFT
//            cv::dft(complexI, complexI);
//
//            // Split the image into different channels
//            std::vector<cv::Mat> fftChannels(2);
//            split(complexI, fftChannels);
//
//            cv::Mat& real = fftChannels[0];
//            int width = real.rows;
//            float half_width = width * 0.5;
//
//            Vec2 center(half_width, half_width);
//
//    //        for (int x = width / 2; x < width; x++)
//            for (int x = 0; x < width; x++)
//            {
//    //            for (int y = width / 2; y < width; y++)
//                for (int y = 0; y < width; y++)
//                {
//    //                float real_part = real.at<float>(y, x);
//    //                Vec2 offset = Vec2(x, y) - Vec2(half_width, half_width);
//    //                float weight = sq(offset.length() / (width * 0.5));
//    //                score += std::abs(real_part * weight);
//
//    //                Vec2 offset = Vec2(x, y) - center;
//    //                float length = offset.length();
//                    float length = (Vec2(x, y) - center).length();
//                    float weight = sq(length / half_width);
//                    if (weight < 1)
//                    {
//                        float real_part = real.at<float>(y, x);
//                        score += std::abs(real_part * weight);
//                    }
//                }
//            }
//            cached_high_frequency_score_ = score;
//        }
//        return score;
//    }

float Texture::highFrequencyScore()
{
    // TODO maybe cache the rendered image used here, or just case the score?
    Timer t("    highFrequencyScore");
    float score = cached_high_frequency_score_;
    if (score == 0)
    {
        // Render this texture to monochrome (square image, size x size).
        int size = 101;
        cv::Mat temp = *raster_;  // Save raster_
        Texture::rasterizeToImageCache(size, false);
        cv::Mat monochrome;
        cv::cvtColor(*raster_, monochrome, cv::COLOR_BGR2GRAY);
        // restore raster_
        *raster_ = temp;
        
        // Complex plane to contain the DFT coefficients {[0]-Real,[1]-Img}
        cv::Mat complexI;
        cv::Mat zeros = cv::Mat::zeros(monochrome.size(), CV_32F);
        std::vector<cv::Mat> planes = { monochrome, zeros };
        cv::merge(planes, complexI);
        
        // Applying DFT
        cv::dft(complexI, complexI);
        
        // Split the image into different channels
        std::vector<cv::Mat> fftChannels(2);
        split(complexI, fftChannels);
        
        cv::Mat& real = fftChannels[0];
        int width = real.rows;
        float half_width = width * 0.5;
        Vec2 center(half_width, half_width);
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < width; y++)
            {
                float length = (Vec2(x, y) - center).length();
//                float weight = sq(length / half_width);
                float weight = length / half_width;
                if (weight < 1)
                {
                    float real_part = real.at<float>(y, x);
                    score += std::abs(real_part * weight);
                }
            }
        }
        cached_high_frequency_score_ = score;
    }
    return score;
}


//    void sampleColors(Individual* individual, std::vector<Color>& samples)
//    {
//        int n = 10;
//        samples.clear();
//        std::vector<Vec2> positions;
//        jittered_grid_NxN_in_square(n, 1.4, LPRS(), positions);
//        Texture* texture = GP::textureFromIndividual(individual);
//        for (auto& p : positions)
//            samples.push_back(texture->getColor(p).clipToUnitRGB());
//    }

// TODO just a prototype
// Optional cache of 100 colors randomly sampled in unit-diameter disk.
const std::vector<Color>& Texture::cachedRandomColorSamples(RandomSequence& rs)
{
    if (cached_random_color_samples_.empty())
    {
        int n = 10;
//        samples.clear();
        std::vector<Vec2> positions;
//        jittered_grid_NxN_in_square(n, 1.4, LPRS(), positions);
        jittered_grid_NxN_in_square(n, 1.4, rs, positions);
//        Texture* texture = GP::textureFromIndividual(individual);
        for (auto& p : positions)
        {
            Color sample = getColor(p).clipToUnitRGB();
            cached_random_color_samples_.push_back(sample);
        }
    }
    return cached_random_color_samples_;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO 20201203 experiment change default type from CV_32FC3 to CV_8UC3
int Texture::default_opencv_mat_type_ = CV_32FC3;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
