#include <iostream>

#include "lib/math/matrix.hpp"
#include "lib/aes-128/key_schedule.hpp"

Matrix KeySchedule::rot_word(Matrix M)
{
    if(M.data.size() != MATRIX_SIZE)
    {
        std::cout << "KeySchedule::ERROR::Wrong Matrix Size" << std::endl;
        return Matrix();
    }

    Matrix result(MATRIX_SIZE, 1);

    result.data[0][0] = M.data[1][0];
    result.data[1][0] = M.data[2][0];
    result.data[2][0] = M.data[3][0];
    result.data[3][0] = M.data[0][0];

    return result;
}

Matrix KeySchedule::sub_word(Matrix M)
{
    if(M.data.size() != MATRIX_SIZE)
    {
        std::cout << "KeySchedule::ERROR::Wrong Matrix Size" << std::endl;
        return Matrix();
    }

    unsigned char sbox[256] = AES128_SBOX;
    Matrix result(MATRIX_SIZE, 1);

    result.data[0][0] = sbox[M.data[0][0]];
    result.data[1][0] = sbox[M.data[1][0]];
    result.data[2][0] = sbox[M.data[2][0]];
    result.data[3][0] = sbox[M.data[3][0]];

    return result;
}

Matrix KeySchedule::rcon(Matrix M, unsigned int round)
{
    if(M.data.size() != MATRIX_SIZE)
    {
        std::cout << "KeySchedule::ERROR::Wrong Matrix Size" << std::endl;
        return Matrix();
    }

    unsigned char rcon[11] = RCON_11;
    Matrix result(MATRIX_SIZE, 1);

    result.data[0][0] = rcon[round] ^ M.data[0][0];

    return result;
}
