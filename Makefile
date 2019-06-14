
llvm:
	clang++ -g src/main.cpp \
    	-Wall \
		-std=c++14 \
    	-o build/llvm/main.exe \
    	-I ./glfw/include/ \
    	-I ./ext/ \
    	-L ./glfw/lib-vc2017/ \
		-Xlinker /NODEFAULTLIB:libcmt \
		-l user32 -l gdi32 -l shell32  -l opengl32 -l glfw3 -l msvcrt\

gcc:
	g++ -g src/main.cpp \
    	-Wall \
    	-o build/mingw/main.exe \
    	-I ./ext/ \
    	-l glfw3 -l gdi32 -opengl

run:
	./build/llvm/main.exe
