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
using namespace std;

bool gen_random_bool_value(std::random_device &engine)
{
    binomial_distribution<int> dist;
    return dist(engine);
}
int main()
{
    cout << "homo nand" << endl;
    std::random_device engine;
    using P = Test;
    clock_t total, begin, end;
    cout << "generate keys ..." << endl;
    begin = clock();
    auto key = secret_key<P>(engine);
    auto ks = key_switching_key<P>(engine, key);
    auto bkey = bootstrapping_key<P>(engine, key);
    end = clock();
    fprintf(stderr, "%.3f sec\n", (double)(end - begin) / CLOCKS_PER_SEC);

    size_t times = 10, cnt = 0;
    cout << "test homo nand " << times << " times ..." << endl;
    for (size_t i = 0; i < times; i++)
    {
        cout << i + 1 << "/" << times << endl;
        begin = clock();
        bool lhs = gen_random_bool_value(engine);
        bool rhs = gen_random_bool_value(engine);
        auto lhs_tlwe = tlwe<P, 0>::encrypt_bool(lhs, key, engine);
        auto rhs_tlwe = tlwe<P, 0>::encrypt_bool(rhs, key, engine);

        auto res_tlwe = homo_nand(lhs_tlwe, rhs_tlwe, bkey, ks);
        bool res = res_tlwe.decrypt_bool(key);
        end = clock();
        fprintf(stderr, "%.3f sec\n", (double)(end - begin) / CLOCKS_PER_SEC);
        total += end - begin;
        if (res == !(rhs & lhs))
        {
            cnt += 1;
            cout << "YES" << endl;
        }
        else
        {
            cout << "NO" << endl;
        }
    }

    cout << "result : " << cnt << "/" << times << endl;
    if (cnt == times)
    {
        cout << "ALL CLEAR" << endl;
    }
    fprintf(stderr, "%.3f sec\n", (double)(total) / CLOCKS_PER_SEC / (double)times);
}