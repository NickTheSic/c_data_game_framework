
#ifndef NL_PLATFORM_H_
#define NL_PLATFORM_H_

#include <nl_framework.h>
#include <nl_input.h>
#include <nl_ui.h>
#include <nl_viewport.h>

struct Platform
{
#ifdef _WIN32
    void* window;
    void* device;
    void* context;
#endif

#ifdef __EMSCRIPTEN__
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
#endif

    Framework fw;
    Input input;
    UI ui;
    Viewport viewport;
};

typedef Platform NLPlatform;

// Helper functions if I want to retrieve the pointer 
Platform* GetGlobalPlatform();
void SetGlobalPlatform(Platform* platform);

Platform* CreatePlatform(int width, int height, const char* title);
void DestroyPlatform(Platform* platform);

bool NLPollEvents(Platform* platform);
void NLSwapBuffers(Platform* platform);

void NLSetWindowShouldClose(Platform* platform);

#endif //NL_PLATFORM_H_