#include <VuScene.h>
#include <VuFS.h>
#include <VuGui.h>
#include <VuScript.h>
#include <VuShader.h>
#include <VuWindow.h>
#include <graphics/VertexArray.h>

#include <memory>

int main()
{
    VuWindow window("VuMod", 1280, 720);
    VuGui gui(window);

    VuFS files;
    files.Mount("data", "data/");
    files.Unmount("data");

    VuScript scripting;

    VuShader shader;

    VertexArray vao;

    float vertices[] = {
            0, 0, 0,
            0, 1, 0,
            1, 1, 0,
            1, 0, 0};

    unsigned int indices[] = {
            0, 2, 1,
            2, 0, 3};

    auto vbo = std::make_shared<VertexBuffer>(vertices, 12);
    auto ibo = std::make_shared<IndexBuffer>(indices, 6);
    BufferLayout layout {{GL_FLOAT, 2},
                         {GL_FLOAT, 3}};

    vao.AddVertexBuffer(vbo);
    vao.SetIndexBuffer(ibo);

    VuScene scene(window, shader);
    scene.LoadFile("data/triple.obj");
    scene.LoadFile("data/double.obj");

    while (window.Continue()) {
        window.Begin();

        scene.UpdateCamera();
        scene.Draw();

        gui.Begin();
        drawGui(scene);
        gui.End();

        scripting.InterpretCommands();

        window.End();
    }
}
