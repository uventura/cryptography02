#include <iostream>
#include <array>

class Key
{
	public:
		static std::array<uint8_t, 16> generate_key_128();
};
