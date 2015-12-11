#ifndef CUDA_HELPER_H_
#define CUDA_HELPER_H_

uint32_t single_fft_abs(std::vector<float> *data, std::vector<float> *result);
uint32_t multi_fft_abs(std::vector<float> *data, std::vector<float> *result, uint32_t offset, uint32_t stride, uint32_t *return_stride);
uint32_t single_fft_complex(std::vector<float> *data, std::vector<std::pair<float, float> > *result);
uint32_t multi_fft_complex(std::vector<float> *data, std::vector<std::pair<float, float> > *result, uint32_t offset, uint32_t stride, uint32_t *return_stride);

#endif
