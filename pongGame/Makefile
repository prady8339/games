
# Name of your output binary
TARGET = app

# Source file(s)
SRCS = main.cpp

# Compiler
CXX = clang++

# Compiler and linker flags
CXXFLAGS = -I/opt/homebrew/Cellar/raylib/5.5/include
LDFLAGS = -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib \
          -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# Default target
$(TARGET): $(SRCS)
	$(CXX) -o $(TARGET) $(SRCS) $(CXXFLAGS) $(LDFLAGS)

# Clean target
clean:
	rm -f $(TARGET)
