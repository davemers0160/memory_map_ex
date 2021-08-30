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

const int sigma_slider_max = 20;
int sigma_slider = 8;

cv_mem *test_data = NULL;

mem_map<cv_mem> mm;

static void on_trackbar(int, void*)
{

    double sigma = 2.0 * (sigma_slider/ (double)sigma_slider_max) + 0.1;
    
    //std::cout << "sigma: " << sigma << std::endl;

    // write sigma out to memory
    test_data->sigma = sigma;

    cv::imshow(window_name, cv_image);
}   // end of on_trackbar

// ----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    std::string sdate, stime;

    uint64_t idx = 0, jdx = 0;

    typedef std::chrono::duration<double> d_sec;
    auto start_time = std::chrono::system_clock::now();
    auto stop_time = std::chrono::system_clock::now();
    auto elapsed_time = std::chrono::duration_cast<d_sec>(stop_time - start_time);

    mm.init(mem_space_name, test_data);

    std::cout << "This side will display an image in an OpenCV window.  Use the slider to change the blur of the image." << std::endl;
    std::cout << "The other side will open the image and perform the image manipulation based on the slider value sent" << std::endl;
    std::cout << "press enter once the other side is up and running" << std::endl;
    std::cin.ignore();

    cv_image = cv::Mat(test_data->img_h, test_data->img_w, CV_8UC1, test_data->data);

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
    
    mm.close(test_data);
        
    cv::destroyAllWindows();
    
    std::cout << "Press enter to close..." << std::endl;
    std::cin.ignore();

    return 0;
    
}   // end of main
