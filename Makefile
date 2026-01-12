CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I./include -I./src -ggdb
LDFLAGS := -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -I./include -I./src

SRC_DIR := src
BUILD_DIR := build
TARGET := $(BUILD_DIR)/executable

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
