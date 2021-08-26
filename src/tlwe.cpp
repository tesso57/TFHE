#include <random>
#include <array>
#include "util.hpp"
#include "tlwe.hpp"
#include "params.hpp"
#include "key.hpp"

// P : params
template <class P, int level>
tlwe<P, level>::tlwe()
{
    this->text = 0;
}

template <class P, int level>
constexpr size_t tlwe<P, level>::N()
{
    if constexpr (level == 0)
        return P::n;
    else
        return P::N;
}

template <class P, int level>
std::array<torus, tlwe<P, level>::N()> tlwe<P, level>::select_key(secret_key<P> &key)
{
    if constexpr (level == 0)
        return key.level0;
    else
        return key.level1;
}

template <class P, int level>
tlwe<P, level> tlwe<P, level>::encrypto(torus text, secret_key<P> &key, std::random_device &engine)
{
    tlwe instance = tlwe<P, level>();
    size_t i;
    std::array<torus, N()> &s = select_key(key);
    for (torus &v : instance.a)
        v = torus_uniform_dist_val(engine);
    text += torus_modular_normal_dist_val(engine, P::alpha);
    for (i = 0; i < P::N; i++)
        text += instance.a[i] * s[i];

    instance.text = text;
    return instance;
}

template <class P, int level>
tlwe<P, level> tlwe<P, level>::encrypto_bool(bool text, secret_key<P> &key, std::random_device &engine)
{
    const torus mu = 1u << 29;
    return encrypto(text ? mu : -mu, key, P::N, P::alpha, engine);
}

template <class P, int level>
torus tlwe<P, level>::decrypto(secret_key<P> &key)
{
    torus deciphertext = this->text;
    size_t i;
    std::array<torus, N()> &s = select_key(key);

    for (i = 0; i < P::N; i++)
        deciphertext -= a[i] * s[i];
    return deciphertext;
}

template <class P, int level>
bool tlwe<P, level>::decrypto_bool(secret_key<P> &key)
{
    return decrypto(key) > 0;
}
