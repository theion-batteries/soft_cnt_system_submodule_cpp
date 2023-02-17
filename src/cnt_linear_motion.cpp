/**
 * @file Iaxis_motion.h
 * @author sami dhiab
 * @version 0.1
 * @date 2022-11-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "cnt_linear_motion.h"


cnt_linear_motion::cnt_linear_motion(const std::string &ip, const uint16_t port)
{
        std::cout << "creating dispenser  client" << std::endl;
        
        _motion_axis_struct.ip=ip;
        _motion_axis_struct.port=port;
}

cnt_linear_motion::~cnt_linear_motion()
{
}

std::string cnt_linear_motion::sendDirectCmd(std::string cmd)
{
    if (axis_client_sock == nullptr) return "not connected";
    std::cout<< "sending linear axis command "<< cmd <<std::endl;
    cmd = cmd+"\r\n";
    if (axis_client_sock->write(cmd) != ssize_t(std::string(cmd).length())) {
        std::cout << "Error writing to the TCP stream: "
            << axis_client_sock->last_error_str() << std::endl;
    }
    return waitForResponse();
}

std::string cnt_linear_motion::waitForResponse()
{
    static int attempts = 0;
    if (attempts == cnt_linear_motion::max_attempts)
    {
        std::cout << "attempts: " << attempts << std::endl;
        attempts = 0;
        return "NA";
    }
    std::cout << "awaiting server response" << std::endl;
    if (axis_client_sock->is_connected())
    {
        char str_holder[5012];
        ssize_t n = axis_client_sock->read_n(&str_holder, 1024);
        if (n > 0)
        {
            std::cout << "n bytes received: " << n << std::endl;
            axis_incoming_data = str_holder;
            axis_incoming_data.resize(n);
            std::cout << "server replied : " << axis_incoming_data << std::endl;
            return axis_incoming_data;
        }
        else
        {
            std::cout << "no server response " << n << std::endl;
            attempts++;
            return waitForResponse();
        }
    }
}

bool cnt_linear_motion::getStatus()
{
    return axisReady;
}


/**
 * @brief TODO: change the function call
 *
 * @param new_position
 */
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_to(int new_position)
{
    if (new_position < 0) 
    {
        if(move_down_to(abs(new_position)) == sub_success) return sub_success;
        return sub_error;
    } 
    else
    {
        if(move_up_to(abs(new_position)) == sub_success) return sub_success;
                return sub_error;

    }
    
    return sub_error;

}

wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::connect()
{
    std::cout << "connecting controller to axis server" << std::endl;
    auto axis_server_addr = sockpp::tcp_connector::addr_t{_motion_axis_struct.ip,_motion_axis_struct.port};

    axis_client_sock =std::make_unique<sockpp::tcp_connector>(axis_server_addr);

    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (!axis_client_sock->is_connected()) {
        std::cerr << "Error connecting to axis server at "
            << sockpp::inet_address(_motion_axis_struct.ip, _motion_axis_struct.port)
            << " -> " << axis_client_sock->last_error_str();
        axisReady = false;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    std::cout << "Created a connection from " << axis_client_sock->address() << std::endl;
    std::cout << "Created a connection to " << axis_client_sock->peer_address() << std::endl;
    // Set a timeout for the responses
    if (!axis_client_sock->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            << axis_client_sock->last_error_str() << std::endl;
        axisReady = false;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    axisReady = true;
    return wgm_feedbacks::enum_sub_sys_feedback::sub_success;

}

wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::disconnect()
{
    axisReady = false;
    if (axis_client_sock->close()) return sub_success;
    return sub_error;
}


/**
 * @brief
 *
 * @return double
 */
double cnt_linear_motion::get_position()
{
    double axis_pos = 0;
    std::cout << "get axis curent position" << std::endl;
    auto command = axis_cmds.find("get_position");
    std::cout << "sending command: " << command->second << '\n';
    auto resp =sendDirectCmd(command->second);
    std::string extracted = resp.substr(resp.find_first_of(":") + 1, resp.find_first_of(",") - 1 - resp.find_first_of(":"));
    axis_pos = std::stod(extracted); // to double
    std::cout << "filter val : " << axis_pos << std::endl;
    axis_last_position.push_front(axis_pos); // add to table
    std::cout << "value added to table " << axis_last_position.front() << std::endl;
    return axis_pos;
}

/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_home()
{
   auto command = axis_cmds.find("home");
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        auto reply = sendDirectCmd(command->second);
        std::cout << "move home reply received " << reply << '\n';
        if (reply == "ok") return sub_success;
        return sub_error;
    }
    return sub_error;

}
/**
 * @brief
 *
 */
double cnt_linear_motion::get_speed()
{
    double speed = 0;
    std::cout << "get axis curent spped" << std::endl;
    auto command = axis_cmds.find("get_setting");
    std::cout << "sending command: " << command->second << '\n';

    auto resp = sendDirectCmd(command->second);
    if (!resp.find("ok"))
    {
        std::cout << "missing ok, error" << std::endl;
        return 0;
    }
    //$110=800.000
    std::string extracted = [resp]() {
        size_t found1 = resp.find("$110=");
        size_t found2 = resp.find("$111=");
        if (found1 != std::string::npos && found2 != std::string::npos) {
            auto rep = resp.substr(found1 + 5, found2);
            std::cout << "Substring found " << rep << std::endl;
            return rep;

        }
        else {
            std::cout << "Substring not found" << std::endl;
            return std::string("0");
        }
    }(); // Note the added semicolon here
    speed = std::stod(extracted); // to double
    std::cout << "filter val : " << speed << std::endl;
    return speed;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::set_speed(double_t new_val)
{

return sub_error;
}


wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_up_to(double_t new_pos)
{
    std::cout << "moving up by " << new_pos << std::endl;
    auto command = axis_cmds.find("move");
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << new_pos << '\n';
        std::string args = "-" + std::to_string(new_pos);
        auto cmd = (command->second) + args;
        // X-new_pos
        auto reply = sendDirectCmd(cmd);
        if (reply == "ok") return sub_success;
        std::cout << "move down reply received " << reply << '\n';
        return sub_error;
    }
    return sub_error;
}
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_down_to(double_t new_pos)
{
    std::cout << "moving down to " << new_pos << std::endl;
    auto command = axis_cmds.find("move");
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << new_pos << '\n';
        std::string args = std::to_string(-new_pos);
        auto cmd = (command->second) + args;
        // X-Steps
        auto reply = sendDirectCmd(cmd);
        if (reply == "ok") return sub_success;
        std::cout << "move down reply received " << reply << '\n';
        return sub_error;
    }
    return sub_error;
}



wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_up_by(double_t steps)
{
    std::cout << "moving up by " << steps << std::endl;
    auto command = axis_cmds.find("move");
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << steps << '\n';
        std::string args = std::to_string(steps);
        auto cmd = (command->second) + args;
        // X-Steps
        auto reply = sendDirectCmd(cmd);
        if (reply == "ok") return sub_success;
        std::cout << "move down reply received " << reply << '\n';
        return sub_error;
    }
    return sub_error;
}

/**
 * @brief
 *
 * @param steps
 */
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_down_by(double_t steps)
{

    std::cout << "moving down by " << steps << std::endl;
    auto command = axis_cmds.find("move");
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << steps << '\n';
        std::string args = std::to_string(-steps);
        auto cmd = (command->second) + args;
        // X-Steps
        auto reply = sendDirectCmd(cmd);
        std::cout << "move down reply received " << reply << '\n';
        if (reply == "ok") return sub_success;
        return sub_error;
    }
    return sub_error;

}






wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_center()
{
    auto command = axis_cmds.find("move");
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        auto reply = sendDirectCmd(command->second);
        std::cout << "move center reply received " << reply << '\n';
        std::string args = std::to_string(-_motion_axis_struct.cnt_max_travel);
        auto cmd = (command->second) + args;
        if (reply == "ok") return sub_success;
        return sub_error;
    }
    return sub_error;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::unlock()
{
    auto command = axis_cmds.find("unlock");
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        auto reply = sendDirectCmd(command->second);
        std::cout << "unlock reply received " << reply << '\n';
        if (reply == "ok") return sub_success;
        return sub_error;
    }
    return sub_error;
}