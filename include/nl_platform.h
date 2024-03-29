
#ifndef NL_PLATFORM_H_
#define NL_PLATFORM_H_

#include <nl_framework.h>
#include <nl_input.h>
#include <nl_ui.h>
#include <nl_viewport.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <EGL/egl.h>
#endif

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

    void *game_data;
};

Platform* CreatePlatform(int width, int height, const char* title);
void DestroyPlatform(Platform* platform);

bool NLPollEvents(Platform* platform);
void NLSwapBuffers(Platform* platform);

void NLSetWindowShouldClose(Platform* platform);

void NLGetWindowDPIScaling(Platform* platform, float* x, float* y);
void UpdateScreenSize(Platform* platform, int width, int height);

/* TO BE IMPLEMENTED 

    will need to keep track of the old screen size as well as the new full screen size
    void SetFullscreen(bool);

*/

#endif //NL_PLATFORM_H_