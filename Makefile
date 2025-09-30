# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -MMD -MP
LDFLAGS = -lncurses

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Object & dep files differ by build type
OBJ_RELEASE = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/release/%.o,$(SRC))
OBJ_DEBUG   = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/debug/%.o,$(SRC))
DEPS_RELEASE = $(OBJ_RELEASE:.o=.d)
DEPS_DEBUG   = $(OBJ_DEBUG:.o=.d)

# Output executables
TARGET_RELEASE = $(BIN_DIR)/pong
TARGET_DEBUG   = $(BIN_DIR)/pong_debug

# Default target (release build)
all: release

# --- Release build ---
release: CXXFLAGS += -O2
release: $(TARGET_RELEASE)

$(TARGET_RELEASE): $(OBJ_RELEASE) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/release/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)/release
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Debug build ---
debug: CXXFLAGS += -g -O0
debug: $(TARGET_DEBUG)

$(TARGET_DEBUG): $(OBJ_DEBUG) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/debug/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)/debug
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Run targets ---
run: release
	./$(TARGET_RELEASE)

run-debug: debug
	./$(TARGET_DEBUG)

# --- Directory rules ---
$(OBJ_DIR)/release:
	mkdir -p $@

$(OBJ_DIR)/debug:
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

# --- Clean ---
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# --- Include generated dependency files ---
-include $(DEPS_RELEASE) $(DEPS_DEBUG)

.PHONY: all release debug run run-debug clean
