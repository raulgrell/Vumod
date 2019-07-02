mkdir build-llvm
clang++ ^
    ext/ext.cpp ^
    src/util/util.cpp ^
    src/graphics/Buffer.cpp ^
    src/graphics/Framebuffer.cpp ^
    src/graphics/PostProcessing.cpp ^
    src/VuScene.cpp ^
    src/VuCamera.cpp ^
    src/VuFS.cpp ^
    src/VuGui.cpp ^
    src/VuMaterial.cpp ^
    src/VuObject.cpp ^
    src/VuRenderer.cpp ^
    src/VuScript.cpp ^
    src/VuShader.cpp ^
    src/VuTexture.cpp ^
    src/VuWindow.cpp ^
    src/main.cpp ^
    -std=c++17 -g -O0 ^
    -o ./build-llvm/main.exe ^
    -I ./src/ ^
    -I ./include/ ^
    -I ./ext/ ^
    -I ./glfw/include/ ^
    -L ./glfw/lib-vc2017/ ^
    -Xlinker /NODEFAULTLIB:libcmt ^
    -l user32 -l gdi32 -l shell32  -l opengl32 -l glfw3 -l msvcrt
    