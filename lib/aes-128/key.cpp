#include "key.hpp"

#include <random>
#include <limits>
#include <iostream>
#include <array>

std::array<uint8_t, 16> Key::generate_key_128()
{
	std::array<uint8_t, 16> key;
	for(int i = 0; i < 16; ++i)
	{
		std::random_device dev;
        std::mt19937 random_value(dev());
        std::uniform_int_distribution<uint32_t> distribution(0, 255);

		key[i] = distribution(random_value);
	}

	return key;
}

