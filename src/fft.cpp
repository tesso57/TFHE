#include "fft.hpp"
#include "util.hpp"
#include "params.hpp"

#include <array>

template <size_t N, bool inverse>
void butterflyAdd(double *const a, double *const b, double rre, double rim)
{
    double are = a[0];
    double aim = a[N];
    double bre = b[0];
    double bim = b[N];

    if (!inverse)
    {

        const double tmpre = are;
        are += bre;
        bre = tmpre - bre;

        const double tmpim = aim;
        aim += bim;
        bim = tmpim - bim;

        const double temp = std::fma(bre, rim, bim * rre);
        bre = std::fma(bre, rre, -bim * rim);
        bim = temp;
    }
    else
    {
        const double temp = std::fma(bre, rim, bim * rre);
        bre = std::fma(bre, rre, -bim * rim);
        bim = temp;

        const double tmpre = are;
        are += bre;
        bre = tmpre - bre;

        const double tmpim = aim;
        aim += bim;
        bim = tmpim - bim;
    }
}

template <size_t Nbit, int step = 0>
void fft(double *const res)
{
    if (Nbit > 32)
        return;
    uint32_t size = 1 << (Nbit - step);
    double *const res0 = &res[0];
    double *const res1 = &res[size / 2];
    for (size_t i = 0; i < size; i++)
    {
        butterflyAdd<Nbit, true>(res0, res1, std::cos(2 * M_PI * i / size), std::sin(2 * M_PI * i / size));
    }
    // if (size > 2)
    // {
    //     fft<Nbit, step + 1>(res0);
    //     fft<Nbit, step + 1>(res1);
    // }
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
    fft<Nbit - 1, 0>(res.data());
}
template <uint32_t Nbit, uint32_t size>
void ifft(double *const res)
{
    double *const res0 = &res[0];
    double *const res1 = &res[size / 2];
    if (size > 2)
    {
        ifft<Nbit, (size >> 1)>(res0);
        ifft<Nbit, (size >> 1)>(res1);
    }

    for (size_t i = 0; i < size; i++)
    {
        butterflyAdd<Nbit, true>(res0, res1, std::cos(2 * M_PI * i / size), std::sin(2 * M_PI * i / size));
    }
}

template <class P>
void IFFT(std::array<torus, P::N> &res, std::array<double, P::N> &a)
{
    constexpr size_t N = P::N, Nbit = P::Nbit;

    ifft<Nbit - 1, (N >> 1)>(a.data());

    //重みづけ
    for (size_t i = 0; i < N / 2; i++)
    {
        double are = static_cast<double>(a[i]);
        double aim = static_cast<double>(a[i + N / 2]);
        double rre = -cos(i * M_PI / N);
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