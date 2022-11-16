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

class Icnt_high_voltage
{
public:
    Icnt_high_voltage(/* args */);
    ~Icnt_high_voltage();

    virtual void disconnect()=0;
    virtual void connect()=0;
    virtual void sendCmd(std::string cmd, sockpp::tcp_connector* client, std::string args = std::string());
    virtual void waitForResponse()=0;
    virtual void start()=0;
    virtual void stop()=0;
    virtual void pulse() = 0;
    virtual void modulate() = 0;    
    virtual void tune_param(std::pair<std::string, int> new_param_val)=0; // tune param
    virtual bool getStatus() =0;
    virtual double get_input_current()=0;
    virtual double get_input_frequency()=0;
    virtual double get_input_voltage()=0;
    virtual double get_output_voltage()=0;
    virtual double get_output_frequency()=0;
    virtual double get_output_current()=0;


};
