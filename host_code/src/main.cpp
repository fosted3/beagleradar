#include "image.h"
#include "cuda_helper.h"
#include "network.h"
#include <vector>
#include <math.h>
#include <iostream>

int main(void)
{
	std::vector<float> inputs(2000 * 1000);
	std::vector<float> outputs;
	init_image();
	for (unsigned int i = 0; i < 2000; i++)
	{
		for (unsigned int j = 0; j < 1000; j++)
		{
			inputs[i * 1000 + j] = cos(j * 3.1415 * i / 1000.0);
		}
	}
	uint32_t rs;
	multi_fft_abs(&inputs, &outputs, 0, 1000, &rs);
	generate_image_flat(&outputs, "out2.png", rs);
	deinit_image();
	return 0;
}
