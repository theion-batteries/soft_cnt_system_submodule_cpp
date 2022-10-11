#pragma once
#include "cnt_dispenser.h"
#include "cnt_motion.h"
#include "cnt_high_voltage.h"
class cnt_controller
{
private:
    cnt_dispenser dispenser;

public:
    cnt_controller(/* args */);
    ~cnt_controller();
    void cnt_controller_connect();
    void cnt_dispenser_activate();
    void cnt_dispenser_deactivate();
    void cnt_dispenser_vibrate();
    void cnt_dispenser_setVibrateDuration(u_int durationSecond);
};


