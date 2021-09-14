#include <random>
#include <iostream>
#include "util.hpp"
#include "trlwe.hpp"
#include "params.hpp"
#include "key.hpp"
#define DEBUG false
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

template <class P>
void isEqual(std::array<torus, P::N> &l, std::array<torus, P::N> &r)
{
    size_t i;
    for (i = 0; i < P::N; i++)
    {
        if (l[i] != r[i])
        {
            cout << "Not Equal" << endl;
            return;
        }
    }
    cout << "Equal" << endl;
}

template <class P>
void isEqual_bool(std::array<bool, P::N> &l, std::array<bool, P::N> &r)
{
    size_t i;
    for (i = 0; i < P::N; i++)
    {
        if (l[i] != r[i])
        {
            cout << "Not Equal" << endl;
            return;
        }
    }
    cout << "Equal" << endl;
}

int main()
{
    std::random_device engine;
    using P = Test;
    auto key = secret_key<P>(engine);

    cout << "plain" << endl;
    //平文
    auto plaintext = torus_poly<P>();
    for (auto &v : plaintext)
        v = torus_uniform_dist_val(engine);
    // cout << "plaintext >>" << endl;
    auto t = trlwe<P>::encrypt(plaintext, key, engine);
    auto deciphertext = t.decrypto(key);
    isEqual<P>(plaintext, deciphertext);

    cout << "bool" << endl;
    auto m = gen_random_bool_array<P::N>(engine);
    t = trlwe<P>::encrypt_bool(m, key, engine);
    auto dec = t.decrypto_bool(key);
    isEqual_bool<P>(m, dec);

    cout << "zero" << endl;
    auto z = trlwe<P>::encrypt_zero(key, engine);
    auto zero = z.decrypto(key);
    for (auto v : zero)
    {
        if (v != 0)
        {
            cout << "Not Equal" << endl;
            return 0;
        }
    }
    cout << "Equal" << endl;
}
