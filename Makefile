# Compiler
CXX := g++

# Compiler Flags
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS := -Llibs/freeglut -Wl,-rpath,'$$ORIGIN/../libs/freeglut'
LDLIBS := -lglut -lGL -lGL

# Source files from src/
SRC := $(shell find src -name '*.cpp')

# Map src/path/to/file.cpp -> bin/path/to/file.o
OBJ := $(patsubst src/%.cpp, bin/%.o, $(SRC))

# Output executable directly in the root folder
TARGET := game

.PHONY: all clean run test-hud

all: $(TARGET)

# Link all .o files into the root executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

# Compile src/%.cpp into bin/%.o
bin/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ----------------------------------------------------------------
# Standalone HUD preview test (shares the game's render classes)
# ----------------------------------------------------------------
TEST_HUD_OBJ := bin/tests/hud_test.o bin/render/Font.o bin/render/HUD.o \
                bin/render/Renderer.o bin/entities/Character.o \
                bin/graphics/Animation.o bin/graphics/Sprite.o bin/graphics/Texture.o

test-hud: $(TEST_HUD_OBJ)
	$(CXX) $(TEST_HUD_OBJ) -o test-hud $(LDFLAGS) $(LDLIBS)

bin/tests/%.o: tests/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

run-test-hud: test-hud
	./test-hud

clean:
	rm -f $(TARGET)
	rm -f test-hud
	rm -rf bin
