#include "lib/rsa/rsa.hpp"

#include <iostream>
#include <utility>
#include <tuple>
#include <gmp.h>
#include <gmpxx.h>

bool RSA::is_prime(const mpz_class& n, int k)
{
    if (n <= 1) {
        return false;
    }
    if (n <= 3) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }

    mpz_class d = n - 1;
    int r = 0;
    while (d % 2 == 0) {
        d /= 2;
        r++;
    }

    for (int i = 0; i < k; i++) {
        mpz_class a;
        gmp_randstate_t state;
        gmp_randinit_default(state);
        gmp_randseed_ui(state, time(NULL));
        mpz_urandomm(a.get_mpz_t(), state, n.get_mpz_t());
        gmp_randclear(state);
        if (a < 2) {
            a += 2;
        }

        mpz_class x;
        mpz_powm(x.get_mpz_t(), a.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
        if (x == 1 || x == n - 1) {
            continue;
        }

        bool composite = true;
        for (int j = 0; j < r - 1; j++) {
            mpz_powm_ui(x.get_mpz_t(), x.get_mpz_t(), 2, n.get_mpz_t());
            if (x == n - 1) {
                composite = false;
                break;
            }
        }

        if (composite) {
            return false;
        }
    }

    return true;
}

mpz_class RSA::generate_prime(int bit_length) {
    mpz_class p;
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));

    do {
        mpz_urandomb(p.get_mpz_t(), state, bit_length);
        mpz_setbit(p.get_mpz_t(), bit_length - 1);
        mpz_setbit(p.get_mpz_t(), 0);
    } while (!is_prime(p));

    gmp_randclear(state);
    return p;
}

RSAKey RSA::key_generation()
{
    RSAKey keys;

    mpz_class p = generate_prime();
    mpz_class q = generate_prime();
    mpz_class n = p;

    mpz_class totient = (p - 1) * (q - 1);
    mpz_class e = E_CONSTANT;

    mpz_class d = -1, gcd_v = 1;
    mpz_gcdext(gcd_v.get_mpz_t(), d.get_mpz_t(), _k_element.get_mpz_t(), e.get_mpz_t(), totient.get_mpz_t());

    keys.private_key = std::make_pair(d, n);
    keys.public_key = std::make_pair(e, n);

    return keys;
}
