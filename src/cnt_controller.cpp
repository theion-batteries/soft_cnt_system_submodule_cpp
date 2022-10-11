#include "cnt_controller.h"


cnt_controller::cnt_controller(/* args */)
{
}

cnt_controller::~cnt_controller()
{
}



void cnt_controller::cnt_controller_connect()
{
   dispenser.dispenser_client_connect();
}

void cnt_controller::cnt_dispenser_activate()
{

      dispenser.activate();

}