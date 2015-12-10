#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cuda_runtime.h>
#include <cufft.h>
#include <stdint.h>
//#include <helper_functions.h>
//#include <helper_cuda.h>

#define handle_error(ans) { cuda_assert((ans), __FILE__, __LINE__); }
#define handle_fft(ans) { fft_assert((ans), __FILE__, __LINE__); }
inline void cuda_assert(cudaError_t code, const char *file, int line, bool abort=true)
{
	if (code != cudaSuccess) 
	{
		fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
		if (abort) exit(code);
	}
}

inline void fft_assert(cufftResult code, const char *file, int line, bool abort=true)
{
	if (code != CUFFT_SUCCESS)
	{
		fprintf(stderr, "GPUassert: FFT error %s %d\n", file, line);
		if (abort) exit(code);
	}
}

uint32_t c2c_fft(const float *input, float *output, uint32_t size)
{
	cufftHandle plan;
	float2 *host_input = NULL;
	float2 *host_output = NULL;
	float2 *device_input = NULL;
	float2 *device_output = NULL;
	uint32_t i;
	handle_error(cudaMallocHost(&host_input, sizeof(float2) * size));
	handle_error(cudaMallocHost(&host_output, sizeof(float2) * size));
	for (i = 0; i < size; i++)
	{
		host_input[i].x = input[i];
		host_input[i].y = 0;
	}
	handle_error(cudaMalloc(&device_input, sizeof(float2) * size));
	handle_error(cudaMalloc(&device_output, sizeof(float2) * size));
	handle_error(cudaMemcpy(device_input, host_input, sizeof(float2) * size, cudaMemcpyHostToDevice));
	handle_fft(cufftPlan1d(&plan, size, CUFFT_C2C, 1));
	handle_fft(cufftExecC2C(plan, (cufftComplex *) device_input, (cufftComplex *) device_output, CUFFT_FORWARD));
	handle_error(cudaMemcpy(host_output, device_output, sizeof(float2) * size, cudaMemcpyDeviceToHost));
	for (i = 0; i < size; i++)
	{
		output[i] = sqrt(host_output[i].x * host_output[i].x + host_output[i].y * host_output[i].y);
	}
	handle_error(cudaFreeHost(host_input));
	handle_error(cudaFreeHost(host_output));
	handle_error(cudaFree(device_input));
	handle_error(cudaFree(device_output));
	handle_fft(cufftDestroy(plan));
	return 0;
}
