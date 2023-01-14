#ifndef 
#define _NL_AUDIO_H_

struct Audio
{
    // BMG Buffer
    // SFX Buffers
};

bool InitAudio(/*Params...*/);
void CleanupAudio(/*Params...*/);

bool LoadSound(/*Params*/);
bool LoadBGM(/*Params*/);

void SetBackgroundMusic(/*Params...*/);
void PlaySound(/*Params...*/);

#endif