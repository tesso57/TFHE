// そのうち
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <iostream>
#include "params.hpp"
#include "tlwe.hpp"
#include "key.hpp"
#include "util.hpp"
using namespace std;
// TODO
// int main()
// {
//     std::random_device seed_gen;
//     std::mt19937 engine(seed_gen());
//     double alpha = 3.0517578125e-05;
//     const unsigned int N = 630;
//     secret_key key = std::vector<bool>(N);
//     std::binomial_distribution<unsigned int> dist;
//     for (size_t i = 0; i < N; i++)
//         key[i] = dist(engine);
//     torus m = torus_uniform_dist_val(engine);
//     std::cout << "plaintext >>" << std::endl;
//     std::cout << m << std::endl;
//     tlwe t = tlwe::encrypto(m, key, N, alpha, engine);
//     std::cout << "ciphertext >>" << std::endl;
//     std::cout << t.text << std::endl;
//     std::cout << "deciphertext >>" << std::endl;
//     std::cout << t.decrypto(key) << std::endl;
// }

int main()
{
    std::random_device engine;
    using P = Test;
    auto key = secret_key<P>(engine);
}