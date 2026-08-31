# ============================================================
# OS Detection
# ============================================================

ifeq ($(OS),Windows_NT)

	DETECTED_OS := Windows
	EXE_EXT     := .exe
	RM          := del /Q /F
	MKDIR        = if not exist "$(1)" mkdir "$(1)"

	GRAPHICS_LIBS := -lfreeglut -lopengl32 -lglu32 -lmingw32 -lSDL2main -lSDL2
	EXTRA_LIBS    := -lm
	LDFLAGS_OS    := -static-libgcc -static-libstdc++

else

	DETECTED_OS := $(shell uname -s)
	EXE_EXT     :=
	RM          := rm -f
	MKDIR        = mkdir -p $(1)

	GRAPHICS_LIBS := -lGL -lGLU -lglut -lSDL2
	EXTRA_LIBS    := -pthread -lm -ldl
	LDFLAGS_OS    := -Wl,-rpath=libs

endif


# ============================================================
# Compiler and Flags
# ============================================================

CXX      := g++

CXXFLAGS := -Iinclude \
            -Wall \
            -Wextra \
            -Wpedantic \
            -std=c++17 \
            -O2 \
            -g3

LINKER   := -Llibs $(LDFLAGS_OS) $(GRAPHICS_LIBS) $(EXTRA_LIBS)


# ============================================================
# Directories
# ============================================================

SRC_DIR        := src
BUILD_DIR      := build

TEST_DIR       := test
TEST_BUILD_DIR := build/test


# ============================================================
# Main Program Sources
# ============================================================

# Automatically finds every .cpp inside src/
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

OBJS := $(patsubst \
	$(SRC_DIR)/%.cpp, \
	$(BUILD_DIR)/%.o, \
	$(SRCS))


TARGET := fighting_game$(EXE_EXT)


# ============================================================
# Main Build
# ============================================================

all: $(TARGET) dlls


$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LINKER)


# Compile src/*.cpp -> build/*.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(BUILD_DIR):
	$(call MKDIR,$(BUILD_DIR))


# ============================================================
# Runtime DLLs (Windows only)
# ============================================================

# freeglut's import library (libfreeglut.dll.a) is hard-wired to look
# for a file named "libfreeglut.dll" at runtime, but the DLL in libs/
# is named "freeglut.dll" -- so we copy it under both names.

ifeq ($(DETECTED_OS),Windows)
dlls:
	copy /Y libs\*.dll . >nul
	copy /Y libs\freeglut.dll libfreeglut.dll >nul
else
dlls:
endif

.PHONY: dlls


# ============================================================
# Unit Tests
# ============================================================

# Each test lists the src modules it needs.
#
# Example:
#
# character_test_DEPS := Character Sprite Animation Texture Input HitBox Renderer
#
# becomes:
#
# build/test/character_test.o
# build/Character.o
# build/Sprite.o
# build/Animation.o
# ...


texture_test_DEPS := \
	Texture

hitbox_test_DEPS := \
	HitBox

renderer_test_DEPS := \
	Renderer Texture

font_test_DEPS := \
	Font

sprite_test_DEPS := \
	Sprite Renderer Texture

animation_test_DEPS := \
	Animation Texture

input_test_DEPS := \
	Input

character_test_DEPS := \
	Character Sprite Animation Texture Input HitBox Renderer AudioManager

hud_test_DEPS := \
	Character HUD Sprite Animation Texture Input HitBox Renderer Font AudioManager

round_timer_test_DEPS := \
	RoundTimer Character HUD Sprite Animation Texture Input HitBox Renderer Font AudioManager

camera_test_DEPS := \
	Camera RoundTimer Character HUD Sprite Animation Texture Input HitBox Renderer Font AudioManager

audio_manager_test_DEPS := \
	AudioManager

config_test_DEPS := \
	Config Input


TEST_NAMES := \
	texture_test \
	hitbox_test \
	renderer_test \
	font_test \
	sprite_test \
	animation_test \
	input_test \
	character_test \
	hud_test \
	round_timer_test \
	camera_test \
	audio_manager_test \
	config_test


# ============================================================
# Test Object Compilation
# ============================================================

$(TEST_BUILD_DIR):
	$(call MKDIR,$(TEST_BUILD_DIR))


# test/foo_test.cpp -> build/test/foo_test.o
$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(TEST_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


# ============================================================
# Test Linking
# ============================================================

define MAKE_TEST

$1$(EXE_EXT): $(TEST_BUILD_DIR)/$1.o $$(addprefix $(BUILD_DIR)/,$$($1_DEPS:=.o))
	$$(CXX) $$^ -o $$@ $$(LINKER)

.PHONY: $1

$1: $1$(EXE_EXT) dlls

endef


$(foreach test,$(TEST_NAMES),$(eval $(call MAKE_TEST,$(test))))


# ============================================================
# Build All Tests
# ============================================================

tests: $(TEST_NAMES)


# ============================================================
# Cleaning
# ============================================================

clean:
	-$(RM) $(TARGET)
	-$(RM) $(BUILD_DIR)/*.o
	-$(RM) $(TEST_BUILD_DIR)/*.o
	-$(RM) *_test$(EXE_EXT)


.PHONY: all tests clean