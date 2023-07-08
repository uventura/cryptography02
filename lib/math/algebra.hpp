#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <cstdlib>

class Algebra
{
    public:
        static int mod(int dividend, int divisor);
};

int Algebra::mod(int dividend, int divisor)
{
    divisor = std::abs(divisor);

    int remainder = dividend % divisor;
    if (remainder < 0) {
        remainder += std::abs(divisor);
    }

    return remainder;
}

#endif
