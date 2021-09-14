#include <array>
#include <random>
#include <iostream>
#include "util.hpp"
#include "trlwe.hpp"
#include "key.hpp"
template <class P>
trlwe<P>::trlwe()
{
    for (auto &v : this->text)
    {
        v = 0;
    }
    for (auto &v : this->a)
    {
        v = 0;
    }
}

template <class P>
trlwe<P> trlwe<P>::encrypto(torus_poly<P> text, secret_key<P> &key, std::random_device &engine)
{
    trlwe<P> instance = trlwe<P>();
    std::array<torus, P::N> &s = key.level1, as, e;

    for (torus &v : instance.a)
        v = torus_uniform_dist_val(engine);
    poly_mult(as, instance.a, s);
    for (auto &v : e)
        v = torus_modular_normal_dist_val(engine, P::alpha);

    for (size_t i = 0; i < P::N; i++)
        text[i] += as[i] + e[i];
    instance.text = text;

    return instance;
}

template <class P>
trlwe<P> trlwe<P>::encrypto_bool(bool_poly<P> text, secret_key<P> &key, std::random_device &engine)
{
    const torus mu = 1u << 29;
    torus_poly<P> t;
    size_t i;
    for (i = 0; i < P::N; i++)
        t[i] = text[i] ? mu : -mu;
    return encrypto(t, key, engine);
}

template <class P>
trlwe<P> trlwe<P>::encrypto_zero(secret_key<P> &key, std::random_device &engine)
{
    //すべてが0の配列
    torus_poly<P> m;
    for (auto &v : m)
    {
        v = 0;
    }
    return encrypto(m, key, engine);
}

template <class P>
torus_poly<P> trlwe<P>::decrypto(secret_key<P> &key)
{
    torus_poly<P> deciphertext, as;
    size_t i;
    std::array<torus, P::N> &s = key.level1;
    poly_mult<torus, torus, torus, P::N>(as, a, s);
    for (i = 0; i < P::N; i++)
    {
        deciphertext[i] = text[i] - as[i];
    }
    return deciphertext;
}

template <class P>
bool_poly<P> trlwe<P>::decrypto_bool(secret_key<P> &key)
{
    torus_poly<P> t = decrypto(key);
    bool_poly<P> deciphertext;
    size_t i;
    for (i = 0; i < P::N; i++)
        deciphertext[i] = (static_cast<int32_t>(t[i]) > 0 ? true : false);
    return deciphertext;
}

template <class P>
void trlwe<P>::decompose(std::array<std::array<torus, P::N>, P::l> &out, torus_poly<P> &a)
{
    torus Bg = P::Bg;
    size_t l = P::l, N = P::N, Bgbit = P::Bgbit, i, j;
    torus offset = 0;
    for (size_t i = 0; i < l; i++)
    {
        offset += Bg / 2 * (1u << (32 - (i + 1) * Bgbit));
    }
    std::array<torus, P::N> a_tilde;
    for (i = 0; i < N; i++)
        a_tilde[i] = a[i] + offset;

    for (i = 0; i < l; i++)
        for (j = 0; j < N; j++)
            out[i][j] =
                ((a_tilde[j] >> (32 - Bgbit * (i + 1))) & (Bg - 1)) - Bg / 2;
}