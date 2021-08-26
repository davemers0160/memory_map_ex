#ifndef _MMAP_TEST_H_
#define _MMAP_TEST_H_

// C/C++ Includes
#include <cstdint>
#include <string>
#include <vector>

#if defined(_WIN32) | defined(__WIN32__) | defined(__WIN32) | defined(_WIN64) | defined(__WIN64)

#else

#endif

const std::string mem_space_name = "left_right_test";
const uint64_t mem_size = 512 * 512 + 1 + 8;

const uint64_t MM_START = 0;
const uint64_t MM_SIGMA_POS = MM_START + sizeof(uint8_t);
const uint64_t MM_IMAGE_POS = MM_SIGMA_POS + sizeof(double);



typedef struct cv_mem
{

	double sigma;
	int32_t img_w;
	int32_t img_h;

	uint8_t data[mem_size];

	//cv_mem(uint8_t* d, uint64_t size)
	//{
	//	sigma = *reinterpret_cast<double*>(d);

	//	img_w = *reinterpret_cast<int32_t*>(d + 8);
	//	img_h = *reinterpret_cast<int32_t*>(d + 12);
	//	data = d + 16;

	//}



} cv_mem;








// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------

#endif  // _MMAP_TEST_H_
