#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>

#include "lib/aes-128/aes_128.hpp"
#include "lib/math/matrix.hpp"
#include "lib/aes-128/key.hpp"
#include "lib/aes-128/defines.hpp"
#include "lib/aes-128/key_schedule.hpp"

AES128::AES128()
{
    _mix_colums_matrix = Matrix(MATRIX_SIZE, MATRIX_SIZE);
    _mix_colums_matrix.data = MIX_COLUMNS;

    _inv_mix_columns_matrix = Matrix(MATRIX_SIZE, MATRIX_SIZE);
    _inv_mix_columns_matrix.data = MIX_COLUMNS_INVERSE;

    _lookup_mix_columns.resize(15);
    _lookup_mix_columns[2]  = LOOKUP_MIX_COLUMN_2;
    _lookup_mix_columns[3]  = LOOKUP_MIX_COLUMN_3;
    _lookup_mix_columns[9]  = LOOKUP_MIX_COLUMN_9;
    _lookup_mix_columns[11] = LOOKUP_MIX_COLUMN_11;
    _lookup_mix_columns[13] = LOOKUP_MIX_COLUMN_13;
    _lookup_mix_columns[14] = LOOKUP_MIX_COLUMN_14;
}

//--| AES Functionalities |---
std::vector<MATRIX_TYPE> AES128::encrypt(std::string message, Key key)
{
    generate_key_expansion(key);

    std::vector<Matrix> blocks_message = get_blocks_matrix(message);
    std::vector<Matrix> blocks_result;

    for(unsigned int index_block = 0; index_block < blocks_message.size(); ++index_block)
    {
        Matrix block = add_round_key(blocks_message[index_block], key.key_matrix());
        block = sub_bytes(block);
        block = shift_rows(block);
        block = mix_columns(block);
        blocks_result.push_back(block);
    }

    std::vector<MATRIX_TYPE> result = get_blocks_vector(blocks_result);
    return result;
}

std::string AES128::decrypt(std::vector<MATRIX_TYPE> encrypted_text, Key key)
{
    unsigned int step = MATRIX_SIZE * MATRIX_SIZE;
    for(auto vec = encrypted_text.begin(); vec != encrypted_text.end(); vec += step)
    {
        std::vector<MATRIX_TYPE> sub_block_vec(vec, vec + step);
        Matrix block = Matrix::vector_to_matrix(sub_block_vec, MATRIX_SIZE, MATRIX_SIZE);

        (inv_sub_bytes(inv_shift_rows(inv_mix_columns(block))) ^ key.key_matrix()).display();
        std::cout << "\n";
    }
    return "";
}

//---| AES Steps |---
void AES128::generate_key_expansion(Key key)
{
    keys_schedule_buffer.push_back(key.key_matrix());
    for(unsigned int i = 1; i <= ROUNDS; ++i)
    {
        Matrix last_key = keys_schedule_buffer.back();

        Matrix column_key(MATRIX_SIZE, 1);
        column_key.data = {
            {last_key.data[0][3]},
            {last_key.data[1][3]},
            {last_key.data[2][3]},
            {last_key.data[3][3]},
        };

        column_key = KeySchedule::rot_word(column_key);
        column_key = KeySchedule::sub_word(column_key);
        column_key = KeySchedule::rcon(column_key, i);

        auto col_v = column_key.data;

        Matrix next_key(MATRIX_SIZE, MATRIX_SIZE);
        for(unsigned int col = 0; col < MATRIX_SIZE; ++col)
        {
            col_v = {
                {(unsigned char)(col_v[0][0] ^ last_key.data[0][col])},
                {(unsigned char)(col_v[1][0] ^ last_key.data[1][col])},
                {(unsigned char)(col_v[2][0] ^ last_key.data[2][col])},
                {(unsigned char)(col_v[3][0] ^ last_key.data[3][col])}
            };

            next_key.set_element(0, col, col_v[0][0]);
            next_key.set_element(1, col, col_v[1][0]);
            next_key.set_element(2, col, col_v[2][0]);
            next_key.set_element(3, col, col_v[3][0]);
        }

        keys_schedule_buffer.push_back(next_key);
    }
}

Matrix AES128::add_round_key(Matrix block, Matrix key)
{
    return block ^ key;
}

Matrix AES128::sub_bytes(Matrix block)
{
    Matrix result(MATRIX_SIZE, MATRIX_SIZE);
    for(unsigned int row = 0; row < block.data.size(); ++row)
        for(unsigned int col = 0; col < block.data[0].size(); ++col)
            result.data[row][col] = _sbox[block.data[row][col]];
    return result;
}

Matrix AES128::inv_sub_bytes(Matrix block)
{
    Matrix result(MATRIX_SIZE, MATRIX_SIZE);
    for(unsigned int row = 0; row < block.data.size(); ++row)
        for(unsigned int col = 0; col < block.data[0].size(); ++col)
            result.data[row][col] = _inv_sbox[block.data[row][col]];
    return result;
}

Matrix AES128::shift_rows(Matrix block)
{
    Matrix result = block;
    result.shift(1, 1);
    result.shift(2, 2);
    result.shift(3, 3);
    return result;
}

Matrix AES128::inv_shift_rows(Matrix block)
{
    Matrix result = block;
    result.shift(1, -1);
    result.shift(2, -2);
    result.shift(3, -3);
    return result;
}

Matrix AES128::mix_columns(Matrix block)
{
    Matrix result(MATRIX_SIZE, MATRIX_SIZE);
    for(unsigned int col = 0; col < MATRIX_SIZE; ++col)
    {
        Matrix col_matrix(MATRIX_SIZE, 1);
        for(unsigned int row = 0; row < MATRIX_SIZE; ++row)
            col_matrix.data[row][0] = block.data[row][col];

        Matrix col_result = _galois_multiply(_mix_colums_matrix, col_matrix);
        for(unsigned int row = 0; row < MATRIX_SIZE; ++row)
            result.data[row][col] = col_result.data[row][0];
    }
    return result;
}

Matrix AES128::inv_mix_columns(Matrix block)
{
    Matrix result(MATRIX_SIZE, MATRIX_SIZE);
    for(unsigned int col = 0; col < MATRIX_SIZE; ++col)
    {
        Matrix col_matrix(MATRIX_SIZE, 1);
        for(unsigned int row = 0; row < MATRIX_SIZE; ++row)
            col_matrix.data[row][0] = block.data[row][col];

        Matrix col_result = _galois_multiply(_inv_mix_columns_matrix, col_matrix);
        for(unsigned int row = 0; row < MATRIX_SIZE; ++row)
            result.data[row][col] = col_result.data[row][0];
    }
    return result;
}

// PRIVATE
std::vector<Matrix> AES128::create_empty_blocks(unsigned int blocks_number)
{
    std::vector<Matrix> blocks;
    for(unsigned int i = 0; i < blocks_number; ++i)
    {
        blocks.push_back(Matrix(4, 4));
    }
    return blocks;
}

Matrix AES128::_galois_multiply(Matrix A, Matrix B)
{
    // A => 4X4; B => 4X1
    auto matrix_a = A.data;
    auto matrix_b = B.data;

    Matrix result(MATRIX_SIZE, 1);

    for (unsigned int i = 0; i < MATRIX_SIZE; ++i) {
        for (unsigned int k = 0; k < MATRIX_SIZE; ++k) {
            auto table = matrix_a[i][k];
            if(table != 1)
                result.data[i][0] ^= _lookup_mix_columns[table][matrix_b[k][0]];
            else
                result.data[i][0] ^= matrix_b[k][0];
        }
    }

    return result;
}
// PUBLIC

std::string AES128::remove_white_spaces(std::string message)
{
    std::string result = message;
    result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());
    return result;
}

std::string AES128::create_padding(std::string message)
{
    int padding_amount = message.size() % 16;
    if(padding_amount == 0)
        return message;
    padding_amount = 16 - padding_amount;
    for(int i = 0; i < padding_amount; ++i) message += ".";
    return message;
}

std::vector<Matrix> AES128::get_blocks_matrix(std::string message)
{
    // std::string input = remove_white_spaces(message);
    std::string input = message;
    input = create_padding(input);

    std::vector<Matrix> blocks = create_empty_blocks(input.size() / 16);

    long unsigned int index_block = 0;
    unsigned int row = 0, col = 0;

    for(long unsigned int index_letter = 0; index_letter < input.size(); ++index_letter)
    {
        if(index_letter != 0 && index_letter % 16 == 0)
        {
            index_block += 1;

            blocks[index_block].data[0][0] = input[index_letter];

            col = 1;
            row = 0;
        }
        else
        {
            blocks[index_block].data[row][col] = input[index_letter];
            col += 1;
            if(col >= 4)
            {
                col = 0;
                row += 1;
            }
        }
    }

    return blocks;
}

std::vector<MATRIX_TYPE> AES128::get_blocks_vector(std::vector<Matrix> blocks)
{
    std::vector<MATRIX_TYPE> result;
    for(unsigned int i = 0; i < blocks.size(); ++i)
    {
        std::vector<MATRIX_TYPE> block_vector = blocks[i].vector();
        result.insert(result.end(), block_vector.begin(), block_vector.end());
    }

    return result;
}
