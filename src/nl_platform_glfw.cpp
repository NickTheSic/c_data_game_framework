
#include <nl_platform.h>
#include <nl_input.h>
#include <nl_debug.h>

#include <GLFW/glfw3.h>
#undef near
#undef far

NLPlatform*
CreatePlatform(int width, int height, const char* title)
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
    
    glfwSetKeyCallback((GLFWwindow*)platform->window, 
                       [](GLFWwindow* window, int key, int scancode, int action, int mods){
                           UNUSED_VARIABLE(window);
                           UNUSED_VARIABLE(scancode);
                           UNUSED_VARIABLE(mods);

                           if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                           {
                              glfwSetWindowShouldClose(window, GLFW_TRUE);
                           }
                           
                           HandleAction(action, key);
                       });
    
    return platform;
}

void
DestroyPlatform(NLPlatform* platform)
{
    delete platform;
    glfwTerminate();
}

bool
NLPollEvents(NLPlatform* platform)
{
    glfwPollEvents();
    return !glfwWindowShouldClose((GLFWwindow*)platform->window);
}

void 
NLSwapBuffers(NLPlatform* platform)
{
    glfwSwapBuffers((GLFWwindow*)platform->window);
}
