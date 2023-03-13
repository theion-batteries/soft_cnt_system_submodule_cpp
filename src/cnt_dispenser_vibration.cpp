#include "cnt_dispenser_vibration.h"
#include <thread>
#include <mutex>
#include <chrono>

cnt_dispenser_vibration::cnt_dispenser_vibration(const std::string& ip, uint16_t port, const uint16_t timeout)
{
    std::cout << "creating dispenser  client" << "\n";
    _dispenser_server.ip = ip;
    _dispenser_server.port = port;
    _dispenser_server.timeout = timeout;
}

cnt_dispenser_vibration::~cnt_dispenser_vibration()
{
}



std::optional<u_int> cnt_dispenser_vibration::convert_to_double(const std::string& str) {

    try {
        double val = std::stoi(str);
        return (u_int)val;
    }
    catch (std::exception& e) {
        std::cerr << " Error in double conversion " << __FILE__ << " " << __LINE__ << " " << e.what() << "\n";
    }
    return std::nullopt;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::connect()
{
    std::cout << "connecting to dispenser server" << "\n";
    auto dispenser_server_addr = sockpp::tcp_connector::addr_t{ _dispenser_server.ip, _dispenser_server.port };
    _client = std::make_unique<sockpp::tcp_connector>(dispenser_server_addr);
    _client->set_non_blocking();
    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (!_client) {
        std::cerr << "Error connecting to server at "
            << sockpp::inet_address(_dispenser_server.ip, _dispenser_server.port)
            << "\n\t" << _client->last_error_str() << "\n";
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    std::cout << "Created a connection from " << _client->address() << "\n";
    std::cout << "Created a connection to " << _client->peer_address() << "\n";
    // Set a timeout for the responses
    if (!_client->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            << _client->last_error_str() << "\n";
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
    if (blocking) _client->set_non_blocking(false);

    if (_client.get() == nullptr) return "not connected";
    cmd = cmd + "\r\n";
    std::cout << "sending cnt dispenser command " << cmd << " size of cmd: " << cmd.size() << "\n";

    if (_client->write(cmd) != ssize_t(std::string(cmd).length())) {
        std::cout << "Error writing to the TCP stream: "
            << _client->last_error_str() << "\n";
    }
    return waitForResponse();
}


std::string cnt_dispenser_vibration::waitForResponse()
{
    // std::cout << "awaiting server response" << "\n";
    auto start = std::chrono::steady_clock::now();
    while (_client->is_connected())
    {
        char Strholder[5012];
        ssize_t n = _client->read_n(&Strholder, sizeof(Strholder));
        if (n > 0)
        {
            std::cout << "n bytes received: " << n << "\n";
            incoming_data = Strholder;
            incoming_data.resize(n);
            std::cout << "server replied : " << incoming_data << "\n";
            break;
        }
        else
        {
            //  // std::cout << "no server response, retry " << n << "\n";
            incoming_data = "NA";
            long long timeout = _dispenser_server.timeout;
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count();
            if (duration >= timeout)
            {
                std::cout << "no response within a timeout of " << duration << " seconds, " << "aborting.." << "\n";
                std::cerr << "The error is: " << _client->last_error_str() << "\n";
                break;
            }
            continue;
        }
    }
    blocking = false;
    _client->set_non_blocking(true);
    return incoming_data;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::activate()
{
    std::cout << "turn on dispenser" << "\n";
    auto command = dispenser_cmds.find("ON");
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        auto response = sendDirectCmd(command->second);
        if (response == "ok") return sub_success;
        return sub_error;
    }
    return sub_error;
}


std::string cnt_dispenser_vibration::get_help()
{
    std::cout << "Getting help from the dispenser " << "\n";
    auto command = dispenser_cmds.find("HELP");
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        auto response = sendDirectCmd(command->second);
        if (!response.find("ok")) return "NA";
        return response;
    }
    return "NA";
}



wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::deactivate()
{
    std::cout << "turn off dispenser" << "\n";
    auto command = dispenser_cmds.find("OFF");
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        auto response = sendDirectCmd(command->second);
        if (response == "ok") return sub_success;
        return sub_error;
    }
    return sub_error;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::vibrate()
{
    std::cout << "vibrate dispenser" << "\n";
    auto command = dispenser_cmds.find("VIBRATE");
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        auto response = sendDirectCmd(command->second);
        if (response == "ok") return sub_success;
        return sub_error;
    }
    return sub_error;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::setVibrateDuration(const u_int durationSecond)
{
    std::cout << "set dispenser Duration" << "\n";
    auto command = dispenser_cmds.find("SETDUR");
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << durationSecond << '\n';
        std::string args = "=" + std::to_string(durationSecond);
        auto response = sendDirectCmd(command->second + args);
        if (response == "ok") return sub_success;
        return sub_error;
    }
    return sub_error;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_dispenser_vibration::setVibrateFreq(const u_int new_freq)
{
    std::cout << "set dispenser Frequency" << "\n";
    auto command = dispenser_cmds.find("SETFREQ");
    if (command != dispenser_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << new_freq << '\n';
        std::string args = "=" + std::to_string(new_freq);
        auto response = sendDirectCmd(command->second + args);
        if (response == "ok") return sub_success;
        return sub_error;
    }
    return sub_error;
}

double cnt_dispenser_vibration::getDuration()
{
    _client->set_non_blocking(false);
    std::optional<u_int> duration = 0;
    std::cout << "get dispenser Duration" << "\n";
    auto command = dispenser_cmds.find("GETDUR");
    std::cout << "sending command: " << command->second << '\n';
    auto resp = sendDirectCmd(command->second);
    duration = convert_to_double(resp); // to double
    if (!duration.has_value()) duration = 0;
    _client->set_non_blocking(true);
    return duration.value();
}


double cnt_dispenser_vibration::getFrequency()
{
    _client->set_non_blocking(false);

    std::cout << "get dispenser Frequency" << "\n";
    auto command = dispenser_cmds.find("GETFREQ");
    std::cout << "sending command: " << command->second << '\n';
    auto resp = sendDirectCmd(command->second);
    std::optional<u_int> frequency = convert_to_double(resp); // to double
    if (!frequency.has_value()) frequency = 0;
    _client->set_non_blocking(true);

    return frequency.value();
}

bool cnt_dispenser_vibration::getStatus()
{
    return dispenserReady;
}

void cnt_dispenser_vibration::setModeBlocking(bool setblockingMode)
{
    if (setblockingMode) blocking = true;
}