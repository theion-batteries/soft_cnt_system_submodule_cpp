#include "cnt_controller.h"
#include <any>
#include <optional>
#include <type_traits>
#include <functional>
enum options
{
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

std::unordered_map<enum options, std::string> subsystem{
	{SEND_AXIS_CMD, "Axis"},
	{SEND_DISPENSER_CMD, "Dispenser"}};

void print_result(const std::string &cmd, const std::any result)
{
	if (std::is_same_v<enum_sub_sys_feedback, decltype(result.type())>)
	{
		switch (std::any_cast<wgm_feedbacks::enum_sub_sys_feedback>(result))
		{
		case sub_success:
			std::cout << cmd << ": success"
					  << "\n";
			break;

		case sub_error:
			std::cout << cmd << ": error"
					  << "\n";
			break;
		}
	}

	else if (std::is_same_v<double, decltype(result.type())>)
		std::cout << cmd << " " << std::any_cast<double>(result) << "\n";

	else if (std::is_same_v<std::string, decltype(result.type())>)
	{
		std::cout << cmd << " " << std::any_cast<std::string>(result) << "\n";
	}
}

std::optional<double> string_to_double(const std::string &cmd, const size_t initial_pos)
{
	double pos = 0.0;
	try
	{
		pos = std::stod(std::string(cmd.begin() + initial_pos, cmd.end()));
	}
	catch (std::exception &e)
	{
		std::cout << "conversion error: " << e.what() << "\n";
		return std::nullopt;
	}
	return pos;
}

void axis_cmd(cnt_controller &controller, const std::string &cmd)
{
	std::any result = "";
	if (cmd == "!")
		result = controller.cnt_motion_pause();
	else if (cmd == "~")
		result = controller.cnt_motion_resume();
	else if (cmd == "?")
		result = controller.get_axis_position();
	else if (cmd == "$H" || "$h")
		result = controller.cnt_motion_move_home();
	else if (cmd == "$X" || '$x')
		result = controller.cnt_motion_unlock();
	else if (cmd == "$$")
		result = controller.cnt_motion_get_settings();
	else if (cmd[0] == 'X' || 'x')
	{
		std::optional<double> pos = string_to_double(cmd, 1);
		std::cout << pos.value() << "\n";
		if (pos.has_value())
		{
			if (pos.value() <= 0)
			{
				result = controller.cnt_motion_move_to(pos.value());
			}
			else
				std::cerr << "The value must be lesser than equal to 0"
						  << "\n";
		}
	}
	else
	{
		std::cerr << " Error: Wrong command"
				  << "\n";
	}
	print_result(cmd, result);
}

void dispenser_cmd(cnt_controller &controller, std::string &cmd)
{
	std::any result = " ";
	std::optional<double> pos = std::nullopt;
	if (cmd == "help")
		result = controller.cnt_dispenser_help();
	else if (cmd == "on")
		result = controller.cnt_dispenser_activate();
	else if (cmd == "off")
		result = controller.cnt_dispenser_deactivate();
	else if (cmd == "dur?")
		result = controller.get_dispenser_duration();
	else if (cmd == "freq?")
		result = controller.get_dispenser_frequency();
	else if (cmd.substr(0, 4) == "dur=")
	{
		 pos = string_to_double(cmd,4);
		 if(pos.has_value()) result = controller.cnt_dispenser_setVibrateDuration((u_int)pos.value());
	}
	else if (cmd.substr(0, 5) == "freq=")
	{
		pos = string_to_double(cmd, 5);
		auto v = (u_int)pos.value();
		if (pos.has_value())
			result = controller.set_dispenser_frequency((u_int)pos.value());
	}
	else
	{
		std::cerr << " Error: Wrong command"
				  << "\n";
	}
	print_result(cmd, result);
}

void processCmd(cnt_controller &controller, options option)
{
	std::string cmd;
	while (true)
	{
		std::cout << "Type E or e to exit \n";
		std::cout << "Enter command for " << subsystem[option] << ": \n";
		std::cin >> cmd;
		if (cmd == "E" || cmd == "e")
			break;
		if (option == SEND_AXIS_CMD)
			axis_cmd(controller, cmd);
		if (option == SEND_DISPENSER_CMD)
			dispenser_cmd(controller, cmd);
	}
}

int main()
{
	cnt_controller controller;
	options choices = CLOSE;
	int choice = -1;
	std::string cmd = "";
	std::string ip = "";

	while (choice != CLOSE)
	{
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
		std::cout << " Enter Choice: ";
		std::cin >> choice;
		switch (choice)
		{
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
			if (controller.get_motion_status())
			{
				processCmd(controller, SEND_AXIS_CMD);
			}
			else
				std::cout << "No connection to CNT Motion Axis \n";
			break;

		case SEND_DISPENSER_CMD:
			if (controller.get_dispenser_status())
			{
				processCmd(controller, SEND_DISPENSER_CMD);
			}
			else
				std::cout << "No connection to CNT Dispenser \n";
			break;

		case SEND_HVAC_CMD:
			if (controller.get_hvac_status())
			{
				std::cout << "Enter command: ";
				std::cin >> cmd;
				std::cout << controller.sendDirectCmdHvac(cmd) << std::endl;
			}
			else
				std::cout << "No connection to CNT High Voltage AC \n";
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