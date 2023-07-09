#include "lib/rsa/defines.hpp"

#include <utility>
#include <gmp.h>
#include <gmpxx.h>

struct RSAKey
{
    std::pair<mpz_class, mpz_class> public_key;
    std::pair<mpz_class, mpz_class> private_key;
};

class RSA
{
    private:
        int _bit_length = 1024;
        mpz_class _k_element;

    public:
        bool is_prime(const mpz_class& n, int k = 25);
        mpz_class generate_prime(int bit_length = RSA_BIT_LENGTH);

        RSAKey key_generation();
        inline mpz_class get_k_element(){return _k_element;};
};
