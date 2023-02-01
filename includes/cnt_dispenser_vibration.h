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
    const char* ip = "192.168.0.203";
    uint16_t port = 8881;
};


class cnt_dispenser_vibration : public Icnt_dispenser
{
private:
    cnt_dispenser_vibration_server _dispenser_server;
    sockpp::socket_initializer sockInit;

    std::map<u_int, std::string> dispenser_cmds = {
         {0,"help"}, {1,"on"}, {2,"off"},
        {3,"vibrate"}, {4,"dur?"},
        {5,"dur"}, {6,"freq?"}, {7,"freq"}
    };
    std::string dispenser_incoming_data;
    u_int dispenser_data_length = 1024;
    bool dispenserReady = false;
    double frequency = 0;

public:
    sockpp::tcp_connector* _dispenser_client=nullptr;
    cnt_dispenser_vibration(/* args */);
    virtual ~cnt_dispenser_vibration();
    wgm_feedbacks::enum_sub_sys_feedback connect() override;
    bool getStatus() override;
    void disconnect() override;
    void activate() override;
    void deactivate() override;
    void vibrate() override;
    void setVibrateDuration(u_int durationSecond) override;
    std::string waitForResponse() override;
    double getFrequency() override;
    void sendCmd(std::string& cmd, sockpp::tcp_connector* client, std::string args  = std::string());
    void setVibrateFreq(u_int new_freq) override;
    double getDuration() override;
     std::string sendDirectCmd(std::string cmd) override;
    
};
