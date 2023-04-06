
#ifndef NL_UI_H_
#define NL_UI_H_

#include <nl_camera.h>
#include <nl_math.h>
#include <nl_spritesheet.h>
#include <nl_utils.h>

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


// Basically a sprite
struct UIElement
{
    v2f origin; 
    v2f size;

    SpriteHandle sprite;

    bool is_active;
};

struct UISprites
{
    // All buttons use the same 3 sprites
    // TODO: Should the sprites be turned into a 9x9 grid so they scale better?
    SpriteHandle button[3];

    // Hold all the sprites for the letters of the alphabet
    // This could definitely be handle differently,  I don't __need__ 32x32 letter sprites but that is what the sprite sheet expects?
    // TODO: Make this 1 array instread of 3
    SpriteHandle letters[(unsigned long long)(END_FONT_CHARACTERS - START_FONT_CHARACTERS)+1];
    SpriteHandle numbers[(unsigned long long)(END_NUMBER_CHARACTER - START_NUMBER_CHARACTER)+1];
    //TODO: Add
    SpriteHandle punctuation[1];

    // Might as well have an error sprite
    SpriteHandle error;
};

// Would Like a better way to handle the sprite sheet.  We need to end the draw call to switch to the UI anyway.
// Passing around a sprite sheet is a lot, but I could have multiple textures bound to the one.  All stuff to consider
struct UI
{
    Camera cam;

    UISprites sprites;

    int max_ui_elements;
    int element_draw_count;
    UIElement* elements;
};

void InitUI(UI* ui, struct Platform* platform, int max_ui_elements);
void CleanupUI(UI* ui);

void UpdateUI(UI* ui, struct Platform* platform);
void UpdateUICamera(UI* ui, const v2f& size);

void RenderUI(UI* ui, struct Framework* fw);
void EndUIRender(UI* ui, struct Framework* fw);

// Would like to update in the future to only take origin, size and label
// I could give the 'global UI context' a mouse position and if button was pressed this frame variable
bool HandleButton(UI* ui, const v2f& origin, const v2f& size, const char* label, const v2f& mouse_pos, bool mouse_button_down);
// bool HandleToggleButton(...); // This button remains clicked until it is clicked again
void HandleText(UI* ui, const char* text, const v2f& pos, const v2f& font_size, int* text_count = 0);

#endif //NL_UI_H_
