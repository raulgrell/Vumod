cl src/main.cpp ^
    /Fo./build/msvc/main.obj /Fd./build/msvc/main_obj.pdb /Fe: build/msvc/main.exe ^
    /I./ext /I./glfw/include /EHsc /Zi ^
    /link /LIBPATH:glfw/lib-vc2017 /NODEFAULTLIB:LIBCMT ^
    glfw3.lib user32.lib gdi32.lib shell32.lib opengl32.lib msvcrt.lib