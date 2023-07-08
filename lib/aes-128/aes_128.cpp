#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>

#include "lib/aes-128/aes_128.hpp"
#include "lib/math/matrix.hpp"
#include "lib/aes-128/key.hpp"

//--| AES Functionalities |---
std::vector<MATRIX_TYPE> AES128::encrypt(std::string message, Key key)
{
    std::vector<Matrix> blocks_message = get_blocks_matrix(message);

    blocks_message[0].display();

    add_round_key(blocks_message[0], key).display();

    std::vector<MATRIX_TYPE> result;
    return result;
}

//---| AES Steps |---
Matrix AES128::add_round_key(Matrix block, Key key)
{
    return block ^ key.key_matrix();
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
