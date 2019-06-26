mkdir build-msvc
mkdir build-msvc\obj
cd build-msvc\obj
cl  ..\..\src\main.cpp ^
    ..\..\ext\ext.cpp ^
    ..\..\src\util\util.cpp ^
    ..\..\src\VuScene.cpp ^
    ..\..\src\VuCamera.cpp ^
    ..\..\src\VuFS.cpp ^
    ..\..\src\VuGui.cpp ^
    ..\..\src\VuObject.cpp ^
    ..\..\src\VuScript.cpp ^
    ..\..\src\VuShader.cpp ^
    ..\..\src\VuTexture.cpp ^
    ..\..\src\VuWindow.cpp ^
    /EHsc /Zi /Fe:..\main.exe ^
    /I..\..\src ^
    /I..\..\include ^
    /I..\..\ext ^
    /I..\..\glfw\include ^
    /link /LIBPATH:..\..\glfw\lib-vc2017 /NODEFAULTLIB:LIBCMT ^
    glfw3.lib user32.lib gdi32.lib shell32.lib opengl32.lib msvcrt.lib
cd ..\..