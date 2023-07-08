#ifndef AES_128_KEY
#define AES_128_KEY

#include <iostream>
#include <array>

#include "lib/aes-128/defines.hpp"
#include "lib/math/matrix.hpp"

class Key
{
	private:
		std::array<MATRIX_TYPE, KEY_SIZE> _key;

		void generate_key_128();
	public:
		Key();

		inline void set_key(std::array<MATRIX_TYPE, KEY_SIZE> key_val){_key = key_val;};

		Matrix key_matrix();
		inline std::array<MATRIX_TYPE, KEY_SIZE> get_key(){return _key;};

		//---| Key Operators |---
		friend std::ostream& operator<<(std::ostream& stream, Key& key)
		{
			const std::array<MATRIX_TYPE, KEY_SIZE>& key_value = key.get_key();
			for(int i = 0; i < KEY_SIZE; ++i)
				stream << (unsigned int)(key_value[i]) << " ";
			return stream;
		}
};

#endif
