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

.PHONY: all clean run

all: $(TARGET)

# Link all .o files into the root executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

# Compile src/%.cpp into bin/%.o
bin/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -rf bin
