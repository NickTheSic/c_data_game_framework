
#ifndef NL_UI_H_
#define NL_UI_H_

#include <nl_camera.h>
#include <nl_math.h>
#include <nl_utils.h>

typedef void(*ButtonPressCallback)();

// Name overlaps with the input
enum class UIButtonState : unsigned char
{
    Inactive, Hovered, Pressed
};

struct Button
{
    ButtonPressCallback press_callback;

    // are these screen coords or sprite coords?
    // TODO: figure this out
    // I would rather have a pos + size instead of this I think
    // The UI can draw a button as a button from the normal sprite sheet?
    v2f bl_coord;
    v2f ur_coord;

    UIButtonState active_state;
};

struct UI
{
    Camera cam;
    std::vector<Button> buttons;    
};

void UpdateUI(UI* ui, struct Platform* platform);
void RenderUI(UI* ui, struct SpriteSheet* sprite_sheet);
void HandleButton(Button* button, const v2f& mouse_pos, bool mouse_button_down);

#endif //NL_UI_H_
