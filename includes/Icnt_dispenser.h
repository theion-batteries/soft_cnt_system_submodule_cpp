/**
 * @file Icnt_dispenser.h
 * @author sami dhiab sami@theion.de
 * @brief
 * @version 0.1
 * @date 2022-11-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once
#include "sockpp/socket.h"
#include <iostream>
#include <string>
#include "sockpp/tcp_connector.h"
#include <iostream>
#include "subsystem_feedback.h"
using enum wgm_feedbacks::enum_hw_feedback;
using enum wgm_feedbacks::enum_sub_sys_feedback;

class Icnt_dispenser
{
private:
    /* data */
public:
    virtual  ~Icnt_dispenser() {};
    virtual wgm_feedbacks::enum_sub_sys_feedback connect() = 0;
    wgm_feedbacks::enum_sub_sys_feedback sendCmd(std::string& cmd, sockpp::tcp_connector* client, std::string args = std::string());
    virtual bool getStatus() = 0;
    virtual wgm_feedbacks::enum_sub_sys_feedback disconnect() = 0;
    virtual std::string waitForResponse() = 0;
    virtual wgm_feedbacks::enum_sub_sys_feedback activate() = 0;
    virtual wgm_feedbacks::enum_sub_sys_feedback deactivate() = 0;
    virtual wgm_feedbacks::enum_sub_sys_feedback vibrate() = 0;
    virtual wgm_feedbacks::enum_sub_sys_feedback setVibrateDuration(u_int durationSecond) = 0;
    virtual wgm_feedbacks::enum_sub_sys_feedback setVibrateFreq(u_int new_freq) = 0;
    virtual std::string get_help() = 0;
    virtual double getFrequency() = 0;
    virtual double getDuration() = 0;
    virtual std::string sendDirectCmd(std::string cmd)=0;
};

