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

	GRAPHICS_LIBS := -lGL -lGLU -lglut -lSDL2
	LDFLAGS_OS    := -Wl,-rpath=libs
endif

# Compiler and Flags
CXX        := g++
CXXFLAGS   := -Iinclude -Wall -Wextra -Wpedantic -std=c++17 -O2 -g3
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
	$(RM) $(TEST_BUILD_DIR)/*.o
	$(RM) *_test$(EXE_EXT)

.PHONY: all clean

# ============================================================
# Unit test targets - only for already-built, standalone systems.
# Each test links against ONLY the src/*.cpp files it actually
# depends on, not the whole engine (so e.g. texture_test doesn't
# need to pull in Character.cpp or Game.cpp at all).
# ============================================================

TEST_DIR       := test
TEST_BUILD_DIR := build/test

# One line per test file, listing which src/ modules (no .cpp) it
# needs compiled in alongside it. Add a new line here whenever you
# add a new *_test.cpp - nothing else in this file needs to change.
texture_test_DEPS    := Texture
hitbox_test_DEPS      := HitBox
renderer_test_DEPS    := Renderer Texture
font_test_DEPS         := Font
sprite_test_DEPS       := Sprite Renderer Texture
animation_test_DEPS   := Animation Texture
input_test_DEPS         := Input
character_test_DEPS   := Character Sprite Animation Texture Input HitBox Renderer
hud_test_DEPS			:= Character HUD Sprite Animation Texture Input HitBox Renderer Font HitBox
round_timer_test_DEPS			:= RoundTimer

TEST_NAMES := texture_test hitbox_test renderer_test font_test sprite_test animation_test input_test character_test hud_test round_timer_test

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

# Compile a test's own .cpp (test/foo_test.cpp -> build/test/foo_test.o)
$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(TEST_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Template: build ONE test - compile its deps (reusing the existing
# src object rule), compile the test file itself, link, then run it.
# $(1) = test name (e.g. texture_test)
define MAKE_TEST
$(1): $(TEST_BUILD_DIR)/$(1).o $$(patsubst %,$(BUILD_DIR)/%.o,$$($(1)_DEPS))
	$(CXX) $$^ -o $(1)$(EXE_EXT) $(LINKER)
	./$(1)$(EXE_EXT)

.PHONY: $(1)
endef

$(foreach t,$(TEST_NAMES),$(eval $(call MAKE_TEST,$(t))))