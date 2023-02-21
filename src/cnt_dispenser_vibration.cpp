#include "cnt_dispenser_vibration.h"


cnt_dispenser_vibration::cnt_dispenser_vibration(const std::string &ip, uint16_t port)
{
        std::cout << "creating dispenser  client" << std::endl;
        _dispenser_server.ip=ip;
        _dispenser_server.port=port;
}

cnt_dispenser_vibration::~cnt_dispenser_vibration()
{
}



wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::connect()
{
    std::cout << "connecting to dispenser server" << std::endl;
    auto dispenser_server_addr = sockpp::tcp_connector::addr_t{_dispenser_server.ip, _dispenser_server.port};
    _dispenser_client = std::make_unique<sockpp::tcp_connector>(dispenser_server_addr);
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

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::disconnect()
{
     dispenserReady = false;
    if (_dispenser_client->close()) return sub_success;
    return sub_error;
}

std::string cnt_dispenser_vibration::sendDirectCmd(std::string cmd)
{
    if (_dispenser_client.get() == nullptr) return "not connected";
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
    if (attempts == cnt_dispenser_vibration::max_attempts)
    {
        std::cout << "attempts: " << attempts << std::endl;
        attempts = 0;
        return "NA";
    }
    std::cout << "awaiting server response" << std::endl;
    if (_dispenser_client->is_connected())
    {
        char str_holder[5012];
        ssize_t n = _dispenser_client->read_n(&str_holder, 1024);
        if (n > 0)
        {
            std::cout << "n bytes received: " << n << std::endl;
            dispenser_incoming_data = str_holder;
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

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::activate()
{
    auto command = dispenser_cmds.find("ON");
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        auto response=sendDirectCmd(command->second);
        if(response=="ok") return sub_success;
        return sub_error;   
    }
    return sub_error;
}
wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::deactivate()
{
    auto command = dispenser_cmds.find("OFF");
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
       auto response= sendDirectCmd(command->second);
       if(response=="ok") return sub_success;
        return sub_error;   
    }
    return sub_error;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::vibrate()
{
    auto command = dispenser_cmds.find("VIBRATE");
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
       auto response= sendDirectCmd(command->second);
       if(response=="ok") return sub_success;
        return sub_error;   
    }
    return sub_error;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::setVibrateDuration(u_int durationSecond)
{
    auto command = dispenser_cmds.find("SETDUR");
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << durationSecond << '\n';
        std::string args = "=" + std::to_string(durationSecond);
       auto response= sendDirectCmd(command->second);
       if(response=="ok") return sub_success;
        return sub_error;   
    }
    return sub_error;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::setVibrateFreq(u_int new_freq)
{
    auto command = dispenser_cmds.find("SETFREQ");
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << new_freq << '\n';
        std::string args = "=" + std::to_string(new_freq);
       auto response= sendDirectCmd(command->second+args);
       if(response=="ok") return sub_success;
        return sub_error;   
    }
    return sub_error;
}

double cnt_dispenser_vibration::getDuration()
{
    double duration = 0;
    std::cout << "get axis curent position" << std::endl;
    auto command = dispenser_cmds.find("GETDUR");
    std::cout << "sending command: " << command->second << '\n';
    auto resp =sendDirectCmd(command->second);
    duration = std::stod(resp); // to double
    return duration;
}


double cnt_dispenser_vibration::getFrequency()
{
    std::cout << "get axis curent position" << std::endl;
    auto command = dispenser_cmds.find("GETFREQ");
    std::cout << "sending command: " << command->second << '\n';
    auto resp =sendDirectCmd(command->second);
    frequency = std::stod(resp); // to double
    return frequency;
}





bool cnt_dispenser_vibration::getStatus()
{
    return dispenserReady;
}


