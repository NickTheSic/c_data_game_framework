
#ifndef _NL_GAME_H
#define _NL_GAME_H

#include <nl_camera.h>
#include <nl_renderer.h>
#include <nl_shader.h>

struct GameData
{
    Camera camera;
    SpriteSheet sprite_sheet;
    Shader shader;
    
    SpriteAnimation player_animations[2];
    v3f player_pos;
    
    SpriteHandle sprite_handle1;
    SpriteHandle sprite_handle2;
    int active_player_anim;
};

void GameInitialize(GameData* data);
void GameUpdate(GameData* data, float delta_time);
void GameRender(GameData* data);
void GameCleanup(GameData* data);

#endif //_NL_GAME_H
