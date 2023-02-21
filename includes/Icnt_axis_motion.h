/**
 * @file Icnt_axis_motion.h
 * @author sami dhiab
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

class Icnt_axis_motion
{
private:
    /* data */
public:
    Icnt_axis_motion();
    virtual ~Icnt_axis_motion();
    virtual wgm_feedbacks::enum_sub_sys_feedback connect() = 0;
    virtual wgm_feedbacks::enum_sub_sys_feedback disconnect() = 0;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_home() = 0;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_to(const double_t new_position) = 0;
    virtual std::string sendDirectCmd(std::string cmd)=0;
    virtual double get_position() = 0;
    virtual bool getStatus() =0;
    virtual double get_speed() =0;
    virtual wgm_feedbacks::enum_sub_sys_feedback set_speed(const double_t new_val) =0;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_up_to(const double_t new_pos) =0;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_down_to(const double_t new_pos) =0;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_up_by(const double_t steps) =0;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_down_by(const double_t steps) =0;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_center() =0;
    virtual wgm_feedbacks::enum_sub_sys_feedback unlock()=0;
};



