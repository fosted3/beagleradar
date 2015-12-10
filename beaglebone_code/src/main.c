#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include "libbeaglelogic.h"
#include <assert.h>
#include "i2c.h"


typedef enum
{
	ERR_SUCCESS = 0,
	ERR_OPEN_FAIL,
	ERR_ALLOC_FAIL,
	ERR_BUFF_FAIL,
	ERR_I2C_FAIL,
	ERR_INIT_FAIL
} result_t;

typedef struct
{
	uint16_t sample : 12;
	uint16_t clock : 1;
	uint16_t overflow : 1;
	uint16_t reserved : 2;
} sample_t;

typedef struct
{
	int beaglelogic_fd;
	int i2c_fd;
	uint8_t *beaglelogic_mem;
	sample_t *buffer;
	uint32_t to_read;
	uint16_t dac;
	unsigned char dac_buf[2];
} state_t;

result_t update_dac(state_t *state)
{
	state -> i2c_fd = i2c_open(I2CBUS, 0x60);
	state -> dac_buf[0] = ((state -> dac) >> 8) & 0x0F;
	state -> dac_buf[1] = (state -> dac) & 0xFF;
	if (i2c_write(state -> i2c_fd, state -> dac_buf, 2) != 2)
	{
		return ERR_I2C_FAIL;
	}
	i2c_close(state -> i2c_fd);
	return ERR_SUCCESS;
}

result_t init(state_t *state)
{
	assert(sizeof(sample_t) == 2); //check that sample struct is actually 2 bytes
	state -> beaglelogic_fd = beaglelogic_open(); //open beaglelogic
	if (state -> beaglelogic_fd == -1) //didn't open
	{
		return ERR_OPEN_FAIL;
	}
	state -> beaglelogic_mem = beaglelogic_mmap(state -> beaglelogic_fd); //mmap beaglelogic memory
	if (state -> beaglelogic_mem == NULL) //didn't mmap
	{
		return ERR_ALLOC_FAIL;
	}
	beaglelogic_set_buffersize(state -> beaglelogic_fd, 128 * 1024 * 1024); //request 128MiB
	beaglelogic_get_buffersize(state -> beaglelogic_fd, &(state -> to_read)); //read back actual size
	assert((state -> to_read) % 32 == 0);
	state -> buffer = calloc((state -> to_read) / 32, 32); //alloc buffer
	if (state -> buffer == NULL) //alloc failed
	{
		return ERR_BUFF_FAIL;
	}
	beaglelogic_set_samplerate(state -> beaglelogic_fd, 100 * 1000 * 1000); //set to 100MSPS
	beaglelogic_set_sampleunit(state -> beaglelogic_fd, BL_SAMPLEUNIT_16_BITS); //set to 16b sample size
	beaglelogic_set_triggerflags(state -> beaglelogic_fd, BL_TRIGGERFLAGS_ONESHOT); //one capture
	state -> dac = 2048;
	if (update_dac(state))
	{
		return ERR_I2C_FAIL;
	}
	return ERR_SUCCESS;
}

result_t capture(state_t *state)
{
	int32_t size;
	uint32_t offset = 0;
	//uint32_t samples = (state -> to_read) / 2; //2B per sample
	//uint32_t i;
	beaglelogic_start(state -> beaglelogic_fd); //start capture
	memset(state -> buffer, 0xFF, state -> to_read);
	do
	{
		size = read(state -> beaglelogic_fd, (state -> buffer) + (offset / 2), 4 * 1024 * 1024); //read in 4 MiB chunks
		if (size > 0) { offset += size; } //actually got data
		//printf("offset is %u\n\r", offset);
	} while (offset < state -> to_read);
	/*for (i = 0; i < samples; i++)
	{
		printf("sample %u is %x %x %x %04x\n\r", i, ((state -> buffer)[i]).reserved, ((state -> buffer)[i]).clock, ((state -> buffer)[i]).overflow, ((state -> buffer)[i]).sample);
	}*/
	return ERR_SUCCESS;
}

void cleanup(state_t *state)
{
	beaglelogic_munmap(state -> beaglelogic_fd, state -> beaglelogic_mem);
	beaglelogic_close(state -> beaglelogic_fd);
	free(state -> buffer);
}

int main(void)
{
	state_t state;
	if (init(&state))
	{
		return ERR_INIT_FAIL;
	}
	capture(&state);
	cleanup(&state);
	return ERR_SUCCESS;
}
