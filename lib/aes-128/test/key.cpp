#include "lib/aes-128/defines.hpp"
#include "lib/aes-128/key.hpp"

#include "gtest/gtest.h"

Key key;
std::array<MATRIX_TYPE, KEY_SIZE> key_value = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

class KeyTest : public ::testing::Test {
protected:
    void SetUp() override{}
};

TEST_F(KeyTest, SetGetKey) {
    key.set_key(key_value);

    EXPECT_EQ(key.get_key(), key_value);
}

TEST_F(KeyTest, GetMatrix) {
    key.set_key(key_value);
    Matrix key_matrix = key.key_matrix();

    std::vector<std::vector<MATRIX_TYPE>> expected_matrix = {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12},
        {13,14,15,16}
    };

    EXPECT_EQ(expected_matrix, key_matrix.data);
}
