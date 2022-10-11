#include "cnt_system.h"
cnt_controller controller;
int main()
{
    controller.cnt_controller_connect();
    controller.cnt_dispenser_activate();
    Sleep(2000);

    controller.cnt_dispenser_setVibrateDuration(10000);
    Sleep(2000);

    controller.cnt_dispenser_vibrate();
    Sleep(5000);

    controller.cnt_dispenser_deactivate();
    return 0;
}