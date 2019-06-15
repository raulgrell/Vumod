
llvm:
	clang++ -g src/main.cpp \
		-Wall \
		-std=c++14 \
		-o build/llvm/main.exe \
		-I ./ext/ \
		-I ./glfw/include/ \
		-L ./glfw/lib-vc2017/ \
		-Xlinker /NODEFAULTLIB:libcmt \
		-l user32 -l gdi32 -l shell32  -l opengl32 -l glfw3 -l msvcrt

gcc:
	g++ -g src/main.cpp \
		-Wall \
		-o build/mingw/main.exe \
		-I ./ext/ \
		-l glfw3 -l gdi32 -opengl

msvc: 
	cl src/main.cpp ^
		/Fo./build/msvc/main.obj /Fd./build/msvc/main_obj.pdb /Fe: build/msvc/main.exe ^
		/Iext /Iglfw/include /EHsc /Zi ^
		/link /LIBPATH:glfw/lib-vc2017 /NODEFAULTLIB:LIBCMT ^
		glfw3.lib user32.lib gdi32.lib shell32.lib opengl32.lib msvcrt.lib 

run:
	./build/llvm/main.exe
