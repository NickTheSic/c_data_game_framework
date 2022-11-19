#include <nl_game.h>

#include <nl_camera.h>
#include <nl_debug.h>
#include <nl_framework.h>
#include <nl_gl.h>
#include <nl_input.h>
#include <nl_key.h>
#include <nl_renderer.h>
#include <nl_shader.h>
#include <nl_spritesheet.h>
#include <nl_utils.h>

struct Player
{
    v3f pos;
    v2f velocity;
    SpriteAnimation animations[2];
    int active_anim;
};

struct GameData
{
    Framework fw;
    
    Player player;
    
    SpriteHandle sprite_handle1;
    SpriteHandle sprite_handle2;
    SpriteHandle player_outline;
};

static void
UnAttackAnim(void* data)
{
    if (data != 0)
    {
        ((GameData*)data)->player.active_anim = 0;
    }
}

static void 
PlayerAttack(KeyState action, Key code, void* data)
{
    int& val = *(int*)data;
    if (val == 1) return;

    if (code == Key::E && action == KeyState::Down)
    {
        val = 1;
    }
}

static void
PlayerMove(KeyState action, Key code, void* data)
{
    v2f& velocity = *(v2f*)data;
    float player_speed = 200;

    if (action == KeyState::Repeat) return;
    
    switch(code)
    {
        case Key::W:
        {
            velocity.y += (action == KeyState::Down) ? player_speed : -player_speed;
        } break;

        case Key::S:
        {
            velocity.y -= (action == KeyState::Down) ? player_speed : -player_speed;
        } break;

        case Key::D:
        {
            velocity.x += (action == KeyState::Down) ? player_speed : -player_speed;
        } break;

        case Key::A:
        {
            velocity.x -= (action == KeyState::Down) ? player_speed : -player_speed;
        } break;
    }
}

static void
mouse_callback(int mouse, int state, int pos_x, int pos_y, void* data)
{
    UNUSED(data);
    LOG("Mouse: %d, State: %s, X: %d, Y: %d\n", mouse, state == 1 ? "Down" : "Up", pos_x, pos_y);
}

static void
mouse_on_player(int mouse, int state, int pos_x, int pos_y, void* data)
{
    UNUSED(mouse);
    UNUSED(state);

    Player* p = static_cast<Player*>(data);

    if (PointInRect({static_cast<float>(pos_x), static_cast<float>(pos_y)}, {p->pos.x, p->pos.y}, {p->pos.x +32, p->pos.y + 32}))
    {
        LOG("Yes?");
    }
}

GameData* 
GameInitialize()
{
    GameData* data = new GameData();

    data->player.pos = {0.f, 0.f, 0.f};

    InitializeFramework(&data->fw, {512,512}, {200.f, 200.f}, data->player.pos, 12);

    InitializeSpriteAnim(&data->player.animations[0], 4, 5);
    data->player.animations[0].sprite_handles[0] = LoadSprite(&data->fw.sprite_sheet, "data/testanim-01.png");
    data->player.animations[0].sprite_handles[1] = LoadSprite(&data->fw.sprite_sheet, "data/testanim-02.png");
    data->player.animations[0].sprite_handles[2] = LoadSprite(&data->fw.sprite_sheet, "data/testanim-03.png");
    data->player.animations[0].sprite_handles[3] = LoadSprite(&data->fw.sprite_sheet, "data/testanim-04.png");
    
    data->sprite_handle1 = LoadSprite(&data->fw.sprite_sheet, "data/blue64.png");
    
    InitializeSpriteAnim(&data->player.animations[1], 3, 10.f);
    data->player.animations[1].sprite_handles[0] = LoadSprite(&data->fw.sprite_sheet, "data/testanimattack-01.png");
    data->player.animations[1].sprite_handles[1] = LoadSprite(&data->fw.sprite_sheet, "data/testanimattack-02.png");
    data->player.animations[1].sprite_handles[2] = LoadSprite(&data->fw.sprite_sheet, "data/testanimattack-03.png");
    data->player.animations[1].user_data = data;
    data->player.animations[1].callback = &UnAttackAnim;
    
    data->sprite_handle2 = LoadSprite(&data->fw.sprite_sheet, "data/blue64.png");
    data->player_outline = LoadSprite(&data->fw.sprite_sheet, "data/rect_outline.png");

    AddActionCallback(&data->player.active_anim, &PlayerAttack);
    AddActionCallback(&data->player.velocity, &PlayerMove);
    AddMouseCallback(nullptr, &mouse_callback);
    AddMouseCallback(&data->player, &mouse_on_player);

    return data;
}

void 
GameUpdate(GameData* data, float delta_time)
{
    UpdateSpriteAnimation(&data->player.animations[data->player.active_anim], delta_time);

    data->player.pos.x += data->player.velocity.x * delta_time;
    data->player.pos.y += data->player.velocity.y * delta_time;

    CreateViewMatrixFollow(&data->fw.main_camera, data->player.pos);
}

void 
GameRender(GameData* data)
{
    SetUniform(&data->fw.shader, "view", data->fw.main_camera.view);

    SpriteSheetBeginRender(&data->fw.sprite_sheet);
    
    RenderSpriteAnimationFrame(&data->fw.sprite_sheet, &data->player.animations[data->player.active_anim], data->player.pos);

    AddSizedSpriteToRender(&data->fw.sprite_sheet, data->player_outline, v3f{data->player.pos.x, data->player.pos.y, 2}, v2f{32.f,32.f});
    
    float offset_pos = 30;
    AddSpriteToRender(&data->fw.sprite_sheet, data->sprite_handle1, v3f{-offset_pos, -offset_pos, -1.0});
    AddSpriteToRender(&data->fw.sprite_sheet, data->sprite_handle2, v3f{ offset_pos,  offset_pos,  1.0});
    
    DisplayEntireSheet(&data->fw.sprite_sheet, {0.f,0.f, 0.0f}, {400.4f,400.4f});
    
    EndRender(&data->fw.sprite_sheet.renderer);
}

void
GameCleanup(GameData*& data)
{
    CleanupSpriteAnimation(&data->player.animations[0]);
    CleanupSpriteAnimation(&data->player.animations[1]);

    CleanupFramework(&data->fw);

    delete data;
    data = 0;
}