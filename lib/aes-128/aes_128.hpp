#ifndef AES_128
#define AES_128

#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))

#include <vector>
#include <string>

#include "lib/math/matrix.hpp"
#include "lib/aes-128/defines.hpp"
#include "lib/aes-128/key.hpp"

class AES128
{
    private:
        std::vector<Matrix> create_empty_blocks(unsigned int blocks_number);

        unsigned char _sbox[256] = AES128_SBOX;
        unsigned char _inv_sbox[256] = AES128_INV_SBOX;

        Matrix _mix_colums_matrix, _inv_mix_columns_matrix;
        std::vector<std::vector<MATRIX_TYPE>> _lookup_mix_columns;

        Matrix _galois_multiply(Matrix A, Matrix B);

        std::vector<Matrix> keys_schedule_buffer;
    public:
        AES128();

        //--| AES Functionalities |---
        std::vector<MATRIX_TYPE> encrypt(std::string message, Key key);
        std::string decrypt(std::vector<MATRIX_TYPE> encrypted_text, Key key);

        //--| Data Functions |---
        std::string remove_white_spaces(std::string message);
        std::string create_padding(std::string message);

        std::vector<Matrix> get_blocks_matrix(std::string message);
        std::vector<MATRIX_TYPE> get_blocks_vector(std::vector<Matrix> blocks);

        inline unsigned char look_sbox(unsigned char value){return _sbox[value];};
        inline unsigned char look_inv_sbox(unsigned char value){return _inv_sbox[value];};

        //--| AES Steps |---
        void generate_key_expansion(Key key);

        Matrix round(Matrix block, Matrix key);
        Matrix inv_round(Matrix block, Matrix key);

        Matrix add_round_key(Matrix block, Matrix key);

        Matrix sub_bytes(Matrix block);
        Matrix inv_sub_bytes(Matrix block);

        Matrix shift_rows(Matrix block);
        Matrix inv_shift_rows(Matrix block);

        Matrix mix_columns(Matrix block);
        Matrix inv_mix_columns(Matrix block);
};

#endif
