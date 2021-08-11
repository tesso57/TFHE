#include <random>
#include <math.h>
#include <iostream>
#include <array>

std::random_device seed_gen;
std::mt19937 engine(seed_gen());
double alpha = 0.0;
const unsigned int N = 630;

unsigned int torus_modular_normal_dist_val()
{
    std::normal_distribution<> dist{0.0, alpha};
    return (unsigned int)(fmod(dist(engine), 1) * std::pow(2, std::numeric_limits<unsigned int>::digits));
}

unsigned int tourus_uniform_dist_val()
{
    std::uniform_int_distribution<unsigned int> dist{0, std::numeric_limits<unsigned int>::max()};
    return dist(engine);
}

unsigned int encrypto(unsigned int m, std::array<unsigned int, N> a, std::array<bool, N> s)
{
    unsigned int b;
    unsigned int e = torus_modular_normal_dist_val();
    b = m + e;
    for (int i = 0; i < N; i++)
        b += a[i] * s[i];
    return b;
}

unsigned int decrypto(unsigned int b, std::array<unsigned int, N> a, std::array<bool, N> s)
{
    for (int i = 0; i < N; i++)
        b -= a[i] * s[i];
    return b;
}

void twle(unsigned int m, std::array<unsigned int, N> a, std::array<bool, N> s)
{
    //暗号文
    unsigned int b;
    //誤差
    unsigned int e = torus_modular_normal_dist_val();
    std::cout << "plaintext : " << m << std::endl;
    b = m + e;
    for (int i = 0; i < N; i++)
        b += a[i] * s[i];

    //暗号文の表示
    std::cout << "ciphertext : " << b << std::endl;

    //decrypto
    for (int i = 0; i < N; i++)
        b -= a[i] * s[i];

    std::cout << "deciphertext : " << b << std::endl;
}

int main()
{
    //平文
    unsigned int m1, m2;

    //準備 1
    std::array<unsigned int, N> a1, a2;
    std::array<bool, N> s;

    m1 = tourus_uniform_dist_val();
    m2 = tourus_uniform_dist_val();
    for (unsigned int &v : a1)
        v = tourus_uniform_dist_val();
    for (unsigned int &v : a2)
        v = tourus_uniform_dist_val();

    std::binomial_distribution<unsigned int> dist;
    for (bool &v : s)
        v = dist(engine);

    // std::cout << "1" << std::endl;
    // twle(m1, a1, s);
    // std::cout << std::endl;
    // std::cout << "2" << std::endl;
    // twle(m2, a2, s);
    // std::cout << std::endl;

    //加法性
    unsigned int b1 = encrypto(m1, a1, s);
    unsigned int b2 = encrypto(m2, a2, s);
    std::array<unsigned int, N> a3;
    for (int i = 0; i < N; i++)
        a3[i] = a1[i] + a2[i];

    unsigned int m3 = decrypto(b1 + b2, a3, s);
    std::cout << "m1 + m2" << std::endl;
    std::cout << m1 + m2 << std::endl;
    std::cout << m3 << std::endl;
}