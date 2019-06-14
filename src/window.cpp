#include "common.h"

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void initWindow(VuWindow &vw)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    vw.window = glfwCreateWindow(1280, 720, "Simple example", NULL, NULL);
    if (!vw.window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(vw.window, key_callback);
    glfwMakeContextCurrent(vw.window);
    gladLoadGL();
    glfwSwapInterval(1);
}

void destroyWindow(VuWindow &vw)
{
    glfwDestroyWindow(vw.window);
    glfwTerminate();
}
