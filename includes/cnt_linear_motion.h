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
    std::string ip = "192.168.0.203";
    uint16_t port = 8882;
    double cnt_max_travel = 300;
    double cnt_max_speed = 800;
    uint16_t timeout = 10;
};

class cnt_linear_motion : public Icnt_axis_motion
{
private:
    cnt_axis_motion_server _motion_axis_struct;
    sockpp::socket_initializer sockInit;
    std::string motion_incoming_data;
    const u_int motion_data_length = 5012;
    bool axisReady = false;
    int max_attempts = 10;

private:
    std::map<std::string, std::string> axis_cmds = {
        {"unlock", "$X"}, {"get_position", "?"}, {"move", "X"}, 
        {"get_setting", "$$"}, {"set_speed", "$110="}, 
        {"home", "$H"}, {"pause", "!"}, {"resume", "~"}};
    std::deque<double> axis_last_position; // FIFO last 10 values
    std::string incoming_data;
    u_int axis_data_length = 5012;
    static inline bool blocking = false;

public:

    std::unique_ptr<sockpp::tcp_connector> _client = nullptr;
    cnt_linear_motion(const std::string &ip, const uint16_t port, const uint16_t timeout);
    virtual ~cnt_linear_motion();
    virtual wgm_feedbacks::enum_sub_sys_feedback move_home() override;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_to(const double_t new_position) override;
    virtual wgm_feedbacks::enum_sub_sys_feedback connect() override;
    virtual wgm_feedbacks::enum_sub_sys_feedback disconnect() override;
    double get_position() override;
    double get_speed() override;

    virtual wgm_feedbacks::enum_sub_sys_feedback pause() override;
    virtual wgm_feedbacks::enum_sub_sys_feedback resume() override;
    virtual std::string get_settings() override;
    virtual wgm_feedbacks::enum_sub_sys_feedback set_speed(const double_t new_val) override;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_up_to(const double_t new_pos) override;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_down_to(const double_t new_pos) override;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_up_by(const double_t steps) override;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_down_by(const double_t steps) override;
    virtual bool getStatus() override;
    virtual std::string sendDirectCmd(std::string cmd) override;
    std::string waitForResponse();
    wgm_feedbacks::enum_sub_sys_feedback move_center() override;
    wgm_feedbacks::enum_sub_sys_feedback unlock() override;
    static void setModeBlocking(bool setblockingMode);
};
