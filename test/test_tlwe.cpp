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
bool gen_random_bool_value(std::random_device &engine)
{
     binomial_distribution<int> dist;
     return dist(engine);
}

int main()
{
     std::random_device engine;
     using P = Test;
     auto key = secret_key<P>(engine);
     {
          cout << "plain" << endl;
          torus text = torus_uniform_dist_val(engine);
          cout << "plaintext >>" << endl
               << text << endl;
          auto t = tlwe<P, 0>::encrypt(text, key, engine);
          cout << "ciphertext >>" << endl
               << t.text << endl
               << "deciphertext >>" << endl
               << t.decrypt(key) << endl;
     }

     {
          cout << "binary" << endl;
          auto text = gen_random_bool_value(engine);
          auto t = tlwe<P, 0>::encrypt_bool(text, key, engine);
          cout << "ciphertext >>" << endl
               << t.text << endl
               << "deciphertext >>" << endl
               << t.decrypt_bool(key) << endl;
          cout << (text == t.decrypt_bool(key) ? "YES" : "NO") << endl;
     }
}