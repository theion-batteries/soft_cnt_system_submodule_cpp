#include "cnt_controller.h"
#include "yaml-cpp/yaml.h"

cnt_controller controller;
int main()
{

    YAML::Node config= YAML::LoadFile("C:/Users/SamiDhiab/Theion_Repos/soft_wgm_user_interface_qt_cpp/config/config.yaml");
	std::cout << "config file loaded, printing parameters: " << std::endl;
	std::cout << "mm_steps: " << config["mm_steps"].as<std::string>() << std::endl;
	std::cout << "delay_to_move_request: " << config["delay_to_move_request"].as<std::string>() << std::endl;
	std::cout << "ref_dis: " << config["ref_dis"].as<std::string>() << std::endl;
	std::cout << "thickness: " << config["thickness"].as<std::string>() << std::endl;
	std::cout << "mm_step_res: " << config["mm_step_res"].as<std::string>() << std::endl;
	std::cout << "python path: " << config["python_path"].as<std::string>() << std::endl;
	std::cout << "python script: " << config["python_script"].as<std::string>() << std::endl;

    controller.cnt_controller_connect();

    Sleep(1000); // debug sleep
    controller.cnt_motion_unlock();

    Sleep(2000); // debug sleep
    controller.cnt_motion_move_home();

    Sleep(5000); // debug sleep
    controller.cnt_motion_move_to_center();

    Sleep(5000); // debug sleep
    controller.cnt_dispenser_activate();

    Sleep(1000); // debug sleep
    controller.cnt_dispenser_vibrate();

    Sleep(2000); // debug sleep
    controller.cnt_dispenser_deactivate();

    Sleep(1000); // debug sleep
    controller.cnt_motion_move_home();
    Sleep(3000);
    
    return 0;
}