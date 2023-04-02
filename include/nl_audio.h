#ifndef _NL_AUDIO_H_
#define _NL_AUDIO_H_

bool InitAudio(/*Params...*/);
void CleanupAudio(/*Params...*/);

bool LoadSound(/*Params*/);
bool LoadBGM(/*Params*/);

void SetBackgroundMusic(/*Params...*/);
void PlaySound(/*Params...*/);

// Should take 0 - 100 by default and calculated per implementation
void SetVolume(char volume);
char GetVolume(); // Not sure if I return a value or pass by reference
void GetVolume(char& volume);


#endif //_NL_AUDIO_H_