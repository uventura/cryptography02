#include "lib/rsa/defines.hpp"

#include <utility>
#include <gmp.h>
#include <gmpxx.h>
#include <vector>
#include <map>

#include <cryptopp/sha3.h>
#include <cryptopp/hex.h>

struct RSAKey
{
    std::pair<mpz_class, mpz_class> public_key;
    std::pair<mpz_class, mpz_class> private_key;
};

class RSA
{
    private:
        int _bit_length = 1024;
        std::map<std::string, char> _base64;
        std::map<char, std::string> _base64_inv;
    public:
        RSA();

        bool is_prime(const mpz_class& n, int k = 25);
        mpz_class generate_prime(int bit_length = RSA_BIT_LENGTH);

        RSAKey key_generation();

        std::vector<ENCRYPT_TYPE> transform_text_to_num(std::string text);
        std::string inv_transform_text_to_num(std::vector<ENCRYPT_TYPE> enc_text);
        std::vector<ENCRYPT_TYPE> encrypt(std::string message, std::pair<mpz_class, mpz_class> public_key);
        std::string decrypt(std::vector<ENCRYPT_TYPE> encrypted, std::pair<mpz_class, mpz_class> private_key);

        // Signature
        std::string apply_sha3_256(std::string message);
        std::string encode_base64(std::string message);
        std::string decode_base64(std::string message);
        std::vector<ENCRYPT_TYPE> sign_message(std::string message, std::pair<mpz_class, mpz_class> private_key);
};
