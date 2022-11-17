#include "Icnt_high_voltage.h"



Icnt_high_voltage::Icnt_high_voltage(/* args */)
{
}

Icnt_high_voltage::~Icnt_high_voltage()
{
}

/**
 * @brief
 *
 * @param cmd
 * @param client
 * @param args
 */
void Icnt_high_voltage::sendCmd(std::string& cmd, sockpp::tcp_connector* client, std::string args)
{
    if (client->write(cmd + args) != ssize_t(std::string(cmd + args).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << client->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd + args << " sent" << std::endl;
}