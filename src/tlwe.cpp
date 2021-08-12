#include <random>
#include <math.h>
#include <iostream>
#include <vector>
#include "util.hpp"

std::random_device seed_gen;
std::mt19937 engine(seed_gen());
double alpha = 3.0517578125e-05;
const unsigned int N = 630;

struct tlwe
{
    torus N;
    std::vector<torus> a;
    torus text;
    template <RandGen RG>
    tlwe(torus N)
    {
        this->N = N;
        this->text = 0;
        this->a = std::vector<torus>(N);
    }
    template <RandGen RG>
    static tlwe encrypto(torus text, secret_key &key, unsigned int N, torus alpha, RG &engine)
    {
        tlwe instance(N);
        size_t i;
        for (torus &v : instance.a)
            v = tourus_uniform_dist_val(engine);

        text += torus_modular_normal_dist_val(engine, alpha);

        for (i = 0; i < N; i++)
            text += instance.a[i] * key[i];

        instance.text = text;
        return instance;
    }
    template <RandGen RG>
    static tlwe encrypto_bool(bool text, RG &engine, secret_key &key)
    {
        const torus mu = 1u << 29;
        return encrypto(text ? mu : -mu, engine, key);
    }

    torus decrypto(secret_key &key)
    {
        torus deciphertext = this->text;
        size_t i;
        for (i = 0; i < N; i++)
            deciphertext -= a[i] * key[i];
        return deciphertext;
    }

    bool decrypto_bool(secret_key &key)
    {
        return decrypto(key) > 0;
    }
};

int main()
{
}