
#include "util.hpp"
#include <random>
#include <array>
#include "key.hpp"

#ifndef HEADER_TLWE_
#define HEADER_TLWE_
// P : params, level : TLWE level
template <class P, int level>
struct tlwe
{
    std::vector<torus> a;
    torus text;
    tlwe();
    static tlwe<P, level> encrypto(torus text, secret_key &key, RG &engine);
    static tlwe<P, level> encrypto_bool(bool text, secret_key &key, RG &engine);
    torus decrypto(secret_key &key);
    bool decrypto_bool(secret_key &key);
};
#endif