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
void poly_mult(std::array<T, N> &out, std::array<T, N> &a, std::array<T, N> &b)
{
    for (T &v : out)
        v = 0;

    size_t i = 0, j = 0;
    for (auto v : a)
    {
        for (auto v2 : b)
        {
            if (i + j < N)
                out[i + j] += v * v2;
            else
                out[i + j - N] -= v * v2;
            j++;
        }
        i++;
        j = 0;
    }
}
