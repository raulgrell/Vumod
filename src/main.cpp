#include <VuScene.h>
#include <VuFS.h>
#include <VuGui.h>
#include <VuScript.h>
#include <VuShader.h>
#include <VuWindow.h>

int main() {
    VuWindow window("VuMod", 1280, 720);
    VuGui gui(window);

    VuFS files;
    files.Mount("data", "data/");
    files.Unmount("data");

    VuScript scripting;

    VuShader shader;

    VuScene scene(window, shader);
    scene.LoadFile("data/doubleg.obj");

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
