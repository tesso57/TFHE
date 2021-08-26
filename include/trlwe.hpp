#pragma once

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
    static trlwe encrypto(torus_poly<P> text, secret_key<P> &key, std::random_device &engine);
    static trlwe encrypto_bool(bool_poly<P> text, secret_key<P> &key, std::random_device &engine);
    static trlwe encrypto_zero(secret_key<P> &key, std::random_device &engine);
    torus_poly<P> decrypto(secret_key<P> &key);
    bool_poly<P> decrypto_bool(secret_key<P> &key);
    static std::array<torus_poly<P>, P::l> decompose(torus_poly<P> &a);
};