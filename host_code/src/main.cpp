#include "image.h"
#include "cuda_helper.h"
#include "network.h"
#include <vector>
#include <math.h>

int main(void)
{
	std::vector<float> input;
	std::vector<float> output;
	input.reserve(8000);
	for (unsigned int i = 0; i < 8000; i++)
	{
		input.push_back(cos(i * 3.1415 / 500.0));
	}
	unsigned int result = compute_fft(&input, &output);
	return 0;
}
