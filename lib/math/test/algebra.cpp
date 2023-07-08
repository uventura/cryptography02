#include "lib/math/algebra.hpp"

#include "gtest/gtest.h"

class AlgebraTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(AlgebraTest, ModOperation) {
    EXPECT_EQ(Algebra::mod(-1, 5), 4);
    EXPECT_EQ(Algebra::mod(-1, 3), 2);
    EXPECT_EQ(Algebra::mod(-1, 7), 6);
}
