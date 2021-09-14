#pragma once

#include "params.hpp"
#include "trgsw.hpp"
#include "key.hpp"

template <class P>
struct bootstrapping_key
{
    std::array<trgsw<P>, P::n> data;
    bootstrapping_key();
    bootstrapping_key(std::random_device &engine, secret_key<P> &key);
};

template class bootstrapping_key<Test>;
