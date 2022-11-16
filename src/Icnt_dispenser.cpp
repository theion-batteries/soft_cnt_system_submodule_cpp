
#include "Icnt_dispenser.h"


Icnt_dispenser::Icnt_dispenser(/* args */)
{
}

Icnt_dispenser::~Icnt_dispenser()
{
}


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

 void Icnt_dispenser::vibrate(){};
 void Icnt_dispenser::setVibrateDuration(u_int durationSecond){};