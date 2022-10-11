#include "cnt_system.h"
cnt_controller controller;
int main()
{
    controller.cnt_controller_connect();
    controller.cnt_dispenser_activate();
    return 0;
}