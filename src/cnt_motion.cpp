#include "cnt_motion.h"


cnt_motion::cnt_motion(/* args */)
{
}

cnt_motion::~cnt_motion()
{
}

void cnt_motion::sendCmd(std::string cmd, sockpp::tcp_connector* client, std::string args)
{
    if (client->write(cmd + args) != ssize_t(std::string(cmd + args).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << client->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd + args << " sent" << std::endl;
    waitForResponse();
}

void cnt_motion::motion_client_connect()
{
    std::cout << "connecting to motion server" << std::endl;
    _motion_client = new sockpp::tcp_connector({ _motion_server.ip, _motion_server.port });
    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (!_motion_client) {
        std::cerr << "Error connecting to server at "
            << sockpp::inet_address(_motion_server.ip, _motion_server.port)
            << "\n\t" << _motion_client->last_error_str() << std::endl;
        return;
    }
    std::cout << "Created a connection from " << _motion_client->address() << std::endl;
    std::cout << "Created a connection to " << _motion_client->peer_address() << std::endl;
    // Set a timeout for the responses
    if (!_motion_client->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            << _motion_client->last_error_str() << std::endl;
    }
}

void cnt_motion::close_all_sockets()
{
    if (!_motion_client) _motion_client->close();
}

void cnt_motion::waitForResponse()
{
    std::cout << "awaiting server response" << std::endl;

    while (_motion_client->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = _motion_client->read(&motion_incoming_data[0], motion_data_length);
        if (n > 0)
        {
            std::cout << "server replied : " << motion_incoming_data.c_str() << std::endl;
            break;
        }
    }
}

void cnt_motion::move_home()
{
    auto command = motion_cmds.find(1);
    if (command != motion_cmds.end()) {
        std::cout << "sending command: " << command->second <<'\n';
        sendCmd(command->second, _motion_client);
    }

}

void cnt_motion::move_center()
{
    auto command = motion_cmds.find(2);
    if (command != motion_cmds.end()) {
        std::cout << "sending command: " << command->second <<  '\n';
        sendCmd(command->second, _motion_client);
    }
}