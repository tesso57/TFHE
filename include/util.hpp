#ifndef HEADER_UTIL_
#define HEADER_UTIL_
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
template <class P>
inline std::array<torus_poly<P>, P::l> decompose(torus_poly<P> &a)
{
    torus Bg = P::Bg;
    size_t l = P::l, N = P::N, Bgbit = P::Bgbit, i, j;
    torus roundoffset = 1 << (32 - l * Bgbit - 1);
    std::array<torus_poly<P>, P::l> a_hat, a_bar;
    //TODO 高速化
    for (i = 0; i < l; i++)
        for (j = 0; j < N; j++)
            a_hat[i][j] = (((a[j] + roundoffset) >> (32 - Bgbit * i)) & (Bg - 1));

    for (i = l - 1; i >= 0; i--)
        for (j = 0; j < N; j++)
        {
            if (a_hat[i][j] >= Bg / 2)
            {
                a_bar[i][j] = a_hat[i][j] - Bg;
                a_hat[i - 1][j]++;
            }
            else
            {
                a_bar[i][j] = a_hat[i][j];
            }
        }
    return a_bar;
}
#endif