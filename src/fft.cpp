#include "fft.hpp"
#include "util.hpp"
#include "params.hpp"

#include <array>
#include <iostream>
#include <cmath>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
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

template <size_t Nbit>
void fft(double *const res, size_t step)
{
    uint32_t size = 1 << (Nbit - step);
    double *const res0 = &res[0];
    double *const res1 = &res[size / 2];
    for (size_t i = 0; i < size / 2; i++)
    {
        butterflyAdd<Nbit, false>(res0 + i, res1 + i, std::cos(-2. * M_PI * i / static_cast<double>(size)), std::sin(-2. * M_PI * i / static_cast<double>(size)));
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
        double are = static_cast<double>(static_cast<int64_t>(a[i]));
        double aim = static_cast<double>(static_cast<int64_t>(a[i + N / 2]));
        double rre = cos(i * M_PI / static_cast<double>(N));
        double rim = sin(i * M_PI / static_cast<double>(N));
        double aimrim = aim * rim;
        double aimrre = aim * rre;
        res[i] = std::fma(are, rre, -aimrim);
        res[i + N / 2] = std::fma(are, rim, aimrre);
    }
    fft<Nbit - 1>(res.data(), 0);
}
template <uint32_t Nbit>
void ifft(double *const res, size_t step)
{
    uint32_t size = 1 << (Nbit - step);
    double *const res0 = &res[0];
    double *const res1 = &res[size / 2];
    if (size > 2)
    {
        ifft<Nbit>(res0, step + 1);
        ifft<Nbit>(res1, step + 1);
    }
    for (size_t i = 0; i < size / 2; i++)
    {
        butterflyAdd<Nbit, true>(res0 + i, res1 + i, std::cos(-2. * M_PI * i / static_cast<double>(size)), std::sin(2. * M_PI * i / static_cast<double>(size)));
    }
}

template <class P>
void IFFT(std::array<torus, P::N> &res, std::array<double, P::N> &a)
{
    constexpr size_t N = P::N, Nbit = P::Nbit;
    ifft<Nbit - 1>(a.data(), 0);
    cpp_int intpower = (1ull << 32);
    cpp_int x;
    //重みづけ
    for (size_t i = 0; i < N / 2; i++)
    {
        double are = a[i];
        double aim = a[i + N / 2];
        double rre = cos(i * M_PI / N);
        double rim = -sin(i * M_PI / N);
        double tmp = std::fma(are, rre, -aim * rim) * (2.0 / N);
        x.assign(tmp);
        x %= intpower;
        if (x < 0)
        {
            x += intpower;
        }
        res[i] = static_cast<uint64_t>(x);
        tmp = std::fma(are, rim, aim * rre) * (2.0 / N);
        x.assign(tmp);
        x %= intpower;
        if (x < 0)
        {
            x += intpower;
        }
        res[i + N / 2] = static_cast<uint64_t>(x);
    }
}

template <size_t N>
void mul_in_fd(std::array<double, N> &res, const std::array<double, N> &a,
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
void polymult_fft(std::array<torus, P::N> &res, std::array<torus, P::N> &a, std::array<torus, P::N> &b)
{
    constexpr size_t N = P::N;
    std::array<double, N> ffta, fftb, tmp;
    FFT<P>(ffta, a);
    FFT<P>(fftb, b);
    mul_in_fd(tmp, ffta, fftb);
    IFFT<P>(res, tmp);
}

template void polymult_fft<Test>(std::array<torus, Test::N> &res, std::array<torus, Test::N> &a, std::array<torus, Test::N> &b);
template void polymult_fft<CASE1>(std::array<torus, CASE1::N> &res, std::array<torus, CASE1::N> &a, std::array<torus, CASE1::N> &b);

template void IFFT<Test>(std::array<torus, Test::N> &res, std::array<double, Test::N> &a);

template void FFT<Test>(std::array<double, Test::N> &res, std::array<torus, Test::N> &a);