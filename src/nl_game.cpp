#include <nl_game.h>
#include <nl_gl.h>
#include <GLFW/glfw3.h>
#include <nl_input.h>

struct GameData
{
    Camera camera;
    SpriteSheet sprite_sheet;
    Shader shader;
    
    SpriteAnimation player_animations[2];
    v3f player_pos;
    v2f player_velocity;
    
    SpriteHandle sprite_handle1;
    SpriteHandle sprite_handle2;
    int active_player_anim;
};

static void
UnAttackAnim(void* data)
{
    if (data != 0)
    {
        ((GameData*)data)->active_player_anim = 0;
    }
}

static void 
PlayerAttack(int action, int code, void* data)
{
    int& val = *(int*)data;
    if (val == 1) return;

    if (code == GLFW_KEY_E && action == GLFW_PRESS)
    {
        val = 1;
    }
}

static void
PlayerMove(int action, int code, void* data)
{
    v2f& velocity = *(v2f*)data;
    float player_speed = 200;

    if (action == GLFW_REPEAT) return;

    switch(code)
    {
        case GLFW_KEY_W:
        {
            velocity.y += (action == GLFW_PRESS) ? player_speed : -player_speed;
        } break;

        case GLFW_KEY_S:
        {
            velocity.y -= (action == GLFW_PRESS) ? player_speed : -player_speed;
        } break;

        case GLFW_KEY_D:
        {
            velocity.x += (action == GLFW_PRESS) ? player_speed : -player_speed;
        } break;

        case GLFW_KEY_A:
        {
            velocity.x -= (action == GLFW_PRESS) ? player_speed : -player_speed;
        } break;
    }
}

void
GameInitialize(GameData*& data)
{
    data = new GameData();

    data->player_pos.z = 0.f;
    InitializeRenderer(&data->sprite_sheet.renderer, 8, sizeof(SpriteVertexData));
    InitializeSpriteSheet(&data->sprite_sheet, 512, 512);
    data->camera.size.x = 200.f;
    data->camera.size.y = 200.f;
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

    AddActionCallback(&data->active_player_anim, &PlayerAttack);
    AddActionCallback(&data->player_velocity, &PlayerMove);
}

void 
GameUpdate(GameData* data, float delta_time)
{
    UpdateSpriteAnimation(&data->player_animations[data->active_player_anim], delta_time);

    data->player_pos.x += data->player_velocity.x * delta_time;
    data->player_pos.y += data->player_velocity.y * delta_time;

    CreateViewMatrixFollow(&data->camera, data->player_pos);
}

void 
GameRender(GameData* data)
{
    SetUniform(&data->shader, "view", data->camera.view);

    BeginRender(&data->sprite_sheet);
    
    RenderSpriteAnimationFrame(&data->sprite_sheet, &data->player_animations[data->active_player_anim], data->player_pos);
    
    float offset_pos = 30;
    AddSpriteToRender(&data->sprite_sheet, data->sprite_handle1, v3f{-offset_pos, -offset_pos, -1.0});
    AddSpriteToRender(&data->sprite_sheet, data->sprite_handle2, v3f{ offset_pos,  offset_pos,  1.0});
    
    DisplayEntireSheet(&data->sprite_sheet, {0.f,0.f, 0.0f}, {400.4f,400.4f});
    
    EndRender(&data->sprite_sheet.renderer);
}

void
GameCleanup(GameData*& data)
{
    CleanupSpriteAnimation(&data->player_animations[0]);
    CleanupSpriteAnimation(&data->player_animations[1]);
    CleanupRenderer(&data->sprite_sheet.renderer);
    CleanupSpriteSheet(&data->sprite_sheet);

    delete data;
    data = 0;
}