#ifndef HEADER_TRGSW_
#define HEADER_TRGSW_
#include "util.hpp"
#include "key.hpp"
template <class P>
struct trgsw
{
    trgsw();
    template <RandGen RG>
    static trgsw<P> encrypto(poly<P> mu, secret_key<P> key);
};

#endif