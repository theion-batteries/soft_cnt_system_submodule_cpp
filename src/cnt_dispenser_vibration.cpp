#include "cnt_dispenser_vibration.h"
#include <thread>
#include <mutex>
#include <chrono>

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
    _client = std::make_unique<sockpp::tcp_connector>(dispenser_server_addr);
    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (!_client) {
        std::cerr << "Error connecting to server at "
            << sockpp::inet_address(_dispenser_server.ip, _dispenser_server.port)
            << "\n\t" << _client->last_error_str() << std::endl;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    std::cout << "Created a connection from " << _client->address() << std::endl;
    std::cout << "Created a connection to " << _client->peer_address() << std::endl;
    // Set a timeout for the responses
    if (!_client->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            << _client->last_error_str() << std::endl;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    dispenserReady = true;
    return wgm_feedbacks::enum_sub_sys_feedback::sub_success;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::disconnect()
{
     dispenserReady = false;
    if (_client->close()) return sub_success;
    return sub_error;
}

std::string cnt_dispenser_vibration::sendDirectCmd(std::string cmd)
{
    if (_client.get() == nullptr) return "not connected";
    std::cout << "sending linear dispenser command " << cmd << std::endl;
    cmd = cmd + "\r\n";
    if (_client->write(cmd) != ssize_t(std::string(cmd).length())) {
        std::cout << "Error writing to the TCP stream: "
            << _client->last_error_str() << std::endl;
    }
    return waitForResponse();
}


std::string cnt_dispenser_vibration::waitForResponse()
{
    std::cout << "awaiting server response" << std::endl;
    auto start = std::chrono::steady_clock::now();
    while (_client->is_connected())
    {

        char Strholder[5012];
        
            ssize_t n = _client->read_n(&Strholder, sizeof(Strholder));
        if (n > 0)
        {
            std::cout << "n bytes received: " << n << std::endl;
            incoming_data = Strholder;
            incoming_data.resize(n);
            std::cout << "server replied : " << incoming_data << std::endl;
            break;
        }
        else
        {
            std::cout << "no server response, retry " << n << std::endl;
            incoming_data = "NA";
            long long timeout = 10;
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count();
            if(duration >= timeout)
            {
            std::cout << "no response within a timeout of "<<duration<< " seconds, " <<"aborting.."<< std::endl;
            break;
            } 
            continue;
        }

    }
    return incoming_data;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::activate()
{
    std::cout << "turn on dispenser" << std::endl;
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
    std::cout << "turn off dispenser" << std::endl; 
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
    std::cout << "vibrate dispenser" << std::endl; 
    auto command = dispenser_cmds.find("VIBRATE");
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
       auto response= sendDirectCmd(command->second);
       if(response=="ok") return sub_success;
        return sub_error;   
    }
    return sub_error;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::setVibrateDuration(const u_int durationSecond)
{
    std::cout << "set dispenser Duration" << std::endl;
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

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::setVibrateFreq(const u_int new_freq)
{
    std::cout << "set dispenser Frequency" << std::endl;
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
    std::cout << "get dispenser Duration" << std::endl;
    auto command = dispenser_cmds.find("GETDUR");
    std::cout << "sending command: " << command->second << '\n';
    auto resp =sendDirectCmd(command->second);
    duration = std::stod(resp); // to double
    return duration;
}


double cnt_dispenser_vibration::getFrequency()
{
    std::cout << "get dispenser Frequency" << std::endl;
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

