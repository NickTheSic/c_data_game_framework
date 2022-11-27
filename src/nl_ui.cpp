#include <nl_ui.h>
#include <nl_input.h>
#include <nl_platform.h>
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
    
    //GetMouseInViewportWithCamera(&mouse_pos, &platform->viewport, &ui->cam, platform->input.mouse_pos);
    bool mouse_down = platform->input.mouse_button[(unsigned char)MouseButton::Left] == ButtonState::Down;

    for (auto& button : ui->buttons)
    {
        HandleButton(&button, mouse_pos, mouse_down);
    }
}

void 
RenderUI(UI* ui, struct SpriteSheet* sprite_sheet)
{
    const float UI_TOP_POS = 1.0f;
    for (auto& button : ui->buttons)
    {
        AddSizedSpriteToRender(sprite_sheet, ui->button_sprites[(unsigned char)button.active_state], 
                              {button.bl_coord.x, button.bl_coord.y, UI_TOP_POS}, button.ur_coord);
    }
}

void 
HandleButton(Button* button, const v2f& mouse_pos, bool mouse_button_down)
{
    switch(button->active_state)
    {
        case UIButtonState::Inactive:
        {
            if (PointInRect(mouse_pos, button->bl_coord, button->ur_coord))
            {
                button->active_state = UIButtonState::Hovered;
            }
        } break;

        case UIButtonState::Hovered:
        {
            if (!PointInRect(mouse_pos, button->bl_coord, button->ur_coord))
            {
                button->active_state = UIButtonState::Inactive;
            }
            else if (mouse_button_down)
            {
                button->active_state = UIButtonState::Pressed;
            }
        } break;

        case UIButtonState::Pressed:
        {
            if (!PointInRect(mouse_pos, button->bl_coord, button->ur_coord))
            {
                button->active_state = UIButtonState::Inactive;
            }
            else if (!mouse_button_down)
            {
                button->press_callback();
            }
        } break;

        case UIButtonState::COUNT:
        // Intentionally empty to surpress warning
        break;
    }
}
