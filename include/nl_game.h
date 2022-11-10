
#ifndef _NL_GAME_H
#define _NL_GAME_H

#include <nl_camera.h>
#include <nl_renderer.h>
#include <nl_shader.h>

typedef struct GameData GameData;

void GameInitialize(GameData*& data);
void GameUpdate(GameData* data, float delta_time);
void GameRender(GameData* data);
void GameCleanup(GameData*& data);

#endif //_NL_GAME_H
