
#ifndef NL_UI_H_
#define NL_UI_H_

#include <nl_camera.h>
#include <nl_math.h>
#include <nl_spritesheet.h>
#include <nl_utils.h>

// Not very immediate mode UI
// Might want a 'do_button' function that makes and handles a button 
// Also need to add the 'hot item', mainly for controller support

typedef void(*ButtonPressCallback)();

// Name overlaps with the input
enum class UIButtonState : unsigned char
{
    Inactive = 0, Hovered, Pressed, Released,
    COUNT
};

struct Button
{
    ButtonPressCallback press_callback;

    // are these screen coords or sprite coords?
    // TODO: figure this out
    // I would rather have a pos + size instead of this I think
    // The UI can draw a button as a button from the normal sprite sheet?
    v2f bl_coord; // Make a position - doesn't need to be 3d though
    v2f ur_coord;

    UIButtonState active_state;
};

// considering having my own UI sprite sheet
struct UI
{
    Camera cam;

    // Rendering Stuff
    SpriteHandle button_sprites[(unsigned long long)UIButtonState::COUNT];

    std::vector<Button> buttons;    
};

void InitUI(UI* ui, struct Platform* platform);
void UpdateUI(UI* ui, struct Platform* platform);

void RenderUI(UI* ui, struct Framework* fw);

bool HandleButton(Button* button, const v2f& mouse_pos, bool mouse_button_down);

#endif //NL_UI_H_
