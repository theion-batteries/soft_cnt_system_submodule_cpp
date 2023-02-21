#pragma once

#include <sockpp/socket.h>
#include <iostream>
#include <string>
#include <sockpp/tcp_connector.h>
#include "Windows.h" 
#include <shellapi.h> 
#include <map>
#include <thread>
#include <atomic>
#include "Icnt_dispenser.h"

struct cnt_dispenser_vibration_server
{
    std::string ip = "192.168.0.203";
    uint16_t port = 8881;
};


class cnt_dispenser_vibration : public Icnt_dispenser
{
private:
    cnt_dispenser_vibration_server _dispenser_server;
    sockpp::socket_initializer sockInit;

    std::map<std::string, std::string> dispenser_cmds = {
         {"HELP","help"}, {"ON","on"}, {"OFF","off"},
        {"VIBRATE","vibrate"}, {"GETDUR","dur?"},
        {"SETDUR","dur"}, {"GETFREQ","freq?"}, {"SETFREQ","freq"}
    };
    std::string dispenser_incoming_data;
    u_int dispenser_data_length = 5012;
    bool dispenserReady = false;
    double frequency = 0;
    const int max_attempts = 10;

public:
    std::unique_ptr<sockpp::tcp_connector> _dispenser_client;
    cnt_dispenser_vibration(const std::string &ip, const uint16_t port);
    virtual ~cnt_dispenser_vibration();
    wgm_feedbacks::enum_sub_sys_feedback connect() override;
    bool getStatus() override;
    wgm_feedbacks::enum_sub_sys_feedback disconnect() override;
    wgm_feedbacks::enum_sub_sys_feedback activate() override;
    wgm_feedbacks::enum_sub_sys_feedback deactivate() override;
    wgm_feedbacks::enum_sub_sys_feedback vibrate() override;
    wgm_feedbacks::enum_sub_sys_feedback setVibrateDuration(u_int durationSecond) override;
    std::string waitForResponse() override;
    double getFrequency() override;
    wgm_feedbacks::enum_sub_sys_feedback setVibrateFreq(u_int new_freq) override;
    double getDuration() override;
    std::string sendDirectCmd(std::string cmd) override;
    
};
