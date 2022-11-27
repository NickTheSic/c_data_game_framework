
#ifndef NL_INPUT_H_
#define NL_INPUT_H_

#include <nl_key.h>
#include <vector>

// NOTE: I think this immediately falls apart if I start using a vector as the pointer would change
// TODO: Could start defining parameters so I only have to change them in 1 spot

typedef void(*InputActionCallback)(Key key, ButtonState state, void* data);
typedef void(*InputAxisCallback)(float val, void* data);
typedef void(*MouseInputCallback)(MouseButton mouse_button, ButtonState mouse_state, void* data);

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

struct Input
{
    InputCallbacks callbacks;

    // here for now since it is used to calculate the on screen mouse position
    // Less input and more of a screen thing

//Make this its own struct of data for asier passing around?
    v2i mouse_pos, prev_mouse_pos;
    //TODO: this should be okay but I know some web key values go above 256
    ButtonState keys[256]; 
    ButtonState mouse_button[3];

    //Mouse Scroll value?
};

void UpdateMousePosition(Input* input, int mouse_x, int mouse_y);
void UpdateKeyState(Input* input, Key key, ButtonState state);
void UpdateMouseState(Input* input, MouseButton button, ButtonState state);

void AddActionCallback(Input* input, void* user_data, InputActionCallback callback);
void AddAxisCallback(Input* input, void* user_data, InputAxisCallback callback);
void AddMouseCallback(Input* input, void* user_data, MouseInputCallback callback);

void HandleAction(Input* input, ButtonState state, Key key_code);
void HandleAxis(Input* input, float value);
void HandleMouseButton(Input* input, MouseButton mouse_button, int state);

#endif //NL_INPUT_H_
