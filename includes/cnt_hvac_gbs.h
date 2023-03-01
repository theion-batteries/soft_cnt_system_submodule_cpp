#pragma once

#include "sockpp/socket.h"
#include <iostream>
#include <string>
#include "sockpp/tcp_connector.h"
#include "Windows.h"
#include <shellapi.h>
#include <unordered_map>
#include <thread>
#include <atomic>

#include "Icnt_high_voltage.h"
using wgm_feedbacks::enum_hw_feedback;
using wgm_feedbacks::enum_sub_sys_feedback::sub_error;
using wgm_feedbacks::enum_sub_sys_feedback::sub_success;

struct cnt_hvac_gbs_server
{
    std::string ip = "192.168.0.100";
    uint16_t port = 80;
    uint16_t timeout = 10;
};

struct cnt_hvac_gbs_paramteres
{
    std::map<std::string, int> parameters = {{"current", 0}, {"voltage", 0}, {"frequency", 0}, {"amplitude", 0}};
};

class cnt_hvac_gbs : public Icnt_high_voltage
{
private:
    cnt_hvac_gbs_server _hv_server;
    sockpp::socket_initializer sockInit;
    std::unordered_map<std::string, std::string> hv_cmds = {
        {"STOP", "stop"}, {"START", "start"}, {"INPUT_CURRENT", "ip_current"}, {"INPUT_VOLT", "ip_voltage"}, {"INPUT_FREQ", "ip_frequency"}, {"INPUT_AMP", "ip_amplitude"}, {"OUTPUT_CURRENT", "op_current"}, {"OUTPUT_VOLT", "op_voltage"}, {"OUTPUT_FREQ", "op_frequency"}, {"OUTPUT_AMP", "op_amplitude"}, {"SET_OUTPUT_VOLT", "set_op_voltage"}, {"SET_OUTPUT_FREQ", "set_op_frequency"}, {"GET_OUTPUT_RESIS", "get_op_resistivity"}};

    std::string incoming_data;
    u_int hv_data_length = 5012;
    bool hvReady = false;
    cnt_hvac_gbs_paramteres params;
    auto process_cmd(std::string, std::string);
    static inline bool blocking = false;

public:
    std::unique_ptr<sockpp::tcp_connector> _client = nullptr;
    cnt_hvac_gbs(const std::string &ip, const uint16_t port, const uint16_t timeout);
    ~cnt_hvac_gbs();

    virtual wgm_feedbacks::enum_sub_sys_feedback disconnect() override;
    virtual wgm_feedbacks::enum_sub_sys_feedback connect() override;
    virtual std::string sendDirectCmd(std::string cmd) override;
    virtual std::string waitForResponse() override;
    virtual enum_sub_sys_feedback start() override;
    virtual enum_sub_sys_feedback stop() override;
    virtual enum_sub_sys_feedback tune_param(std::pair<std::string, int> new_param_val) override; // tune param
    virtual bool getStatus() override;
    virtual enum_sub_sys_feedback pulse() override;
    virtual enum_sub_sys_feedback modulate() override;
    virtual double get_input_current() override;
    virtual double get_input_frequency() override;
    virtual double get_input_voltage() override;
    virtual double get_output_voltage() override;
    virtual double get_output_frequency() override;
    virtual double get_output_current() override;

    virtual enum_sub_sys_feedback set_output_voltage(const double) override;
    virtual double get_output_resistivity() override;
    virtual enum_sub_sys_feedback set_output_frequency(const double) override;
    static void setModeBlocking(bool setblockingMode);

};
