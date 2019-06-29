cmake_minimum_required(VERSION 3.14)
project(Viewer)

set(CMAKE_CXX_STANDARD 17)

include_directories(ext)
include_directories(glfw/include)
include_directories(src)
include_directories(include)

link_directories(glfw/lib-mingw-w64)
link_libraries(glfw3 opengl32 user32 gdi32 shell32)

add_executable(Viewer
        ext/ext.cpp
        src/util/util.cpp
        src/VuScene.cpp
        src/VuCamera.cpp
        src/VuFS.cpp
        src/VuGui.cpp
        src/VuObject.cpp
        src/VuScript.cpp
        src/VuShader.cpp
        src/VuTexture.cpp
        src/VuWindow.cpp
        src/main.cpp src/VuRenderer.cpp src/VuRenderer.h src/VuMaterial.cpp src/VuMaterial.h src/shader/VertexBuffer.cpp src/shader/VertexBuffer.h src/shader/IndexBuffer.cpp src/shader/IndexBuffer.h src/shader/BufferLayout.cpp src/shader/BufferLayout.h src/shader/Framebuffer.cpp src/shader/Framebuffer.h src/shader/PostProcessing.cpp src/shader/PostProcessing.h)

target_compile_options(Viewer PRIVATE )

add_custom_command(TARGET Viewer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data/ $<TARGET_FILE_DIR:Viewer>/data)
