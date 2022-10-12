#pragma once


#include "sockpp/socket.h"
#include <iostream>
#include <string>
#include "sockpp/tcp_connector.h"
#include "Windows.h" 
#include <shellapi.h> 
#include <map>
#include <thread>
#include <atomic>


struct cnt_hv_server
{
    const char* ip = "192.168.0.100";
    uint16_t port = 80;
};

struct cnt_hv_paramteres
{
    std::map<std::string,int> parameters= {{"current", 0}, {"voltage", 0},{"frequency", 0},{"amplitude", 0}};
};
class cnt_hv
{
private:
    cnt_hv_server _hv_server;
    sockpp::socket_initializer sockInit;
    std::map<u_int, std::string> hv_cmds = {
        {0,"stop"}, {1,"start"}, {2,"current"},
        {3,"voltage"},{4,"frequency"},{5,"amplitude"}
    };
    std::string hv_incoming_data;
    u_int hv_data_length = 1024;
    bool hvReady = false;
    bool cntReady = false;
    cnt_hv_paramteres params;

public:
    sockpp::tcp_connector* _hv_client;
    cnt_hv(/* args */);
    ~cnt_hv();
    /******* client controller methods ***/
    void close_all_sockets();
    void hv_client_connect();
    void sendCmd(std::string cmd, sockpp::tcp_connector* client, std::string args = std::string());
    void waitForResponse();
    void start();
    void stop();
    void tune_param(std::pair<std::string, int> new_param_val); // tune param
};
