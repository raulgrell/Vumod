"C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" amd64

cl.exe /Fo./build/msvc/main.obj /Fd./build/msvc/main_obj.pdb /Iext /Iglfw/include /EHsc /Zi /Fe: build/msvc/main.exe src/main.cpp /link /LIBPATH:glfw/lib-vc2017 glfw3.lib user32.lib gdi32.lib shell32.lib opengl32.lib msvcrt.lib /NODEFAULTLIB:LIBCMT