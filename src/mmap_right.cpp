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

cv_mem *test_data = NULL;

mem_map<cv_mem> mm(mem_space_name, test_data);

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

    cv::Mat src_image = cv::imread("../checker_board_512x512.png", cv::IMREAD_GRAYSCALE);

    cv_image = cv::Mat(src_image.rows, src_image.cols, CV_8UC1, test_data->data);

    test_data->img_h = src_image.rows;
    test_data->img_w = src_image.cols;
    test_data->sigma = 0.1;

    try
    {
        char key = 0;
        while (key != 'q')
        {
            //std::cout << "sigma: " << test_data->sigma << std::endl;

            //do the blurring on the image
            if(test_data->sigma > 0.0)
            {
                cv::GaussianBlur(src_image, cv_image, cv::Size(0, 0), test_data->sigma, test_data->sigma, cv::BORDER_REPLICATE);
            }
            
            cv::imshow("test", cv_image);
            key = cv::waitKey(20);

        }

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
