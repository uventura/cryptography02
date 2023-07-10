#include "lib/rsa/rsa.hpp"

#include <iostream>
#include <utility>
#include <tuple>
#include <gmp.h>
#include <gmpxx.h>

RSA::RSA()
{
    _base64 = BASE64;
    _base64_inv = INV_BASE64;
}

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

    while(true)
    {
        mpz_class p = generate_prime();
        mpz_class q = generate_prime();
        mpz_class n = p;

        mpz_class totient = (p - 1) * (q - 1);
        mpz_class e = E_CONSTANT;

        mpz_class d = -1, k, gcd_v = 1;
        mpz_gcdext(gcd_v.get_mpz_t(), d.get_mpz_t(), k.get_mpz_t(), e.get_mpz_t(), totient.get_mpz_t());

        if(d * e + k * totient == 1)
        {
            keys.private_key = std::make_pair(d, n);
            keys.public_key = std::make_pair(e, n);
            return keys;
        }
    }
}

std::vector<ENCRYPT_TYPE> RSA::transform_text_to_num(std::string text)
{
    std::vector<ENCRYPT_TYPE> result;
    for(unsigned int i=0; i<text.size(); ++i)
    {
        mpz_class converted_value(text[i]);
        result.push_back(converted_value);
    }
    return result;
}

std::string RSA::inv_transform_text_to_num(std::vector<ENCRYPT_TYPE> enc_text)
{
    std::string result="";
    for(unsigned int i=0; i < enc_text.size(); ++i)
    {
        // std::cout << enc_text[i] << "\n";
        size_t size = mpz_size(enc_text[i].get_mpz_t());
        std::vector<char> buffer(size);
        // std::cout << buffer.size() << "\n";
        mpz_export(buffer.data(), nullptr, 1, sizeof(unsigned char), 0, 0, enc_text[i].get_mpz_t());
        result += (char)(buffer[0]);
    }
    return result;
}

std::vector<ENCRYPT_TYPE> RSA::encrypt(std::string message, std::pair<mpz_class, mpz_class> public_key)
{
    std::vector<ENCRYPT_TYPE> result = transform_text_to_num(message);
    for(auto& element:result)
    {
        // Doing c = m^e(mod n)
        mpz_powm(element.get_mpz_t(), element.get_mpz_t(), public_key.first.get_mpz_t(), public_key.second.get_mpz_t());
    }
    return result;
}

std::string RSA::decrypt(std::vector<ENCRYPT_TYPE> encrypted, std::pair<mpz_class, mpz_class> private_key)
{
    std::vector<ENCRYPT_TYPE> result_block;
    for(auto element:encrypted)
    {
        ENCRYPT_TYPE new_element;
        // Doing c = m^e(mod n)
        mpz_powm(new_element.get_mpz_t(), element.get_mpz_t(), private_key.first.get_mpz_t(), private_key.second.get_mpz_t());

        result_block.push_back(new_element);
    }
    return inv_transform_text_to_num(result_block);
}

//---| SIGNATURE |---
std::string RSA::apply_sha3_256(std::string message)
{
    CryptoPP::SHA3_256 sha3;

    // Compute Hash
    sha3.Update(reinterpret_cast<const unsigned char*>(message.data()), message.size());

    // Retrieve Hash
    unsigned char hash[CryptoPP::SHA3_256::DIGESTSIZE];
    sha3.Final(hash);

    // Transform into string
    std::string result;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(result));
    encoder.Put(hash, sizeof(hash));
    encoder.MessageEnd();

    return result;
}

std::string RSA::encode_base64(std::string message)
{
    std::string binary_message = "";
    for(auto letter: message)
    {
        std::string binary_letter = "";
        for(unsigned int shifts = 0; shifts < 8; ++shifts)
        {
            char bit_value = (letter & (1 << shifts)) ? '1':'0';
            binary_letter = bit_value + binary_letter;
        }
        binary_message += binary_letter;
    }

    while (binary_message.size() % 6 != 0) {
        binary_message += '0';
    }

    std::string result = "";
    for (size_t i = 0; i < binary_message.size(); i += 6) {
        std::string sub_binary = binary_message.substr(i, 6);
        char new_value = _base64[sub_binary];
        result += new_value;
    }

    while (result.size() % 4 != 0) {
        result += '=';
    }

    return result;
}

std::string RSA::decode_base64(std::string message)
{
    std::string decoded = "";

    std::string encoded_without_padding = message;
    encoded_without_padding.erase(std::remove(encoded_without_padding.begin(), encoded_without_padding.end(), '='), encoded_without_padding.end());

    std::string binary;
    for (char c : encoded_without_padding) {
        binary += _base64_inv[c];
    }

    for (unsigned int i = 0; i < binary.size(); i += 8) {
        std::string sub_binary = binary.substr(i, 8);
        char decoded_char = 0;
        for (unsigned int j = 0; j < sub_binary.size(); ++j) {
            decoded_char = (decoded_char << 1) | (sub_binary[j] - '0');
        }
        if(decoded_char != 0)
            decoded += decoded_char;
    }
    return decoded;
}

std::vector<ENCRYPT_TYPE> RSA::sign_message(std::string message, std::pair<mpz_class, mpz_class> private_key)
{
    return encrypt(message, private_key);
}
