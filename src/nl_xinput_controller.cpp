#include <nl_controller.h>
#include <xinput.h>

void InitControllerSystem();
void CleanupControllerSystem();

void PollControllers()
{
    // For loop for each controller
    XINPUT_STATE state = {};
    if (XInputGetState(0, &state) == 0)
    {   
        XINPUT_GAMEPAD* pad = &state.Gamepad;
        
    }
    else
    {
        // Controller is not available
    }
}
