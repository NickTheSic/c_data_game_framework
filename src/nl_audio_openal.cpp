#include <nl_audio.h>
#include <nl_debug.h>

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
} audio_device = {};

bool InitAudio(/*Params... My version took a buffer count, but I could just hard set it*/)
{
    ALenum error = {};

    audio_device.device= alcOpenDevice(nullptr);
    if (!audio_device.device)
    {
        LOG("The Audio Device device was nullptr");
        return false;
    }

    audio_device.context = alcCreateContext(audio_device.device, nullptr);
    if (!audio_device.context)
    {
        LOG("The audio device context was nullptr");
        return false;
    }

    if (!alcMakeContextCurrent(audio_device.context))
    {
        LOG("Was unable to make the audio device context current");
        return false;
    }

    error = alGetError();

    audio_device.buffer_count = 1;
    audio_device.buffers = (ALuint*)calloc(audio_device.buffer_count, sizeof(ALuint));

    alGenBuffers(audio_device.buffer_count, audio_device.buffers);
    error = alGetError();
    if (error != AL_NO_ERROR)
    {
        //alDisplayALError("alGenBuffers: %i", error);
        return false;
    }

    // Not sure if there is more initialization required

    return true;
}

void PlaySound(/*Params...*/)
{
    assert(false); // Not Implemented
    // Requires some way to handle which sounds are currently in use
    // Allocate one section for BGM or sounds
}

void CleanupAudio(/*Params...*/)
{
    // Cleanup buffers?
    assert(false); //incomplete

    alcMakeContextCurrent(NULL);
    alcDestroyContext(audio_device.context);
    alcCloseDevice(audio_device.device);
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
