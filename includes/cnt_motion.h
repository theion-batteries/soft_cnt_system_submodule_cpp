/**
 * @file cnt_motion.h
 * @author your name (you@domain.com)
 * @brief this class is usefull if the cnt_motion is interfaced seperatly
 * @version 0.1
 * @date 2022-10-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

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


struct cnt_motion_server
{
    const char* ip = "192.168.0.100";
    uint16_t port = 80;
};

class cnt_motion
{
private:
    cnt_motion_server _motion_server;
    sockpp::socket_initializer sockInit;
    std::map<u_int, std::string> motion_cmds = {
        {0,"unlock"}, {1,"move_up"}, {2,"move_down"}
        
    };
    std::string motion_incoming_data;
    u_int motion_data_length = 1024;
    bool motionReady = false;
    bool cntReady = false;

public:
    sockpp::tcp_connector* _motion_client;
    cnt_motion(/* args */);
    ~cnt_motion();
    /******* client controller methods ***/
    void close_all_sockets();
    void motion_client_connect();
    void sendCmd(std::string cmd, sockpp::tcp_connector* client, std::string args = std::string());
    void waitForResponse();
    void move_home();
    void move_center();
};
