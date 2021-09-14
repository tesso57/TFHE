#include <random>
#include <iostream>
#include <array>
#include "util.hpp"
#include "trgsw.hpp"
#include "trlwe.hpp"
#include "params.hpp"
#include "key.hpp"
using namespace std;

template <size_t N>
array<bool, N> gen_random_bool_array(std::random_device &engine)
{
    array<bool, N> ret;
    binomial_distribution<int> dist;
    for (bool &v : ret)
        v = dist(engine);
    return ret;
}

bool gen_random_bool_value(std::random_device &engine)
{
    binomial_distribution<int> dist;
    return dist(engine);
}

template <class P>
void dump(std::array<torus, P::N> &ary, bool debug)
{
    if (debug)
    {
        std::cout << "[";
        for (auto v : ary)
            std::cout << v << ", ";
        std::cout << "]" << std::endl;
    }
}

int main()
{
    //test external_product
    std::random_device engine;
    using P = Test;

    auto key = secret_key<P>(engine);

    auto m = gen_random_bool_array<P::N>(engine);
    for (auto &v : m)
    {
        v = 0;
    }
    auto c = gen_random_bool_value(engine);
    auto trlwe_ = trlwe<P>::encrypto_bool(m, key, engine);
    auto trgsw_ = trgsw<P>::encrypto_bool(c, key, engine);
    auto res = trgsw<P>::external_product(trgsw_, trlwe_);
    auto res_text = res.decrypto_bool(key);
    cout << c << endl;
    if (c)
    {
        cout << (m == res_text ? "OK" : "NO") << endl;
        return 0;
    }
    else
    {
        for (auto v : res_text)
        {
            if (v != 0)
            {
                cout << "NO" << endl;
                return 0;
            }
        }
        cout << "OK" << endl;
    }
}