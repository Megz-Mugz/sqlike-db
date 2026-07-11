CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -Iincludes
LDFLAGS :=
GTEST_CFLAGS := $(shell pkg-config --cflags gtest)
GTEST_LIBS := $(shell pkg-config --libs gtest) -lgtest_main

TARGET := sqllike
BUILD_DIR := build
SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)
TEST_TARGET := sqllike_tests
TEST_SRC := $(wildcard tests/*.cpp)
TEST_OBJ := $(patsubst tests/%.cpp,$(BUILD_DIR)/tests/%.o,$(TEST_SRC))
TEST_APP_OBJ := $(filter-out $(BUILD_DIR)/main.o,$(OBJ))
TEST_DEP := $(TEST_OBJ:.o=.d)

.PHONY: all run test debug debug-run clean

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/tests/%.o: tests/%.cpp
	@mkdir -p $(BUILD_DIR)/tests
	$(CXX) $(CXXFLAGS) $(GTEST_CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/$(TEST_TARGET): $(TEST_APP_OBJ) $(TEST_OBJ)
	$(CXX) $^ $(LDFLAGS) $(GTEST_LIBS) -o $@

run: all
	./$(BUILD_DIR)/$(TARGET)

r: run

test: $(BUILD_DIR)/$(TEST_TARGET)
	./$(BUILD_DIR)/$(TEST_TARGET)

debug: CXXFLAGS += -g -O0
debug: clean all

debug: debug
	lldb ./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEP) $(TEST_DEP)
