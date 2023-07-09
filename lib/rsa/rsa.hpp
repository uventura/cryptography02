#include "lib/rsa/defines.hpp"

#include <gmp.h>
#include <gmpxx.h>

class RSA
{
    private:
        int _bit_length = 1024;
    public:
        bool is_prime(const mpz_class& n, int k = 25);
        mpz_class generate_prime(int bit_length = RSA_BIT_LENGTH);
};
