#pragma once

#include <array>
#include "util.hpp"
template <class P>
struct secret_key
{

    secret_key();
    template <RandGen RG>
    secret_key(RG &engine);
    std::array<torus, P::n> level0;
    std::array<torus, P::N> level1;
};