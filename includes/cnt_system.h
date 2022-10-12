/**
 * @file cnt_alignment_system.h
 * @author sami dhiab (sami@theion.de)
 * @brief cnt alignment system package
 * @version 0.1
 * @date 2022-07-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <iostream>
#include "cnt_motion.h"
#include "cnt_controller.h"

namespace cnt_alignment_system
{
  /********** cnt dispensing **************/
  class Icnt_dispensing
  {
  public:
    virtual void start_vibrating() = 0;
    virtual void stop_vibrating() = 0;
    Icnt_dispensing();
    virtual ~Icnt_dispensing();
  };
  // implement 
  class cnt_dispensing : public Icnt_dispensing
  {
  protected:
    virtual void start_vibrating();
    virtual void stop_vibrating();
    cnt_dispenser dispenser;
  };
  /****************** cnt motion ******************/
  class Icnt_sys_motion
  {
  public:
    virtual void move_down_to_center() = 0;
    virtual void move_back_to_reference() = 0;
    Icnt_sys_motion();
    virtual ~Icnt_sys_motion();
  };
  //implement
  class cnt_sys_motion : public Icnt_sys_motion
  {
  protected:
    virtual void move_down_to_center();
    virtual void move_back_to_reference();
  private:
    cnt_motion motion;
  };
  /****************** hv controller ******************/
  class Ihv_controller
  {
  public:
    virtual void start_hv() = 0;
    virtual void stop_hv() = 0;
    virtual double get_input_voltage() = 0;
    virtual double get_output_voltage() = 0;
    virtual double get_input_current() = 0;
    virtual double get_output_current() = 0;
    Ihv_controller();
    virtual ~Ihv_controller();
  };
  // implement
  class hv_controller : public Ihv_controller
  {
  protected:
    virtual void start_hv();
    virtual void stop_hv();
    virtual double get_input_voltage();
    virtual double get_output_voltage();
    virtual double get_input_current();
    virtual double get_output_current();
  private:
    double input_voltage;
    double output_voltage;
    double input_current;
    double output_current;

    cnt_hv hv;
  };
  /*********** voltage struct *************/
  struct voltage
  {
    double Vin; //voltage
    double Vout;
  };
  /********** current struct ****************/
  struct current
  {
    double Cin; //current
    double Cout;
  };
  /*********** cnt controller ************/
  class Icnt_aligning_controller
  {
  public:
    Icnt_aligning_controller();
    virtual ~Icnt_aligning_controller();
    virtual void start_aligning() = 0;
    virtual void stop_aligning() = 0;
    virtual voltage get_voltage_struct() = 0;
    virtual current get_current_struct() = 0;
  };
  //implement
  class cnt_aligning_controller : public Icnt_aligning_controller
  {
  private:
    Icnt_dispensing* cnt_dispenser;
    Icnt_sys_motion* cnt_motion_controller;
    Ihv_controller* hv_controll;
  public:
    cnt_aligning_controller();
    virtual ~cnt_aligning_controller();
  public:

  protected:
    voltage V;
    current C;
    virtual void start_aligning();
    virtual void stop_aligning();
    virtual voltage get_voltage_struct();
    virtual current get_current_struct();
    cnt_controller controller;

  };
  
}

