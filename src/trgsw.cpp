#include <array>
#include "trgsw.hpp"
#include "trlwe.hpp"
#include "fft.hpp"
template <class P>
trgsw<P> trgsw<P>::encrypt(poly<P> mu, secret_key<P> &key, std::random_device &engine)
{
    constexpr size_t N = P::N, l = P::l, Bgbit = P::Bgbit;
    trgsw<P> instance;
    for (auto &v : instance.data)
        v = trlwe<P>::encrypt_zero(key, engine);
    for (size_t i = 0; i < l; i++)
        for (size_t j = 0; j < N; j++)
        {
            torus t = static_cast<torus>(mu[j]) * (1u << (32 - (i + 1) * Bgbit));
            instance.data[i].a[j] += t;
            instance.data[l + i].text[j] += t;
        }
    return instance;
}
template <class P>
trgsw<P> trgsw<P>::encrypt_bool(bool mu, secret_key<P> &key, std::random_device &engine)
{
    poly<P> tmp;
    for (auto &v : tmp)
    {
        v = 0;
    }
    tmp[0] = mu ? 1 : 0;
    return trgsw<P>::encrypt(tmp, key, engine);
}

template <class P>
trlwe<P> trgsw<P>::external_product(trgsw<P> C, trlwe<P> in)
{
    std::array<std::array<torus, P::N>, P::l> dec_a, dec_text;
    trlwe<P>::decompose(dec_a, in.a);
    trlwe<P>::decompose(dec_text, in.text);
    trlwe<P> instance;
    torus_poly<P> tmp;
    for (size_t i = 0; i < P::l; i++)
    {
        polymult_fft<P>(tmp, dec_a[i], C.data[i].a);
        for (size_t j = 0; j < P::N; j++)
            instance.a[j] += tmp[j];
        polymult_fft<P>(tmp, dec_text[i], C.data[P::l + i].a);
        for (size_t j = 0; j < P::N; j++)
            instance.a[j] += tmp[j];
        polymult_fft<P>(tmp, dec_a[i], C.data[i].text);
        for (size_t j = 0; j < P::N; j++)
            instance.text[j] += tmp[j];
        polymult_fft<P>(tmp, dec_text[i], C.data[P::l + i].text);
        for (size_t j = 0; j < P::N; j++)
            instance.text[j] += tmp[j];
    }
    return instance;
}

template <class P>
trlwe<P> cmux(trgsw<P> &cond, trlwe<P> &thn, trlwe<P> &els)
{
    trlwe<P> out;
    trlwe<P> tmp0;
    for (size_t i = 0; i < P::N; i++)
    {
        tmp0.a[i] = thn.a[i] - els.a[i];
        tmp0.text[i] = thn.text[i] - els.text[i];
    }
    trlwe<P> tmp1;
    tmp1 = trgsw<P>::external_product(cond, tmp0);

    for (size_t i = 0; i < P::N; i++)
    {
        out.a[i] = tmp1.a[i] + els.a[i];
        out.text[i] = tmp1.text[i] + els.text[i];
    }
    return out;
}

template trlwe<Test> cmux(trgsw<Test> &cond, trlwe<Test> &thn, trlwe<Test> &els);
template trlwe<CASE1> cmux(trgsw<CASE1> &cond, trlwe<CASE1> &thn, trlwe<CASE1> &els);