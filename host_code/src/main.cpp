#include "image.h"
#include "cuda_helper.h"
#include "network.h"
#include <vector>
#include <math.h>
#include <iostream>

int main(void)
{
	std::vector<std::vector<float> > inputs(2000, std::vector<float>(1000));
	std::vector<std::vector<float> > outputs(2000, std::vector<float>(1000));
	init_image();
	for (unsigned int i = 0; i < 2000; i++)
	{
		for (unsigned int j = 0; j < 1000; j++)
		{
			inputs[i][j] = cos(j * 3.1415 * i / 1000.0);
		}
		single_fft_abs(&(inputs[i]), &(outputs[i]));
	}
	generate_image(&outputs, "out.png");
	deinit_image();
	return 0;
}
