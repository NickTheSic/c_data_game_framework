
#ifndef NL_CONTROLLER_H_
#define NL_CONTROLLER_H_

struct Controller
{
    unsigned char dpad_up, dpad_down, dpad_left, dpad_right;
    unsigned char a, b, x, y;
    unsigned char right_shoulder, left_shoulder;
    unsigned char right_stick, left_stick

    unsigned char start, select;
    
    float right_trigger, left_trigger;
}

void InitControllerSystem(int controller_count);
void CleanupControllerSystem();

void PollControllers();



#endif //NL_CONTROLLER_H_