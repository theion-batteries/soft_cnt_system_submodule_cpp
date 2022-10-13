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

void cnt_controller::cnt_dispenser_deactivate()

{
      dispenser.deactivate();
}
void cnt_controller::cnt_dispenser_vibrate()

{
      dispenser.vibrate();
}
void cnt_controller::cnt_dispenser_setVibrateDuration(u_int durationSecond)

{
      dispenser.setVibrateDuration(durationSecond);
}

void cnt_controller::cnt_motion_move_home()
{
dispenser.move_home();

}
void cnt_controller::cnt_motion_move_to_center()
{

dispenser.move_center();

}

void cnt_controller::cnt_motion_unlock()
{

dispenser.unlock();

}