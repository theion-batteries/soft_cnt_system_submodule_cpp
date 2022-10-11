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
};


