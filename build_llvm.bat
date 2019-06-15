clang++ src/main.cpp ext/glad/glad.cpp ext/imgui/imgui.cpp ext/imgui/imgui_widgets.cpp ext/imgui/imgui_draw.cpp ext/imgui/imgui_demo.cpp ^
    -Wall -std=c++11 ^
    -o main.exe ^
    -I ./glfw/include/ ^
    -I ./ext/ ^
    -L ./glfw/lib-vc2017/ ^
    -nodefaultlibs -l kernel32 -l user32 -l gdi32 -l shell32  -l opengl32 -l glfw3 -l msvcrt