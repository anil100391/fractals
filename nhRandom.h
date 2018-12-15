#pragma once
#include <random>

#ifndef _NH_RANDOM_H_
#define _NH_RANDOM_H_

class nhRandomUniformGenerator
{
public:

    nhRandomUniformGenerator()
        : _generator(std::random_device()())
    {}

    std::mt19937& Generator() noexcept
    {
        return _generator;
    }

private:

    // mersenne_twister_engine
    std::mt19937 _generator;
};

#endif // _NH_RANDOM_H_
