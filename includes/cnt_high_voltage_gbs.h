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

#include "Icnt_high_voltage.h"

struct cnt_high_voltage_gbs_server
{
    std::string ip = "192.168.0.100";
    uint16_t port = 80;
};

struct cnt_high_voltage_gbs_paramteres
{
    std::map<std::string, int> parameters = { {"current", 0}, {"voltage", 0},{"frequency", 0},{"amplitude", 0} };
};
class cnt_high_voltage_gbs : public Icnt_high_voltage
{
private:
    cnt_high_voltage_gbs_server _hv_server;
    sockpp::socket_initializer sockInit;
    std::map<u_int, std::string> hv_cmds = {
        {0,"stop"}, {1,"start"}, {2,"current"},
        {3,"voltage"},{4,"frequency"},{5,"amplitude"}
    };
    std::string hv_incoming_data;
    u_int hv_data_length = 1024;
    bool hvReady = false;
    bool cntReady = false;
    cnt_high_voltage_gbs_paramteres params;

public:
    sockpp::tcp_connector* _hv_client=nullptr;
    cnt_high_voltage_gbs(const std::string &ip, const uint16_t port);
    ~cnt_high_voltage_gbs();

    void disconnect() override;
    wgm_feedbacks::enum_sub_sys_feedback connect() override;
    void waitForResponse() override;
    void start() override;
    void stop() override;
    void tune_param(std::pair<std::string, int> new_param_val) override; // tune param
    bool getStatus() override;
    void pulse() override;
    void modulate() override;
    double get_input_current()override;
    double get_input_frequency()override;
    double get_input_voltage()override;
    double get_output_voltage()override;
    double get_output_frequency()override;
    double get_output_current()override;
};
