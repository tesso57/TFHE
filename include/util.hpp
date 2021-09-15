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

inline torus torus_modular_normal_dist_val(std::random_device &engine, double alpha)
{
    std::normal_distribution<> dist{0.0, alpha};
    return (torus)(fmod(dist(engine), 1.0) * std::pow(2, std::numeric_limits<torus>::digits));
}
inline torus torus_uniform_dist_val(std::random_device &engine)
{
    std::uniform_int_distribution<torus> dist{0, std::numeric_limits<torus>::max()};
    return dist(engine);
}

template <typename T, typename T1, typename T2, size_t N>
void poly_mult(std::array<T, N> &out, std::array<T1, N> &a, std::array<T2, N> &b)
{
    for (T &v : out)
        v = 0;

    size_t i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            if (i + j < N)
                out[i + j] += a[i] * b[j];
            else
                out[i + j - N] -= a[i] * b[j];
}

//動かす
template <typename T, typename T1, size_t N>
void poly_mult_by_X_k(std::array<T, N> &out, std::array<T1, N> &poly, size_t k)
{
    if (k < N)
    {
        for (size_t i = 0; i < N - k; i++)
            out[i + k] = poly[i];
        for (size_t i = N - k; i < N; i++)
            out[i + k - N] = -poly[i];
    }
    else
    {
        size_t l = k - N;
        for (size_t i = 0; i < N - l; i++)
            out[i + l] = -poly[i];
        for (size_t i = N - l; i < N; i++)
            out[i + l - N] = poly[i];
    }
}
