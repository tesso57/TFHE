#include "trgsw.hpp"
#include "trlwe.hpp"

template <class P>
trgsw<P>::trgsw()
{
}

template <class P>
template <RandGen RG>
trgsw<P> trgsw<P>::encrypto(poly<P> mu, secret_key<P> key, RG &engine)
{
    size_t N = P::N, l = P::l, Bgbit = P::Bgbit, i, j;
    trgsw<P> instance;
    for (auto &v : instance.data)
        v = trlwe<P>::encrypto_zero(key, engine);
    for (i = 0; i < l; i++)
        for (j = 0; j < N; j++)
        {
            torus t = (torus)(mu[j]) * (1u << (32 - (i + 1) * Bgbit));
            instance.data[i].a[j] += t;
            instance.data[l + i].text[j] += t;
        }

    return instance;
}
