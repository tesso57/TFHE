#include <array>
#include <random>
#include "util.hpp"
#include "trlwe.hpp"
#include "trgsw.hpp"
#include "key.hpp"
#include "tlwe.hpp"
#include "fft.hpp"
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
trlwe<P> trlwe<P>::encrypt(torus_poly<P> text, secret_key<P> &key, std::random_device &engine)
{
    trlwe<P> instance = trlwe<P>();
    std::array<torus, P::N> &s = key.level1, as, e;

    for (torus &v : instance.a)
        v = torus_uniform_dist_val(engine);
    polymult_fft<P>(as, instance.a, s);
    for (auto &v : e)
        v = torus_modular_normal_dist_val(engine, P::alpha_bk);

    for (size_t i = 0; i < P::N; i++)
        text[i] += as[i] + e[i];
    instance.text = text;

    return instance;
}

template <class P>
trlwe<P> trlwe<P>::encrypt_bool(bool_poly<P> text, secret_key<P> &key, std::random_device &engine)
{
    const torus mu = 1u << 29;
    torus_poly<P> t;
    size_t i;
    for (i = 0; i < P::N; i++)
        t[i] = text[i] ? mu : -mu;
    return encrypt(t, key, engine);
}

template <class P>
trlwe<P> trlwe<P>::encrypt_zero(secret_key<P> &key, std::random_device &engine)
{
    //すべてが0の配列
    torus_poly<P> m;
    for (auto &v : m)
    {
        v = 0;
    }
    return encrypt(m, key, engine);
}

template <class P>
trlwe<P> trlwe<P>::encrypt_trivial(torus_poly<P> text)
{
    trlwe<P> instance = trlwe<P>();
    for (size_t i = 0; i < P::N; i++)
    {
        instance.a[i] = 0;
    }
    instance.text = text;
    return instance;
}

template <class P>
torus_poly<P> trlwe<P>::decrypt(secret_key<P> &key)
{
    torus_poly<P> deciphertext, as;
    size_t i;
    std::array<torus, P::N> &s = key.level1;
    polymult_fft<P>(as, a, s);
    for (i = 0; i < P::N; i++)
    {
        deciphertext[i] = text[i] - as[i];
    }
    return deciphertext;
}

template <class P>
bool_poly<P> trlwe<P>::decrypt_bool(secret_key<P> &key)
{
    torus_poly<P> t = decrypt(key);
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

template <class P>
tlwe<P, 1> sample_extract_index(trlwe<P> &in, size_t k)
{
    size_t N = P::N;
    tlwe<P, 1> out = tlwe<P, 1>();

    out.text = in.text[k];
    for (size_t i = 0; i < N; i++)
    {
        if (i <= k)
        {
            out.a[i] = in.a[k - i];
        }
        else
        {
            out.a[i] = -in.a[N + k - i];
        }
    }
    return out;
}

template tlwe<Test, 1> sample_extract_index(trlwe<Test> &in, size_t k);
template tlwe<CASE1, 1> sample_extract_index(trlwe<CASE1> &in, size_t k);