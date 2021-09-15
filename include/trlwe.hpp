#pragma once

#include <array>
#include "util.hpp"
#include "key.hpp"
#include "tlwe.hpp"
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
    static trlwe encrypt_trivial(torus_poly<P> text);
    torus_poly<P> decrypto(secret_key<P> &key);
    bool_poly<P> decrypto_bool(secret_key<P> &key);
    static void decompose(std::array<std::array<torus, P::N>, P::l> &out, torus_poly<P> &a);
};
template struct trlwe<Test>;

template <class P>
tlwe<P, 1> sample_extract_index(trlwe<P> &in, size_t k);
