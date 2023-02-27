#include <gtest/gtest.h>
#include <gmock/gmock.h> // Brings in gMock.

#include "cnt_hvac_gbs.h"
#include <memory>
using ::testing::Return;

class CntHvacTest : public ::testing::Test
{
protected:
    std::unique_ptr<cnt_hvac_gbs> mover_;

    void SetUp() override
    {
        mover_ = std::make_unique<cnt_hvac_gbs>("127.0.0.1", 8883, 10);
        mover_->connect();
    }

    void TearDown() override
    {
        mover_->disconnect();
    }
};

TEST_F(CntHvacTest, Connect)
{
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->connect());
}

TEST_F(CntHvacTest, Disconnect)
{
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->disconnect());
}

TEST_F(CntHvacTest, IsConnected)
{
    EXPECT_TRUE(mover_->getStatus());
}

TEST_F(CntHvacTest, Start)
{
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->start());
}

TEST_F(CntHvacTest, Stop)
{
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->stop());
}

TEST_F(CntHvacTest, OutputCurrent)
{
    EXPECT_EQ(200.00, mover_->get_output_current());
}

TEST_F(CntHvacTest, OutputVoltage)
{
    EXPECT_EQ(200.00, mover_->get_output_voltage());
}

TEST_F(CntHvacTest, OutputFrequency)
{
    EXPECT_EQ(200.00, mover_->get_output_frequency());
}

TEST_F(CntHvacTest, OutputResistivity)
{
    EXPECT_EQ(1.00, mover_->get_output_resistivity());
}

TEST_F(CntHvacTest, SetOutputVoltage)
{
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->set_output_voltage(200));
}

TEST_F(CntHvacTest, SetOutputFrequency)
{
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->set_output_frequency(200));
}