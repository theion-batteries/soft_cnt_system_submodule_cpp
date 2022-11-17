
#include "Icnt_dispenser.h"

/**
 * @brief
 *
 * @param cmd
 * @param client
 * @param args
 */
void Icnt_dispenser::sendCmd(std::string& cmd, sockpp::tcp_connector* client, std::string args)
{
    if (client->write(cmd + args) != ssize_t(std::string(cmd + args).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << client->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd + args << " sent" << std::endl;
}
