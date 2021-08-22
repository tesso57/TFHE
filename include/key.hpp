#ifndef HEADER_KEY_
#define HEADER_KEY_
#include <array>
#include "util.hpp"
template <class P>
struct secret_key
{

    secret_key();
    std::array<torus, P::n> level0;
    std::array<torus, P::N> level1;
};
#endif