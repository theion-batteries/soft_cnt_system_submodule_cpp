#include "cnt_system.h"
 cnt_controller controller;
int main()
{
    controller.cnt_controller_connect();
    std::cout<<"test main"<<std::endl;
    return 0;
}