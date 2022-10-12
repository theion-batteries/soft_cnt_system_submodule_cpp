#include "cnt_high_voltage.h"



cnt_hv::cnt_hv(/* args */)
{
}

cnt_hv::~cnt_hv()
{
}

void cnt_hv::sendCmd(std::string cmd, sockpp::tcp_connector* client, std::string args)
{
    if (client->write(cmd + args) != ssize_t(std::string(cmd + args).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << client->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd + args << " sent" << std::endl;
    waitForResponse();
}

void cnt_hv::hv_client_connect()
{
    std::cout << "connecting to hv server" << std::endl;
    _hv_client = new sockpp::tcp_connector({ _hv_server.ip, _hv_server.port });
    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (!_hv_client) {
        std::cerr << "Error connecting to server at "
            << sockpp::inet_address(_hv_server.ip, _hv_server.port)
            << "\n\t" << _hv_client->last_error_str() << std::endl;
        return;
    }
    std::cout << "Created a connection from " << _hv_client->address() << std::endl;
    std::cout << "Created a connection to " << _hv_client->peer_address() << std::endl;
    // Set a timeout for the responses
    if (!_hv_client->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            << _hv_client->last_error_str() << std::endl;
    }
}

void cnt_hv::close_all_sockets()
{
    if (!_hv_client) _hv_client->close();
}

void cnt_hv::waitForResponse()
{
    std::cout << "awaiting server response" << std::endl;

    while (_hv_client->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = _hv_client->read(&hv_incoming_data[0], hv_data_length);
        if (n > 0)
        {
            std::cout << "server replied : " << hv_incoming_data.c_str() << std::endl;
            break;
        }
    }
}

void cnt_hv::start()
{
    auto command = hv_cmds.find(1);
    if (command != hv_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendCmd(command->second, _hv_client);
    }

}

void cnt_hv::stop()
{
    auto command = hv_cmds.find(0);
    if (command != hv_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendCmd(command->second, _hv_client);
    }
}




void cnt_hv::tune_param(std::pair<std::string, int> new_param_val)
{
    // check which param to change 
    for (auto& param : params.parameters)
    {

        if (new_param_val.first == param.first) {
            param.second = new_param_val.second;
            for (auto& cmd: hv_cmds)
            {
                if (cmd.second == param.first)
                {
                std::cout << "sending command: " << cmd.second << " args: " << param.second << '\n';
                std::string args = "=" + std::to_string(param.second);
                sendCmd(cmd.second, _hv_client, args);
                break;
                }
            }
        }
    }
}