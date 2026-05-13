CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -Iincludes
LDFLAGS :=

TARGET := sqllike
BUILD_DIR := build
SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

.PHONY: all run debug clean

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: all
	./$(BUILD_DIR)/$(TARGET)

debug: CXXFLAGS += -g -O0
debug: clean all

clean:
	rm -rf $(BUILD_DIR)

-include $(DEP)
