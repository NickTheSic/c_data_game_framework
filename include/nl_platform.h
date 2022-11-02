
#ifndef NL_PLATFORM_H
#define NL_PLATFORM_H

typedef struct nlPlatform nlPlatform;
struct nlPlatform
{
    void*  window;
    void*  device;
    void* context;

    int window_width, window_height;
};

nlPlatform* NLCreatePlatform(int width, int height, const char* title);
void NLFreePlatform(nlPlatform* platform);

bool NLPollEvents(nlPlatform* platform);
void NLSwapBuffers(nlPlatform* platform);

#endif // NL_PLATFORM_H