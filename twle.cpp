#include <random>
#include <math.h>
#include <iostream>
#include <array>

std::random_device seed_gen;
std::mt19937 engine(seed_gen());
double alpha = 3.0517578125e-05;
const unsigned int N = 630;

unsigned int torus_modular_normal_dist_val()
{
    std::normal_distribution<> dist{0.0, alpha};
    return (unsigned int)(fmod(dist(engine), 1.0) * std::pow(2, std::numeric_limits<unsigned int>::digits));
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
    unsigned int m1, m2, res;
    unsigned int b1, b2;

    //準備
    std::array<unsigned int, N> a1, a2, a3;
    std::array<bool, N> s;
    std::binomial_distribution<unsigned int> dist;
    for (bool &v : s)
        v = dist(engine);

    // 0 0
    m1 = -1 * 1u << 29;
    m2 = -1 * 1u << 29;
    for (unsigned int &v : a1)
        v = tourus_uniform_dist_val();
    for (unsigned int &v : a2)
        v = tourus_uniform_dist_val();

    b1 = encrypto(m1, a1, s);
    b2 = encrypto(m2, a2, s);
    for (int i = 0; i < N; i++)
        a3[i] = a1[i] + a2[i];

    res = decrypto(b1 + b2, a3, s);
    std::cout << "0 0" << std::endl;
    std::cout << m1 + m2 << std::endl;
    std::cout << res << std::endl;
    std::cout << std::endl;

    // 1 0
    m1 = 1u << 29;
    m2 = -1 * 1u << 29;
    for (unsigned int &v : a1)
        v = tourus_uniform_dist_val();
    for (unsigned int &v : a2)
        v = tourus_uniform_dist_val();

    b1 = encrypto(m1, a1, s);
    b2 = encrypto(m2, a2, s);
    for (int i = 0; i < N; i++)
        a3[i] = a1[i] + a2[i];

    res = decrypto(b1 + b2, a3, s);
    std::cout << "1 0" << std::endl;
    std::cout << m1 + m2 << std::endl;
    std::cout << res << std::endl;
    std::cout << std::endl;

    // 1 1
    m1 = 1u << 29;
    m2 = 1u << 29;
    for (unsigned int &v : a1)
        v = tourus_uniform_dist_val();
    for (unsigned int &v : a2)
        v = tourus_uniform_dist_val();

    b1 = encrypto(m1, a1, s);
    b2 = encrypto(m2, a2, s);
    for (int i = 0; i < N; i++)
        a3[i] = a1[i] + a2[i];

    res = decrypto(b1 + b2, a3, s);
    std::cout << "1 1" << std::endl;
    std::cout << m1 + m2 << std::endl;
    std::cout << res << std::endl;
    std::cout << std::endl;
}