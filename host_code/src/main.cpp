#include "image.h"
#include "cuda_helper.h"
#include "network.h"
#include <vector>
#include <math.h>
#include <iostream>

int main(void)
{
	std::vector<std::vector<float> > inputs(1000, std::vector<float>(8000));
	std::vector<std::vector<float> > outputs(1000, std::vector<float>(8000));
	init_image();
	for (unsigned int i = 0; i < 1000; i++)
	{
		for (unsigned int j = 0; j < 8000; j++)
		{
			inputs[i][j] = cos(j * 3.1415 * i / 1000.0);
		}
		compute_fft(&(inputs[i]), &(outputs[i]));
	}
	generate_image(&outputs, "out.png");
	deinit_image();
	return 0;
}
