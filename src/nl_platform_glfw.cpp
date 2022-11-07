
#include <nl_platform.h>

#include <GLFW/glfw3.h>

NLPlatform* CreatePlatform(int width, int height, const char* title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    NLPlatform* platform = new NLPlatform();

    platform->window = glfwCreateWindow(width, height, title, 0, 0);

    if (platform->window == nullptr)
    {
        DestroyPlatform(platform);
        return nullptr;
    }

    glfwMakeContextCurrent((GLFWwindow*)platform->window);

    return platform;
}

void DestroyPlatform(NLPlatform* platform)
{
    delete platform;
    glfwTerminate();
}

bool NLPollEvents(NLPlatform* platform)
{
    glfwPollEvents();
    return !glfwWindowShouldClose((GLFWwindow*)platform->window);
}

void NLSwapBuffers(NLPlatform* platform)
{
    glfwSwapBuffers((GLFWwindow*)platform->window);
}
