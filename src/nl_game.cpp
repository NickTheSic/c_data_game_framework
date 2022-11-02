#include <nl_game.h>
#include <nl_gl.h>
#include <GLFW/glfw3.h>

static void
UnAttackAnim(void* data)
{
    if (data != 0)
    {
        ((GameData*)data)->active_player_anim = 0;
    }
}

void
GameInitialize(GameData* data)
{
    data->player_pos.z = 1.f;
    InitializeRenderer(&data->sprite_sheet.renderer, 8, sizeof(SpriteVertexData));
    InitializeSpriteSheet(&data->sprite_sheet, 512, 512);
    CreateViewMatrixFollow(&data->camera, data->player_pos);
    CompileSpriteShaderProgram(&data->sprite_sheet.renderer);
    data->shader.program = data->sprite_sheet.renderer.shader_program;
    SetUniform(&data->shader, "view", data->camera.view);
    
    InitializeSpriteAnim(&data->player_animations[0], 4, 5);
    data->player_animations[0].sprite_handles[0] = LoadSprite(&data->sprite_sheet, "data/testanim-01.png");
    data->player_animations[0].sprite_handles[1] = LoadSprite(&data->sprite_sheet, "data/testanim-02.png");
    data->player_animations[0].sprite_handles[2] = LoadSprite(&data->sprite_sheet, "data/testanim-03.png");
    data->player_animations[0].sprite_handles[3] = LoadSprite(&data->sprite_sheet, "data/testanim-04.png");
    
    data->sprite_handle1 = LoadSprite(&data->sprite_sheet, "data/blue64.png");
    
    InitializeSpriteAnim(&data->player_animations[1], 3, 10.f);
    data->player_animations[1].sprite_handles[0] = LoadSprite(&data->sprite_sheet, "data/testanimattack-01.png");
    data->player_animations[1].sprite_handles[1] = LoadSprite(&data->sprite_sheet, "data/testanimattack-02.png");
    data->player_animations[1].sprite_handles[2] = LoadSprite(&data->sprite_sheet, "data/testanimattack-03.png");
    data->player_animations[1].user_data = data;
    data->player_animations[1].callback = &UnAttackAnim;
    
    data->sprite_handle2 = LoadSprite(&data->sprite_sheet, "data/blue64.png");
}

void 
GameUpdate(GameData* data, float delta_time)
{
    UpdateSpriteAnimation(&data->player_animations[data->active_player_anim], delta_time);
}

void 
GameRender(GameData* data)
{
    (void)(data);
}

void
GameCleanup(GameData* data)
{
    CleanupSpriteAnimation(&data->player_animations[0]);
    CleanupSpriteAnimation(&data->player_animations[1]);
    CleanupRenderer(&data->sprite_sheet.renderer);
    CleanupSpriteSheet(&data->sprite_sheet);
}