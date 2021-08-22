
#include <random>
#include "util.hpp"
#ifndef HEADER_TLWE_
#define HEADER_TLWE_
struct tlwe
{
    torus N;
    std::vector<torus> a;
    torus text;
    tlwe(unsigned int N);
    template <RandGen RG>
    static tlwe encrypto(torus text, secret_key &key, unsigned int N, double alpha, RG &engine);
    template <RandGen RG>
    static tlwe encrypto_bool(bool text, secret_key &key, unsigned int N, double alpha, RG &engine);
    torus decrypto(secret_key &key);
    bool decrypto_bool(secret_key &key);
};
#endif