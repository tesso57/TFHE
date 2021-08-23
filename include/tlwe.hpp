#pragma once

#include "util.hpp"
#include <random>
#include <array>
#include "key.hpp"

// P : params, level : TLWE level
template <class P, int level>
struct tlwe
{
    static constexpr size_t N();
    std::array<torus, N()> a;
    torus text;
    tlwe();

    static std::array<torus, tlwe<P, level>::N()> select_key(secret_key<P> &key);
    template <RandGen RG>
    static tlwe<P, level> encrypto(torus text, secret_key<P> &key, RG &engine);
    template <RandGen RG>
    static tlwe<P, level> encrypto_bool(bool text, secret_key<P> &key, RG &engine);
    torus decrypto(secret_key<P> &key);
    bool decrypto_bool(secret_key<P> &key);
};
