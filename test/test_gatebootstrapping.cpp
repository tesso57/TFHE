#include <iostream>
#include <random>

#include "params.hpp"
#include "tlwe.hpp"
#include "trlwe.hpp"
#include "trgsw.hpp"
#include "gatebootstrapping.hpp"
using namespace std;
bool gen_random_bool_value(std::random_device &engine)
{
    binomial_distribution<int> dist;
    return dist(engine);
}
int main()
{
    cout << "bindrotate" << endl;
    std::random_device engine;
    using P = Test;

    auto p = gen_random_bool_value(engine);
    auto key = secret_key<P>(engine);

    auto bkey = bootstrapping_key<P>(engine, key);
    auto input = tlwe<P, 0>::encrypt_bool(p, key, engine);
    auto testvec = gate_bootstrapping_test_vector<P>();
    trlwe<P> res_trlwe = trlwe<P>();
    tlwe<P, 1> res_tlwe = tlwe<P, 1>();
    res_trlwe = blind_rotate(input, testvec, bkey);
    res_tlwe = sample_extract_index(res_trlwe, 0);
    auto res_p = res_tlwe.decrypt_bool(key);

    if (p != res_p)
    {
        cout << "NO" << endl;
    }
    else
    {
        cout << "YES" << endl;
    }
    return 0;
}