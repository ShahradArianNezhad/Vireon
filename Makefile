CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -I./include -I./src -I/usr/include/freetype2 -O3
LDFLAGS := -lglfw -lGL -lX11 -lpthread -lfreetype -lXrandr -lXi -ldl -I./include -I./src -O3

SRC_DIR := src
BUILD_DIR := build
TARGET := bin/executable

SRCS := $(shell find $(SRC_DIR) -name "*.c*")
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

re:clean all

.PHONY: all clean re
