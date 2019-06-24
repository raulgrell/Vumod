clang++ src/main.cpp ext/ext.cpp ^
    -Wall -std=c++17 ^
    -o main.exe ^
    -I ./glfw/include/ ^
    -I ./ext/ ^
    -L ./glfw/lib-vc2017/ ^
    -Xlinker /NODEFAULTLIB:libcmt ^
    -l user32 -l gdi32 -l shell32  -l opengl32 -l glfw3 -l msvcrt -l
    