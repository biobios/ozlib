BUILD_DIR ?= build

.PHONY: all build clean

all: build

build: $(BUILD_DIR)/CMakeCache.txt
	cmake --build $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/CMakeCache.txt:
	cmake -S . -B $(BUILD_DIR)
