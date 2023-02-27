#pragma once

#include "sockpp/socket.h"
#include <iostream>
#include <string>
#include "sockpp/tcp_connector.h"
#include "Windows.h"
#include <shellapi.h>
#include <map>
#include <thread>
#include <atomic>
#include "subsystem_feedback.h"
using wgm_feedbacks::enum_sub_sys_feedback;

class Icnt_high_voltage
{
public:
    Icnt_high_voltage(/* args */);
    ~Icnt_high_voltage();

    virtual wgm_feedbacks::enum_sub_sys_feedback disconnect() = 0;
    virtual wgm_feedbacks::enum_sub_sys_feedback connect() = 0;
    virtual std::string sendDirectCmd(std::string cmd) = 0;
    ;
    virtual std::string waitForResponse() = 0;
    virtual enum_sub_sys_feedback start() = 0;
    virtual enum_sub_sys_feedback stop() = 0;
    virtual enum_sub_sys_feedback pulse() = 0;
    virtual enum_sub_sys_feedback modulate() = 0;
    virtual enum_sub_sys_feedback tune_param(std::pair<std::string, int> new_param_val) = 0; // tune param
    virtual enum_sub_sys_feedback set_output_voltage(const double) = 0;
    virtual double get_output_resistivity() = 0;
    virtual enum_sub_sys_feedback set_output_frequency(const double) = 0;

    virtual bool getStatus() = 0;
    virtual double get_input_current() = 0;
    virtual double get_input_frequency() = 0;
    virtual double get_input_voltage() = 0;
    virtual double get_output_voltage() = 0;
    virtual double get_output_frequency() = 0;
    virtual double get_output_current() = 0;
};
