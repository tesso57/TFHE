#ifndef HEADER_TRLWE_
#define HEADER_TRLWE_
#include <array>
#include "util.hpp"
#include "key.hpp"
//P : params
template <class P>
struct trlwe
{
    torus_poly<P> a;
    torus_poly<P> text;
    trlwe();
    template <RandGen RG>
    static trlwe encrypto(torus_poly<P> text, secret_key<P> &key, RG &engine);
    template <RandGen RG>
    static trlwe encrypto_bool(bool_poly<P> text, secret_key<P> &key, RG &engine);
    template <RandGen RG>
    static trlwe encrypto_zero(secret_key<P> &key, RG &engine);
    torus_poly<P> decrypto(secret_key<P> &key);
    bool_poly<P> decrypto_bool(secret_key<P> &key);
};
#endif
