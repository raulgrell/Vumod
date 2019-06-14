
llvm:
	clang++ -g src/main.cpp ext/glad/glad.cpp ext/imgui/imgui.cpp ext/imgui/imgui_widgets.cpp ext/imgui/imgui_draw.cpp ext/imgui/imgui_demo.cpp \
    	-Wall \
		-std=c++11 -stdlib=libc++ \
    	-o build/llvm/main.exe \
    	-I ./glfw/include/ \
    	-I ./ext/ \
    	-L ./glfw/lib-vc2015/ \
    	-nodefaultlibs -l kernel32 -l user32 -l gdi32 -l shell32  -l opengl32 -l glfw3 -l msvcrt

gcc:
	g++ -g src/main.cpp ext/glad/glad.cpp ext/imgui/imgui.cpp ext/imgui/imgui_widgets.cpp ext/imgui/imgui_draw.cpp ext/imgui/imgui_demo.cpp \
    	-Wall \
    	-o build/mingw/main.exe \
    	-I ./ext/ \
    	-l glfw3 -l gdi32 -opengl
