#pragma once
#include "cnt_dispenser_vibration.h"
#include "cnt_linear_motion.h"
#include "cnt_high_voltage_gbs.h"
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

class cnt_controller
{
private:
    std::shared_ptr <Icnt_dispenser> dispenser;
    std::shared_ptr <Icnt_axis_motion> motion;
    std::shared_ptr <Icnt_high_voltage> hv_Dev;
    bool cntReady = false;
public:
    cnt_controller(/* args */);
    ~cnt_controller();
    // controller
    void cnt_controller_connect();

    // motion
    void cnt_motion_connect();
    void cnt_motion_move_home();
    void cnt_motion_move_to_center();
    void cnt_motion_unlock();

    // dispenser
    void cnt_dispenser_connect();
    void cnt_dispenser_activate();
    void cnt_dispenser_deactivate();
    void cnt_dispenser_vibrate();
    void cnt_dispenser_setVibrateDuration(u_int durationSecond);

    // hv
    void cnt_hv_connect();

    /********* helper functions */
    bool get_motion_status();
    bool get_dispenser_status();
    bool get_hv_status();
    bool get_cnt_controller_status();
    /*     helper getter */
    double get_dispenser_frequency();
    double get_axis_position();
    Icnt_axis_motion* get_axis_ptr();
    Icnt_dispenser* get_dispenser_ptr();
    Icnt_high_voltage* get_hv_ptr();
    // direct call
    void sendDirectCmd(std::string& cmd);
    std::string sendDirectCmdAxis(std::string& cmd);
     void reload_config_file();
};


