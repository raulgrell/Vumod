cmake_minimum_required(VERSION 3.14)
project(Viewer)

set(CMAKE_CXX_STANDARD 17)

include_directories(ext)
include_directories(glfw/include)
include_directories(src)
include_directories(include)

link_directories(glfw/lib-mingw-w64)
link_libraries(glfw3 opengl32 user32 gdi32 shell32)

add_executable(
        TestMath
        test/test_math.cpp
        src/math/Math.cpp
        )

add_executable(
        TestShader
        test/test_shader.cpp
        )

add_executable(
        Viewer
        ext/ext.cpp
        src/files/VFS.cpp
        src/fx/ContrastShader.cpp
        src/fx/ImageRenderer.cpp
        src/fx/PostProcessing.cpp
        src/graphics/VertexBuffer.cpp
        src/graphics/IndexBuffer.cpp
        src/graphics/FrameBuffer.cpp
        src/graphics/Graphics.cpp
        src/graphics/Loader.cpp
        src/gui/Console.cpp
        src/gui/Debug.cpp
        src/gui/FileBrowser.cpp
        src/gui/VuGui.cpp
        src/graphics/VertexArray.cpp
        src/input/MousePicker.cpp
        src/input/InputManager.cpp
        src/light/Light.cpp
        src/light/Colour.cpp
        src/loader/ObjLoader.cpp
        src/loader/Vertex.cpp
        src/math/Math.cpp
        src/models/ModelRenderer.cpp
        src/models/NormalMappingRenderer.cpp
        src/models/NormalMappingShader.cpp
        src/models/VuObject.cpp
        src/particles/Particle.cpp
        src/particles/ParticleMaster.cpp
        src/particles/ParticleRenderer.cpp
        src/particles/ParticleShader.cpp
        src/particles/ParticleSystem.cpp
        src/particles/ParticleTexture.cpp
        src/platform/VuWindow.cpp
        src/camera/Camera.cpp
        src/scene/Entity.cpp
        src/scene/Scene.cpp
        src/scene/SceneRenderer.cpp
        src/scene/SceneShader.cpp
        src/shaders/ShaderLibrary.cpp
        src/shaders/StaticShader.cpp
        src/shaders/ShaderGL.cpp
        src/skybox/SkyboxRenderer.cpp
        src/skybox/SkyboxShader.cpp
        src/script/VuScript.cpp
        src/terrain/HeightsGenerator.cpp
        src/terrain/Terrain.cpp
        src/terrain/TerrainRenderer.cpp
        src/terrain/TerrainShader.cpp
        src/texture/Material.cpp
        src/texture/VuTexture.cpp
        src/files/FileUtils.cpp
        src/util/StringUtils.cpp
        src/util/Utils.cpp
        src/water/WaterFrameBuffers.cpp
        src/water/WaterRenderer.cpp
        src/water/WaterShader.cpp
        src/main.cpp
        src/graphics/FloatBuffer.cpp
)

target_compile_options(Viewer PRIVATE )

# add_custom_target(
#         data
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_SOURCE_DIR}/data/ ${CMAKE_SOURCE_DIR}/cmake-build-debug/data)
#
# add_dependencies(Viewer data)