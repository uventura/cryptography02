#include <string>

#include "lib/aes-128/aes_128.hpp"
#include "lib/aes-128/matrix.hpp"
#include "lib/aes-128/defines.hpp"

#include "gtest/gtest.h"

AES128 aes;

class AES128Test : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(AES128Test, RemoveWhiteSpacesTest) {
    std::string result = aes.remove_white_spaces("This is my string ");
    EXPECT_EQ(result, "Thisismystring");
}

TEST_F(AES128Test, PaddingIncorrectString)
{
    std::string result = aes.create_padding("aaaaaaaaaaaaaaaaaaaa");
    EXPECT_EQ(result, "aaaaaaaaaaaaaaaaaaaa............");
}

TEST_F(AES128Test, TestingBlocksConstruction)
{
    std::vector<Matrix> result = aes.get_blocks_matrix("Lorem ipsum dolor sit amet, consectet");

    //---| Check Number of Generated Blocks |---
    EXPECT_EQ(result.size(), 2);

    //---| Check Blocks Content |---
    std::vector<std::vector<MATRIX_TYPE>> result_0 = {
        {'L','o','r','e'},
        {'m','i','p','s'},
        {'u','m','d','o'},
        {'l','o','r','s'}
    };
    EXPECT_EQ(result[0].data, result_0);

    std::vector<std::vector<MATRIX_TYPE>> result_1 = {
        {'i','t','a','m'},
        {'e','t',',','c'},
        {'o','n','s','e'},
        {'c','t','e','t'}
    };
    EXPECT_EQ(result[1].data, result_1);
}
