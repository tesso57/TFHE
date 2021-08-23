#ifndef HEADER_TRGSW_
#define HEADER_TRGSW_
#include "util.hpp"
#include "key.hpp"
#include "trlwe.hpp"
template <class P>
struct trgsw
{
    std::array<trlwe<P>, P::l> data;
    trgsw();
    template <RandGen RG>
    static trgsw<P> encrypto(poly<P> mu, secret_key<P> key, RG &engine);
};

#endif