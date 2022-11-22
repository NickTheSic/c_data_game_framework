#include <nl_input.h>
#include <nl_debug.h>

void UpdateScreenSize(Input* input, int sx, int sy)
{
    LOG("SX: %d, SY: %d\n", sx, sy);

    input->screen_size.x = sx;
    input->screen_size.y = sy;
}

void Input_GetScreenSize(Input* input, int* sx, int* sy)
{
    *sx = input->screen_size.x;
    *sy = input->screen_size.y;
}

void UpdateMousePosition(Input* input, int mouse_x, int mouse_y)
{
    input->prev_mouse_pos.x = input->mouse_pos.x;
    input->prev_mouse_pos.y = input->mouse_pos.y;
    input->mouse_pos.x = mouse_x;
    input->mouse_pos.y = mouse_y;

    //LOG("MX: %d,MY: %d\n", mouse_x, mouse_y);
}

void UpdateKeyState(Input* input, Key key, ButtonState state)
{
    // By using a unsigned char it should always be in range
    input->keys[(unsigned char)key] = state;

    HandleAction(state, key);
}

void UpdateMouseState(Input* input, MouseButton button, ButtonState state)
{
    unsigned char mv = (unsigned char)button;
    if (mv < 0 || mv > 3)
    {
        LOG("Mouse Value was out of range: %d", mv);
    }

    input->mouse_button[mv] = state;
}

void AddActionCallback(Input* input, void* user_data, InputActionCallback callback)
{
    InputCommand ic = {};

    ic.user_data = user_data;
    ic.action = callback;

    input->callbacks.actions.push_back(ic);
}

void AddAxisCallback(Input* input, void* user_data, InputAxisCallback callback)
{
    InputCommand ic = {};
    
    ic.user_data = user_data;
    ic.axis = callback;

    input->callbacks.axese.push_back(ic);
}

void AddMouseCallback(Input* input, void* user_data, MouseInputCallback callback)
{
    InputCommand ic = {};

    ic.user_data = user_data;
    ic.mouse = callback;

    input->callbacks.mouse.push_back(ic);
}

void HandleAction(Input* input, KeyState state, Key key_code)
{
    // This runs during Repeat with GLFW and I am not sure that I want this too
    // Maybe I can separate these callbacks into a press and release section so that there are less
    // even though I probably won't have a lot anyway.
    // fprintf(stdout, TEXT("Running Input callback\n"));
    for (auto& callback : input->callbacks.actions)
    {   
        callback.action(state, key_code, callback.user_data);
    }
}

void HandleAxis(Input* input, float value)
{
    for (auto& callback : input->callbacks.axese)
    {
        callback.axis(value, callback.user_data);
    }
}

void HandleMouseButton(Input* input, MouseButton mouse_button, int state, int mouse_x, int mouse_y)
{
    for (auto& callback : input->callbacks.mouse)
    {
        #warning Casting to an int from the mouse button state
        callback.mouse((int)mouse_button, state, mouse_x, mouse_y, callback.user_data);
    }
}
