#pragma once
#include <random>
#include <array>
#include "tlwe.hpp"

template <class P>
struct key_switching_key
{
    std::array<tlwe<P, 0>, P::N * P::t *((1 << P::basebit) - 1)> data;
    key_switching_key(std::random_device &engine, secret_key<P> key);
};
