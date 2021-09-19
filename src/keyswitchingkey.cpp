#include <random>

#include "keyswitchingkey.hpp"

template <class P>
key_switching_key<P>::key_switching_key(std::random_device &engine, secret_key<P> key)
{
    data = std::vector<tlwe<P, 0>>(P::N * P::t * ((1 << P::basebit) - 1));
    size_t N = P::N, t = P::t, basebit = P::basebit, base = (1 << basebit);
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < t; j++)
        {
            for (size_t k = 1; k <= base - 1; k++)
            {
                torus p = k * key.level1[i] * (1u << (32 - basebit * (j + 1)));
                this->data[(k - 1) + (base - 1) * (j + t * i)] = tlwe<P, 0>::encrypt(p, key, engine);
            }
        }
    }
}

template <class P>
tlwe<P, 0> identity_key_switch(tlwe<P, 1> &src, key_switching_key<P> &ks)
{
    size_t n = P::n, N = P::N, basebit = P::basebit, t = P::t, base = (1 << basebit);
    tlwe<P, 0> out = tlwe<P, 0>();
    out.text = src.text;
    for (size_t i = 0; i < n; i++)
    {
        out.a[i] = 0;
    }

    size_t prec_offset = 1u << (32 - (1 + basebit * t));
    for (size_t i = 0; i < N; i++)
    {
        size_t abar = src.a[i] + prec_offset;
        for (size_t j = 0; j < t; j++)
        {
            size_t k = (abar >> (32 - (j + 1) * basebit)) & ((1u << basebit) - 1);
            if (k == 0)
                continue;
            auto ks_ijk = ks.data[(k - 1) + (base - 1) * (j + t * i)];
            for (size_t l = 0; l < n; l++)
                out.a[l] -= ks_ijk.a[l];
            out.text -= ks_ijk.text;
        }
    }
    return out;
}

template tlwe<Test, 0> identity_key_switch(tlwe<Test, 1> &src, key_switching_key<Test> &ks);
template tlwe<CASE1, 0> identity_key_switch(tlwe<CASE1, 1> &src, key_switching_key<CASE1> &ks);