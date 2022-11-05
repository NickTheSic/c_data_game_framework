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

    g_input.actions.push_back(ic);
}

void HandleAction(int state, int key_code)
{
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



// Probably need to remove these too at some point
void AddActionCallback(InputCallbacks* input, void* user_data, InputActionCallback callback)
{
    InputCommand ic = {};

    ic.user_data = user_data;
    ic.action = callback;

    input->actions.push_back(ic);
}

void AddAxisCallback(InputCallbacks* input, void* user_data, InputAxisCallback callback)
{
    InputCommand ic = {};
    
    ic.user_data = user_data;
    ic.axis = callback;

    input->actions.push_back(ic);
}

void HandleAction(InputCallbacks* input, int state, int key_code)
{
    for (auto& callback : input->actions)
    {
        callback.action(state, key_code, callback.user_data);
    }
}

void HandleAxis(InputCallbacks* input, float value)
{
    for (auto& callback : input->axese)
    {
        callback.axis(value, callback.user_data);
    }
}
