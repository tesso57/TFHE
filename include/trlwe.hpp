#pragma once

#include <array>
#include "util.hpp"
#include "key.hpp"
#include "params.hpp"
template <class P>
struct trlwe
{
    torus_poly<P> a;
    torus_poly<P> text;
    trlwe();
    static trlwe encrypt(torus_poly<P> text, secret_key<P> &key, std::random_device &engine);
    static trlwe encrypt_bool(bool_poly<P> text, secret_key<P> &key, std::random_device &engine);
    static trlwe encrypt_zero(secret_key<P> &key, std::random_device &engine);
    torus_poly<P> decrypto(secret_key<P> &key);
    bool_poly<P> decrypto_bool(secret_key<P> &key);
    static void decompose(std::array<std::array<torus, P::N>, P::l> &out, torus_poly<P> &a);
};
template struct trlwe<Test>;
