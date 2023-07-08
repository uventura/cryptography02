#ifndef KEY_SCHEDULE_H
#define KEY_SCHEDULE_H

#include "lib/aes-128/defines.hpp"
#include "lib/math/matrix.hpp"

class KeySchedule
{
    public:
        static Matrix rot_word(Matrix column_matrix);
        static Matrix sub_word(Matrix column_matrix);
        static Matrix rcon(Matrix colum_matrix, unsigned int round);
};

#endif
