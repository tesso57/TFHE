#include "fft.hpp"
#include "util.hpp"
#include "params.hpp"

#include <array>
#include <cmath>
#include <string>

template <size_t N>
constexpr std::array<double, 2 * N> make_table()
{
    std::array<double, 2 * N> ret;
    for (size_t i = 0; i < N; i++)
    {
        ret[i] = cos(2 * i * M_PI / N);
        ret[i + N] = sin(2 * i * M_PI / N);
    }
    return ret;
}

template <size_t N>
constexpr std::array<double, N> make_weight()
{
    std::array<double, N> ret;
    for (size_t i = 0; i < N / 2; i++)
    {
        ret[i] = cos(i * M_PI / N);
        ret[i + N / 2] = sin(i * M_PI / N);
    }
    return ret;
}

template <size_t Nbit, bool inverse>
inline void butterflyAdd(double *const a, double *const b, double rre, double rim)
{
    constexpr size_t N = (1 << Nbit);
    double *const are = a;
    double *const aim = a + N;
    double *const bre = b;
    double *const bim = b + N;

    if (!inverse)
    {

        double tmpre = are[0];
        are[0] += bre[0];
        bre[0] = tmpre - bre[0];

        double tmpim = aim[0];
        aim[0] += bim[0];
        bim[0] = tmpim - bim[0];

        double bimrim = bim[0] * rim;
        double brerim = bre[0] * rim;
        bre[0] = std::fma(bre[0], rre, -bimrim);
        bim[0] = std::fma(bim[0], rre, brerim);
    }
    else
    {
        double bimrim = bim[0] * rim;
        double brerim = bre[0] * rim;
        bre[0] = std::fma(bre[0], rre, -bimrim);
        bim[0] = std::fma(bim[0], rre, brerim);

        double tmpre = are[0];
        are[0] += bre[0];
        bre[0] = tmpre - bre[0];

        double tmpim = aim[0];
        aim[0] += bim[0];
        bim[0] = tmpim - bim[0];
    }
}

template <size_t Nbit, size_t NN>
inline void fft(double *const res, const std::array<double, NN> &table)
{
    constexpr size_t N = (1 << Nbit);
    for (size_t e = 1; e <= Nbit; e++)
    {
        size_t half = 1 << (Nbit - e);
        size_t top = 1 << (Nbit - e + 1);
        for (size_t base = 0; base < N; base += top)
        {
            for (size_t i = base; i < base + half; i++)
            {
                size_t diff = i - base;
                butterflyAdd<Nbit, false>(&res[i], &res[i + half], table[diff * (1 << (e - 1))], table[diff * (1 << (e - 1)) + N]);
            }
        }
    }
}
template <class P>
inline void FFT(std::array<double, P::N> &res, std::array<torus, P::N> &a, const std::array<double, P::N> &table)
{
    constexpr size_t N = P::N, Nbit = P::Nbit;
    const auto weight = make_weight<P::N>();
    //重みづけ
    for (size_t i = 0; i < N / 2; i++)
    {
        double are = static_cast<double>(static_cast<int32_t>(a[i]));
        double aim = static_cast<double>(static_cast<int32_t>(a[i + N / 2]));
        double rre = weight[i];
        double rim = weight[i + N / 2];
        double aimrim = aim * rim;
        double aimrre = aim * rre;
        res[i] = std::fma(are, rre, -aimrim);
        res[i + N / 2] = std::fma(are, rim, aimrre);
    }
    fft<Nbit - 1>(res.data(), table);
}
template <uint32_t Nbit, size_t NN>
inline void ifft(double *const res, const std::array<double, NN> &table)
{
    constexpr size_t N = (1 << Nbit);
    for (size_t e = Nbit; e >= 1; e--)
    {
        size_t half = 1 << (Nbit - e);
        size_t top = 1 << (Nbit - e + 1);
        for (size_t base = 0; base < N; base += top)
        {
            for (size_t i = base; i < base + half; i++)
            {
                size_t diff = i - base;
                butterflyAdd<Nbit, true>(&res[i], &res[i + half], table[diff * (1 << (e - 1))], -table[diff * (1 << (e - 1)) + N]);
            }
        }
    }
}

template <class P>
inline void IFFT(std::array<torus, P::N> &res, std::array<double, P::N> &a, const std::array<double, P::N> &table)
{
    constexpr size_t N = P::N, Nbit = P::Nbit;
    constexpr double power = (1ull << 32);
    const auto weight = make_weight<P::N>();

    ifft<Nbit - 1>(a.data(), table);
    //重みづけ
    for (size_t i = 0; i < N / 2; i++)
    {
        double are = a[i];
        double aim = a[i + N / 2];
        double rre = weight[i];
        double rim = -weight[i + N / 2];
        res[i] = static_cast<uint32_t>(std::fmod(std::round(std::fma(are, rre, -aim * rim) * (2.0 / N)), power));
        res[i + N / 2] = static_cast<uint32_t>(std::fmod(std::round(std::fma(are, rim, aim * rre) * (2.0 / N)), power));
    }
}

template <size_t N>
inline void mul_in_fd(std::array<double, N> &res, const std::array<double, N> &a,
                      const std::array<double, N> &b)
{
    for (size_t i = 0; i < N / 2; i++)
    {
        double aimbim = a[i + N / 2] * b[i + N / 2];
        double arebim = a[i] * b[i + N / 2];
        res[i] = std::fma(a[i], b[i], -aimbim);
        res[i + N / 2] = std::fma(a[i + N / 2], b[i], arebim);
    }
}

template <class P>
inline void polymult_fft(std::array<torus, P::N> &res, std::array<torus, P::N> &a, std::array<torus, P::N> &b)
{
    constexpr size_t N = P::N;
    constexpr auto table = make_table<N / 2>();
    std::array<double, N> ffta, fftb, tmp;
    FFT<P>(ffta, a, table);
    FFT<P>(fftb, b, table);
    mul_in_fd(tmp, ffta, fftb);
    IFFT<P>(res, tmp, table);
}

template void polymult_fft<Test>(std::array<torus, Test::N> &res, std::array<torus, Test::N> &a, std::array<torus, Test::N> &b);
template void polymult_fft<CASE1>(std::array<torus, CASE1::N> &res, std::array<torus, CASE1::N> &a, std::array<torus, CASE1::N> &b);

template void IFFT<Test>(std::array<torus, Test::N> &res, std::array<double, Test::N> &a, const std::array<double, Test::N> &table);

template void FFT<Test>(std::array<double, Test::N> &res, std::array<torus, Test::N> &a, const std::array<double, Test::N> &table);