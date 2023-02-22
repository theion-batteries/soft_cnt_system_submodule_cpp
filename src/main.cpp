#include "cnt_controller.h"



enum options {
	CLOSE = 0,
	OPEN_CONFIG = 1,
	RESET_CONFIG = 2,
	CONNECT = 3,
	SEND_CMD = 4,
	CHANGE_IP = 5,
};


int main()

{
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
		std::cout << "4: SEND_CMD\n";
		std::cout << "5: CHANGE_IP\n";
		std::cin >> choice;

		switch (choice) {
		case CLOSE:
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
		case SEND_CMD:
			std::cout << "Enter command: ";
			std::cin >> cmd;
			std::cout << controller.sendDirectCmdAxis(cmd) <<std::endl;
			break;
		case CHANGE_IP:
			std::cout << "Enter IP: ";
			std::cin >> ip;
			std::cout << "New IP: " << ip << std::endl;
			break;
		default:
			std::cout << "Invalid option. Please choose again.\n";
			break;
		}
	}

	return 0;

}