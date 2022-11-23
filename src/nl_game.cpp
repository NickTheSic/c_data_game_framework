#include <nl_game.h>

#include <nl_camera.h>
#include <nl_debug.h>
#include <nl_framework.h>
#include <nl_gl.h>
#include <nl_input.h>
#include <nl_key.h>
#include <nl_platform.h>
#include <nl_renderer.h>
#include <nl_shader.h>
#include <nl_spritesheet.h>
#include <nl_utils.h>

struct GameData
{
};

GameData* 
GameInitialize(Platform* platform)
{
    GameData* data = new GameData();
    InitializeFramework(&platform->fw, {512,512}, {200.f, 200.f}, {0.0f, 0.0f, 0.0f}, 12);

    return data;
}

void 
GameUpdate(Platform* platform, GameData* data, float delta_time)
{
    UNUSED(platform); UNUSED(data); UNUSED(delta_time);
}

void 
GameRender(Platform* platform, GameData* data)
{
    UNUSED(platform); UNUSED(data);
}

void
GameCleanup(GameData* data)
{
    delete data;
}