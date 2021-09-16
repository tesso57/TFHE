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

    cout << "External Product" << endl;
    auto m = gen_random_bool_array<P::N>(engine);
    auto c = gen_random_bool_value(engine);
    auto trlwe_ = trlwe<P>::encrypt_bool(m, key, engine);
    auto trgsw_ = trgsw<P>::encrypt_bool(c, key, engine);
    auto res = trgsw<P>::external_product(trgsw_, trlwe_);
    auto res_text = res.decrypt_bool(key);

    bool f = false;
    if (c)
    {
        cout << (m == res_text ? "OK" : "NO") << endl;
    }
    else
    {
        for (auto v : res_text)
        {
            if (v != 0)
            {
                cout << "NO" << endl;
                break;
                f = true;
            }
        }
        if (!f)
            cout << "OK" << endl;
    }

    cout << "CUMX" << endl;
    auto m1 = gen_random_bool_array<P::N>(engine);
    auto m2 = gen_random_bool_array<P::N>(engine);
    auto c2 = gen_random_bool_value(engine);

    auto thn = trlwe<P>::encrypt_bool(m1, key, engine);
    auto els = trlwe<P>::encrypt_bool(m2, key, engine);
    auto cond = trgsw<P>::encrypt_bool(c2, key, engine);

    auto r = cmux(cond, thn, els);
    auto r_text = r.decrypt_bool(key);

    if (c2)
    {
        cout << (m1 == r_text ? "OK" : "NO") << endl;
    }
    else
    {
        cout << (m2 == r_text ? "OK" : "NO") << endl;
    }
}