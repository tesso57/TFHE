#include "fft.hpp"
#include "util.hpp"
#include "params.hpp"

#include <array>
#include <iostream>

template <size_t Nbit, bool inverse>
void butterflyAdd(double *const a, double *const b, double rre, double rim)
{
    size_t N = (1 << Nbit);
    double *const are = a;
    double *const aim = a + N;
    double *const bre = b;
    double *const bim = b + N;

    if (!inverse)
    {

        const double tmpre = are[0];
        are[0] += bre[0];
        bre[0] = tmpre - bre[0];

        const double tmpim = aim[0];
        aim[0] += bim[0];
        bim[0] = tmpim - bim[0];
        const double temp = std::fma(bre[0], rim, bim[0] * rre);
        bre[0] = std::fma(bre[0], rre, -bim[0] * rim);
        bim[0] = temp;
    }
    else
    {
        const double temp = std::fma(bre[0], rim, bim[0] * rre);
        bre[0] = std::fma(bre[0], rre, -bim[0] * rim);
        bim[0] = temp;

        const double tmpre = are[0];
        are[0] += bre[0];
        bre[0] = tmpre - bre[0];

        const double tmpim = aim[0];
        aim[0] += bim[0];
        bim[0] = tmpim - bim[0];
    }
}

template <size_t Nbit>
void fft(double *const res, size_t step)
{
    uint32_t size = 1 << (Nbit - step);
    double *const res0 = &res[0];
    double *const res1 = &res[size / 2];
    for (size_t i = 0; i < size / 2; i++)
    {
        butterflyAdd<Nbit, false>(res0 + i, res1 + i, std::cos(-2 * M_PI * i / size), std::sin(-2 * M_PI * i / size));
    }

    if (size > 2)
    {
        fft<Nbit>(res0, step + 1);
        fft<Nbit>(res1, step + 1);
    }
}
template <class P>
void FFT(std::array<double, P::N> &res, std::array<torus, P::N> &a)
{
    constexpr size_t N = P::N, Nbit = P::Nbit;
    //重みづけ
    for (size_t i = 0; i < N / 2; i++)
    {
        double are = static_cast<double>(a[i]);
        double aim = static_cast<double>(a[i + N / 2]);
        double rre = cos(i * M_PI / N);
        double rim = sin(i * M_PI / N);
        res[i] = std::fma(are, rre, -aim * rim);
        res[i + N / 2] = std::fma(are, rim, aim * rre);
    }
    fft<Nbit - 1>(res.data(), 0);
}
template <uint32_t Nbit>
void ifft(double *const res, size_t step)
{
    uint32_t size = 1 << (Nbit - step);
    size_t N = 1 << Nbit;
    double *const res0 = &res[0];
    double *const res1 = &res[size / 2];
    if (size > 2)
    {
        ifft<Nbit>(res0, step + 1);
        ifft<Nbit>(res1, step + 1);
    }
    for (size_t i = 0; i < size / 2; i++)
    {
        butterflyAdd<Nbit, true>(res0 + i, res1 + i, std::cos(2 * M_PI * i / size), std::sin(2 * M_PI * i / size));
    }
}

template <class P>
void IFFT(std::array<torus, P::N> &res, std::array<double, P::N> &a)
{
    constexpr size_t N = P::N, Nbit = P::Nbit;

    ifft<Nbit - 1>(a.data(), 0);
    //重みづけ
    for (size_t i = 0; i < N / 2; i++)
    {
        double are = a[i];
        double aim = a[i + N / 2];
        double rre = cos(i * M_PI / N);
        double rim = -sin(i * M_PI / N);
        res[i] = static_cast<uint32_t>(std::fma(are, rre, -aim * rim) * (2.0 / N));
        res[i + N / 2] = static_cast<uint32_t>(std::fma(are, rim, aim * rre) * (2.0 / N));
    }
}

template <class P>
void polymult_fft(std::array<torus, P::N> &res, std::array<torus, P::N> &a, std::array<torus, P::N> &b)
{
    constexpr size_t N = P::N;
    std::array<double, N> ffta;
    FFT<P>(ffta, a);

    std::array<double, N> fftb;
    FFT<P>(fftb, b);

    for (size_t i = 0; i < N / 2; i++)
    {
        double aimbim = ffta[i + N / 2] * fftb[i + N / 2];
        double arebim = ffta[i] * fftb[i + N / 2];
        ffta[i] = std::fma(ffta[i], fftb[i], -aimbim);
        ffta[i + N / 2] = std::fma(ffta[i + N / 2], fftb[i], arebim);
    }
    IFFT<P>(res, ffta);
}

template void polymult_fft<Test>(std::array<torus, Test::N> &res, std::array<torus, Test::N> &a, std::array<torus, Test::N> &b);

template void IFFT<Test>(std::array<torus, Test::N> &res, std::array<double, Test::N> &a);

template void FFT<Test>(std::array<double, Test::N> &res, std::array<torus, Test::N> &a);