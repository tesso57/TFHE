#pragma once

#include "params.hpp"
#include "trgsw.hpp"
#include "key.hpp"
#include "tlwe.hpp"
#include <array>
#include <vector>

template <class P>
struct bootstrapping_key
{
    std::vector<trgsw<P>> data;
    bootstrapping_key();
    bootstrapping_key(std::random_device &engine, secret_key<P> &key);
};

template <class P>
trlwe<P> blind_rotate(tlwe<P, 0> &src, trlwe<P> &testvec, bootstrapping_key<P> &key);

template <class P>
trlwe<P> gate_bootstrapping_test_vector();

template struct bootstrapping_key<Test>;

template <class P>
tlwe<P, 1> gate_booststrapping_tlwe_to_tlwe(tlwe<P, 0> &src, bootstrapping_key<P> &bkey);
