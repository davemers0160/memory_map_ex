#define _CRT_SECURE_NO_WARNINGS

// C/C++ includes
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>

// OpenCV includes
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>


// Project Includes
#include "mmap_test.h"
#include "mmap.h"


// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------
std::string window_name = "image";
cv::Mat cv_image;

const int sigma_slider_max = 10;
int sigma_slider = 8;

mem_map mm(mem_space_name, mem_size);

std::vector<uint8_t> image_data;


static void on_trackbar(int, void*)
{
    //scale = 1.0 / (double)(scale_slider + 1);
//    scale = ((0.01 - 0.005) / 20.0) * scale_slider + 0.001;
    double sigma = (sigma_slider+1.0)/ (double)sigma_slider_max;
    uint64_t position = MM_SIGMA_POS;

    mm.write(MM_SIGMA_POS, sigma);

    position = MM_IMAGE_POS;

    mm.read_range(position, 512*512, image_data);

    cv_image = cv::Mat(512, 512, CV_8UC1, image_data.data());

    cv::imshow(window_name, cv_image);
}



// ----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    std::string sdate, stime;

    uint64_t idx = 0, jdx = 0;

    typedef std::chrono::duration<double> d_sec;
    auto start_time = std::chrono::system_clock::now();
    auto stop_time = std::chrono::system_clock::now();
    auto elapsed_time = std::chrono::duration_cast<d_sec>(stop_time - start_time);

    std::cout << "This side will display an image in an OpenCV window.  Use the slider to change the blur of the image." << std::endl;

    std::cout << "The other side will open the image and perform the image manipulation based on the slider value sent" << std::endl;

    std::cout << "press enter once the other side is up and running" << std::endl;

    std::cin.ignore();

    cv_image = cv::Mat(10, 10, CV_8UC3, cv::Scalar::all(255));

    uint64_t position = MM_START;

    mm.write(position, (uint8_t)0);


    try
    {
        cv::namedWindow(window_name, cv::WINDOW_NORMAL); // Create Window
        cv::resizeWindow(window_name, 600, 600);

        std::string sigma_tb = "Sigma";

        cv::createTrackbar(sigma_tb, window_name, &sigma_slider, sigma_slider_max-1, on_trackbar);



        on_trackbar(0, 0);
        cv::waitKey(0);


        int bp = 1;
    }
    catch (std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }
    
        
    std::cout << "Press enter to close..." << std::endl;
    std::cin.ignore();

    return 0;
    
}   // end of main
