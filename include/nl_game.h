
#ifndef _NL_GAME_H
#define _NL_GAME_H

typedef struct GameData GameData;

void GameInitialize(GameData*& data);
void GameUpdate(GameData* data, float delta_time);
void GameRender(GameData* data);
void GameCleanup(GameData*& data);

#endif //_NL_GAME_H
