
#ifndef NL_CONTROLLER_H_
#define NL_CONTROLLER_H_

struct Controller
{
    int player_index;

    bool dpad_up, dpad_down, dpad_left, dpad_right;
    bool start, select;
    bool left_stick, right_stick; // Stick Click
    bool left_shoulder, right_shoulder;
    bool a, b, x, y;

    float right_trigger, left_trigger;

    float left_x_axis, left_y_axis;
    float right_x_axis, right_y_axis;
};

void InitControllerSystem();
void CleanupControllerSystem();

void PollController(Controller* controller);


#endif //NL_CONTROLLER_H_