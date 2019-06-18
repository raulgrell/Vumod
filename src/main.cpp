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
    initWindow(vw);

    VuGui vg;
    initGui(vw, vg);

    VuShader vs;
    initShader(vs);

    VuFS vf;
    Mount(vf, "data", "data");
    Unmount(vf, "data");

    VuScript vk;

    VuScene vc;
    vc.LoadFile("data/blender.obj");

    while (!glfwWindowShouldClose(vw.window)) {
        int width, height;
        mat4x4 m, p, mvp;
        glfwGetFramebufferSize(vw.window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        float ratio = width / (float) height;
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(vs.program);
        glUniformMatrix4fv(vs.mvp_location, 1, GL_FALSE, (const GLfloat *) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        CheckErrors();

        vg.Begin();
        drawGui();
        vg.End();

        vk.InterpretCommands();

        glfwSwapBuffers(vw.window);
        glfwPollEvents();
    }

    destroyWindow(vw);

    exit(EXIT_SUCCESS);
}
