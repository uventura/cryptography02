#include "lib/rsa/rsa.hpp"
#include "lib/rsa/defines.hpp"

#include "gtest/gtest.h"

RSA rsa;

class RSATest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(RSATest, IsPrimeOperation) {
    mpz_class number("166344436937662474135736369092294326726574908589066069804427717459569968952117226951815278766986545503763233408493325564877113677185529165050960490680017438812911881082346048698360858364126684716985573245312167774754196785843262166631344192781074528446158213236153445128305728871203547589205156494119991161261");

    EXPECT_EQ(true, rsa.is_prime(number));
}

TEST_F(RSATest, IsPrimeGenerationOperation) {
    EXPECT_EQ(true, rsa.is_prime(rsa.generate_prime()));
}

TEST_F(RSATest, TransformTextToNum) {
    std::vector<ENCRYPT_TYPE> result = {'m', 'e', 's', 's'};
    EXPECT_EQ(result, rsa.transform_text_to_num("mess"));
}

TEST_F(RSATest, InvTransformTextToNum) {
    std::vector<ENCRYPT_TYPE> result = {'m', 'e', 's', 's'};
    EXPECT_EQ("mess", rsa.inv_transform_text_to_num(result));
}

TEST_F(RSATest, EncryptionDecryption) {
    auto key = rsa.key_generation();
    auto encrypt = rsa.encrypt("mess", key.public_key);
    auto decrypt = rsa.decrypt(encrypt, key.private_key);
    EXPECT_EQ("mess", decrypt);
}

TEST_F(RSATest, ApplySHA3) {
    EXPECT_EQ("D46E9BEBD3F20DC424D44C15F33E8FB947F8B10D4FC1C1A1C6BD0A3F12634397",
            rsa.apply_sha3_256("Oh boy, here we go again :("));
}

TEST_F(RSATest, EncodeBase64) {
    EXPECT_EQ("YWI=",
            rsa.encode_base64("ab"));
}

TEST_F(RSATest, DecodeBase64) {
    std::string result = rsa.decode_base64("YWI=");
    std::string expected("ab\0");
    EXPECT_EQ(expected, result);
}

TEST_F(RSATest, EncodeDecodeBase64) {
    std::string result = "hello";
    EXPECT_EQ(rsa.decode_base64(rsa.encode_base64(result)), result);
}

TEST_F(RSATest, SignatureDocument) {
    auto keys = rsa.key_generation();
    std::string message = "This is my ultra secret message muahahaha";

    EXPECT_EQ(true, rsa.verify_message(rsa.sign_message(message, keys.private_key), keys.public_key));
}
