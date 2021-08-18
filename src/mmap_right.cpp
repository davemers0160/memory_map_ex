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
cv::Mat cv_image;

mem_map mm(mem_space_name, mem_size);

std::vector<uint8_t> image_data;


// ----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    std::string sdate, stime;

    uint64_t idx = 0, jdx = 0;

    typedef std::chrono::duration<double> d_sec;
    auto start_time = std::chrono::system_clock::now();
    auto stop_time = std::chrono::system_clock::now();
    auto elapsed_time = std::chrono::duration_cast<d_sec>(stop_time - start_time);

    std::cout << "This side will load in an image and store in an OpenCV Mat" << std::endl;

    std::cout << "The other side will send the sigma value using the memory mapped location" << std::endl;

    std::cout << "press enter once the other side is up and running" << std::endl;

    //std::cin.ignore();

    cv::Mat src_image = cv::imread("../checker_board_512x512.png", cv::IMREAD_GRAYSCALE);

    cv_image = cv::Mat(src_image.rows, src_image.cols, CV_8UC1);
    uint64_t img_size = src_image.rows * src_image.cols;

    //uint64_t position = MM_START;
    mm.write(MM_START, (uint8_t)0);
    mm.write(MM_SIGMA_POS, (double)0.1);

    double sigma = 0.0;

    try
    {
        char key = 0;
        while (key != 'q')
        {
            // read in the sigma value
            //position = MM_SIGMA_POS;
            mm.read(MM_SIGMA_POS, sigma);

            std::cout << "sigma: " << sigma << std::endl;

            //do the blurring on the image
            if(sigma > 0.0)
            {
                std::cout << "y";
                cv::GaussianBlur(src_image, cv_image, cv::Size(0, 0), sigma, sigma, cv::BORDER_REPLICATE);
            }
            
            cv::imshow("test", cv_image);
            
            mm.write_range(MM_IMAGE_POS, img_size, cv_image.ptr(0));

            key = cv::waitKey(20);

        }

        int bp = 1;
    }
    catch (std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }
    
    mm.close();

    cv::destroyAllWindows();
        
    std::cout << "Press enter to close..." << std::endl;
    std::cin.ignore();

    return 0;
    
}   // end of main
