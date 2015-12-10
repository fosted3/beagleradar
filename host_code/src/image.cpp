#include <FreeImage.h>
#include <vector>
#include <string>
#include <cassert>
#include <iostream>

int clamp(int a, int x, int b)
{
	if (x < a) { return a; }
	if (x > b) { return b; }
	return x;
}

void init_image(void)
{
	FreeImage_Initialise();
}

void deinit_image(void)
{
	FreeImage_DeInitialise();
}

int generate_image (std::vector<std::vector<float> > *input, std::string filename)
{
	assert(input -> empty() == false);
	assert(input -> at(0).empty() == false);
	FIBITMAP *image = FreeImage_Allocate(input -> size(), input -> at(0).size(), 24);
	RGBQUAD pixel;
	float max = 0;
	int v;
	if (!image)
	{
		std::cerr << "Can't allocate memory for image. Exiting." << std::endl;
		exit(1);
	}
	for (unsigned int x = 0; x < input -> size(); x++)
	{
		for (unsigned int y = 0; y < input -> at(0).size(); y++)
		{
			if (input -> at(x).at(y) > max)
			{
				max = input -> at(x).at(y);
			}
		}
	}
	for (unsigned int x = 0; x < input -> size(); x++)
	{
		for (unsigned int y = 0; y < input -> at(0).size(); y++)
		{
			v = clamp(0, (int) (255.0 * input -> at(x).at(y) / max), 255);
			pixel.rgbRed = v;
			pixel.rgbGreen = v;
			pixel.rgbBlue = v;
			FreeImage_SetPixelColor(image, x, y, &pixel);
		}
	}
	if (!FreeImage_Save(FIF_PNG, image, filename.c_str(), 0))
	{
		std::cerr << "Can't save " << filename << ". Exiting." << std::endl;
		exit(1);
	}
	FreeImage_Unload(image);
	return 0;
}
