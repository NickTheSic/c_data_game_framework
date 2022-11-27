#include <nl_ui.h>
#include <nl_input.h>
#include <nl_platform.h>
#include <nl_spritesheet.h>
#include <nl_viewport.h>

void 
UpdateUI(UI* ui, struct Platform* platform)
{
    v2f mouse_pos = {};
    GetMouseInViewportWithCamera(&mouse_pos, &platform->viewport, &ui->cam, platform->input.mouse_pos);
    bool mouse_down = platform->input.mouse_button[(unsigned char)MouseButton::Left] == ButtonState::Down;

    for (auto& button : ui->buttons)
    {
        HandleButton(&button, mouse_pos, mouse_down);
    }
}

void 
RenderUI(UI* ui, struct SpriteSheet* sprite_sheet)
{

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
    }
}
