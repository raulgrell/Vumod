
#include "VuScene.h"
#include "VuFS.h"
#include "VuGui.h"
#include "VuScript.h"
#include "VuShader.h"
#include "VuWindow.h"

#include "common.h"

int main() {
    VuWindow vw("VuMod", 1280, 720);
    VuGui vg(vw);

    VuFS vf;
    vf.Mount("data", "data/");
    vf.Unmount("data");

    VuScript vk;

    VuShader vs;
    initShader(vs);

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
}
