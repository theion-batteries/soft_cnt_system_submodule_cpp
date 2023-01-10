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
    dispenserReady = true;
    return wgm_feedbacks::enum_sub_sys_feedback::sub_success;

}

void cnt_dispenser_vibration::disconnect()
{
    if (!_dispenser_client) _dispenser_client->close();
}
std::string cnt_dispenser_vibration::sendDirectCmd(std::string cmd)
{
    if (_dispenser_client == nullptr) return "not connected";
    std::cout << "sending linear axis command " << cmd << std::endl;
    cmd = cmd + "\r\n";
    if (_dispenser_client->write(cmd) != ssize_t(std::string(cmd).length())) {
        std::cout << "Error writing to the TCP stream: "
            << _dispenser_client->last_error_str() << std::endl;
    }
    return waitForResponse();
}
std::string cnt_dispenser_vibration::waitForResponse()
{
    static int attempts = 0;
    if (attempts == 10)
    {
        std::cout << "attempts: " << attempts << std::endl;
        attempts = 0;
        return "NA";
    }
    std::cout << "awaiting server response" << std::endl;
    if (_dispenser_client->is_connected())
    {
        char Strholder[1024];
        ssize_t n = _dispenser_client->read_n(&Strholder, 1024);
        if (n > 0)
        {
            std::cout << "n bytes received: " << n << std::endl;
            dispenser_incoming_data = Strholder;
            dispenser_incoming_data.resize(n);
            std::cout << "server replied : " << dispenser_incoming_data << std::endl;
            return dispenser_incoming_data;
        }
        else
        {
            std::cout << "no server response " << n << std::endl;
            attempts++;
            return waitForResponse();
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
    auto command = dispenser_cmds.find(1);
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

void cnt_dispenser_vibration::setVibrateFreq(u_int new_freq)
{
    auto command = dispenser_cmds.find(7);
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << new_freq << '\n';
        std::string args = "=" + std::to_string(new_freq);
        sendCmd(command->second, _dispenser_client, args);
    }
}
double cnt_dispenser_vibration::getDuration()
{
   double duration = 0;
    std::cout << "get axis curent position" << std::endl;
    auto command = dispenser_cmds.find(4);
    std::cout << "sending command: " << command->second << '\n';
    auto resp =sendDirectCmd(command->second);
    duration = std::stod(resp); // to double
    return duration;
}


double cnt_dispenser_vibration::getFrequency()
{
    std::cout << "get axis curent position" << std::endl;
    auto command = dispenser_cmds.find(6);
    std::cout << "sending command: " << command->second << '\n';
    auto resp =sendDirectCmd(command->second);
    frequency = std::stod(resp); // to double
    return frequency;
}





bool cnt_dispenser_vibration::getStatus()
{
    return dispenserReady;
}


void cnt_dispenser_vibration::sendCmd(std::string& cmd, sockpp::tcp_connector* client, std::string args)
{
    if (client->write(cmd + args) != ssize_t(std::string(cmd + args).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << client->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd + args << " sent" << std::endl;
}