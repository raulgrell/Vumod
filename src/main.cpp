#include "common.h"

#include "graphics.cpp"
#include "gui/gui.cpp"
#include "gui/console.cpp"
#include "script.cpp"
#include "shader.cpp"
#include "util.cpp"
#include "vfs.cpp"
#include "viewer.cpp"
#include "window.cpp"

#include <ext.cpp>

static bool show_demo_window = true;
static bool show_console_window = true;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

static std::vector<std::string> commands;
static char command_text[4096] = "System.print(\"lol\")";

static void drawGui()
{
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    {
        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
        ImGui::InputTextMultiline("##source", command_text, IM_ARRAYSIZE(command_text), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), flags);

        if (ImGui::Button("Run", ImVec2(80, 30))) {
            std::string command{command_text};
            commands.push_back(command);
        }
    }

    if (show_console_window)
        ShowConsole(&show_console_window);
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
    if (!LoadObjAndConvert(bmin, bmax, objects, materials, textures, "data/blender.obj"))
    {
        fatal_error("Could not load model.");
    }

    WrenConfiguration config;
    wrenInitConfiguration(&config);
    config.writeFn = write;

    WrenVM *vm = wrenNewVM(&config);

    WrenInterpretResult result = wrenInterpret(
        vm,
        "vumod",
        "System.print(\"I am running in a VM!\")");

    if (result != WREN_RESULT_SUCCESS)
    {
        fatal_error("Could run script.");
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
        mat4x4_rotate_Z(m, m, (float)glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(vs.program);
        glUniformMatrix4fv(vs.mvp_location, 1, GL_FALSE, (const GLfloat *)mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        CheckErrors();

        beginGui(vg);
        drawGui();
        endGui(vg);

        for (auto &&command : commands)
            wrenInterpret(vm, "vumod", command.c_str());

        commands.clear();
        
        glfwSwapBuffers(vw.window);
        glfwPollEvents();
    }

    wrenFreeVM(vm);
    destroyGui(vg);
    destroyWindow(vw);

    exit(EXIT_SUCCESS);
}
