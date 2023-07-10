#include "lib/aes-128/aes_128.hpp"
#include "lib/rsa/rsa.hpp"
#include "lib/math/random.hpp"

#include "gtest/gtest.h"

AES128 aes;
Key random_key;
std::string random_message;

RSA rsa;

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

TEST_F(CryptographyTest, EncryptionDecryption) {
    auto key = rsa.key_generation();
    auto encrypt = rsa.encrypt("mess", key.public_key);
    auto decrypt = rsa.decrypt(encrypt, key.private_key);
    EXPECT_EQ("mess", decrypt);
}

TEST_F(CryptographyTest, SignatureDocument) {
    auto keys = rsa.key_generation();
    std::string message = "This is my ultra secret message muahahaha";

    EXPECT_EQ(true, rsa.verify_message(rsa.sign_message(message, keys.private_key), keys.public_key));
}
