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

class Icnt_dispenser
{
private:
    /* data */
public:
    virtual  ~Icnt_dispenser(){};
    virtual wgm_feedbacks::enum_sub_sys_feedback connect() = 0;
    void sendCmd(std::string& cmd, sockpp::tcp_connector* client, std::string args = std::string());
    virtual bool getStatus() =0;
    virtual void disconnect() =0;
    virtual void waitForResponse()=0;
    virtual void activate() =0;
    virtual void deactivate() =0;
    virtual void vibrate()=0;
    virtual void setVibrateDuration(u_int durationSecond)=0;
    virtual double getFrequency()=0;

};

