
#ifndef NL_PLATFORM_H_
#define NL_PLATFORM_H_

struct NLPlatform
{
    void* window;

#ifndef GLFW_PLATFORM_LAYER
    void* device;
    void* context;
#endif
};

NLPlatform* CreatePlatform(int width, int height, const char* title);
void DestroyPlatform(NLPlatform* platform);

bool NLPollEvents(NLPlatform* platform);
void NLSwapBuffers(NLPlatform* platform);

void NLSetWindowShouldClose(NLPlatform* platform);

#endif //NL_PLATFORM_H_