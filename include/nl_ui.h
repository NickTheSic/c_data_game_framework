
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

/*
TODO: NOTES: Ascii 
Space = 32 
0 = 40
A = 65
a = 97
z = 122 

*/

static const char START_FONT_CHARACTERS = 'a';
static const char END_FONT_CHARACTERS = 'z';

static const char START_NUMBER_CHARACTER = '0';
static const char END_NUMBER_CHARACTER = '9';

// considering having my own UI sprite sheet as I may need a UI shader
// Can handle screen size separate than game size
struct UI
{
    Camera cam;

    // Rendering Stuff
    SpriteHandle button_sprites[(unsigned long long)UIButtonState::COUNT];

    // Hold all the sprites for the letters of the alphabet
    // This could definitely be handle differently,  I don't __need__ 32x32 letter sprites but that is what the sprite sheet expects?
    SpriteHandle letter_sprites[(unsigned long long)(END_FONT_CHARACTERS - START_FONT_CHARACTERS)+1];
    SpriteHandle number_sprites[(unsigned long long)(END_NUMBER_CHARACTER - START_NUMBER_CHARACTER)+1];
    SpriteHandle error_sprite;

    std::vector<Button> buttons;    
};

void InitUI(UI* ui, struct Platform* platform);
void UpdateUI(UI* ui, struct Platform* platform);

void RenderUI(UI* ui, struct Framework* fw);

bool HandleButton(Button* button, const v2f& mouse_pos, bool mouse_button_down);
void DrawText(UI* ui, struct Framework* fw, const char* text, const v2f& pos, const v2f& font_size);

#endif //NL_UI_H_
