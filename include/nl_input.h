
#ifndef NL_INPUT_H_
#define NL_INPUT_H_

#include <vector>

typedef void(*InputActionCallback)(int state, int key, void* data);
typedef void(*InputAxisCallback)(float val, void* data);

struct InputCommand
{
    void* user_data;
    union
    {
        InputActionCallback action;
        InputAxisCallback axis;
    };
};

struct InputCallbacks
{
    std::vector<InputCommand> actions;
    std::vector<InputCommand> axese;
};

// Probably need to remove these too at some point
//void AddActionCallback(InputCallbacks* input, void* user_data, InputActionCallback callback);
//void AddAxisCallback(InputCallbacks* input, void* user_data, InputAxisCallback callback);
//
//void HandleAction(InputCallbacks* input, int state, int key_code);
//void HandleAxis(InputCallbacks* input, float value);

void AddActionCallback(void* user_data, InputActionCallback callback);
void AddAxisCallback(void* user_data, InputAxisCallback callback);

void HandleAction(int state, int key_code);
void HandleAxis(float value);

#endif //NL_INPUT_H_