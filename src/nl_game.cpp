#include <nl_game.h>

#include <nl_camera.h>
#include <nl_controller.h>
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

    SpriteAnimation player_sprite;

    Controller controller = {};
};

GameData* 
GameInitialize(Platform* platform)
{
    GameData* data = new GameData();
    
    float ratio = (float)platform->viewport.screen_size.x / (float)platform->viewport.screen_size.y;
    InitializeFramework(&platform->fw, {512,512}, {ratio*200.f,200.f}, {0.0f, 0.0f, 0.0f}, 100);

    InitControllerSystem();

    data->player_pos.x = 0.f;
    data->player_pos.y = 0.f;
    data->player_pos.z = 0.f;
    data->player_velocty.x = 0.f;
    data->player_velocty.y = 0.f;

    InitializeSpriteAnim(&data->player_sprite, 4, 10);
    data->player_sprite.sprite_handles[0] = LoadSprite(&platform->fw.sprite_sheet,"data/testanim-01.png");
    data->player_sprite.sprite_handles[1] = LoadSprite(&platform->fw.sprite_sheet,"data/testanim-02.png");
    data->player_sprite.sprite_handles[2] = LoadSprite(&platform->fw.sprite_sheet,"data/testanim-03.png");
    data->player_sprite.sprite_handles[3] = LoadSprite(&platform->fw.sprite_sheet,"data/testanim-04.png");

    return data;
}

void 
GameUpdate(Platform* platform, GameData* data, float delta_time)
{
    PollController(&data->controller);
    //LogControllerState(&data->controller);

    data->player_velocty.x = data->player_velocty.y = 0.f;
    UpdateSpriteAnimation(&data->player_sprite, delta_time);

    const float Speed = 100.f;

    if (platform->input.keys[(int)Key::W] == ButtonState::Down || data->controller.dpad_up == 1 || data->controller.left_y_axis > 0.1f)
        data->player_velocty.y = 1.f;
    if (platform->input.keys[(int)Key::S] == ButtonState::Down || data->controller.dpad_down == 1 || data->controller.left_y_axis < -0.1f)
        data->player_velocty.y = -1.f;
    if (platform->input.keys[(int)'A'] == ButtonState::Down || data->controller.dpad_left == 1 || data->controller.left_x_axis < -0.1f)
        data->player_velocty.x = -1.f;
    if (platform->input.keys[(int)'D'] == ButtonState::Down || data->controller.dpad_right == 1 || data->controller.left_x_axis > 0.1f)
        data->player_velocty.x = 1.f;

    data->player_pos.x += data->player_velocty.x * Speed * delta_time;
    data->player_pos.y += data->player_velocty.y * Speed * delta_time;


    CreateViewMatrixFollow(&platform->fw.main_camera, data->player_pos);

}

void 
GameRender(Platform* platform, GameData* data)
{ 
    SetViewUniform(&platform->fw.shader, platform->fw.main_camera.view);
    RenderSpriteAnimationFrame(&platform->fw.sprite_sheet, &data->player_sprite, data->player_pos);

    DisplayEntireSheet(&platform->fw.sprite_sheet, {-100.0f, 100.f, 0.0f}, {256.f,128.f});
}

void
GameCleanup(GameData* data)
{
    CleanupSpriteAnimation(&data->player_sprite);
    CleanupControllerSystem();
    delete data;
}