#include "cnt_system.h"
cnt_controller controller;
int main()
{
    controller.cnt_controller_connect();
    Sleep(500); // debug sleep
    controller.cnt_motion_unlock();
    Sleep(500); // debug sleep
    controller.cnt_motion_move_home();
    Sleep(500); // debug sleep
    controller.cnt_motion_move_to_center();
    Sleep(500); // debug sleep
    controller.cnt_dispenser_activate();
    Sleep(500); // debug sleep
    controller.cnt_dispenser_vibrate();
    Sleep(500); // debug sleep
    controller.cnt_dispenser_deactivate();
    Sleep(500); // debug sleep
    controller.cnt_motion_move_home();
    return 0;
}