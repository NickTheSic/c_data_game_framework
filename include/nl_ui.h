
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

    // Currently a Rectangle like the sprites
    v2f origin; 
    v2f size; 

    UIButtonState active_state;
};

// considering having my own UI sprite sheet as I may need a UI shader
// Can handle screen size separate than game size
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
