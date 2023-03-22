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

cnt_linear_motion::cnt_linear_motion(const std::string &ip, const uint16_t port, const uint16_t timeout)
{
    std::cout << "creating linear motion  client"
              << "\n";

    _motion_axis_struct.ip = ip;
    _motion_axis_struct.port = port;
    _motion_axis_struct.timeout = timeout;
}

cnt_linear_motion::~cnt_linear_motion()
{
}

std::string cnt_linear_motion::sendDirectCmd(std::string cmd)
{

    if (_client == nullptr)
        return "not connected";
    if (blocking)
        _client->set_non_blocking(false);

    std::cout << "sending linear axis command " << cmd << "\n";
    cmd = cmd + "\r\n";

    std::cout << " command :" << cmd << " writing to " << _client->peer_address() << "\n";
    if (_client->write(cmd) != ssize_t(std::string(cmd).length()))
    {
        std::cout << "Error writing to the TCP stream: "
                  << _client->last_error_str() << "\n";
    }
    return waitForResponse();
}

std::string cnt_linear_motion::waitForResponse()
{
    std::cout << "awaiting server response" << std::endl;
    auto start = std::chrono::steady_clock::now();
    try
    {
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
                incoming_data = "NA";
                long long timeout = _motion_axis_struct.timeout;
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count();
                if (duration >= timeout)
                {
                    std::cout << "no response within a timeout of " << duration << " seconds, "
                              << "aborting.." << std::endl;
                    break;
                }
                continue;
            }
        }
        blocking = false;
        _client->set_non_blocking(true);
    }
    catch (std::exception &e)
    {
        std::cout << " Linear Motion " << e.what() << "\n";
    }
    return incoming_data;
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
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_to(const double_t new_position)
{
    if (new_position < 0)
    {
        if (move_down_to(abs(new_position)) == sub_success)
            return sub_success;
        return sub_error;
    }
    else
    {
        if (move_up_to(abs(new_position)) == sub_success)
            return sub_success;
        return sub_error;
    }
    return sub_error;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::connect()
{
    std::cout << "connecting controller to axis server"
              << "\n";
    auto axis_server_addr = sockpp::tcp_connector::addr_t{_motion_axis_struct.ip, _motion_axis_struct.port};

    _client = std::make_unique<sockpp::tcp_connector>(axis_server_addr);
    _client->set_non_blocking();

    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (!_client->is_connected())
    {
        std::cerr << "Error connecting to axis server at "
                  << sockpp::inet_address(_motion_axis_struct.ip, _motion_axis_struct.port)
                  << " -> " << _client->last_error_str();
        axisReady = false;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    std::cout << "Created a connection from " << _client->address() << "\n";
    std::cout << "Created a connection to " << _client->peer_address() << "\n";
    // Set a timeout for the responses
    if (!_client->read_timeout(std::chrono::seconds(5)))
    {
        std::cerr << "Error setting timeout on TCP stream: "
                  << _client->last_error_str() << "\n";
        axisReady = false;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    axisReady = true;
    return wgm_feedbacks::enum_sub_sys_feedback::sub_success;
}

wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::disconnect()
{
    if (_client->close())
    {
        axisReady = false;
        return sub_success;
    }
    return sub_error;
}

/**
 * @brief
 *
 * @return double
 */
double cnt_linear_motion::get_position()
{
    _client->set_non_blocking(false);

    std::optional<double> axis_pos = 0;
    std::cout << "get axis curent position"
              << "\n";
    auto command = axis_cmds.find("get_position");
    std::cout << "sending command: " << command->second << '\n';
    auto resp = sendDirectCmd(command->second);
    std::string extracted = resp.substr(resp.find("MPos:") + 5, resp.find_first_of(",") - 1);
    std::cout << "string extracted: " << extracted << "\n";

    try
    {

        axis_pos = string_to_double(extracted);
        axis_pos = std::stod(extracted); // to double
        if (axis_pos.value())
        {
            std::cout << "filter val : " << axis_pos.value() << "\n";
            axis_last_position.push_front(axis_pos.value()); // add to table
            std::cout << "value added to table " << axis_last_position.front() << "\n";
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception caught in getting position " << __FILE__ << " " << __LINE__ << " " << e.what() << "\n";
    }
    _client->set_non_blocking(true);

    return axis_pos.value_or(0.0);
}

/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_home()
{
    auto command = axis_cmds.find("home");
    if (command != axis_cmds.end())
    {
        std::cout << "sending command: " << command->second << '\n';
        auto reply = sendDirectCmd(command->second);
        std::cout << "move home reply received " << reply << '\n';
        if (reply.find("ok") != std::string::npos)
            return sub_success;

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
    _client->set_non_blocking(false);

    double speed = 0;
    std::cout << "get axis curent speed"
              << "\n";
    auto command = axis_cmds.find("get_setting");
    std::cout << "sending command: " << command->second << '\n';

    auto resp = sendDirectCmd(command->second);
    if (!resp.find("ok"))
    {
        std::cout << "missing ok, error"
                  << "\n";
        return 0;
    }
    //$110=800.000
    std::string extracted = [resp]()
    {
        size_t found1 = resp.find("$110=");
        size_t found2 = resp.find("$111=");
        if (found1 != std::string::npos && found2 != std::string::npos)
        {
            auto rep = resp.substr(found1 + 5, found2);
            std::cout << "Substring found " << rep << "\n";
            return rep;
        }
        else
        {
            std::cout << "Substring not found"
                      << "\n";
            return std::string("0");
        }
    }();                          // Note the added semicolon here
    speed = std::stod(extracted); // to double
    std::cout << "filter val : " << speed << "\n";
    _client->set_non_blocking(true);

    return speed;
}

/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::set_speed(double_t new_val)
{
    std::cout << "set  axis curent spped"
              << "\n";
    auto command = axis_cmds.find("set_speed");
    if (command != axis_cmds.end())
    {
        std::cout << "sending command: " << command->second << " args: " << new_val << '\n';
        std::string args = std::to_string(new_val);
        auto cmd = (command->second) + args;
        // X-new_val
        auto reply = sendDirectCmd(cmd);
        if (reply.find("ok") != std::string::npos)
            return sub_success;
        std::cout << "move down reply received " << reply << '\n';
        return sub_error;
    }
    return sub_error;
}

/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_up_to(double_t new_pos)
{
    return sub_success;
}

/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_down_to(double_t new_pos)
{
    std::cout << "moving down to " << new_pos << "\n";
    auto command = axis_cmds.find("move");
    if (command != axis_cmds.end())
    {
        std::cout << "sending command: " << command->second << " args: " << new_pos << '\n';
        std::string args = std::to_string(-new_pos);
        auto cmd = (command->second) + args;
        // X-Steps
        blocking = true;
        auto reply = sendDirectCmd(cmd);
        if (reply.find("ok") != std::string::npos)
        {
            std::cout << "move down reply received " << reply << '\n';
            while (get_position() < abs(new_pos))
            {
                if (abs(get_position()) == abs(new_pos))
                    break;
                std::cout << abs(get_position()) << " " << abs(new_pos) << "\n";
            }
            return sub_success;
        }
        return sub_error;
    }
    return sub_error;
}

/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_up_by(double_t steps)
{
    std::cout << "moving up by " << steps << "\n";
    auto command = axis_cmds.find("move");
    if (command != axis_cmds.end())
    {
        std::cout << "sending command: " << command->second << " args: " << steps << '\n';
        std::string args = std::to_string(steps);
        auto cmd = (command->second) + args;
        // X-Steps
        auto reply = sendDirectCmd(cmd);
        if (reply.find("ok") != std::string::npos)
            return sub_success;
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

    std::cout << "moving down by " << steps << "\n";
    auto command = axis_cmds.find("move");
    if (command != axis_cmds.end())
    {
        std::cout << "sending command: " << command->second << " args: " << steps << '\n';
        std::string args = std::to_string(-steps);
        auto cmd = (command->second) + args;
        // X-Steps
        auto reply = sendDirectCmd(cmd);
        std::cout << "move down reply received " << reply << '\n';
        if (reply.find("ok") != std::string::npos)
            return sub_success;
        return sub_error;
    }
    return sub_error;
}

/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::move_center()
{
    auto command = axis_cmds.find("move");
    if (command != axis_cmds.end())
    {
        std::string args = std::to_string(-_motion_axis_struct.cnt_max_travel);
        std::cout << "sending command: " << command->second << " args: " << args << '\n';
        auto cmd = command->second + args;
        auto reply = sendDirectCmd(cmd);
        std::cout << "move center reply received " << reply << '\n';
        if (reply.find("ok") != std::string::npos)
            return sub_success;
        return sub_error;
    }
    return sub_error;
}

/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::pause()
{
    auto command = axis_cmds.find("pause");
    if (command != axis_cmds.end())
    {
        std::cout << "sending command: " << command->second << '\n';
        auto reply = sendDirectCmd(command->second);
        std::cout << "unlock reply received " << reply << '\n';
        if (reply.find("ok") != std::string::npos)
            return sub_success;
        return sub_error;
    }
    return sub_error;
}

/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::resume()
{
    auto command = axis_cmds.find("resume");
    if (command != axis_cmds.end())
    {
        std::cout << "sending command: " << command->second << '\n';
        auto reply = sendDirectCmd(command->second);
        std::cout << "unlock reply received " << reply << '\n';
        if (reply.find("ok") != std::string::npos)
            return sub_success;
        return sub_error;
    }
    return sub_error;
}

/**
 * @brief
 *
 */
std::string cnt_linear_motion::get_settings()
{
    std::cout << "get axis curent speed"
              << "\n";
    auto command = axis_cmds.find("get_setting");
    std::cout << "sending command: " << command->second << '\n';

    auto resp = sendDirectCmd(command->second);
    if (!resp.find("ok"))
    {
        std::cout << "missing ok, error"
                  << "\n";
        return "NA";
    }
    return resp;
}

/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback cnt_linear_motion::unlock()
{
    auto command = axis_cmds.find("unlock");
    if (command != axis_cmds.end())
    {
        std::cout << "sending command: " << command->second << '\n';
        auto reply = sendDirectCmd(command->second);
        std::cout << "unlock reply received " << reply << '\n';
        if (reply.find("ok") != std::string::npos)
            return sub_success;
        return sub_error;
    }
    return sub_error;
}

void cnt_linear_motion::setModeBlocking(bool setblockingMode)
{
    if (setblockingMode)
        blocking = true;
}
