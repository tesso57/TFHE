#pragma once
#include <random>
#include <array>
#include <vector>
#include "tlwe.hpp"

template <class P>
struct key_switching_key
{
    std::vector<tlwe<P, 0>> data;
    key_switching_key(std::random_device &engine, secret_key<P> key);
};

template struct key_switching_key<Test>;
template struct key_switching_key<CASE1>;

template <class P>
tlwe<P, 0> identity_key_switch(tlwe<P, 1> &src, key_switching_key<P> &ks);