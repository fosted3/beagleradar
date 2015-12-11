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
	assert(data != NULL);
	assert(result != NULL);
	assert(data -> empty() == false);
	if (actual_size & (actual_size - 1))
	{
		actual_size = next_pow_2(actual_size);
		data -> reserve(actual_size);
		while (data -> size() < actual_size)
		{
			data -> push_back(0);
		}
	}
	result -> clear();
	result -> reserve(actual_size);
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

uint32_t multi_fft_abs(std::vector<float> *data, std::vector<float> *result, uint32_t offset, uint32_t stride) //multiple sweeps starting at offset with size stride
{
	uint32_t i;
	uint32_t j;
	uint32_t stride_size = stride;
	uint32_t actual_size = 0;
	uint32_t elements = 0;
	assert(data != NULL);
	assert(result != NULL);
	assert(data -> empty() == false);
	if(stride_size & (stride_size - 1))
	{
		stride_size = next_pow_2(stride_size);
	}
	elements = (data -> size() - offset) / stride;
	actual_size = elements * stride_size;
	result -> clear();
	result -> reserve(actual_size);
	float *new_input = new float[actual_size];
	for (i = 0; i < elements; i++)
	{
		for (j = 0; j < stride_size; j++)
		{
			new_input[i * stride_size + j] = (j >= stride) ? 0 : data -> at(i * stride + j);
		}
	}
	float *temp = new float[actual_size];
	if (compute_multi_fft_abs(new_input, temp, actual_size, stride_size))
	{
		delete new_input;
		delete temp;
		return 1;
	}
	for (i = 0; i < actual_size; i++)
	{
		result -> push_back(temp[i]);
	}
	return 0;
}

uint32_t single_fft_complex(std::vector<float> *data, std::vector<std::pair<float, float> > *result) //single sweep returns mag & arg
{
	uint32_t i;
	uint32_t actual_size = data -> size();
	assert(data != NULL);
	assert(result != NULL);
	assert(data -> empty() == false);
	if (actual_size & (actual_size - 1))
	{
		actual_size = next_pow_2(actual_size);
		data -> reserve(actual_size);
		while (data -> size() < actual_size)
		{
			data -> push_back(0);
		}
	}
	result -> clear();
	result -> reserve(actual_size);
	float *temp = new float[actual_size * 2];
	if (compute_single_fft_complex(data -> data(), temp, actual_size))
	{
		delete temp;
		return 1;
	}
	for (i = 0; i < actual_size; i++)
	{
		result -> push_back(std::make_pair(temp[2*i], temp[(2*i)+1]));
	}
	delete temp;
	return 0;
}

uint32_t multi_fft_complex(std::vector<float> *data, std::vector<std::pair<float, float> > *result, uint32_t offset, uint32_t stride) //multi sweep starting at offset with size stride returns mag & arg
{
	uint32_t i;
	uint32_t j;
	uint32_t stride_size = stride;
	uint32_t actual_size = 0;
	uint32_t elements = 0;
	assert(data != NULL);
	assert(result != NULL);
	assert(data -> empty() == false);
	if(stride_size & (stride_size - 1))
	{
		stride_size = next_pow_2(stride_size);
	}
	elements = (data -> size() - offset) / stride;
	actual_size = elements * stride_size;
	result -> clear();
	result -> reserve(actual_size);
	float *new_input = new float[actual_size];
	for (i = 0; i < elements; i++)
	{
		for (j = 0; j < stride_size; j++)
		{
			new_input[i * stride_size + j] = (j >= stride) ? 0 : data -> at(i * stride + j);
		}
	}
	float *temp = new float[actual_size * 2];
	if (compute_multi_fft_complex(new_input, temp, actual_size, stride_size))
	{
		delete new_input;
		delete temp;
		return 1;
	}
	for (i = 0; i < actual_size; i++)
	{
		result -> push_back(std::make_pair(temp[2*i], temp[(2*i)+1]));
	}
	return 0;
}
