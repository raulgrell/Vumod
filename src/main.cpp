#include "common.h"

#include "graphics.cpp"
#include "gui.cpp"
#include "script.cpp"
#include "shader.cpp"
#include "util.cpp"
#include "vfs.cpp"
#include "scene.cpp"
#include "window.cpp"

int main() {
    VuWindow vw;
    initWindow(vw, 1280, 720);

    VuGui vg;
    initGui(vw, vg);

    VuFS vf;
    Mount(vf, "data", "data");
    Unmount(vf, "data");

    VuScript vk;

    VuShader vs;
    initShader(vs);

    CheckErrors();

    VuScene vc(vw, vs);
    vc.LoadFile("data/triple.obj");

    while (vw.IsRunning()) {
        vw.Begin();

        vc.ResizeDisplay();
        vc.RotateCamera();
        vc.Draw();

        vg.Begin();
        drawGui();
        vg.End();

        vk.InterpretCommands();

        vw.End();
    }

    destroyWindow(vw);
    exit(EXIT_SUCCESS);
}
