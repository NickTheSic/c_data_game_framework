
#ifndef NL_UI_H_
#define NL_UI_H_

#include <nl_utils.h>

typedef void(*ButtonPressCallback)();

enum class ButtonState
{
    Inactive, Hovered, Pressed
};

struct Button
{
    ButtonPressCallback press_callback;

    v2f bl_coord;
    v2f ur_coord;

    ButtonState active_state;
};


//TODO REMOVE this variable once I have a proper way to get the mouse position
static v2f mouse_pos = {};

void 
HandleButton(Button* button)
{
    switch(button->active_state)
    {
        case ButtonState::Inactive:
        {
            if (PointInRect(mouse_pos, button->bl_coord, button->ur_coord))
            {
                button->active_state == ButtonState::Hovered;
            }
        } break;

        case State::Hovered:
        {
            if (!PointInRect(mouse_pos, button->bl_coord, button->ur_coord))
            {
                button->active_state == ButtonState::Inactive;
            }
            // else Was mouse Pressed?
            // State = pressed
            // button->press_callback();
        } break;
    }
}

#endif //NL_UI_H_
