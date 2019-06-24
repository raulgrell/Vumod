cmake_minimum_required(VERSION 3.14)
project(Viewer)

set(CMAKE_CXX_STANDARD 17)

include_directories(ext)
include_directories(glfw/include)
include_directories(src)

link_directories(glfw/lib-mingw-w64)
link_libraries(glfw3 opengl32 user32 gdi32 shell32)

add_executable(Viewer
        ext/ext.cpp
        src/util.cpp
        src/VuScene.cpp
        src/VuFS.cpp
        src/VuGui.cpp
        src/VuScript.cpp
        src/VuShader.cpp
        src/VuTexture.cpp
        src/VuWindow.cpp
        src/main.cpp)

target_compile_options(Viewer PRIVATE )

add_custom_command(TARGET Viewer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data/ $<TARGET_FILE_DIR:Viewer>/data)
