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

enum class enum_sub_sys_feedback
{
    sub_success = 3,
    sub_error = -2
};
struct cnt_dispenser_server
{
    const char* ip = "192.168.0.100";
    uint16_t port = 80;
};
struct cnt_motion_server
{
    const char* ip = "192.168.0.100";
    uint16_t port = 80;
};

class cnt_dispenser
{
private:
    cnt_dispenser_server _dispenser_server;
    sockpp::socket_initializer sockInit;

    std::map<u_int, std::string> dispenser_cmds = {
        {1,"turn_on"}, {2,"turn_off"},
        {3,"vibrate"}, {4,"set_duration"}
    };
    std::map<std::string, std::string> motion_cmds = {
        {"up","$h\n"}, {"down", "x160\n"}, {"id","$x\n"}
    };
    std::string dispenser_incoming_data;
    u_int dispenser_data_length = 1024;
    bool dispenserReady = false;
    bool motionReady = false;
    bool cntReady = false;

public:
    sockpp::tcp_connector* _dispenser_client;
    cnt_dispenser(/* args */);
    ~cnt_dispenser();
    /******* client controller methods ***/
    void close_all_sockets();
    void dispenser_client_connect();
    void sendCmd(std::string cmd, sockpp::tcp_connector* client, std::string args = std::string());
    void waitForResponse();
    void activate();
};
