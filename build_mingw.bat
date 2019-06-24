g++ -g src/main.cpp ext/ext.cpp ^
    -Wall ^
    -o build/mingw/main.exe ^
    -I ./ext/ ^
    -l glfw3 -l kernel32 -l gdi32 -l opengl32 -l