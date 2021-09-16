#include <random>
#include <array>
#include <iostream>
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
std::array<torus, tlwe<P, level>::N()> tlwe<P, level>::select_key(secret_key<P> &key)
{
    if constexpr (level == 0)
        return key.level0;
    else
        return key.level1;
}

template <class P, int level>
tlwe<P, level> tlwe<P, level>::encrypt(torus text, secret_key<P> &key, std::random_device &engine)
{
    tlwe instance = tlwe<P, level>();
    std::array<torus, N()> s = select_key(key);
    text += torus_modular_normal_dist_val(engine, P::alpha);

    for (auto &v : instance.a)
    {
        v = torus_uniform_dist_val(engine);
    }
    auto it_a = instance.a.begin();
    auto it_s = s.begin();
    while (it_a != instance.a.end() && it_s != s.end())
    {
        text += (*it_a) * (*it_s);
        it_s++;
        it_a++;
    }

    instance.text = text;
    return instance;
}

template <class P, int level>
tlwe<P, level> tlwe<P, level>::encrypt_bool(bool text, secret_key<P> &key, std::random_device &engine)
{
    const torus mu = 1u << 29;
    return encrypt(text ? mu : -mu, key, engine);
}

template <class P, int level>
torus tlwe<P, level>::decrypt(secret_key<P> &key)
{
    torus deciphertext = this->text;
    std::array<torus, N()> s = select_key(key);
    auto it_a = a.begin();
    auto it_s = s.begin();
    while (it_a != a.end() && it_s != s.end())
    {
        deciphertext -= (*it_a) * (*it_s);
        it_s++;
        it_a++;
    }

    return deciphertext;
}

template <class P, int level>
bool tlwe<P, level>::decrypt_bool(secret_key<P> &key)
{
    return static_cast<int32_t>(decrypt(key)) > 0;
}
