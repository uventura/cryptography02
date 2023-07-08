#include <string>

#include "lib/math/matrix.hpp"
#include "lib/aes-128/aes_128.hpp"
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
TEST_F(AES128Test, AES128EncryptDecrypt) {
    auto enc_message = aes.encrypt(random_message, random_key);
    EXPECT_EQ(random_message, aes.decrypt(enc_message, random_key));
}

//---| AES-128 Steps |---
TEST_F(AES128Test, AddRoundKeyTest)
{
    std::vector<Matrix> blocks = aes.get_blocks_matrix(random_message);
    for(unsigned int index_block = 0; index_block < blocks.size(); ++index_block)
    {
        Matrix encrypt_result = aes.add_round_key(blocks[index_block], random_key.key_matrix());
        Matrix decrypt_result = encrypt_result ^ random_key.key_matrix();

        EXPECT_EQ(decrypt_result.data, blocks[index_block].data);
    }
}

TEST_F(AES128Test, SubBytesTest)
{
    Matrix matrix = aes.get_blocks_matrix("Lorem ipsum dolor sit amet, consectet")[0];
    EXPECT_EQ(aes.inv_sub_bytes(aes.sub_bytes(matrix)).data, matrix.data);
    EXPECT_EQ(aes.sub_bytes(aes.inv_sub_bytes(matrix)).data, matrix.data);
}

TEST_F(AES128Test, ShiftRowsTest)
{
    Matrix matrix = aes.get_blocks_matrix("Lorem ipsum dolor sit amet, consectet")[1];
    EXPECT_EQ(aes.inv_shift_rows(aes.shift_rows(matrix)).data, matrix.data);
    EXPECT_EQ(aes.shift_rows(aes.inv_shift_rows(matrix)).data, matrix.data);
}

TEST_F(AES128Test, MixColumnsTest)
{
    Matrix matrix = aes.get_blocks_matrix("ipsum dolor sitLorem amet, consectet")[1];
    EXPECT_EQ(aes.inv_mix_columns(aes.mix_columns(matrix)).data, matrix.data);
    EXPECT_EQ(aes.mix_columns(aes.inv_mix_columns(matrix)).data, matrix.data);
}

//---| AES-128 Basic Functionalities |---
TEST_F(AES128Test, PaddingIncorrectString)
{
    std::string result = aes.create_padding("aaaaaaaaaaaaaaaaaaaa");
    EXPECT_EQ(result, "aaaaaaaaaaaaaaaaaaaa............");
}

TEST_F(AES128Test, TestingBlocksConstruction)
{
    std::vector<Matrix> result = aes.get_blocks_matrix("Loremipsumdolorsitamet,consectet");

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

TEST_F(AES128Test, SBOXTestValue)
{
    unsigned char result = aes.look_sbox('\x61');
    EXPECT_EQ(result, (unsigned char)('\xEF'));
}

TEST_F(AES128Test, InvSBOXTestValue)
{
    unsigned char result = aes.look_inv_sbox('\xA1');
    EXPECT_EQ(result, (unsigned char)('\xF1'));
}

