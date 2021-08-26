#pragma once

#include "util.hpp"
#include "key.hpp"
#include "trlwe.hpp"
template <class P>
struct trgsw
{
    std::array<trlwe<P>, P::l> data;
    trgsw();
    static trgsw<P> encrypto(poly<P> mu, secret_key<P> key, std::random_device &engine);
    static trlwe<P> external_product(trgsw<P> trgsw, trlwe<P> trlwe);
};
