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
    v3f player_pos;
    v2f player_velocty;

    SpriteHandle player_sprite;
};

GameData* 
GameInitialize(Platform* platform)
{
    GameData* data = new GameData();
    InitializeFramework(&platform->fw, {512,512}, {200.f, 200.f}, {0.0f, 0.0f, 0.0f}, 100);

    data->player_pos.x = 0.f;
    data->player_pos.y = 0.f;
    data->player_pos.z = 0.f;
    data->player_velocty.x = 0.f;
    data->player_velocty.y = 0.f;

    data->player_sprite = LoadSprite(&platform->fw.sprite_sheet,"data/testanim-01.png");

    return data;
}

void 
GameUpdate(Platform* platform, GameData* data, float delta_time)
{
    data->player_velocty.x = data->player_velocty.y = 0.f;

    const float Speed = 100.f;

    if (platform->input.keys[(int)Key::W] == ButtonState::Down)
        data->player_velocty.y = 1.f;
    if (platform->input.keys[(int)Key::S] == ButtonState::Down)
        data->player_velocty.y = -1.f;
    if (platform->input.keys[(int)'A'] == ButtonState::Down)
        data->player_velocty.x = -1.f;
    if (platform->input.keys[(int)'D'] == ButtonState::Down)
        data->player_velocty.x = 1.f;

    data->player_pos.x += data->player_velocty.x * Speed * delta_time;
    data->player_pos.y += data->player_velocty.y * Speed * delta_time;


    CreateViewMatrixFollow(&platform->fw.main_camera, data->player_pos);

}

void 
GameRender(Platform* platform, GameData* data)
{ 
    SetViewUniform(&platform->fw.shader, platform->fw.main_camera.view);
    AddSpriteToRender(&platform->fw.sprite_sheet, data->player_sprite, data->player_pos);

    DisplayEntireSheet(&platform->fw.sprite_sheet, {-100.0f, 100.f, 0.0f}, {256.f,128.f});
}

void
GameCleanup(GameData* data)
{
    delete data;
}