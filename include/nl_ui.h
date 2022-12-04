
#ifndef NL_UI_H_
#define NL_UI_H_

#include <nl_camera.h>
#include <nl_math.h>
#include <nl_spritesheet.h>
#include <nl_utils.h>

// Not very immediate mode UI, which is what I originally wanted

typedef void(*ButtonPressCallback)(/*void* user_data*/);

// Name overlaps with the input
enum class UIButtonState : unsigned char
{
    Inactive = 0, Hovered, Pressed, Released,
    COUNT
};

struct Button
{
    ButtonPressCallback press_callback;
    // void* user_data;

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
