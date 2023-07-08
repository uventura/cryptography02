#include <iostream>
#include <array>

#include "lib/aes-128/defines.hpp"

class Key
{
	public:
		static std::array<MATRIX_TYPE, 16> generate_key_128();
};
