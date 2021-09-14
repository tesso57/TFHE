#pragma once

#include "util.hpp"
#include "key.hpp"
#include "trlwe.hpp"
#include "params.hpp"
template <class P>
struct trgsw
{
    std::array<trlwe<P>, 2 * P::l> data;
    trgsw();
    static trgsw<P> encrypto(poly<P> mu, secret_key<P> &key, std::random_device &engine);
    static trgsw<P> encrypto_bool(bool mu, secret_key<P> &key, std::random_device &engine);
    static trlwe<P> external_product(trgsw<P> trgsw, trlwe<P> trlwe);
};

template struct trgsw<Test>;