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

int main()
{
     std::random_device engine;
     using P = Test;
     auto key = secret_key<P>(engine);
     torus text = torus_uniform_dist_val(engine);
     cout << "plaintext >>" << endl
          << text << endl;
     auto t = tlwe<P, 0>::encrypt(text, key, engine);
     cout << "ciphertext >>" << endl
          << t.text << endl
          << "deciphertext >>" << endl
          << t.decrypto(key) << endl;
}