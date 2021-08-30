#pragma once
#include <random>
#include <array>
#include "util.hpp"
#include "params.hpp"
template <class P>
struct secret_key
{
    std::array<torus, P::n> level0;
    std::array<torus, P::N> level1;
    secret_key();
    secret_key(std::random_device &engine);
};
template struct secret_key<CASE1>;
template struct secret_key<Test>;
