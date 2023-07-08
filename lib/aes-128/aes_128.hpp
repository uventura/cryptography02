#ifndef AES_128
#define AES_128

#include <vector>
#include <string>

#include "lib/math/matrix.hpp"
#include "lib/aes-128/defines.hpp"
#include "lib/aes-128/key.hpp"

class AES128
{
    private:
        std::vector<Matrix> create_empty_blocks(unsigned int blocks_number);
    public:
        //--| AES Functionalities |---
        std::vector<MATRIX_TYPE> encrypt(std::string message, Key key);

        //--| Data Functions |---
        std::string remove_white_spaces(std::string message);
        std::string create_padding(std::string message);
        std::vector<Matrix> get_blocks_matrix(std::string message);

        //--| AES Steps |---
        Matrix add_round_key(Matrix block, Key key);
};

#endif
