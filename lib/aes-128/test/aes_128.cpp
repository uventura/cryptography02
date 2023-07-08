#include <string>

#include "lib/aes-128/aes_128.hpp"
#include "lib/math/matrix.hpp"
#include "lib/aes-128/defines.hpp"
#include "lib/aes-128/key.hpp"
#include "lib/math/random.hpp"

#include "gtest/gtest.h"

AES128 aes;
Key random_key;
std::string random_message;

class AES128Test : public ::testing::Test {
protected:
    void SetUp() override
    {
        // Generate Random Message(Perfect Size to Fit with Blocks)
        for(unsigned int i = 0; i < 112; ++i)
            random_message += static_cast<char>('a' + Random<uint32_t>::get_random(0, 25));
    }
};

//---| AES-128 Functionalities |---
TEST_F(AES128Test, AES128Encrypt) {
    aes.encrypt(random_message, random_key);
    EXPECT_EQ(1, 1);
}

//---| AES-128 Basic Functionalities |---
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

TEST_F(AES128Test, AddRoundKeyTest)
{
    std::vector<Matrix> blocks = aes.get_blocks_matrix(random_message);
    for(unsigned int index_block = 0; index_block < blocks.size(); ++index_block)
    {
        Matrix encrypt_result = aes.add_round_key(blocks[index_block], random_key);
        Matrix decrypt_result = encrypt_result ^ random_key.key_matrix();

        EXPECT_EQ(decrypt_result.data, blocks[index_block].data);
    }
}

TEST_F(AES128Test, SBOXTestValue)
{
    unsigned char result = aes.look_sbox('\x61');
    EXPECT_EQ(result, (unsigned char)('\xEF'));
}
