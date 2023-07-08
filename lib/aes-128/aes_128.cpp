#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>

#include "lib/aes-128/aes_128.hpp"
#include "lib/math/matrix.hpp"
#include "lib/aes-128/key.hpp"

AES128::AES128()
{
    generate_sbox();
}

//--| AES Functionalities |---
std::vector<MATRIX_TYPE> AES128::encrypt(std::string message, Key key)
{
    std::vector<Matrix> blocks_message = get_blocks_matrix(message);
    std::vector<Matrix> blocks_result;

    for(unsigned int index_block = 0; index_block < blocks_message.size(); ++index_block)
    {
        Matrix block = add_round_key(blocks_message[index_block], key);
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

        (block ^ key.key_matrix()).display();
        std::cout << "\n";
    }
    return "";
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

// https://en.wikipedia.org/wiki/Rijndael_S-box
void AES128::generate_sbox()
{
    unsigned char p = 1, q = 1;

	/* loop invariant: p * q == 1 in the Galois field */
	do {
		/* multiply p by 3 */
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

		/* divide q by 3 (equals multiplication by 0xf6) */
		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;

		/* compute the affine transformation */
		unsigned char xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);

		_sbox[p] = xformed ^ 0x63;
	} while (p != 1);

	/* 0 is a special case since it has no inverse */
	_sbox[0] = 0x63;
}
