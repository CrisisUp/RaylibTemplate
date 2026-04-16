# Professional CMake Wrapper Makefile

BUILD_DIR = build
TARGET = RalphArcade

all:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && make

run: all
	cd $(BUILD_DIR) && ./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
	rm -f highscore.txt

.PHONY: all run clean
