/**
 * @file cnt_linear_motion.h
 * @author sami dhiab sami@theion.de
 * @brief this class is usefull if the cnt_linear_motion is interfaced seperatly
 * @version 0.1
 * @date 2022-10-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

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
#include <queue>
#include <memory>

#include "Icnt_axis_motion.h"

struct cnt_axis_motion_server
{
    const char* ip = "192.168.0.203";
    uint16_t port = 8882;
    double cnt_max_travel = 300; 
    double cnt_max_speed = 800; 
};

class cnt_linear_motion: public Icnt_axis_motion
{
private:
    cnt_axis_motion_server _motion_axis_struct;
    sockpp::socket_initializer sockInit;
    std::map<u_int, std::string> motion_cmds = {
        {0,"unlock"}, {1,"move_up"}, {2,"move_down"}
        
    };
    std::string motion_incoming_data;
    const u_int motion_data_length = 5012;
    bool axisReady = false;
    int max_attempts = 10;
private:
    std::map<std::string, std::string> axis_cmds = {
        {"unlock","$X"}, {"get_position","?"}, {"move","X"},
        {"get_setting","$$"}, {"set_speed","$110="}, {"home","$H"},
        {"pause","!"}, {"resume","~"}
    };
    std::deque<double> axis_last_position; // FIFO last 10 values
    std::string axis_incoming_data;
    u_int axis_data_length = 5012;
public:
    sockpp::tcp_connector* axis_client_sock=nullptr;
    cnt_linear_motion(/* args */);
    virtual ~cnt_linear_motion();
    virtual wgm_feedbacks::enum_sub_sys_feedback move_home() override;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_to(int new_position) override;
    virtual wgm_feedbacks::enum_sub_sys_feedback connect()override;
    virtual wgm_feedbacks::enum_sub_sys_feedback disconnect() override;
    double get_position() override;
    double get_speed() override;
    wgm_feedbacks::enum_sub_sys_feedback set_speed(double_t new_val) override;
    wgm_feedbacks::enum_sub_sys_feedback move_up_to(double_t new_pos) override;
    wgm_feedbacks::enum_sub_sys_feedback move_down_to(double_t new_pos) override;
    wgm_feedbacks::enum_sub_sys_feedback move_up_by(double_t steps) override;
    wgm_feedbacks::enum_sub_sys_feedback move_down_by(double_t steps) override;
    virtual bool getStatus() override;
    virtual std::string sendDirectCmd(std::string cmd) override;
    std::string waitForResponse();
    wgm_feedbacks::enum_sub_sys_feedback move_center() override;
    wgm_feedbacks::enum_sub_sys_feedback unlock() override;
};
