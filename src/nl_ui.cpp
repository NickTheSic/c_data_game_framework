#include <nl_ui.h>
#include <nl_framework.h>
#include <nl_input.h>
#include <nl_platform.h>
#include <nl_shader.h>
#include <nl_spritesheet.h>
#include <nl_viewport.h>

void 
InitUI(UI* ui, struct Platform* platform)
{
    ui->button_sprites[(unsigned char)UIButtonState::Inactive] = LoadSprite(&platform->fw.sprite_sheet, "data/button_inactive.png");
    ui->button_sprites[(unsigned char)UIButtonState::Hovered] = LoadSprite(&platform->fw.sprite_sheet, "data/button_hovered.png");
    ui->button_sprites[(unsigned char)UIButtonState::Pressed] = LoadSprite(&platform->fw.sprite_sheet, "data/button_pressed.png");

    CopyCameraToFrom(&ui->cam, &platform->fw.main_camera);
}

void 
UpdateUI(UI* ui, struct Platform* platform)
{
    v2f mouse_pos = {};
    mouse_pos.x = platform->input.mouse_pos.x;
    mouse_pos.y = platform->input.mouse_pos.y;

    GetMousePosInCamera(&ui->cam, &mouse_pos, platform->input.mouse_pos);

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
    SetUniform(&fw->shader, "view", ui->cam.view);

    const float UI_TOP_POS = 1.0f;
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
            button->active_state = UIButtonState::Inactive;
            mouse_handled = true;
        } break;

        case UIButtonState::COUNT:
        // Intentionally empty to surpress warning
        break;
    }

    return mouse_handled;
}
