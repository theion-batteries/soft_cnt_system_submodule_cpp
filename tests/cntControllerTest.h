#include <gtest/gtest.h>
#include <gmock/gmock.h> // Brings in gMock.

#include "cnt_controller.h"
#include <memory>
using ::testing::Return;

class CntControllerTest: public ::testing::Test {
  protected:
  std::unique_ptr<cnt_controller> mover_;

  void SetUp() override  {
    mover_ = std::make_unique<cnt_controller>("127.0.0.1",8881,8882,8883);
    mover_->cnt_controller_connect();
  }

  void TearDown() override {
    mover_->cnt_controller_disconnect();
  }
};


TEST_F(CntControllerTest, Connect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->cnt_controller_connect());
}


TEST_F(CntControllerTest, MotionConnect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->cnt_motion_connect());
}

TEST_F(CntControllerTest, MotionMoveHome) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->cnt_motion_move_home());
}

TEST_F(CntControllerTest, MotionMoveCenter) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->cnt_motion_move_to_center(100));
}

TEST_F(CntControllerTest, MotionTargetPosition) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->cnt_motion_move_target_position());
}


TEST_F(CntControllerTest, MotionUnlock) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->cnt_motion_unlock());
}

TEST_F(CntControllerTest, DispenserConnect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->cnt_dispenser_connect());
}


TEST_F(CntControllerTest, DispenserActivate) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->cnt_dispenser_activate());
}

TEST_F(CntControllerTest, DispenserVibrate) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->cnt_dispenser_vibrate());
}


TEST_F(CntControllerTest, DispenseSetVibrateDuration) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->cnt_dispenser_setVibrateDuration(200));
}

TEST_F(CntControllerTest, DispenserDeactivate) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->cnt_dispenser_deactivate());
}

#