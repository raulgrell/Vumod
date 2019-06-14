#include "common.h"

#include "window.cpp"
#include "shaders.cpp"
#include "gui.cpp"

static bool show_demo_window = true;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

int DrawGui(VuGui& vg)
{
    ImGui_GLFW_NewFrame(vg);
    ImGui::NewFrame();

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    {
        static float f = 0.0f;
        static int counter = 0;
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::Checkbox("Demo Window", &show_demo_window);
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        if (ImGui::Button("Button")) counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_GL_RenderDrawData(vg, ImGui::GetDrawData());

    return 0;
}

int main(void)
{
    VuWindow vw;
    VuGui vg;

    initWindow(vw);
    initGui(vw, vg);

    VuShader vs;
    initShader(vs);

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

        DrawGui(vg);

        glfwSwapBuffers(vw.window);
        glfwPollEvents();
    }

    destroyGui(vg);
    destroyWindow(vw);

    exit(EXIT_SUCCESS);
}
