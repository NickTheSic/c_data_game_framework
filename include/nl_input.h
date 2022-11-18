
#ifndef NL_INPUT_H_
#define NL_INPUT_H_

#include <nl_key.h>
#include <vector>

typedef void(*InputActionCallback)(KeyState state, Key key, void* data);
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

    std::vector<Key> action_keys;
};

void AddActionCallback(void* user_data, InputActionCallback callback);
void AddAxisCallback(void* user_data, InputAxisCallback callback);

void HandleAction(KeyState state, Key key_code);
void HandleAxis(float value);


static inline void
handle_mouse_over(int mouse_x, int mouse_y)
{
    
}


#endif //NL_INPUT_H_
