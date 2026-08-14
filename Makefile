# Detect OS
ifeq ($(OS),Windows_NT)
	DETECTED_OS   := Windows
	EXE_EXT       := .exe
	RM            := del /Q

	GRAPHICS_LIBS := -lfreeglut -lopengl32 -lglu32
	LDFLAGS_OS    :=
else
	DETECTED_OS   := $(shell uname -s)
	EXE_EXT       :=
	RM            := rm -f

	GRAPHICS_LIBS := -lGL -lGLU -lglut
	LDFLAGS_OS    := -Wl,-rpath=libs
endif

# Compiler and Flags
CXX        := g++
CXXFLAGS   := -Iinclude -Wall -Wextra -Wpedantic -Werror -std=c++17 -O2 -g3
LINKER     := -Llibs $(LDFLAGS_OS) $(GRAPHICS_LIBS)

# Directories
SRC_DIR    := src
BUILD_DIR  := build

# Source/object discovery - picks up every .cpp in src/ automatically,
# so new files (Character.cpp, CombatSystem.cpp, etc.) don't need to be
# added here by hand.
SRCS       := $(wildcard $(SRC_DIR)/*.cpp)
OBJS       := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TARGET     := fighting_game$(EXE_EXT)

# Build Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LINKER)

# Compile each .cpp into build/*.o, mirroring src/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure build/ exists before compiling into it
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	$(RM) $(TARGET)
	$(RM) $(BUILD_DIR)/*.o

.PHONY: all clean
