
#ifndef NL_CONTROLLER_H_
#define NL_CONTROLLER_H_

struct Controller
{
    bool dpad_up, dpad_down, dpad_left, dpad_right;
    bool a, b, x, y;
    bool right_stick, left_stick
    bool start, select;

    float right_shoulder, left_shoulder;
    
    float right_trigger, left_trigger;
}

void InitControllerSystem(int controller_count);
void CleanupControllerSystem();

void PollController(Controller* controller);


#endif //NL_CONTROLLER_H_