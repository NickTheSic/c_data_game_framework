#include <nl_ui.h>
#include <nl_framework.h>
#include <nl_input.h>
#include <nl_platform.h>
#include <nl_math.h>
#include <nl_shader.h>
#include <nl_spritesheet.h>
#include <nl_viewport.h>

static const float UI_TOP_POS = 1.0f;

// Make this a camera functuion?
static void 
InitUICamera(UI* ui, const v2f& size)
{
    ui->cam.size.x = size.x;
    ui->cam.size.y = size.y;
    
    const float left   = 0.f;
    const float right  = ui->cam.size.x;
    const float bottom = 0.f;
    const float top    = ui->cam.size.y;

    CreateOrtho(ui->cam.view, left, right, bottom, top, 0.0f, 100.0f); 
}

//TODO: This is wrong, as I made an original assumptionabout how my cameras were going to work
static void
GetMousePosInUICamera(Camera* camera, v2f* out_pos, v2i mouse_pos)
{
    // Only really need to use the mouse pos for the UI.
    // It could be ideal to do it this way for the UI as the UI should always be the screen size
    out_pos->x = static_cast<float>(mouse_pos.x);
    out_pos->y = static_cast<float>(mouse_pos.y);
}

void 
InitUI(UI* ui, struct Platform* platform)
{
    ui->button_sprites[(unsigned char)UIButtonState::Inactive] = LoadSprite(&platform->fw.sprite_sheet, "data/button_inactive.png");
    ui->button_sprites[(unsigned char)UIButtonState::Hovered] = LoadSprite(&platform->fw.sprite_sheet, "data/button_hovered.png");
    ui->button_sprites[(unsigned char)UIButtonState::Pressed] = LoadSprite(&platform->fw.sprite_sheet, "data/button_pressed.png");
    ui->error_sprite = LoadSprite(&platform->fw.sprite_sheet, "data/err.png");

    char letter_filepaths[] = "data/font/#.png"; // 10th should be replaceable
    for (char i = START_FONT_CHARACTERS; i <= END_FONT_CHARACTERS; ++i)
    {
        letter_filepaths[10] = i;
        LOG("%s", letter_filepaths);
        ui->letter_sprites[i-START_FONT_CHARACTERS] = LoadSprite(&platform->fw.sprite_sheet, letter_filepaths);
    }

    for (char i = START_NUMBER_CHARACTER; i <= END_NUMBER_CHARACTER; ++i)
    {
        letter_filepaths[10] = i;
        LOG("%s", letter_filepaths);
        ui->number_sprites[i - START_NUMBER_CHARACTER] = LoadSprite(&platform->fw.sprite_sheet, letter_filepaths);
    }

    v2f screen_size;
    screen_size.x = static_cast<float>(platform->viewport.screen_size.x);
    screen_size.y = static_cast<float>(platform->viewport.screen_size.y);
    InitUICamera(ui, screen_size);
}

void 
UpdateUI(UI* ui, struct Platform* platform)
{
    v2f mouse_pos = {};
    mouse_pos.x = platform->input.mouse_pos.x;
    mouse_pos.y = platform->input.mouse_pos.y;

    GetMousePosInUICamera(&ui->cam, &mouse_pos, platform->input.mouse_pos);

    //  Might need a Down/Pressed/Released state instead of just down or up
    bool mouse_down = platform->input.mouse_button[(unsigned char)MouseButton::Left] == ButtonState::Down;

    // Can find mouse pos and loop through only the buttons that are near?
    for (auto& button : ui->buttons)
    {
        if (!HandleButton(&button, mouse_pos, mouse_down))
        {
            continue;
        }
        else
        {
            break;
        }
    }
}

void 
RenderUI(UI* ui,  struct Framework* fw)
{
    SetViewUniform(&fw->shader, ui->cam.view);

    DisplayEntireSheet(&fw->sprite_sheet, {0.0f, 100.f, 0.0f}, {256.f,128.f});

    for (auto& button : ui->buttons)
    {
        AddSizedSpriteToRender(&fw->sprite_sheet, ui->button_sprites[(unsigned char)button.active_state], 
                              {button.origin.x, button.origin.y, UI_TOP_POS}, button.size);
    }
}

bool  
HandleButton(Button* button, const v2f& mouse_pos, bool mouse_button_down)
{
    bool mouse_handled = false;

    switch(button->active_state)
    {
        case UIButtonState::Inactive:
        {
            if (PointInRect(mouse_pos, button->origin, button->size))
            {
                button->active_state = UIButtonState::Hovered;
                mouse_handled = true;
            }
        } break;

        case UIButtonState::Hovered:
        {
            if (!PointInRect(mouse_pos, button->origin, button->size))
            {
                button->active_state = UIButtonState::Inactive;
            }
            else if (mouse_button_down)
            {
                button->active_state = UIButtonState::Pressed;
                mouse_handled = true;
            }
        } break;

        case UIButtonState::Pressed:
        {
            if (!PointInRect(mouse_pos, button->origin, button->size))
            {
                button->active_state = UIButtonState::Inactive;
            }
            else if (!mouse_button_down)
            {
                button->active_state = UIButtonState::Released;
                mouse_handled = true;
            }
        } break;

        case UIButtonState::Released:
        {
            if (button->press_callback)
            {
                button->press_callback();
            }
            button->active_state = UIButtonState::Inactive;
            mouse_handled = true;
        } break;

        case UIButtonState::COUNT:
        // Intentionally empty to surpress warning
        // Could do the pragma scope to surpress file warning
        break;
    }

    return mouse_handled;
}

void
DrawText(UI* ui, struct Framework* fw, const char* text, const v2f& pos, const v2f& font_size)
{
    char c = text[0];
    int count = 0;

    while (c != '\0')
    {
        SpriteHandle sprite = INVALID_SPRITE_HANDLE;

        if (c == ' ')
        {
            c = text[++count];
            continue;
        }

        if (c > START_NUMBER_CHARACTER && c < END_NUMBER_CHARACTER)
        {
            sprite = ui->number_sprites[(unsigned long long)c - START_NUMBER_CHARACTER];
        }
        else if (c > START_FONT_CHARACTERS && c < END_FONT_CHARACTERS)
        {
            sprite = ui->letter_sprites[(unsigned long long)c - START_FONT_CHARACTERS];
        }
        else
        {
            sprite = ui->error_sprite;
        }

        v3f render_pos;
        render_pos.x = pos.x + (font_size.x * count);
        render_pos.y = pos.y;
        render_pos.z = UI_TOP_POS;

        AddSizedSpriteToRender(&fw->sprite_sheet, sprite, render_pos, font_size);

        c = text[++count];
    }
}
