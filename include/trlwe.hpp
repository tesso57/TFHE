#ifndef HEADER_TRLWE_
#define HEADER_TRLWE_
#include "util.hpp"

struct trlwe
{
    torus N;
    torus_poly a;
    torus_poly text;
    trlwe(unsigned int N);
    template <RandGen RG>
    static trlwe encrypto(torus_poly text, secret_key &key, unsigned int N, double alpha, RG &engine);
    template <RandGen RG>
    static trlwe encrypto_bool(bool_poly text, secret_key &key, unsigned int N, double alpha, RG &engine);
    torus_poly decrypto(secret_key &key);
    bool_poly decrypto_bool(secret_key &key);
};
#endif
