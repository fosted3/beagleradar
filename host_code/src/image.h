#ifndef IMAGE_H_
#define IMAGE_H_

#include <vector>
#include <string>

void init_image(void);
void deinit_image(void);
int generate_image(std::vector<std::vector<float> > *input, std::string filename);
int generate_image_flat(std::vector<float> *input, std::string filename, uint32_t stride);

#endif
