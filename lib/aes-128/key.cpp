#include "lib/aes-128/key.hpp"

#include <random>
#include <limits>
#include <iostream>
#include <array>

Key::Key()
{
	generate_key_128();
}

void Key::generate_key_128()
{
	for(int i = 0; i < 16; ++i)
	{
		std::random_device dev;
        std::mt19937 random_value(dev());
        std::uniform_int_distribution<uint32_t> distribution(0, 255);

		_key[i] = distribution(random_value);
	}
}

Matrix Key::key_matrix()
{
    Matrix key_matrix(MATRIX_SIZE, MATRIX_SIZE);

    unsigned int row = 0, col = 0;
    for(unsigned int k_index = 0; k_index < _key.size(); ++k_index)
    {
        key_matrix.data[row][col] = _key[k_index];
        if(col >= MATRIX_SIZE)
		{
			row += 1;
			key_matrix.data[row][0] = _key[k_index];
			col = 1;
		}
		else col += 1;
    }

    return key_matrix;
}

