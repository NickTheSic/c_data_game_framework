#include <nl_controller.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef near
#undef far
#include <xinput.h>

#define X_INPUT_GET_STATE(name) DWORD name(DWORD ControllerNumber, XINPUT_STATE* State)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
    UNUSED(ControllerNumber); UNUSED(State)
    LOG("XInput Get State Stub");
    return 0;
}
x_input_get_state *XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_

#define X_INPUT_SET_STATE(name) DWORD name(DWORD ControllerNumber, XINPUT_VIBRATION* Vibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub)
{
    UNUSED(ControllerNumber); UNUSED(Vibration);
    LOG("XInput Set State Stub");
    return 0;
}
x_input_set_state *XInputSetState_ = XInputSetStateStub;
#define XInputSetState XInputSetState_

HMODULE XInputLibrary = {};

void InitControllerSystem()
{
    XInputLibrary = LoadLibrary("xinput1_4.dll");

    if (!XInputLibrary)
    {
        XInputLibrary = LoadLibrary("xinput1_3.dll");
        if (!XInputLibrary)
        {
            XInputLibrary = LoadLibrary("Xinput9_1_0.dll");
        }
    }

    if (XInputLibrary)
    {
        XInputGetState = (x_input_get_state*)GetProcAddress(XInputLibrary, "XInputGetState");
        XInputSetState = (x_input_set_state*)GetProcAddress(XInputLibrary, "XInputSetState");
    }
}

void CleanupControllerSystem()
{
    FreeLibrary(XInputLibrary);
}

void PollController(Controller* controller)
{
    // For loop for each controller
    XINPUT_STATE state = {};
    if (XInputGetState(controller->player_index, &state) == 0)
    {   
        XINPUT_GAMEPAD* pad = &state.Gamepad;

        controller->dpad_up         = pad->wButtons & XINPUT_GAMEPAD_DPAD_UP;
        controller->dpad_down       = pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
        controller->dpad_left       = pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
        controller->dpad_right      = pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
        controller->start           = pad->wButtons & XINPUT_GAMEPAD_START;
        controller->select          = pad->wButtons & XINPUT_GAMEPAD_BACK;
        controller->left_stick      = pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
        controller->right_stick     = pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
        controller->left_shoulder   = pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
        controller->right_shoulder  = pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
        controller->a               = pad->wButtons & XINPUT_GAMEPAD_A;
        controller->b               = pad->wButtons & XINPUT_GAMEPAD_B;
        controller->x               = pad->wButtons & XINPUT_GAMEPAD_X;
        controller->y               = pad->wButtons & XINPUT_GAMEPAD_Y;

        controller->left_trigger    = (float)pad->bLeftTrigger / 255.f;
        controller->right_trigger   = (float)pad->bRightTrigger / 255.f;
        
    }
    else
    {
        // Controller is not available
    }
}

void VibrateController(Controller* controller, float intensity)
{
    UNUSED(controller); UNUSED(intensity);
    LOG("Vibrate Controller not implemented");
    assert(false);
}
