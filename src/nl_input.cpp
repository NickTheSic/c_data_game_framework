#include <nl_input.h>
#include <nl_debug.h>

Input g_inputs;

void UpdateScreenSize(int sx, int sy)
{
    LOG("SX: %d, SY: %d\n", sx, sy);

    g_inputs.screen_size.x = sx;
    g_inputs.screen_size.y = sy;
}

void Input_GetScreenSize(int* sx, int* sy)
{
    *sx = g_inputs.screen_size.x;
    *sy = g_inputs.screen_size.y;
}

void UpdateMousePosition(int mouse_x, int mouse_y)
{
    g_inputs.prev_mouse_pos.x = g_inputs.mouse_pos.x;
    g_inputs.prev_mouse_pos.y = g_inputs.mouse_pos.y;
    g_inputs.mouse_pos.x = mouse_x;
    g_inputs.mouse_pos.y = mouse_y;

    //LOG("MX: %d,MY: %d\n", mouse_x, mouse_y);
}

void UpdateKeyState(Key key, ButtonState state)
{
    // By using a unsigned char it should always be in range
    g_inputs.keys[(unsigned char)key] = state;

    HandleAction(state, key);
}

void UpdateMouseState(MouseButton button, ButtonState state)
{
    unsigned char mv = (unsigned char)button;
    if (mv < 0 || mv > 3)
    {
        LOG("Mouse Value was out of range: %d", mv);
    }

    g_inputs.mouse_button[mv] = state;
}

void AddActionCallback(void* user_data, InputActionCallback callback)
{
    InputCommand ic = {};

    ic.user_data = user_data;
    ic.action = callback;

    g_inputs.callbacks.actions.push_back(ic);
}

void AddAxisCallback(void* user_data, InputAxisCallback callback)
{
    InputCommand ic = {};
    
    ic.user_data = user_data;
    ic.axis = callback;

    g_inputs.callbacks.axese.push_back(ic);
}

void AddMouseCallback(void* user_data, MouseInputCallback callback)
{
    InputCommand ic = {};

    ic.user_data = user_data;
    ic.mouse = callback;

    g_inputs.callbacks.mouse.push_back(ic);
}

void HandleAction(KeyState state, Key key_code)
{
    // This runs during Repeat with GLFW and I am not sure that I want this too
    // Maybe I can separate these callbacks into a press and release section so that there are less
    // even though I probably won't have a lot anyway.
    // fprintf(stdout, TEXT("Running Input callback\n"));
    for (auto& callback : g_inputs.callbacks.actions)
    {   
        callback.action(state, key_code, callback.user_data);
    }
}

void HandleAxis(float value)
{
    for (auto& callback : g_inputs.callbacks.axese)
    {
        callback.axis(value, callback.user_data);
    }
}

void HandleMouseButton(MouseButton mouse_button, int state, int mouse_x, int mouse_y)
{
    for (auto& callback : g_inputs.callbacks.mouse)
    {
        #warning Casting to an int from the mouse button state
        callback.mouse((int)mouse_button, state, mouse_x, mouse_y, callback.user_data);
    }
}
