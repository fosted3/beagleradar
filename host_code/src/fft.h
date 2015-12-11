#ifndef FFT_H_
#define FFT_H_
#include <stdint.h>
uint32_t compute_single_fft_abs(const float *input, float *output, uint32_t size);
uint32_t compute_multi_fft_abs(const float *input, float *output, uint32_t size, uint32_t stride);
uint32_t compute_single_fft_complex(const float *input, float *output, uint32_t size);
uint32_t compute_multi_fft_complex(const float *input, float *output, uint32_t size, uint32_t stride);
#endif
