#include <gtest/gtest.h>
#include <gmock/gmock.h> // Brings in gMock.

#include "cnt_linear_motion.h"
#include <memory>
using ::testing::Return;

// mocking
class cntMotionMock: public cnt_linear_motion {
public:
cntMotionMock():cnt_linear_motion("192.168.0.203",8882){}
MOCK_METHOD( wgm_feedbacks::enum_sub_sys_feedback, connect, (), (override));
MOCK_METHOD( wgm_feedbacks::enum_sub_sys_feedback, move_home, (), (override));
MOCK_METHOD( double, get_position, (), (override));
};

// Demonstrate some basic mocking.
TEST(cntLinearMotionMock, connectSuccess) {
  cntMotionMock mock_obj;
  //mock_obj.
  // expect connect
  EXPECT_CALL(mock_obj, connect()) .WillOnce (Return(wgm_feedbacks::enum_sub_sys_feedback::sub_success));
  auto res =mock_obj.connect();
  EXPECT_EQ( wgm_feedbacks::enum_sub_sys_feedback::sub_success, res);
}


// Demonstrate some basic mocking.
TEST(cntLinearMotionMock, moveHome) {
  cntMotionMock mock_obj;
  //mock_obj.
  // expect connect
  EXPECT_CALL(mock_obj, move_home()) .Times(1);
  mock_obj.move_home();
  //EXPECT_EQ( wgm_feedbacks::enum_sub_sys_feedback::sub_success, res );
}

// Demonstrate some basic mocking.
TEST(cntLinearMotionMock, getPosition) {
  cntMotionMock mock_obj;
  //mock_obj.
  // expect connect
  EXPECT_CALL(mock_obj, get_position()) .Times(1);
  auto pos = mock_obj.get_position();
  EXPECT_EQ( 0, pos );
}


// Demonstrate some basic assertions.
TEST(cntLinearMotionOBj, connect) {
  // expect connect
  cnt_linear_motion mover("192.168.0.203",8882);
  EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_error, mover.connect());
}
// Demonstrate some basic assertions.
TEST(cntLinearMotionPtr, connect) {
  // expect connect
  cnt_linear_motion mover("192.168.0.203",8882);
  EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_error, mover.connect());
  Icnt_axis_motion* moverPtr = new cnt_linear_motion("192.168.0.203",8882);
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