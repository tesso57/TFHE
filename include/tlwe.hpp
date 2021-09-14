#pragma once

#include "util.hpp"
#include <random>
#include <array>
#include "key.hpp"

// P : params, level : TLWE level
template <class P, int level>
struct tlwe
{
    static constexpr size_t N()
    {
        if constexpr (level == 0)
            return P::n;
        else
            return P::N;
    }
    std::array<torus, N()> a;
    torus text;
    tlwe();

    static std::array<torus, tlwe<P, level>::N()> select_key(secret_key<P> &key);
    static tlwe<P, level> encrypt(torus text, secret_key<P> &key, std::random_device &engine);
    static tlwe<P, level> encrypt_bool(bool text, secret_key<P> &key, std::random_device &engine);
    torus decrypto(secret_key<P> &key);
    bool decrypto_bool(secret_key<P> &key);
};
template struct tlwe<CASE1, 0>;
template struct tlwe<CASE1, 1>;
template struct tlwe<Test, 0>;
template struct tlwe<Test, 1>;
