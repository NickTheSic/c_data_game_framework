#include <nl_controller.h>

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

        controller->dpad_up        = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP)        != 0;
        controller->dpad_down      = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN)      != 0;
        controller->dpad_left      = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT)      != 0;
        controller->dpad_right     = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)     != 0;
        controller->start          = (pad->wButtons & XINPUT_GAMEPAD_START)          != 0;
        controller->select         = (pad->wButtons & XINPUT_GAMEPAD_BACK)           != 0;
        controller->left_stick     = (pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB)     != 0;
        controller->right_stick    = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)    != 0;
        controller->left_shoulder  = (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)  != 0;
        controller->right_shoulder = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
        controller->a              = (pad->wButtons & XINPUT_GAMEPAD_A)              != 0;
        controller->b              = (pad->wButtons & XINPUT_GAMEPAD_B)              != 0;
        controller->x              = (pad->wButtons & XINPUT_GAMEPAD_X)              != 0;
        controller->y              = (pad->wButtons & XINPUT_GAMEPAD_Y)              != 0;

        controller->left_trigger   = (float)pad->bLeftTrigger / 255.f;
        controller->right_trigger  = (float)pad->bRightTrigger / 255.f;

        if (pad->sThumbLX < 0){
            controller->left_x_axis = (float)pad->sThumbLX / 32768.0f;
        } else {
            controller->left_x_axis = (float)pad->sThumbLX / 32767.0f;
        }

        if (pad->sThumbLY < 0){
            controller->left_y_axis = (float)pad->sThumbLY / 32768.f;
        } else {   
            controller->left_y_axis = (float)pad->sThumbLY / 32767.f;
        }   

        if (pad->sThumbRX < 0){
            controller->right_x_axis = (float)pad->sThumbRX / 32768.f;
        } else {
            controller->right_x_axis = (float)pad->sThumbRX / 32767.f;
        }
        
        if (pad->sThumbRY < 0){
            controller->right_y_axis = (float)pad->sThumbRY / 32768.f;
        } else {
            controller->right_y_axis = (float)pad->sThumbRY / 32767.f;
        }

    }
    else
    {
        // Controller is not available
    }
}

void 
LogControllerState(Controller* controller)
{
    if (controller->dpad_up != 0){ LOG("dpad_up is down");} 
    if (controller->dpad_down != 0){ LOG("dpad_down is down");} 
    if (controller->dpad_left != 0){ LOG("dpad_left is down");} 
    if (controller->dpad_right != 0){ LOG("dpad_right is down");} 
    if (controller->start != 0){ LOG("start is down");} 
    if (controller->select != 0){ LOG("select is down");} 
    if (controller->left_stick != 0){ LOG("left_stick is down");} 
    if (controller->right_stick != 0){ LOG("right_stick is down");} 
    if (controller->left_shoulder != 0){ LOG("left_shoulder is down");} 
    if (controller->right_shoulder != 0){ LOG("right_shoulder is down");} 
    if (controller->a != 0){ LOG("a is down");} 
    if (controller->b != 0){ LOG("b is down");} 
    if (controller->x != 0){ LOG("x is down");} 
    if (controller->y != 0){ LOG("y is down");} 

    if (controller->left_trigger != 0.0f){ LOG("controller->left_trigger is %f", controller->left_trigger);} 
    if (controller->right_trigger != 0.0f){ LOG("controller->left_trigger is %f", controller->right_trigger);} 
    if (controller->left_x_axis != 0.0f){ LOG("controller->left_x_axis is %f", controller->left_x_axis);} 
    if (controller->left_y_axis != 0.0f){ LOG("controller->left_y_axis is %f", controller->left_y_axis);} 
    if (controller->right_x_axis != 0.0f){ LOG("controller->right_x_axis is %f", controller->right_x_axis);} 
    if (controller->right_y_axis != 0.0f){ LOG("controller->right_y_axis is %f", controller->right_y_axis);} 
}

void VibrateController(Controller* controller, float intensity)
{
    UNUSED(controller); UNUSED(intensity);
    LOG("Vibrate Controller not implemented");
    assert(false);
}
