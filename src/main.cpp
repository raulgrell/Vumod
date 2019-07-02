#include <VuScene.h>
#include <VuFS.h>
#include <VuGui.h>
#include <VuScript.h>
#include <VuShader.h>
#include <VuWindow.h>

#include <memory>

int main()
{
    VuWindow window("VuMod", 1280, 720);
    VuGui gui(window);

    VuFS files;
    files.Mount("data", "data/");
    files.Unmount("data");

    VuScript scripting;

    VuScene scene(window);
    scene.LoadFile("data/triple.obj");
    scene.LoadFile("data/double.obj");

    while (window.Continue()) {
        window.Begin();

        scene.Update();
        scene.Draw();

        gui.Begin();
        drawGui(scene);
        gui.End();

        scripting.InterpretCommands();

        window.End();
    }
}
