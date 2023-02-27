#include "cnt_controller.h"
#include <string.h>

cnt_controller::cnt_controller(/* args */)
{
    std::cout << "creating subsystem cnt aligning controller " << std::endl;
#ifdef CNT_CONFIG
    std::cout << "loading config file: " << CNT_CONFIG << std::endl;
    std::ifstream filein(CNT_CONFIG);
    for (std::string line; std::getline(filein, line);)
    {
        std::cout << line << std::endl;
    }
    config = YAML::LoadFile(CNT_CONFIG);
    _cnt_params.distance_to_center = config["distance_to_center"].as<double>();
    _cnt_params.dispenser_frequency = config["dispenser_frequency"].as<double>();
    _cnt_params.dispenser_duration = config["dispenser_duration"].as<double>();
    _cnt_params.cnt_max_travel = config["cnt_max_travel"].as<double>();
    _cnt_params.cnt_max_speed = config["cnt_max_speed"].as<double>();
    _cnt_params.cnt_dispenser_server_ip = config["cnt_dispenser_server_ip"].as<std::string>();
    _cnt_params.cnt_dispenser_server_port = config["cnt_dispenser_server_port"].as<uint16_t>();
    _cnt_params.cnt_hv_server_ip = config["cnt_hv_server_ip"].as<std::string>();
    _cnt_params.cnt_hv_server_port = config["cnt_hv_server_port"].as<uint16_t>();
    _cnt_params.cnt_motion_server_ip = config["cnt_motion_server_ip"].as<std::string>();
    _cnt_params.cnt_motion_server_port = config["cnt_motion_server_port"].as<uint16_t>();
    _cnt_params.timeout = config["timeout"].as<uint16_t>();
#endif
#ifdef CNT_DISPENSER_MOCK
    dispenser = std::make_shared<cntDispenserMock>();
#else
    dispenser = std::make_shared<cnt_dispenser_vibration>(_cnt_params.cnt_dispenser_server_ip, _cnt_params.cnt_dispenser_server_port, _cnt_params.timeout);
#endif
#ifdef CNT_AXIS_MOCK
    motion = std::make_shared<cntAxisMock>();
#else
    motion = std::make_shared<cnt_linear_motion>(_cnt_params.cnt_motion_server_ip, _cnt_params.cnt_motion_server_port, _cnt_params.timeout);
#endif
#ifdef CNT_HV_MOCK
    hv_Dev = std::make_shared<cntHvMock>();
#else
    hv_Dev = std::make_shared<cnt_hvac_gbs>(_cnt_params.cnt_hv_server_ip, _cnt_params.cnt_hv_server_port, _cnt_params.timeout);
#endif
}

cnt_controller::cnt_controller(const std::string &ip, const uint16_t motion_port, const uint16_t dispenser_port, const uint16_t hvac_port)
{
    std::cout << "creating subsystem cnt aligning controller " << std::endl;
    std::cout << "loading config file: " << CNT_CONFIG << std::endl;
    std::ifstream filein(CNT_CONFIG);
    for (std::string line; std::getline(filein, line);)
    {
        std::cout << line << std::endl;
    }
    config = YAML::LoadFile(CNT_CONFIG);
    _cnt_params.distance_to_center = config["distance_to_center"].as<double>();
    _cnt_params.dispenser_frequency = config["dispenser_frequency"].as<double>();
    _cnt_params.dispenser_duration = config["dispenser_duration"].as<double>();
    _cnt_params.cnt_max_travel = config["cnt_max_travel"].as<double>();
    _cnt_params.cnt_max_speed = config["cnt_max_speed"].as<double>();
    _cnt_params.timeout = config["timeout"].as<uint16_t>();
    _cnt_params.cnt_dispenser_server_ip = ip;
    _cnt_params.cnt_dispenser_server_port = dispenser_port;
    _cnt_params.cnt_hv_server_ip = ip;
    _cnt_params.cnt_hv_server_port = hvac_port;
    _cnt_params.cnt_motion_server_ip = ip;
    _cnt_params.cnt_motion_server_port = motion_port;
    dispenser = std::make_shared<cnt_dispenser_vibration>(_cnt_params.cnt_dispenser_server_ip, _cnt_params.cnt_dispenser_server_port, _cnt_params.timeout);
    motion = std::make_shared<cnt_linear_motion>(_cnt_params.cnt_motion_server_ip, _cnt_params.cnt_motion_server_port, _cnt_params.timeout);
    hv_Dev = std::make_shared<cnt_hvac_gbs>(_cnt_params.cnt_hv_server_ip, _cnt_params.cnt_hv_server_port, _cnt_params.timeout);
}

cnt_controller::~cnt_controller()
{
}

// controller

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_controller_connect()
{
    if (motion->connect() == sub_error || dispenser->connect() == sub_error /*|| hv_Dev->connect() == sub_error*/)
        return sub_error;
    cntReady = true;
    return sub_success;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_controller_disconnect()
{

    if (motion->disconnect() == sub_error || dispenser->disconnect() == sub_error /* || hv_Dev->disconnect() == sub_error*/)
    {
        cntReady = false;
        return sub_error;
    }
    return sub_success;
}

// dispenser
wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_dispenser_connect()
{
    return dispenser->connect();
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_dispenser_activate()
{
    return dispenser->activate();
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_dispenser_deactivate()
{
    return dispenser->deactivate();
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_dispenser_vibrate()
{
    return dispenser->vibrate();
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_dispenser_setVibrateDuration(const u_int durationSecond)
{
    return dispenser->setVibrateDuration(durationSecond);
}

std::string cnt_controller::cnt_dispenser_help()
{
    return dispenser->get_help();
}

double cnt_controller::get_dispenser_duration()
{
    return dispenser->getDuration();
}

enum_sub_sys_feedback cnt_controller::set_dispenser_frequency(const u_int freq)
{
    return dispenser->setVibrateFreq(freq);
}

// cnt motion methods
wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_motion_connect()
{
    return motion->connect();
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_motion_move_home()
{
    return motion->move_home();
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_motion_move_to_center(const double_t new_pos)
{
    return motion->move_down_to(new_pos);
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_motion_move_to(const double_t new_pos)
{
    return motion->move_to(new_pos);
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_motion_move_target_position()
{
    return motion->move_down_to(_cnt_params.distance_to_center);
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_motion_unlock()
{
    return motion->unlock();
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_motion_pause()
{
    return motion->pause();
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_motion_resume()
{
    return motion->resume();
}

// hv

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::cnt_hvac_connect()
{
    return hv_Dev->connect();
}
wgm_feedbacks::enum_sub_sys_feedback cnt_controller::hvac_start()
{
    return hv_Dev->start();
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::hvac_stop()
{
    return hv_Dev->stop();
}

double cnt_controller::hvac_get_output_voltage()
{
    return hv_Dev->get_output_voltage();
}

double cnt_controller::hvac_get_output_frequency()
{
    return hv_Dev->get_output_voltage();
}

double cnt_controller::hvac_get_output_current()
{
    return hv_Dev->get_output_current();
}

enum_sub_sys_feedback cnt_controller::hvac_set_output_voltage(const double voltage)
{
    return hv_Dev->set_output_voltage(voltage);
}

double cnt_controller::hvac_get_output_resistivity()
{
    return hv_Dev->get_output_resistivity();
}

enum_sub_sys_feedback cnt_controller::hvac_set_output_frequency(const double frequency)
{
    return hv_Dev->set_output_frequency(frequency);
}

/********* helper functions */
bool cnt_controller::get_motion_status()
{
    return motion->getStatus();
}
bool cnt_controller::get_dispenser_status()
{
    return dispenser->getStatus();
}
bool cnt_controller::get_hvac_status()
{
    return hv_Dev->getStatus();
}
bool cnt_controller::get_cnt_controller_status()
{
    return cntReady;
}
/*     helper getter */
double cnt_controller::get_dispenser_frequency()
{
    return dispenser->getFrequency();
}

double cnt_controller::get_axis_position()
{
    return motion->get_position();
}

Icnt_axis_motion *cnt_controller::get_axis_ptr()
{
    return dynamic_cast<Icnt_axis_motion *>(motion.get());
}
Icnt_dispenser *cnt_controller::get_dispenser_ptr()
{
    return dynamic_cast<Icnt_dispenser *>(dispenser.get());
}
Icnt_high_voltage *cnt_controller::get_hv_ptr()
{
    return dynamic_cast<Icnt_high_voltage *>(hv_Dev.get());
}

double cnt_controller::get_center_target_distance()
{
    return _cnt_params.distance_to_center;
}

// direct call
void cnt_controller::sendDirectCmd(std::string &cmd)
{
}

std::string cnt_controller::sendDirectCmdAxis(std::string &cmd)
{
    return motion->sendDirectCmd(cmd);
}

std::string cnt_controller::sendDirectCmdDispenser(std::string &cmd)
{
    return dispenser->sendDirectCmd(cmd);
}

std::string cnt_controller::sendDirectCmdHvac(std::string &cmd)
{
    return hv_Dev->sendDirectCmd(cmd);
}

std::string cnt_controller::cnt_motion_get_settings()
{
    return motion->get_settings();
}

double cnt_controller::cnt_motion_get_speed()
{
    return motion->get_speed();
}

void cnt_controller::reload_config_file()
{
    std::cout << "reloading config file: " << CNT_CONFIG << std::endl;
    std::ifstream filein(CNT_CONFIG);
    for (std::string line; std::getline(filein, line);)
    {
        std::cout << line << std::endl;
    }
    config = YAML::LoadFile(CNT_CONFIG);
    _cnt_params.distance_to_center = config["distance_to_center"].as<double>();
    _cnt_params.dispenser_frequency = config["dispenser_frequency"].as<double>();
    _cnt_params.dispenser_duration = config["dispenser_duration"].as<double>();
    _cnt_params.cnt_max_travel = config["cnt_max_travel"].as<double>();
    _cnt_params.cnt_max_speed = config["cnt_max_speed"].as<double>();
    _cnt_params.timeout = config["timeout"].as<uint16_t>();
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::open_config_file()
{
    std::string file = CNT_CONFIG;
    std::cout << "opening config file in notepad \n";
    std::string command = "notepad.exe " + file;
    auto val = system(command.c_str());
    if (val == 0)
        return sub_success;
    return sub_error;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_controller::reset_config_file() // set config file params to default
{

    std::cout << "resetting config file: " << CNT_CONFIG << std::endl;
    config = YAML::LoadFile(CNT_CONFIG);

    config["distance_to_center"] = _cnt_params.distance_to_center;
    config["dispenser_frequency"] = _cnt_params.dispenser_frequency;
    config["dispenser_duration"] = _cnt_params.dispenser_duration;
    config["cnt_max_travel"] = _cnt_params.cnt_max_travel;
    config["cnt_max_speed"] = _cnt_params.cnt_max_speed;
    config["cnt_dispenser_server_ip"] = _cnt_params.cnt_dispenser_server_ip;
    config["cnt_dispenser_server_port"] = _cnt_params.cnt_dispenser_server_port;
    config["cnt_hv_server_ip"] = _cnt_params.cnt_hv_server_ip;
    config["cnt_hv_server_port"] = _cnt_params.cnt_hv_server_port;
    config["cnt_motion_server_ip"] = _cnt_params.cnt_motion_server_ip;
    config["cnt_motion_server_port"] = _cnt_params.cnt_motion_server_port;
    config["timeout"] = _cnt_params.timeout;
    std::ofstream fout(CNT_CONFIG);
    fout << config;
    fout.close();
    std::ifstream filein(CNT_CONFIG);
    for (std::string line; std::getline(filein, line);)
    {
        std::cout << line << std::endl;
    }
    return sub_success;
}
