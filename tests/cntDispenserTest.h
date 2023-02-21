#include <gtest/gtest.h>
#include <gmock/gmock.h> // Brings in gMock.

#include "cnt_dispenser_vibration.h"
#include <memory>
using ::testing::Return;

class CntDispenserTest: public ::testing::Test {
  protected:
  std::unique_ptr<Icnt_dispenser> mover_;

  void SetUp() override  {
    mover_ = std::make_unique<cnt_dispenser_vibration>("127.0.0.1",8882);
    mover_->connect();
  }

  void TearDown() override {
    mover_->disconnect();
  }
};



TEST_F(CntDispenserTest, Connect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->connect());
}

TEST_F(CntDispenserTest, IsConnected) {
    EXPECT_TRUE(mover_->getStatus());
}


TEST_F(CntDispenserTest, Disconnect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->disconnect());
}

TEST_F(CntDispenserTest, Activate) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->activate());
}


TEST_F(CntDispenserTest, Deactivate) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->deactivate());
}


TEST_F(CntDispenserTest, Vibrate) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->vibrate());
}


TEST_F(CntDispenserTest, GetDuration) {
    EXPECT_EQ(200, mover_->getDuration());
}

TEST_F(CntDispenserTest, SetDuration) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->setVibrateDuration(200));
}


TEST_F(CntDispenserTest, GetFrequency) {
    EXPECT_EQ(200, mover_->getFrequency());
}

TEST_F(CntDispenserTest, SetFrequency) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->setVibrateFreq(200));
}

