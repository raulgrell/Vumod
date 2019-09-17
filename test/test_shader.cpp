

#include <platform/VuWindow.h>
#include <graphics/Graphics.h>
#include <files/VFS.h>
#include <scene/Camera.h>
#include <math/Mat.h>
#include <input/InputManager.h>

int main()
{
    VuWindow window("VuMod", 1280, 720);
    CHECK_GL();

    VFS files;
    files.Mount("data", "data/");
    files.Unmount("data");

    Camera camera;
    Mat4 projectionMatrix =
            Mat4::Perspective(Camera::fov, VuWindow::getAspectRatio(), Camera::near_plane, Camera::far_plane);

    CHECK_GL();

    while (window.Continue()) {
        window.Begin();
        InputManager::Update();

        window.End();
    }
}
