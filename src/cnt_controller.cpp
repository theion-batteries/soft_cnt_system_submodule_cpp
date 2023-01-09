#include "cnt_controller.h"
#include <string.h>


cnt_controller::cnt_controller(/* args */)
{
    std::cout << "creating subsystem cnt aligning controller " << std::endl;
#ifdef CNT_CONFIG
    std::cout << "loading config file: "<< CNT_CONFIG<< std::endl;
    std::ifstream filein(CNT_CONFIG);
    for (std::string line; std::getline(filein, line); )
    {
        std::cout << line << std::endl;
    }
    config = YAML::LoadFile(CNT_CONFIG);
    _cnt_params.distance_to_center = config["distance_to_center"].as<double>();
    _cnt_params.dispenser_frequency = config["dispenser_frequency"].as<double>();
    _cnt_params.dispenser_duration = config["dispenser_duration"].as<double>();

#endif 
#ifdef CNT_DISPENSER_MOCK
    dispenser = std::make_shared< cntDispenserMock>();
#else
    dispenser = std::make_shared< cnt_dispenser_vibration>();
#endif
#ifdef CNT_AXIS_MOCK
    motion = std::make_shared< cntAxisMock>();
#else
    motion = std::make_shared< cnt_linear_motion>();
#endif
#ifdef CNT_HV_MOCK
    hv_Dev = std::make_shared< cntHvMock>();
#else
    hv_Dev = std::make_shared< cnt_high_voltage_gbs>();
#endif
}

cnt_controller::~cnt_controller()
{
}

// controller

void cnt_controller::cnt_controller_connect()
{
    motion->connect();
    dispenser->connect();
    hv_Dev->connect();
}


// dispenser
void cnt_controller::cnt_dispenser_connect()
{
    dispenser->connect();
}
void cnt_controller::cnt_dispenser_activate()
{
    dispenser->activate();
}

void cnt_controller::cnt_dispenser_deactivate()

{
    dispenser->deactivate();
}
void cnt_controller::cnt_dispenser_vibrate()

{
    dispenser->vibrate();
}
void cnt_controller::cnt_dispenser_setVibrateDuration(u_int durationSecond)

{
    dispenser->setVibrateDuration(durationSecond);
}


// cnt motion methods
void cnt_controller::cnt_motion_connect()
{
    motion->connect();

}
void cnt_controller::cnt_motion_move_home()
{
    motion->move_home();

}
void cnt_controller::cnt_motion_move_to_center()
{

    motion->move_center();

}

void cnt_controller::cnt_motion_unlock()
{

    motion->unlock();

}

// hv

void cnt_controller::cnt_hv_connect()
{

    hv_Dev->connect();

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
bool cnt_controller::get_hv_status()
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
Icnt_axis_motion* cnt_controller::get_axis_ptr()
{
    return dynamic_cast<Icnt_axis_motion*>(motion.get());

}
Icnt_dispenser* cnt_controller::get_dispenser_ptr()
{
    return dynamic_cast<Icnt_dispenser*>(dispenser.get());

}
Icnt_high_voltage* cnt_controller::get_hv_ptr()
{
    return dynamic_cast<Icnt_high_voltage*>(hv_Dev.get());

}
// direct call
void cnt_controller::sendDirectCmd(std::string& cmd)
{

}
std::string cnt_controller::sendDirectCmdAxis(std::string& cmd)
{
return motion->sendDirectCmd(cmd);
}
void cnt_controller::reload_config_file()
{

    std::cout << "reloading config file: "<< CNT_CONFIG<< std::endl;
    std::ifstream filein(CNT_CONFIG);
    for (std::string line; std::getline(filein, line); )
    {
        std::cout << line << std::endl;
    }
    config = YAML::LoadFile(CNT_CONFIG);
    _cnt_params.distance_to_center = config["distance_to_center"].as<double>();
    _cnt_params.dispenser_frequency = config["dispenser_frequency"].as<double>();
    _cnt_params.dispenser_duration = config["dispenser_duration"].as<double>();

}