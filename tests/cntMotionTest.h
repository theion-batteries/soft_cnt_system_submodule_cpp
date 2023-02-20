#include <gtest/gtest.h>
#include <gmock/gmock.h> // Brings in gMock.

#include "cnt_linear_motion.h"
#include <memory>
using ::testing::Return;

class CntLinearMotionTest: public ::testing::Test {
  protected:
  std::unique_ptr<Icnt_axis_motion> mover_;

  void SetUp() override  {
    mover_ = std::make_unique<cnt_linear_motion>("127.0.0.1",8882);
    mover_->connect();
  }

  void TearDown() override {
    mover_->disconnect();
  }
};



TEST_F(CntLinearMotionTest, Connect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->connect());
}


TEST_F(CntLinearMotionTest, Disconnect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->disconnect());
}


TEST_F(CntLinearMotionTest, IsConnected) {
    EXPECT_TRUE(mover_->getStatus());
}

TEST_F(CntLinearMotionTest, MoveHome) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->move_home());
}


TEST_F(CntLinearMotionTest, getSpeed) {
    EXPECT_EQ(800, mover_->get_speed());
}
TEST_F(CntLinearMotionTest, setSpeed) {
    EXPECT_EQ(sub_success, mover_->set_speed(100));
}
TEST_F(CntLinearMotionTest, getPosition) {
    EXPECT_EQ(-100, mover_->get_position());
}

TEST_F(CntLinearMotionTest, MoveCenter) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->move_center());
}
TEST_F(CntLinearMotionTest, MoveDownTo) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->move_down_to(100));
}
TEST_F(CntLinearMotionTest, MoveUpTo) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->move_up_to(100));
}
