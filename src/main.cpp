#include "cnt_controller.h"



enum options {
	CLOSE = 0,
	OPEN_CONFIG = 1,
	RESET_CONFIG = 2,
	CONNECT = 3,
	SEND_AXIS_CMD = 4,
	SEND_DISPENSER_CMD = 5,
	SEND_HVAC_CMD = 6,
	CHANGE_IP = 7,
	RELOAD_CONFIG = 8
};

void processAxisCmd(cnt_controller &controller, options option){
	std::string cmd;
 while (true) {
	std::cout<<"Type E or e to exit \n";
	std::cout << "Enter command: ";
    std::cin>>cmd;
   if(cmd=="E" || cmd == "e") break;  
   if(option==SEND_AXIS_CMD)  
    std::cout << controller.sendDirectCmdAxis(cmd) <<std::endl;
   if(option==SEND_DISPENSER_CMD)  
    std::cout << controller.sendDirectCmdDispenser(cmd) <<std::endl;
 }
}

int main() {
	cnt_controller controller;
	options choices = CLOSE;
	int choice = -1;
	std::string cmd = "";
	std::string ip = "";

	while (choice != CLOSE) {
		std::cout << "Please choose an option: \n";
		std::cout << "0: CLOSE\n";
		std::cout << "1: OPEN_CONFIG\n";
		std::cout << "2: RESET_CONFIG\n";
		std::cout << "3: CONNECT\n";
		std::cout << "4: SEND AXIS CMD\n";
		std::cout << "5: SEND DISPENSER CMD\n";
		std::cout << "6: SEND HVAC CMD\n";
		std::cout << "7: CHANGE_IP\n";
		std::cout << "8: RELOAD_CONFIG\n";
		std::cout<<" Enter Choice: ";
		std::cin >> choice;
		switch (choice) { 
		case CLOSE:
			controller.cnt_controller_disconnect();
			break;
		case OPEN_CONFIG:
			controller.open_config_file();
			break;
		case RESET_CONFIG:
			controller.reset_config_file();
			break;
		case CONNECT:
			controller.cnt_controller_connect();
			break;
		case SEND_AXIS_CMD:
		    if (controller.get_motion_status()){ 
				processAxisCmd(controller,SEND_AXIS_CMD);
			} else std::cout<<"No connection to CNT Motion Axis \n";
			break;

		case SEND_DISPENSER_CMD:	
		    if (controller.get_dispenser_status()) {
					processAxisCmd(controller,SEND_DISPENSER_CMD);
			} 
			else std::cout<<"No connection to CNT Dispenser \n";
			break;	

		case SEND_HVAC_CMD:
		    if (controller.get_hvac_status()) {
			std::cout << "Enter command: ";
			std::cin >> cmd;
			std::cout << controller.sendDirectCmdHvac(cmd) <<std::endl;
			} else std::cout<<"No connection to CNT High Voltage AC \n";
			break;

		case CHANGE_IP:
			std::cout << "Enter IP: ";
			std::cin >> ip;
			std::cout << "New IP: " << ip << std::endl;
			break;

		case RELOAD_CONFIG:
			controller.reload_config_file();
			break;

		default:
			std::cout << "Invalid option. Please choose again.\n";
			break;
		}
	}

	return 0;

}