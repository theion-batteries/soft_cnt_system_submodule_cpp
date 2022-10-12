/**
 * @file cnt_alignment_system.cpp
 * @author sami dhiab (sami@theion.de)
 * @brief cnt alignment system package
 * @version 0.1
 * @date 2022-07-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "cnt_system.h"

cnt_alignment_system::Icnt_dispensing::Icnt_dispensing()
{
  std::cout << "creating cnt dispenser" << std::endl;
}

cnt_alignment_system::Icnt_dispensing::~Icnt_dispensing()
{
  std::cout << "deleting cnt dispenser" << std::endl;
}
// implement 

void cnt_alignment_system::cnt_dispensing::start_vibrating()
{
  std::cout << "starting vibration" << std::endl;
  dispenser.vibrate();
}
void cnt_alignment_system::cnt_dispensing::stop_vibrating()
{
  std::cout << "stopping vibration" << std::endl;
  dispenser.deactivate();
}
/****************** cnt motion ******************/
cnt_alignment_system::Icnt_sys_motion::Icnt_sys_motion()
{
  std::cout << "creating cnt motion" << std::endl;
}
cnt_alignment_system::Icnt_sys_motion::~Icnt_sys_motion()
{
  std::cout << "deleting cnt motion" << std::endl;
}
//implement
void cnt_alignment_system::cnt_sys_motion::move_down_to_center()
{
  std::cout << "cnt moving down to centre" << std::endl;
  motion.move_center();
}
void cnt_alignment_system::cnt_sys_motion::move_back_to_reference()
{
  std::cout << "cnt moving back to refrence" << std::endl;
  motion.move_home();
}
/****************** hv controller ******************/
cnt_alignment_system::Ihv_controller::Ihv_controller()
{
  std::cout << "creating hv controller" << std::endl;
}
cnt_alignment_system::Ihv_controller:: ~Ihv_controller()
{
  std::cout << "deleting hv controller" << std::endl;
}
// implement 
void cnt_alignment_system::hv_controller::start_hv()
{
  std::cout << "starting hv controller" << std::endl;
  hv.start();
}
void cnt_alignment_system::hv_controller::stop_hv()
{
  std::cout << "stopping hv controller" << std::endl;
  hv.stop();
}
// external system calls
double cnt_alignment_system::hv_controller::get_input_voltage()
{
  return input_voltage;
}
double cnt_alignment_system::hv_controller::get_input_current()
{
  return input_current;
}
double cnt_alignment_system::hv_controller::get_output_voltage()
{
  return output_current;
}
double cnt_alignment_system::hv_controller::get_output_current()
{
  return output_voltage;
}

/*********** cnt controller ************/
  cnt_alignment_system::Icnt_aligning_controller:: Icnt_aligning_controller()
  {
    std::cout << "creating cnt_aligning_controller" << std::endl;
  }
  cnt_alignment_system::Icnt_aligning_controller:: ~Icnt_aligning_controller()
  {
    std::cout << "deleting cnt_aligning_controller" << std::endl;
  }
//implement
  cnt_alignment_system::cnt_aligning_controller:: cnt_aligning_controller()
  {
    cnt_dispenser = new cnt_dispensing();
    cnt_motion_controller = new cnt_sys_motion();
    hv_controll = new hv_controller();
  }
  cnt_alignment_system::cnt_aligning_controller:: ~cnt_aligning_controller()
  {
    delete cnt_dispenser;
    delete cnt_motion_controller;
    delete hv_controll;
  }
// methods implmenetation
void cnt_alignment_system::cnt_aligning_controller::start_aligning()
{
  hv_controll->start_hv();
  cnt_motion_controller->move_down_to_center();
  cnt_dispenser->start_vibrating();
}

void cnt_alignment_system:: cnt_aligning_controller::stop_aligning()
{
  cnt_dispenser->stop_vibrating();
  cnt_motion_controller->move_back_to_reference();
  hv_controll->stop_hv();
}
cnt_alignment_system::voltage cnt_alignment_system::cnt_aligning_controller::get_voltage_struct()
{
  V.Vin = hv_controll->get_input_voltage();
  V.Vout = hv_controll->get_output_voltage();
  return V;
}
cnt_alignment_system::current cnt_alignment_system::cnt_aligning_controller::get_current_struct()
{
  C.Cin = hv_controll->get_input_voltage();
  C.Cout = hv_controll->get_output_voltage();
  return C;
}


