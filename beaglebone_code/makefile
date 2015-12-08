CC=gcc
CFLAGS=-c -Wall -Wextra
LDFLAGS=-lm -lrt
EXECUTABLE=beagleradar-server
DIRS=bin/debug/ bin/release/ build/debug/ build/release/

all:
	make debug
	make release

debug: CFLAGS += -g -Og
debug: BIN_DIR=bin/debug
debug: BUILD_DIR=build/debug
debug: setup
debug: $(EXECUTABLE)

release: CFLAGS += -O0
release: BIN_DIR=bin/release
release: BUILD_DIR=build/release
release: setup
release: $(EXECUTABLE)

$(EXECUTABLE): $(BUILD_DIR)/beaglelogic.o $(BUILD_DIR)/main.o
	$(CC) $(BUILD_DIR)/beaglelogic.o $(BUILD_DIR)/main.o -o $(BIN_DIR)/$(EXECUTABLE) $(LDFLAGS)

$(BUILD_DIR)/beaglelogic.o: src/beaglelogic.c
	$(CC) $(CFLAGS) src/beaglelogic.c -o $(BUILD_DIR)/beaglelogic.o

$(BUILD_DIR)/main.o: src/main.c
	$(CC) $(CFLAGS) src/main.c -o $(BUILD_DIR)/main.o
	
setup:
	mkdir -p $(DIRS)
	
clean:
	rm -f build/debug/*.o build/release/*.o bin/debug/* bin/release/*
