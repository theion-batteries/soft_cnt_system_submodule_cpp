#include <gtest/gtest.h>
#include "cnt_linear_motion.h"
#include <memory>

// Demonstrate some basic assertions.
TEST(cntLinearMotionOBj, connect) {
  // expect connect
  cnt_linear_motion mover;
  EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_error, mover.connect());
}
// Demonstrate some basic assertions.
TEST(cntLinearMotionPtr, connect) {
  // expect connect
  cnt_linear_motion mover;
  EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_error, mover.connect());
  Icnt_axis_motion* moverPtr = new cnt_linear_motion();
  EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_error, moverPtr->connect());
  delete moverPtr;
}
//// Demonstrate some basic assertions.
//TEST(cntLinearMotionPtr, deletePtr) {
//  // expect connect
// std::unique_ptr< Icnt_axis_motion> moverPtr = std::make_unique< cnt_linear_motion>();
//   moverPtr.reset();
//  EXPECT_EQ(moverPtr, nullptr);
//}