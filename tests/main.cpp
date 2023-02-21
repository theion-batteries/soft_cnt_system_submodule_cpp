#include "cntMotionTest.h"
#include "cntDispenserTest.h"
#include "cntControllerTest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}