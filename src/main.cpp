#include "cnt_system.h"
cnt_controller controller;
int main()
{
    controller.cnt_controller_connect();
    Sleep(1000); // debug sleep
    controller.cnt_motion_unlock();
    Sleep(2000); // debug sleep
    controller.cnt_motion_move_home();
    Sleep(5000); // debug sleep
    controller.cnt_motion_move_to_center();
    Sleep(5000); // debug sleep
    controller.cnt_dispenser_activate();
    Sleep(1000); // debug sleep
    controller.cnt_dispenser_vibrate();
    Sleep(2000); // debug sleep
    controller.cnt_dispenser_deactivate();
    Sleep(1000); // debug sleep
    controller.cnt_motion_move_home();
    Sleep(3000);
    return 0;
}