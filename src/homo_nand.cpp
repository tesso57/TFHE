#include "tlwe.hpp"
#include "trlwe.hpp"
#include "trgsw.hpp"
#include "gatebootstrapping.hpp"
#include "key.hpp"
#include "params.hpp"
#include "keyswitchingkey.hpp"
#include "homo_nand.hpp"

template <class P>
tlwe<P, 0> homo_nand(tlwe<P, 0> &lhs, tlwe<P, 0> &rhs, bootstrapping_key<P> &bkey, key_switching_key<P> &ks)
{
    constexpr size_t n = P::n;
    constexpr torus mu = 1u << 29;

    tlwe<P, 0> input = tlwe<P, 0>();
    for (size_t i = 0; i < n; i++)
    {
        input.a[i] = -lhs.a[i] - rhs.a[i];
    }
    input.text = mu - lhs.text - rhs.text;
    auto tlwe1 = gate_booststrapping_tlwe_to_tlwe(input, bkey);
    return identity_key_switch(tlwe1, ks);
}

template tlwe<Test, 0> homo_nand(tlwe<Test, 0> &lhs, tlwe<Test, 0> &rhs, bootstrapping_key<Test> &bkey, key_switching_key<Test> &ks);
template tlwe<CASE1, 0> homo_nand(tlwe<CASE1, 0> &lhs, tlwe<CASE1, 0> &rhs, bootstrapping_key<CASE1> &bkey, key_switching_key<CASE1> &ks);