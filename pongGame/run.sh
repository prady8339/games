#!/bin/bash

echo "Compiling..."

clang++ -o myapp main.cpp -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

if [ $? -eq 0 ]; then
    echo "Compilation successful. Running app..."
    ./myapp
else
    echo "Compilation failed."
fi
