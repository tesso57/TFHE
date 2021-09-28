#include <random>
#include <iostream>
#include <time.h>
#include <stdio.h>

#include "tlwe.hpp"
#include "trlwe.hpp"
#include "trgsw.hpp"
#include "gatebootstrapping.hpp"
#include "key.hpp"
#include "params.hpp"
#include "keyswitchingkey.hpp"
#include "homo_nand.hpp"
#include "fft.hpp"
using namespace std;
int main()
{
    cout << "fft test" << endl;
    using P = Test;
    std::random_device engine;
    auto res = array<torus, P::N>();
    auto ans = array<torus, P::N>();

    auto a = array<torus, P::N>();
    for (torus i = 0; i < 16; i++)
    {
        a[i] = i + 1;
    }
    auto b = array<torus, P::N>();
    for (torus i = 0; i < 16; i++)
    {
        b[i] = i + 1;
    }
    // polymult_fft<P>(res, a, b);
    // poly_mult<torus, torus, torus, P::N>(ans, a, b);
    std::array<double, P::N> ffta;
    FFT<P>(ffta, a);
    IFFT<P>(res, ffta);

    cout << (res == ans ? "Yes" : "No") << endl;
}