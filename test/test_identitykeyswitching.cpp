#include <iostream>
#include <random>

#include "key.hpp"
#include "keyswitchingkey.hpp"
#include "tlwe.hpp"
using namespace std;

bool gen_random_bool_value(std::random_device &engine)
{
    binomial_distribution<int> dist;
    return dist(engine);
}

int main()
{
    cout << "identity key switching" << endl;
    std::random_device engine;
    using P = Test;
    auto key = secret_key<P>(engine);

    auto ks = key_switching_key<P>(engine, key);
    auto p = gen_random_bool_value(engine);
    auto tlwe1 = tlwe<P, 1>::encrypt_bool(p, key, engine);
    auto tlwe0 = identity_key_switch<P>(tlwe1, ks);
    bool res = tlwe0.decrypt_bool(key);
    cout << (res == p ? "YES" : "NO") << endl;
    return 0;
}