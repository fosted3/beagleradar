CC=g++
NVCC=nvcc
CFLAGS=-c -Wall -std=c++11 -Wextra -march=native -mtune=native
CUDA_CFLAGS=-c -m64 -arch=compute_30 -code=sm_30
LDFLAGS=-lfreeimage
CUDA_LDFLAGS=-lcuda -lcudart -lcufft
EXECUTABLE=beagleradar_host
DIRS=bin/debug/ bin/release/  build/debug/ build/release/ data/ img/
CUDA_INCLUDES=-I/opt/cuda/include

all:
	make debug
	make release

debug: CFLAGS += -g -Og
debug: CUDA_CFLAGS += -g -G -O0
debug: BIN_DIR=bin/debug
debug: BUILD_DIR=build/debug
debug: setup
debug: $(EXECUTABLE)

release: CFLAGS += -O2
release: CUDA_CFLAGS += -O2
release: BIN_DIR=bin/release
release: BUILD_DIR=build/release
release: setup
release: $(EXECUTABLE)

$(EXECUTABLE): $(BUILD_DIR)/main.o $(BUILD_DIR)/image.o $(BUILD_DIR)/cuda_helper.o $(BUILD_DIR)/fft.o $(BUILD_DIR)/network.o
	$(NVCC) $(BUILD_DIR)/main.o $(BUILD_DIR)/image.o $(BUILD_DIR)/cuda_helper.o $(BUILD_DIR)/fft.o $(BUILD_DIR)/network.o -o $(BIN_DIR)/$(EXECUTABLE) $(CUDA_LDFLAGS) $(LDFLAGS)

$(BUILD_DIR)/main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/image.o: src/image.cpp
	$(CC) $(CFLAGS) src/image.cpp -o $(BUILD_DIR)/image.o

$(BUILD_DIR)/cuda_helper.o: src/cuda_helper.cpp
	$(CC) $(CFLAGS) src/cuda_helper.cpp -o $(BUILD_DIR)/cuda_helper.o

$(BUILD_DIR)/network.o: src/network.cpp
	$(CC) $(CFLAGS) src/network.cpp -o $(BUILD_DIR)/network.o

$(BUILD_DIR)/fft.o: src/fft.cu
	$(NVCC) $(CUDA_CFLAGS) src/fft.cu -o $(BUILD_DIR)/fft.o $(CUDA_INCLUDES)

setup:
	mkdir -p $(DIRS)

clean:
	rm -f build/debug/*.o build/release/*.o bin/debug/* bin/release/*

clean_data:
	rm -f data/* img/*

test: debug
	valgrind --leak-check=full --show-reachable=yes --read-var-info=yes --track-origins=yes ./bin/debug/$(EXECUTABLE)
