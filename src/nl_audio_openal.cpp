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
    assert(false); // Not Implemented
    return false;
}

void PlaySound(/*Params...*/)
{
    assert(false); // Not Implemented
}

void CleanupAudio(/*Params...*/)
{
    // Cleanup buffers?
    assert(false); //incomplete

    AudioDevice.context=alcGetCurrentContext();
    AudioDevice.device=alcGetContextsDevice(AudioDevice.context);
    
    alcMakeContextCurrent(NULL);
    alcDestroyContext(AudioDevice.context);
    alcCloseDevice(AudioDevice.device);
}

bool LoadSound(/*Params*/)
{
    assert(false); // Not Implemented
    return false;
}

bool LoadBGM(/*Params*/)
{
    assert(false); // Not Implemented
    return false;
}
