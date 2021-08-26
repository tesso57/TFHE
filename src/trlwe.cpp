#include <array>
#include <random>
#include "util.hpp"
#include "trlwe.hpp"
#include "key.hpp"
template <class P>
trlwe<P>::trlwe()
{
}

template <class P>
trlwe<P> trlwe<P>::encrypto(torus_poly<P> text, secret_key<P> &key, std::random_device &engine)
{
    trlwe<P> instance = trlwe<P>();
    std::array<torus, P::N> &s = key.level1;
    size_t i, j;
    torus_poly _a = instance.a;

    for (torus &v : _a)
        v = torus_uniform_dist_val(engine);
    instance.a = _a;
    //TODO 多項式除算
    for (i = 0; i < P::N; i++)
        for (int j = 0; j < P::N; j++)
            if (i + j < P::N)
                text[i + j] += _a[i] * s[i];
            else
                text[i + j - P::N] -= _a[i] * s[i];
    for (i = 0; i < P::N; i++)
        text[i] += torus_modular_normal_dist_val(engine, P::alpha);

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
    return encrypto(t, key, P::N, P::alpha, engine);
}

template <class P>
trlwe<P> trlwe<P>::encrypto_zero(secret_key<P> &key, std::random_device &engine)
{
    //すべてが0の配列
    torus_poly<P> m = {0};
    return encrypto(m, key, engine);
}

template <class P>
torus_poly<P> trlwe<P>::decrypto(secret_key<P> &key)
{
    torus_poly<P> deciphertext = text;
    size_t i, j;
    std::array<torus, P::N> &s = key.level1;
    //TODO 多項式除算
    for (i = 0; i < P::N; i++)
        for (j = 0; j < P::N; j++)
            if (i + j < P::N)
                deciphertext[i + j] -= a[i] * s[i];
            else
                deciphertext[i + j - P::N] += a[i] * s[i];
    return deciphertext;
}

template <class P>
bool_poly<P> trlwe<P>::decrypto_bool(secret_key<P> &key)
{
    torus_poly<P> t = decrypto(key);
    bool_poly<P> deciphertext;
    size_t i;
    for (i = 0; i < P::N; i++)
        deciphertext[i] = (t[i] > 0 ? true : false);
    return deciphertext;
}

template <class P>
std::array<torus_poly<P>, P::l> trlwe<P>::decompose(torus_poly<P> &a)
{
    torus Bg = P::Bg;
    size_t l = P::l, N = P::N, Bgbit = P::Bgbit, i, j;
    torus roundoffset = 1 << (32 - l * Bgbit - 1);
    std::array<torus_poly<P>, P::l> a_hat, a_bar;
    //TODO 高速化
    for (i = 0; i < l; i++)
        for (j = 0; j < N; j++)
            a_hat[i][j] = (((a[j] + roundoffset) >> (32 - Bgbit * i)) & (Bg - 1));

    for (i = l - 1; i >= 0; i--)
        for (j = 0; j < N; j++)
        {
            if (a_hat[i][j] >= Bg / 2)
            {
                a_bar[i][j] = a_hat[i][j] - Bg;
                a_hat[i - 1][j]++;
            }
            else
            {
                a_bar[i][j] = a_hat[i][j];
            }
        }
    return a_bar;
}