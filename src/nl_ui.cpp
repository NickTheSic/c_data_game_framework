#include <nl_ui.h>
#include <nl_framework.h>
#include <nl_input.h>
#include <nl_platform.h>
#include <nl_math.h>
#include <nl_shader.h>
#include <nl_spritesheet.h>
#include <nl_viewport.h>

static const float UI_TOP_POS = 1.0f;

// Make this a camera functuion?
static void 
InitUICamera(UI* ui, const v2f& size)
{
    ui->cam.size.x = size.x;
    ui->cam.size.y = size.y;
    
    const float left   = 0.f;
    const float right  = ui->cam.size.x;
    const float bottom = 0.f;
    const float top    = ui->cam.size.y;

    CreateOrtho(ui->cam.view, left, right, bottom, top, 0.0f, 10.0f); 
}

void UpdateUICamera(UI* ui, const v2f& size)
{
    InitUICamera(ui, size);
}

//TODO: This is wrong, as I made an original assumptionabout how my cameras were going to work
static void
GetMousePosInUICamera(Camera* camera, v2f* out_pos, v2i mouse_pos)
{
    UNUSED(camera);
    // Only really need to use the mouse pos for the UI.
    // It could be ideal to do it this way for the UI as the UI should always be the screen size
    out_pos->x = static_cast<float>(mouse_pos.x);
    out_pos->y = static_cast<float>(mouse_pos.y);
}

void 
InitUI(UI* ui, struct Platform* platform, int max_ui_elements)
{
    ui->max_ui_elements = max_ui_elements;
    ui->elements = static_cast<UIElement*>(calloc(ui->max_ui_elements, sizeof(UIElement)));
    ui->element_draw_count = 0;

    ui->sprites.button[0] = LoadSprite(&platform->fw.sprite_sheet, "data/button_inactive.png");
    ui->sprites.button[1] = LoadSprite(&platform->fw.sprite_sheet, "data/button_hovered.png");
    ui->sprites.button[2] = LoadSprite(&platform->fw.sprite_sheet, "data/button_pressed.png");
    ui->sprites.error = platform->fw.sprite_sheet.error_sprite;

    char letter_filepaths[] = "data/font/#.png"; // 10th should be replaceable
    for (char i = START_FONT_CHARACTERS; i <= END_FONT_CHARACTERS; ++i)
    {
        letter_filepaths[10] = i;
        //LOG("%s", letter_filepaths);
        ui->sprites.letters[i-START_FONT_CHARACTERS] = LoadSprite(&platform->fw.sprite_sheet, letter_filepaths);
    }

    for (char i = START_NUMBER_CHARACTER; i <= END_NUMBER_CHARACTER; ++i)
    {
        letter_filepaths[10] = i;
        //LOG("%s", letter_filepaths);
        ui->sprites.numbers[i - START_NUMBER_CHARACTER] = LoadSprite(&platform->fw.sprite_sheet, letter_filepaths);
    }

    v2f screen_size;
    screen_size.x = static_cast<float>(platform->viewport.screen_size.x);
    screen_size.y = static_cast<float>(platform->viewport.screen_size.y);
    InitUICamera(ui, screen_size);
}

void 
CleanupUI(UI* ui)
{
    free(ui->elements);
}

void 
UpdateUI(UI* ui, Platform* platform)
{
    // TODO: Could set values inside of the UI here
    // Then I don't need to pass them in as parameters
    v2f mouse_pos = {};
    mouse_pos.x = platform->input.mouse_pos.x;
    mouse_pos.y = platform->input.mouse_pos.y;

    GetMousePosInUICamera(&ui->cam, &mouse_pos, platform->input.mouse_pos);

    //  Might need a Down/Pressed/Released state instead of just down or up
    //bool mouse_down = platform->input.mouse_button[(unsigned char)MouseButton::Left] == ButtonState::Down;
    //UNUSED(mouse_down);
}

void 
RenderUI(UI* ui,  struct Framework* fw)
{
    SetViewUniform(&fw->shader, ui->cam.view);
    DisplayEntireSheet(&fw->sprite_sheet, {0.0f, 100.f, 0.0f}, {256.f,128.f});
}

bool 
HandleButton(UI* ui, const v2f& origin, const v2f& size, const char* label, const v2f& mouse_pos, bool mouse_button_down)
{
    bool button_pressed = false;

    UIElement* element = &ui->elements[ui->element_draw_count++];
    element->origin.x = origin.x;
    element->origin.y = origin.y;

    int count = 1;
    HandleText(ui, label, {origin.x + (size.y/2), origin.y + (size.y/6)}, {size.x - (size.x/3), size.y - (size.y/3)}, &count);

    element->size.x = size.x * (count);
    element->size.y = size.y;

    if (PointInRect(mouse_pos, element->origin, element->size))
    {
        if (mouse_button_down && element->is_active)
        {
            element->sprite = ui->sprites.button[2];
            element->is_active = false;
            
            button_pressed = true;
        }
        else if (mouse_button_down && !element->is_active)
        {
            element->sprite = ui->sprites.button[2];
        }
        else if (!mouse_button_down)
        {
            element->sprite = ui->sprites.button[1];
            element->is_active = true;
        }
        else
        {
            element->is_active = false;
            element->sprite = ui->sprites.button[0];
        }
    }
    else
    {
        element->is_active = false;
        element->sprite = ui->sprites.button[0];
    }

    return button_pressed;
}

void
HandleText(UI* ui, const char* text, const v2f& pos, const v2f& font_size, int* text_count)
{
    char c = text[0];
    int count = 0;

    while (c != '\0')
    {
        SpriteHandle sprite = INVALID_SPRITE_HANDLE;

        if (c == ' ')
        {
            c = text[++count];
            continue;
        }

        if (c >= START_NUMBER_CHARACTER && c <= END_NUMBER_CHARACTER)
        {
            sprite = ui->sprites.numbers[(unsigned long long)c - START_NUMBER_CHARACTER];
        }
        else if (c >= START_FONT_CHARACTERS && c <= END_FONT_CHARACTERS)
        {
            sprite = ui->sprites.letters[(unsigned long long)c - START_FONT_CHARACTERS];
        }
        else
        {
            sprite = ui->sprites.error;
        }

        ui->elements[ui->element_draw_count].origin.x = pos.x + (font_size.x * count);
        ui->elements[ui->element_draw_count].origin.y = pos.y;
        ui->elements[ui->element_draw_count].size.x = font_size.x;
        ui->elements[ui->element_draw_count].size.y = font_size.y;
        ui->elements[ui->element_draw_count].sprite = sprite;
        ui->elements[ui->element_draw_count].is_active = 0;

        ++ui->element_draw_count;

        c = text[++count];
    }

    if (text_count != 0)
    {
        *text_count = count;
    }
}

void 
EndUIRender(UI* ui, struct Framework* fw)
{
    v3f render_pos;
    render_pos.z = UI_TOP_POS;

    for (int i = 0; i < ui->element_draw_count; ++i)
    {
        UIElement* element = &ui->elements[i];
        render_pos.x = element->origin.x;
        render_pos.y = element->origin.y;
        AddSizedSpriteToRender(&fw->sprite_sheet, element->sprite, render_pos, element->size);
    }

    ui->element_draw_count = 0;
}
