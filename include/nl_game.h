
#ifndef _NL_GAME_H
#define _NL_GAME_H

typedef struct GameData GameData;

GameData* GameInitialize(struct Platform* platform);

void GameUpdate(struct Platform* platform, GameData* data, float delta_time);

// I might want to update this to be a platfrom / framework thing
// I could allow the game custom drawing but I also want to just draw the active sprites at their position
void GameRender(struct Platform* platform, GameData* data);

void GameCleanup(GameData* data);

#endif //_NL_GAME_H
