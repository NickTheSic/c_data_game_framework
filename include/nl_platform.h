
#ifndef NL_PLATFORM_H_
#define NL_PLATFORM_H_

#include <nl_framework.h>
#include <nl_input.h>
#include <nl_viewport.h>

struct Platform
{
    void* window;

    #ifndef GLFW_PLATFORM_LAYER // A GLFWwindow is basically everything that my platform would be
    void* device;
    void* context;

    Framework fw;
    Input inputs;
    Viewport viewport;
    #endif
};

typedef Platform NLPlatform;

NLPlatform* CreatePlatform(int width, int height, const char* title);
void DestroyPlatform(NLPlatform* platform);

bool NLPollEvents(NLPlatform* platform);
void NLSwapBuffers(NLPlatform* platform);

void NLSetWindowShouldClose(NLPlatform* platform);

#endif //NL_PLATFORM_H_