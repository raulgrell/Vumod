clang++ src/main.cpp ext/glad/glad.cpp ext/imgui/imgui.cpp ext/imgui/imgui_widgets.cpp ext/imgui/imgui_draw.cpp ext/imgui/imgui_demo.cpp ^
    -Wall -std=c++11 ^
    -o main.exe ^
    -I ./glfw/include/ ^
    -I ./ext/ ^
    -L ./glfw/lib-vc2017/ ^
    -l glfw3dll 