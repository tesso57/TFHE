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
    using P = Test;
    std::random_device engine;
    // {
    //     cout << "fft test" << endl;
    //     auto res = array<torus, P::N>();
    //     auto ans = array<torus, P::N>();
    //     for (torus i = 0; i < P::N; i++)
    //     {
    //         ans[i] = torus_uniform_dist_val(engine);
    //         cout << ans[i] << " ";
    //     }
    //     cout << endl;
    //     std::array<double, P::N> ffta;
    //     FFT<P>(ffta, ans);
    //     IFFT<P>(res, ffta);
    //     cout << (res == ans ? "Yes" : "No") << endl;
    // }

    {
        cout << "polymult test" << endl;
        auto res = array<torus, P::N>();
        auto ans = array<torus, P::N>();

        auto a = array<torus, P::N>();
        auto b = array<torus, P::N>();
        for (auto &v : a)
        {
            v = torus_uniform_dist_val(engine);
            cout << v << " ";
        }
        cout << endl;
        for (auto &v : b)
        {
            v = torus_uniform_dist_val(engine);
            cout << v << " ";
        }
        cout << endl;
        poly_mult(ans, a, b);
        cout << "ANS" << endl;
        for (auto v : ans)
        {
            cout << v << endl;
        }
        cout << "FFT" << endl;
        polymult_fft<P>(res, a, b);
        cout << (res == ans ? "Yes" : "No") << endl;
    }
}