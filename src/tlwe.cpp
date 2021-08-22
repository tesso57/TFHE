#include <random>
#include <math.h>
#include <iostream>
#include <vector>
#include "util.hpp"
#include "tlwe.hpp"
#include "params.hpp"
#include "key.hpp"

// P : params
template <class P, int level>
tlwe<P, level>::tlwe()
{
    this->text = 0;
    this->a = std::vector<torus>(P::N);
}
template <RandGen RG, class P, int level>
tlwe<P, level> tlwe<P, level>::encrypto(torus text, secret_key &key, RG &engine)
{
    tlwe instance = tlwe(N);
    size_t i;
    for (torus &v : instance.a)
        v = torus_uniform_dist_val(engine);
    text += torus_modular_normal_dist_val(engine, alpha);
    for (i = 0; i < N; i++)
        text += instance.a[i] * key[i];

    instance.text = text;
    return instance;
}
template <RandGen RG>
tlwe tlwe::encrypto_bool(bool text, secret_key &key, RG &engine)
{
    const torus mu = 1u << 29;
    return encrypto(text ? mu : -mu, key, N, alpha, engine);
}

torus tlwe::decrypto(secret_key &key)
{
    torus deciphertext = this->text;
    size_t i;
    for (i = 0; i < N; i++)
        deciphertext -= a[i] * key[i];
    return deciphertext;
}

bool tlwe::decrypto_bool(secret_key &key)
{
    return decrypto(key) > 0;
}
