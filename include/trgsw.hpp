#ifndef HEADER_TRGSW_
#define HEADER_TRGSW_
#include "util.hpp"
#include "key.hpp"

struct trgsw
{
    trgsw();
    template <RandGen RG>
    static trgsw encrypto(poly mu, secret_key key);
};

#endif