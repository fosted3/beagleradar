#include "fft.h"
#include <vector>
#include <cstdint>
#include <cassert>
#include <cstdlib>

uint32_t next_pow_2(uint32_t n)
{
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;
	return n;
}

uint32_t single_fft_abs(std::vector<float> *data, std::vector<float> *result) //use for one sweep only
{
	uint32_t i;
	uint32_t actual_size = data -> size();
	assert(data -> empty() == false);
	assert(result != NULL);
	if (actual_size & (actual_size - 1))
	{
		actual_size = next_pow_2(actual_size);
		data -> reserve(actual_size);
		result -> clear();
		result -> reserve(actual_size);
		while (data -> size() < actual_size)
		{
			data -> push_back(0);
		}
	}
	float *temp = new float[actual_size];
	if (compute_single_fft_abs(data -> data(), temp, actual_size))
	{
		delete temp;
		return 1;
	}
	for (i = 0; i < actual_size; i++)
	{
		result -> push_back(temp[i]);
	}
	delete temp;
	return 0;
}

uint32_t multi_fft_abs(std::vector<float> *data, std::vector<float> *result)
{
	uint32_t i;
	//uint32_t actual_size = data -> at(0).size();
	return 0;
}

uint32_t single_fft_complex(std::vector<float> *data, std::vector<std::pair<float, float> > *result)
{
	return 0;
}

uint32_t multi_fft_complex(std::vector<float> *data, std::vector<std::pair<float, float> > *result)
{
	return 0;
}
