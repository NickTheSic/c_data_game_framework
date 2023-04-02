
#ifndef NL_CONTROLLER_H_
#define NL_CONTROLLER_H_

struct Controller
{
    float right_trigger;
    float left_trigger;

    float left_x_axis;
    float left_y_axis;
    float right_x_axis;
    float right_y_axis;

    unsigned short dpad_up        : 1;
    unsigned short dpad_down      : 1;
    unsigned short dpad_left      : 1;
    unsigned short dpad_right     : 1;
    unsigned short start          : 1;
    unsigned short select         : 1;
    unsigned short left_stick     : 1;
    unsigned short right_stick    : 1; 
    unsigned short left_shoulder  : 1;
    unsigned short right_shoulder : 1;
    unsigned short a              : 1;
    unsigned short b              : 1;
    unsigned short x              : 1;
    unsigned short y              : 1;
    
    // Silly to make it a 3 player game max
    unsigned char player_index    : 2;
};

void InitControllerSystem();
void CleanupControllerSystem();

void LogControllerState(Controller* controller);

void PollController(Controller* controller);
void VibrateController(Controller* controller, float intensity);

#endif //NL_CONTROLLER_H_