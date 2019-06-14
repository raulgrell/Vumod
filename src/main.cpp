#include "common.h"

#include "graphics.cpp"
#include "gui.cpp"
#include "shader.cpp"
#include "util.cpp"
#include "vfs.cpp"
#include "viewer.cpp"
#include "window.cpp"

#include <ext.cpp>

static bool show_demo_window = true;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

static void drawGui()
{
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    {
        static float f = 0.0f;
        static int counter = 0;
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::Checkbox("Demo Window", &show_demo_window);
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float *)&clear_color);
        if (ImGui::Button("Button"))
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}

int main(void)
{
    VuWindow vw;
    VuGui vg;

    initWindow(vw);
    initGui(vw, vg);

    VuShader vs;
    initShader(vs);

    float bmin[3], bmax[3];
    std::vector<tinyobj::material_t> materials;
    std::vector<VuObject> objects;
    std::unordered_map<std::string, GLuint> textures;
    if (!LoadObjAndConvert(bmin, bmax, objects, materials, textures, "data/blender.obj")){
        fatal_error("Could not load model.");
    }

    while (!glfwWindowShouldClose(vw.window))
    {
        float ratio;
        int width, height;
        mat4x4 m, p, mvp;
        glfwGetFramebufferSize(vw.window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        ratio = width / (float)height;
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(vs.program);
        glUniformMatrix4fv(vs.mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        beginGui(vg);
        drawGui();
        endGui(vg);

        glfwSwapBuffers(vw.window);
        glfwPollEvents();
    }

    destroyGui(vg);
    destroyWindow(vw);

    exit(EXIT_SUCCESS);
}
