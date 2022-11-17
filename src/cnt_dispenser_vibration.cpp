#include "cnt_dispenser_vibration.h"


cnt_dispenser_vibration::cnt_dispenser_vibration(/* args */)
{
}

cnt_dispenser_vibration::~cnt_dispenser_vibration()
{
}



wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::connect()
{
    std::cout << "connecting to dispenser server" << std::endl;
    _dispenser_client = new sockpp::tcp_connector({ _dispenser_server.ip, _dispenser_server.port });
    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (!_dispenser_client) {
        std::cerr << "Error connecting to server at "
            << sockpp::inet_address(_dispenser_server.ip, _dispenser_server.port)
            << "\n\t" << _dispenser_client->last_error_str() << std::endl;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    std::cout << "Created a connection from " << _dispenser_client->address() << std::endl;
    std::cout << "Created a connection to " << _dispenser_client->peer_address() << std::endl;
    // Set a timeout for the responses
    if (!_dispenser_client->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            << _dispenser_client->last_error_str() << std::endl;
                return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
            return wgm_feedbacks::enum_sub_sys_feedback::sub_success;

}

void cnt_dispenser_vibration::disconnect()
{
    if (!_dispenser_client) _dispenser_client->close();
}

void cnt_dispenser_vibration::waitForResponse()
{
    std::cout << "awaiting server response" << std::endl;

    while (_dispenser_client->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = _dispenser_client->read(&dispenser_incoming_data[0], dispenser_data_length);
        if (n > 0)
        {
            std::cout << "server replied : " << dispenser_incoming_data.c_str() << std::endl;
            break;
        }
    }
}

void cnt_dispenser_vibration::activate()
{
    auto command = dispenser_cmds.find(1);
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendCmd(command->second, _dispenser_client);
    }

}
void cnt_dispenser_vibration::deactivate()
{
    auto command = dispenser_cmds.find(2);
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendCmd(command->second, _dispenser_client);
    }
}
void cnt_dispenser_vibration::vibrate()
{
    auto command = dispenser_cmds.find(3);
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendCmd(command->second, _dispenser_client);
    }
}
void cnt_dispenser_vibration::setVibrateDuration(u_int durationSecond)
{
    auto command = dispenser_cmds.find(4);
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << durationSecond << '\n';
        std::string args = "=" + std::to_string(durationSecond);
        sendCmd(command->second, _dispenser_client, args);
    }
}


bool cnt_dispenser_vibration::getStatus()
{
return dispenserReady;
}
double cnt_dispenser_vibration::getFrequency()
{
    return frequency;
}

void cnt_dispenser_vibration::sendCmd(std::string& cmd, sockpp::tcp_connector* client, std::string args)
{
    if (client->write(cmd + args) != ssize_t(std::string(cmd + args).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << client->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd + args << " sent" << std::endl;
}