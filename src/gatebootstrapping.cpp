#include "gatebootstrapping.hpp"
#include "key.hpp"
#include "trgsw.hpp"
#include "trlwe.hpp"
#include "tlwe.hpp"
#include "util.hpp"
template <class P>
bootstrapping_key<P>::bootstrapping_key(std::random_device &engine, secret_key<P> &key)
{
    data = std::vector<trgsw<P>>(P::n);
    for (size_t i = 0; i < P::n; i++)
    {
        data[i] = trgsw<P>::encrypt_bool(key.level0[i], key, engine);
    }
}

template <class P>
bootstrapping_key<P>::bootstrapping_key()
{
}

template <class P>
trlwe<P> blind_rotate(tlwe<P, 0> &src, trlwe<P> &testvec, bootstrapping_key<P> &key)
{
    trlwe<P> out = trlwe<P>();
    constexpr size_t n = P::n, N = P::N, Nbit = P::Nbit;
    const size_t b_tilda = 2 * N - ((src.text + (1u << (31 - Nbit - 1))) >> (32 - Nbit - 1));
    poly_mult_by_X_k<torus, torus, N>(out.a, testvec.a, b_tilda);
    poly_mult_by_X_k<torus, torus, N>(out.text, testvec.text, b_tilda);

    for (size_t i = 0; i < n; i++)
    {
        const size_t a_tilda = (src.a[i] + (1 << (31 - Nbit - 1))) >> (32 - Nbit - 1);
        trlwe<P> trlwe0 = out;

        trlwe<P> trlwe1 = trlwe<P>();
        poly_mult_by_X_k<torus, torus, N>(trlwe1.a, trlwe0.a, a_tilda);
        poly_mult_by_X_k<torus, torus, N>(trlwe1.text, trlwe0.text, a_tilda);

        out = cmux(key.data[i], trlwe1, trlwe0);
    }
    return out;
}
template trlwe<Test> blind_rotate(tlwe<Test, 0> &src, trlwe<Test> &testvec, bootstrapping_key<Test> &key);
template trlwe<CASE1> blind_rotate(tlwe<CASE1, 0> &src, trlwe<CASE1> &testvec, bootstrapping_key<CASE1> &key);

template <class P>
trlwe<P> gate_bootstrapping_test_vector()
{
    torus mu = 1u << 29; // 1/8
    torus_poly<P> m;
    for (size_t i = 0; i < P::N; i++)
        m[i] = mu;
    return trlwe<P>::encrypt_trivial(m);
}

template trlwe<Test> gate_bootstrapping_test_vector();
template trlwe<CASE1> gate_bootstrapping_test_vector();

template <class P>
tlwe<P, 1> gate_booststrapping_tlwe_to_tlwe(tlwe<P, 0> &src, bootstrapping_key<P> &bkey)
{
    auto testvec = gate_bootstrapping_test_vector<P>();
    auto trlwe_ = blind_rotate(src, testvec, bkey);
    return sample_extract_index(trlwe_, 0);
}

template tlwe<Test, 1> gate_booststrapping_tlwe_to_tlwe(tlwe<Test, 0> &src, bootstrapping_key<Test> &bkey);
template tlwe<CASE1, 1> gate_booststrapping_tlwe_to_tlwe(tlwe<CASE1, 0> &src, bootstrapping_key<CASE1> &bkey);