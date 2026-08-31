# ============================================================
# OS Detection
# ============================================================

ifeq ($(OS),Windows_NT)

    DETECTED_OS := Windows
    EXE_EXT     := .exe
    RM          := del /Q
    MKDIR       := if not exist

    GRAPHICS_LIBS := -lmingw32 -lSDL2main -lSDL2 \
                     -lsetupapi -limm32 -lole32 -loleaut32 \
                     -luser32 -lgdi32 -lversion -lwinmm -lshell32 \
                     -lfreeglut -lopengl32 -lglu32
    LDFLAGS_OS    := -Llibs/windows
    EXTRA_LIBS    := -lm

else

    DETECTED_OS := $(shell uname -s)
    EXE_EXT     :=
    RM          := rm -f
    MKDIR       := mkdir -p

    GRAPHICS_LIBS := -lGL -lGLU -lglut -lSDL2
    LDFLAGS_OS    := -Llibs/linux -Wl,-rpath=libs/linux
    EXTRA_LIBS    := -pthread -lm -ldl

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

LINKER   := $(LDFLAGS_OS) $(GRAPHICS_LIBS) $(EXTRA_LIBS)


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


$(TARGET): $(OBJS) | dlls
	$(CXX) $(OBJS) -o $@ $(LINKER)


# Compile src/*.cpp -> build/*.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(BUILD_DIR):
ifeq ($(OS),Windows_NT)
	$(MKDIR) $(BUILD_DIR) mkdir $(BUILD_DIR)
else
	$(MKDIR) $(BUILD_DIR)
endif


# ============================================================
# Runtime DLLs (Windows only)
# ============================================================

# libs/windows already contains correctly-named DLLs (libfreeglut.dll,
# SDL2.dll). -Llibs/windows only helps the linker find import libs at
# link time -- the actual .dll files still need to sit next to the
# built exe (or be on PATH) to run.

ifeq ($(OS),Windows_NT)
dlls:
	copy /Y libs\windows\*.dll . >nul
else
dlls:
endif

.PHONY: dlls


# ============================================================
# Unit Tests
# ============================================================

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
ifeq ($(OS),Windows_NT)
	$(MKDIR) $(subst /,\,$(TEST_BUILD_DIR)) mkdir $(subst /,\,$(TEST_BUILD_DIR))
else
	$(MKDIR) $(TEST_BUILD_DIR)
endif


# test/foo_test.cpp -> build/test/foo_test.o
$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(TEST_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


# ============================================================
# Test Linking
# ============================================================

define MAKE_TEST

$1$(EXE_EXT): $(TEST_BUILD_DIR)/$1.o $$(addprefix $(BUILD_DIR)/,$$($1_DEPS:=.o)) | dlls
	$$(CXX) $$^ -o $$@ $$(LINKER)

ifneq ($(EXE_EXT),)
$1: $1$(EXE_EXT)
.PHONY: $1
endif

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
ifeq ($(OS),Windows_NT)
	-$(RM) $(TARGET)
	-$(RM) $(subst /,\,$(BUILD_DIR))\*.o
	-$(RM) $(subst /,\,$(TEST_BUILD_DIR))\*.o
	-$(RM) *_test$(EXE_EXT)
else
	$(RM) $(TARGET)
	$(RM) $(BUILD_DIR)/*.o
	$(RM) $(TEST_BUILD_DIR)/*.o
	$(RM) *_test$(EXE_EXT)
endif

.PHONY: all tests clean