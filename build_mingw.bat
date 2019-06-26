mkdir build-mingw
g++ ^
    src/main.cpp ^
    ext/ext.cpp ^
    src/util/util.cpp ^
    src/VuScene.cpp ^
    src/VuCamera.cpp ^
    src/VuFS.cpp ^
    src/VuGui.cpp ^
    src/VuObject.cpp ^
    src/VuScript.cpp ^
    src/VuShader.cpp ^
    src/VuTexture.cpp ^
    src/VuWindow.cpp ^
    -std=c++17 ^
    -g ^
    -o build-mingw/main.exe ^
    -I ./src/ ^
    -I ./include/ ^
    -I ./ext/ ^
    -l glfw3 -l kernel32 -l gdi32 -l opengl32