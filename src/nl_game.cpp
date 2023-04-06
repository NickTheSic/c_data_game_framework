#include <nl_game.h>

#include <nl_camera.h>
#include <nl_controller.h>
#include <nl_debug.h>
#include <nl_framework.h>
#include <nl_gl.h>
#include <nl_grid.h>
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

    Grid world_grid = {};
    Tile tiles[3];
};

void* temp_GetTilePtr(GameData* data)
{
    return data->tiles;
}

GameData* 
GameInitialize(Platform* platform)
{
    platform->game_data = new GameData();
    GameData* data = (GameData*)platform->game_data;
    
    v2f camera_size = {};
    camera_size.x = (float)platform->viewport.screen_size.x / 2.f; 
    camera_size.y = (float)platform->viewport.screen_size.y / 2.f; 

    InitializeFramework(&platform->fw, 
        {512,512}, 
        camera_size, 
        {0.0f, 0.0f, 0.0f}, 
        100);

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

    // Grid
    {
        InitGrid(&data->world_grid, 5, 5);
        Grid* grid = &data->world_grid;
        grid->tile_size = 32;

        SetGridValue(grid, 2, 2, 1);
        // Creates the walls around the outside
        for (int i = 0; i < 5; ++i)
        {
            SetGridValue(grid, 0, i, 2);
            SetGridValue(grid, 4, i, 2);
            SetGridValue(grid, i, 0, 2);
            SetGridValue(grid, i, 4, 2);
        }
        
        InitializeTileSet(platform, data->tiles);
    }

    return (GameData*)platform->game_data;
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
    if (platform->input.keys[(int)Key::A] == ButtonState::Down || data->controller.dpad_left == 1 || data->controller.left_x_axis < -0.1f)
        data->player_velocty.x = -1.f;
    if (platform->input.keys[(int)Key::D] == ButtonState::Down || data->controller.dpad_right == 1 || data->controller.left_x_axis > 0.1f)
        data->player_velocty.x = 1.f;

    data->player_pos.x += data->player_velocty.x * Speed * delta_time;
    data->player_pos.y += data->player_velocty.y * Speed * delta_time;

    CreateViewMatrixFollow(&platform->fw.main_camera, {data->player_pos.x + 16.f, data->player_pos.y + 16.f, 0.f});
}

void 
GameRender(Platform* platform, GameData* data)
{ 
    SetViewUniform(&platform->fw.shader, platform->fw.main_camera.view);

    for (int i = 0; i < data->world_grid.width * data->world_grid.height; ++i)
    {
        int value, x, y;
        GetGridValue(&data->world_grid, i, &value);
        GridGetXYFromIndex(&data->world_grid, i, &x, &y);

        v2f tile_size = {};
        tile_size.x = static_cast<float>(data->world_grid.tile_size);
        tile_size.y = static_cast<float>(data->world_grid.tile_size);
        v3f pos = {};
        pos.x = tile_size.x * x;
        pos.y = tile_size.y * y;
        pos.z = 0.f;

        AddSizedSpriteToRender(&platform->fw.sprite_sheet, data->tiles[value].sprite, pos, tile_size);
    }

    RenderSpriteAnimationFrame(&platform->fw.sprite_sheet, &data->player_sprite, data->player_pos);

    DisplayEntireSheet(&platform->fw.sprite_sheet, {-100.0f, 100.f, 0.0f}, {256.f,128.f});
}

void
GameCleanup(GameData* data)
{
    FreeGrid(&data->world_grid);
    CleanupSpriteAnimation(&data->player_sprite);
    CleanupControllerSystem();
    delete data;
}