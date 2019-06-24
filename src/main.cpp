#include "common.h"

#include "camera.cpp"
#include "texture.cpp"
#include "gui.cpp"
#include "renderer.cpp"
#include "script.cpp"
#include "shader.cpp"
#include "util.cpp"
#include "vfs.cpp"
#include "renderer.cpp"
#include "scene.cpp"
#include "window.cpp"

#ifdef _MSC_VER
    #include "../ext/ext.cpp"
#endif

int main() {
    VuWindow vw;
    initWindow(vw, 1280, 720);

    VuGui vg(vw);

    VuFS vf;
    Mount(vf, "data", "data/");
    Unmount(vf, "data");

    VuScript vk;

    VuShader vs;
    initShader(vs);

    CheckErrors();

    VuScene vc(vw, vs);
    vc.LoadFile("data/triple.obj");

    while (vw.IsRunning()) {
        vw.Begin();

        vc.UpdateCamera();
        vc.Draw();

        vg.Begin();
        drawGui(vc);
        vg.End();

        vk.InterpretCommands();

        vw.End();
    }

    destroyWindow(vw);
    exit(EXIT_SUCCESS);
}
