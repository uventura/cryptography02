#include "lib/aes-128/aes_128.hpp"
#include "lib/rsa/rsa.hpp"
#include "lib/math/random.hpp"

#include "gtest/gtest.h"

AES128 aes;
Key random_key;
std::string random_message;

class CryptographyTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Generate Random Message(Perfect Size to Fit with Blocks)
        for(unsigned int i = 0; i < 112; ++i)
            random_message += static_cast<char>('a' + Random<uint32_t>::get_random(0, 25));
    }
};

TEST_F(CryptographyTest, AES128Authentication) {
    auto enc_message = aes.encrypt(random_message, random_key);
    EXPECT_EQ(random_message, aes.decrypt(enc_message, random_key));
}
