#include <random>
#include <iostream>
#include "util.hpp"
#include "trlwe.hpp"
#include "params.hpp"
#include "key.hpp"
#define DEBUG false
using namespace std;
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

int main()
{
    std::random_device engine;
    using P = Test;
    auto key = secret_key<P>(engine);
    //平文
    auto plaintext = torus_poly<P>();
    for (auto &v : plaintext)
        v = torus_uniform_dist_val(engine);
    // cout << "plaintext >>" << endl;
    dump<P>(plaintext, DEBUG);
    auto t = trlwe<P>::encrypto(plaintext, key, engine);
    dump<P>(t.text, DEBUG);
    auto deciphertext = t.decrypto(key);
    dump<P>(deciphertext, DEBUG);
    isEqual<P>(plaintext, deciphertext);
}
