#include <string>

#include "lib/aes-128/key_schedule.hpp"
#include "lib/aes-128/defines.hpp"
#include "lib/math/matrix.hpp"

#include "gtest/gtest.h"

class KeyScheduleTest : public ::testing::Test {
protected:
    void SetUp() override{}
};

//---| AES-128 Functionalities |---
TEST_F(KeyScheduleTest, RotWord) {
    Matrix col(4, 1);
    col.data = {{1},{2},{3},{4}};

    col = KeySchedule::rot_word(col);

    std::vector<std::vector<MATRIX_TYPE>> expected = {{2},{3},{4},{1}};
    EXPECT_EQ(col.data, expected);
}

TEST_F(KeyScheduleTest, SubWord) {
    Matrix col(4, 1);
    col.data = {{1},{2},{3},{4}};

    col = KeySchedule::sub_word(col);

    std::vector<std::vector<MATRIX_TYPE>> expected = {{124},{119},{123},{242}};
    EXPECT_EQ(col.data, expected);
}
