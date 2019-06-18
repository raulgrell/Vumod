cmake_minimum_required(VERSION 3.14)
project(Viewer)

set(CMAKE_CXX_STANDARD 17)

include_directories(ext)
include_directories(glfw/include)
include_directories(src)

link_directories(glfw/lib-mingw-w64)
link_libraries(glfw3 opengl32 user32 gdi32 shell32 comdlg32 ole32)

add_executable(Viewer
        ext/ext.cpp
        src/main.cpp)


