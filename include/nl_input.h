
#ifndef NL_INPUT_H_
#define NL_INPUT_H_

#include <nl_key.h>
#include <vector>

// NOTE: I think this immediately falls apart if I start using a vector as the pointer would change

typedef void(*InputActionCallback)(KeyState state, Key key, void* data);
typedef void(*InputAxisCallback)(float val, void* data);
typedef void(*MouseInputCallback)(int mouse_button, int mouse_state, int mouse_x, int mouse_y, void* data);

struct InputCommand
{
    void* user_data;
    union
    {
        InputActionCallback action;
        InputAxisCallback axis;
        MouseInputCallback mouse;
    };
};

struct InputCallbacks
{
    std::vector<InputCommand> actions;
    std::vector<InputCommand> axese;
    std::vector<InputCommand> mouse;
};

void AddActionCallback(void* user_data, InputActionCallback callback);
void AddAxisCallback(void* user_data, InputAxisCallback callback);
void AddMouseCallback(void* user_data, MouseInputCallback callback);

void HandleAction(KeyState state, Key key_code);
void HandleAxis(float value);
void HandleMouse(MouseButton mouse_button, int state, int mouse_x, int mouse_y);

#endif //NL_INPUT_H_
