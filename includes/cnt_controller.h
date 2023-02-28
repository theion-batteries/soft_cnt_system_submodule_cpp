#pragma once
#include "cnt_dispenser_vibration.h"
#include "cnt_linear_motion.h"
#include "cnt_hvac_gbs.h"
#include <fstream>
#include "yaml-cpp/yaml.h"
using enum wgm_feedbacks::enum_sub_sys_feedback;
struct cnt_config_yaml_params
{
    double distance_to_center = 200;
    double dispenser_frequency = 80;
    double dispenser_duration = 2;
    double cnt_max_travel = 300;
    double cnt_max_speed = 800;
    std::string cnt_motion_server_ip = "192.168.0.203";
    uint16_t cnt_motion_server_port = 8881;
    std::string cnt_dispenser_server_ip = "192.168.0.203";
    uint16_t cnt_dispenser_server_port = 8882;
    std::string cnt_hv_server_ip = "192.168.0.203";
    uint16_t cnt_hv_server_port = 8882;    
    uint16_t timeout = 10;
};
class cnt_controller
{
private:
    std::shared_ptr <Icnt_dispenser> dispenser;
    std::shared_ptr <Icnt_axis_motion> motion;
    std::shared_ptr <Icnt_high_voltage> hv_Dev;
    bool cntReady = false;
    YAML::Node config;
    cnt_config_yaml_params _cnt_params;
    cnt_config_yaml_params _cnt_params_default;

public:
    cnt_controller(/* args */);
    cnt_controller(const std::string &ip, const uint16_t motion_port, const uint16_t dispenser_port, const uint16_t hvac_port);
    ~cnt_controller();
    // controller
    wgm_feedbacks::enum_sub_sys_feedback open_config_file();
    wgm_feedbacks::enum_sub_sys_feedback reset_config_file();

    wgm_feedbacks::enum_sub_sys_feedback cnt_controller_connect();
    wgm_feedbacks::enum_sub_sys_feedback cnt_controller_disconnect();

    // motion
    wgm_feedbacks::enum_sub_sys_feedback cnt_motion_connect();
    wgm_feedbacks::enum_sub_sys_feedback cnt_motion_move_home();
    wgm_feedbacks::enum_sub_sys_feedback cnt_motion_move_to_center(const double_t new_pos);
    wgm_feedbacks::enum_sub_sys_feedback cnt_motion_move_target_position();
    wgm_feedbacks::enum_sub_sys_feedback cnt_motion_unlock();
    wgm_feedbacks::enum_sub_sys_feedback cnt_motion_pause();
    wgm_feedbacks::enum_sub_sys_feedback cnt_motion_resume();
    wgm_feedbacks::enum_sub_sys_feedback cnt_motion_move_to(const double_t new_pos);
    std::string cnt_motion_get_settings();
    double cnt_motion_get_speed();
    
    // dispenser
    wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_connect();
    wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_activate();
    wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_deactivate();
    wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibrate();
    wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_setVibrateDuration(const u_int durationSecond);
   
    // hv
    wgm_feedbacks::enum_sub_sys_feedback cnt_hvac_connect();
    wgm_feedbacks::enum_sub_sys_feedback hvac_start();
    wgm_feedbacks::enum_sub_sys_feedback hvac_stop();

     double hvac_get_output_voltage();
     double hvac_get_output_frequency();
     double hvac_get_output_current();
      

     enum_sub_sys_feedback hvac_set_output_voltage(const double ) ;
     double hvac_get_output_resistivity() ;
     enum_sub_sys_feedback hvac_set_output_frequency(const double) ;



    /********* helper functions */
    bool get_motion_status();
    bool get_dispenser_status();
    bool get_hvac_status();
    bool get_cnt_controller_status();
    double get_center_target_distance();
    /*     helper getter */
    std::string cnt_dispenser_help(); 
    double get_dispenser_duration();  
    double get_dispenser_frequency();
    double get_axis_position();

    enum_sub_sys_feedback set_dispenser_frequency(const u_int);
    Icnt_axis_motion* get_axis_ptr();
    Icnt_dispenser* get_dispenser_ptr();
    Icnt_high_voltage* get_hv_ptr();
    // direct call
    //void sendDirectCmd(std::string& cmd);
    std::string sendDirectCmdAxis(std::string& cmd);
    std::string sendDirectCmdDispenser(std::string& cmd);
    std::string sendDirectCmdHvac(std::string& cmd);
    void reload_config_file();
    
};


