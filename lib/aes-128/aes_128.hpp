#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>

#include "lib/aes-128/matrix.hpp"
#include "lib/aes-128/defines.hpp"

class AES128
{
    private:
        std::vector<Matrix> create_empty_blocks(unsigned int blocks_number);
    public:
        // Data Functions
        std::string remove_white_spaces(std::string message);
        std::string create_padding(std::string message);
        std::vector<Matrix> get_blocks_matrix(std::string message);

        // AES Steps
        Matrix add_round_key(std::string message, std::array<unsigned char, 16> key);
};
