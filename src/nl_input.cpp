#include <nl_input.h>

InputCallbacks g_input;

// Probably need to remove these too at some point
void AddActionCallback(void* user_data, InputActionCallback callback)
{
    InputCommand ic = {};

    ic.user_data = user_data;
    ic.action = callback;

    g_input.actions.push_back(ic);
}

void AddAxisCallback(void* user_data, InputAxisCallback callback)
{
    InputCommand ic = {};
    
    ic.user_data = user_data;
    ic.axis = callback;

    g_input.axese.push_back(ic);
}

void AddMouseCallback(void* user_data, MouseInputCallback callback)
{
    InputCommand ic = {};

    ic.user_data = user_data;
    ic.mouse = callback;

    g_input.mouse.push_back(ic);
}

void HandleAction(KeyState state, Key key_code)
{
    // This runs during Repeat with GLFW and I am not sure that I want this too
    // Maybe I can separate these callbacks into a press and release section so that there are less
    // even though I probably won't have a lot anyway.
    // fprintf(stdout, TEXT("Running Input callback\n"));
    for (auto& callback : g_input.actions)
    {   
        callback.action(state, key_code, callback.user_data);
    }
}

void HandleAxis(float value)
{
    for (auto& callback : g_input.axese)
    {
        callback.axis(value, callback.user_data);
    }
}

void HandleMouse(MouseButton mouse_button, int state, int mouse_x, int mouse_y)
{
    for (auto& callback : g_input.mouse)
    {
        #warning Casting to an int from the mouse button state
        callback.mouse((int)mouse_button, state, mouse_x, mouse_y, callback.user_data);
    }
}
