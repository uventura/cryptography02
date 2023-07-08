#ifndef MATH_RANDOM_H
#define MATH_RANDOM_H

#include <random>
#include <limits>

template <typename T>
class Random
{
    public:
        static T get_random(T min, T max);
};

template <typename T>
T Random<T>::get_random(T min, T max)
{
    std::random_device dev;
    std::mt19937 random_value(dev());
    std::uniform_int_distribution<T> distribution(min, max);

    return distribution(random_value);
}

#endif
