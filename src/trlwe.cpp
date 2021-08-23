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
template <RandGen RG>
trlwe<P> trlwe<P>::encrypto(torus_poly<P> text, secret_key<P> &key, RG &engine)
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
                text[i + j] -= _a[i] * s[i];
    for (i = 0; i < P::N; i++)
        text[i] += torus_modular_normal_dist_val(engine, P::alpha);

    return instance;
}

template <class P>
template <RandGen RG>
trlwe<P> trlwe<P>::encrypto_bool(bool_poly<P> text, secret_key<P> &key, RG &engine)
{
    const torus mu = 1u << 29;
    torus_poly<P> t;
    size_t i;
    for (i = 0; i < P::N; i++)
        t[i] = text[i] ? mu : -mu;
    return encrypto(t, key, P::N, P::alpha, engine);
}

template <class P>
template <RandGen RG>
trlwe<P> trlwe<P>::encrypto_zero(secret_key<P> &key, RG &engine)
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
                deciphertext[i + j] += a[i] * s[i];
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
