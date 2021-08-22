#include "util.hpp"
#include "trlwe.hpp"

trlwe::trlwe(unsigned int N)
{
    this->N = N;
    this->text = torus_poly(N);
    this->a = torus_poly(N);
}

template <RandGen RG>
trlwe trlwe::encrypto(torus_poly text, secret_key &key, unsigned int N, double alpha, RG &engine)
{
    trlwe instance = trlwe(N);
    size_t i, j;
    torus_poly _a = instance.a;
    for (torus &v : _a)
        v = torus_uniform_dist_val(engine);
    instance.a = _a;
    //TODO 多項式除算
    for (i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (i + j < N)
                text[i + j] += _a[i] * key[i];
            else
                text[i + j] -= _a[i] * key[i];
    for (i = 0; i < N; i++)
        text[i] += torus_modular_normal_dist_val(engine, alpha);

    return instance;
}

template <RandGen RG>
trlwe trlwe::encrypto_bool(bool_poly text, secret_key &key, unsigned int N, double alpha, RG &engine)
{
    const torus mu = 1u << 29;
    torus_poly t;
    size_t i;
    for (i = 0; i < N; i++)
        t[i] = text[i] ? mu : -mu;
    return encrypto(t, key, N, alpha, engine);
}

torus_poly trlwe::decrypto(secret_key &key)
{
    torus_poly deciphertext = text;
    size_t i, j;
    //TODO 多項式除算
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            if (i + j < N)
                deciphertext[i + j] -= a[i] * key[i];
            else
                deciphertext[i + j] += a[i] * key[i];
    return deciphertext;
}

bool_poly trlwe::decrypto_bool(secret_key &key)
{
    torus_poly t = decrypto(key);
    bool_poly deciphertext(this->N);
    size_t i;
    for (i = 0; i < N; i++)
        deciphertext[i] = (t[i] > 0 ? true : false);
    return deciphertext;
}