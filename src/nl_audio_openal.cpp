#include <nl_audio.h>

#include <al.h>
#include <alc.h>

struct OpenAL_AudioDevice
{
    ALCdevice* device;
    ALCcontext* context;

    ALuint* buffers;
    int buffer_count;

    ALuint* sources;
    int source_count;
} AudioDevice = {};

bool InitAudio(/*Params...*/)
{
    #error Implement Audio system function
}

void PlaySound(/*Params...*/)
{
    #error Implement Audio system function
}

void CleanupAudio(/*Params...*/)
{
    // Cleanup buffers?
    #error Implement Audio system function
    AudioDevice.Context=alcGetCurrentContext();
    AudioDevice.Device=alcGetContextsDevice(AudioDevice.context);
    
    alcMakeContextCurrent(NULL);
    alcDestroyContext(AudioDevice.context);
    alcCloseDevice(AudioDevice.device);
}
}

