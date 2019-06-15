g++ -g src/main.cpp ^
    -Wall ^
    -o build/mingw/main.exe ^
    -I ./ext/ ^
    -l glfw3 -l gdi32 -opengl