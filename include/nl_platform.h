
#ifndef NL_PLATFORM_H_
#define NL_PLATFORM_H_

struct NLPlatform
{
    void* window;
    void* device;
    void* context;
};

NLPlatform* CreatePlatform(int width, int height, const char* title);
void DestroyPlatform(NLPlatform* platform);

#endif //NL_PLATFORM_H_