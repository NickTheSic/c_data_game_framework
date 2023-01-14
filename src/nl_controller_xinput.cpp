#include <nl_controller.h>
#include <xinput.h>

void InitControllerSystem();
void CleanupControllerSystem();

void PollController(Controller* controller)
{
    // For loop for each controller
    XINPUT_STATE state = {};
    if (XInputGetState(0, &state) == 0)
    {   
        XINPUT_GAMEPAD* pad = &state.Gamepad;

        controller->dpad_up         = pad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
        controller->dpad_down       = pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
        controller->dpad_left       = pad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
        controller->dpad_right      = pad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
        controller->start           = pad.wButtons & XINPUT_GAMEPAD_START;
        controller->select          = pad.wButtons & XINPUT_GAMEPAD_BACK;
        controller->left_stick      = pad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
        controller->right_stick     = pad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
        controller->left_shoulder   = pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
        controller->right_shoulder  = pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
        controller->a               = pad.wButtons & XINPUT_GAMEPAD_A;
        controller->b               = pad.wButtons & XINPUT_GAMEPAD_B;
        controller->x               = pad.wButtons & XINPUT_GAMEPAD_X;
        controller->y               = pad.wButtons & XINPUT_GAMEPAD_Y;

        controller->left_trigger    = (float)pad.bLeftTrigger / 255.f;
        controller->right_trigger   = (float)pad.bRightTrigger / 255.f;
        
    }
    else
    {
        // Controller is not available
    }
}
