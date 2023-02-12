#ifndef _NL_AUDIO_H_
#define _NL_AUDIO_H_

bool InitAudio(/*Params...*/);
void CleanupAudio(/*Params...*/);

bool LoadSound(/*Params*/);
bool LoadBGM(/*Params*/);

void SetBackgroundMusic(/*Params...*/);
void PlaySound(/*Params...*/);

#endif //_NL_AUDIO_H_