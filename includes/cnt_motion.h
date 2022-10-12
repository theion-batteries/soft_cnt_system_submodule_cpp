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

  class cnt_motion 
  {
  public:
    virtual void move_down_to_center();
    virtual void move_back_to_reference();
  };