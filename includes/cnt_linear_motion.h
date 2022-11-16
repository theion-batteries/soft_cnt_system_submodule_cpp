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

#include "Icnt_axis_motion.h"

struct cnt_axis_motion_server
{
    const char* ip = "192.168.0.100";
    uint16_t port = 80;
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
    u_int motion_data_length = 1024;
    bool axisReady = false;
protected:
    std::map<u_int, std::string> axis_cmds = {
        {0,"$x"}, {1,"?"}, {2,"x160"},
        {3,"x"}, {4,"x"},
        {5,"x"}, {6,"x"},
        {7,"$h"}
    };
    std::deque<double> axis_last_position; // FIFO last 10 values
    std::string axis_incoming_data;
    u_int axis_data_length = 1024;
public:
    sockpp::tcp_connector* axis_client_sock;
    cnt_linear_motion(/* args */);
    ~cnt_linear_motion();
    virtual void move_home() override;
    virtual void move_to(int new_position) override;
    virtual wgm_feedbacks::enum_sub_sys_feedback connect()override;
    virtual void disconnect() override;
    double get_position() override;
    void get_speed() override;
    void set_speed(double_t new_val) override;
    void move_up_to(double_t new_pos) override;
    void move_down_to(double_t new_pos) override;
    void move_up_by(double_t steps) override;
    void move_down_by(double_t steps) override;
    virtual bool getStatus() override;
    virtual std::string sendDirectCmd(std::string& cmd) override;
    void waitForResponse();
    void move_center() override;
    void unlock() override;
};
