#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>

#include "lib/aes-128/aes_128.hpp"
#include "lib/math/matrix.hpp"
#include "lib/aes-128/key.hpp"
#include "lib/aes-128/defines.hpp"

//--| AES Functionalities |---
std::vector<MATRIX_TYPE> AES128::encrypt(std::string message, Key key)
{
    std::vector<Matrix> blocks_message = get_blocks_matrix(message);
    std::vector<Matrix> blocks_result;

    for(unsigned int index_block = 0; index_block < blocks_message.size(); ++index_block)
    {
        Matrix block = add_round_key(blocks_message[index_block], key);
        block = sub_bytes(block);
        block = shift_rows(block);
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

        (inv_sub_bytes(inv_shift_rows(block)) ^ key.key_matrix()).display();
        std::cout << "\n";
    }
    return "";
}

//---| AES Steps |---
Matrix AES128::add_round_key(Matrix block, Key key)
{
    return block ^ key.key_matrix();
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
    std::string input = remove_white_spaces(message);
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
