#include "VuWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3native.h> // for glfwGetWin32Window
#include <input/InputManager.h>

#endif

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        InputManager::pressKey(key);
    } else if (action == GLFW_RELEASE) {
        InputManager::releaseKey(key);
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

}

void fatal_error(const std::string &errorString)
{
    perror(errorString.c_str());
    glfwTerminate();
    system("PAUSE");
    exit(1);
}

VuWindow::VuWindow(const char *title, int width, int height)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwGetFramebufferSize((GLFWwindow *) window, &width, &height);
    glfwSetKeyCallback((GLFWwindow *) window, key_callback);
    glfwMakeContextCurrent((GLFWwindow *) window);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(1);
}

VuWindow::~VuWindow()
{
    glfwDestroyWindow((GLFWwindow *) window);
    glfwTerminate();
}

bool VuWindow::Continue() const
{
    return !glfwWindowShouldClose((GLFWwindow *) window);
}

void VuWindow::Begin() const
{
    glfwPollEvents();
}

void VuWindow::End() const
{
    glfwSwapBuffers((GLFWwindow *) window);
}

void VuWindow::GetSize(int *w, int *h)
{
    glfwGetFramebufferSize((GLFWwindow *) window, w, h);
    width = *w;
    height = *h;
}
