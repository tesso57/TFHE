#pragma once
#include <random>
#include <array>
#include <params.hpp>
using torus = uint32_t;
template <class P>
using torus_poly = std::array<torus, P::N>;
template <class P>
using bool_poly = std::array<bool, P::N>;
template <class P>
using poly = std::array<int, P::N>;

template <class G>
concept RandGen = std::uniform_random_bit_generator<G>;
template <RandGen RG>
inline torus torus_modular_normal_dist_val(RG &engine, double alpha)
{
    std::normal_distribution<> dist{0.0, alpha};
    return (torus)(fmod(dist(engine), 1.0) * std::pow(2, std::numeric_limits<torus>::digits));
}
template <RandGen RG>
inline torus torus_uniform_dist_val(RG &engine)
{
    std::uniform_int_distribution<torus> dist{0, std::numeric_limits<torus>::max()};
    return dist(engine);
}

template <typename T, typename T1, typename T2, size_t N>
void poly_mult(std::array<T, N> &out, std::array<T, N> &a, std::array<T, N> &b)
{
    for (T &v : out)
        v = 0;

    size_t i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            if (i + j < N)
                out[i + j] += a[i] * b[i];
            else
                out[i + j - N] -= a[i] * b[i];
}
