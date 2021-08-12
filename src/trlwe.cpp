// #include <random>
// #include <math.h>
// #include <iostream>
// #define DEBUG 0
// #include <array>

// std::random_device seed_gen;
// std::mt19937 engine(seed_gen());
// double alpha = 0.0;
// const unsigned int N = 1024;

// unsigned int torus_modular_normal_dist_val()
// {
//     std::normal_distribution<> dist{0.0, alpha};
//     return (unsigned int)(fmod(dist(engine), 1.0) * std::pow(2, std::numeric_limits<unsigned int>::digits));
// }

// unsigned int tourus_uniform_dist_val()
// {
//     std::uniform_int_distribution<unsigned int> dist{0, std::numeric_limits<unsigned int>::max()};
//     return dist(engine);
// }
/////
// void encrypto(std::array<unsigned int, N> &b, std::array<unsigned int, N> m, std::array<unsigned int, N> a, std::array<bool, N> s)
// {

//     for (unsigned int &v : b)
//         v = 0;

//     //多項式乗算
//     for (int i = 0; i < N; i++)
//         for (int j = 0; j < N; j++)
//             if (i + j < N)
//                 b[i + j] += a[i] * s[i];
//             else
//                 b[i + j - N] -= a[i] * s[i];
//     for (int i = 0; i < N; i++)
//         b[i] += m[i] + torus_modular_normal_dist_val();
// }

// void decrypto(std::array<unsigned int, N> &b, std::array<unsigned int, N> a, std::array<bool, N> s)
// {
//     //多項式乗算
//     //引くので、符号は逆
//     for (int i = 0; i < N; i++)
//         for (int j = 0; j < N; j++)
//             if (i + j < N)
//                 b[i + j] -= a[i] * s[i];
//             else
//                 b[i + j - N] += a[i] * s[i];
// }

// void dump(std::array<unsigned int, N> &ary)
// {
//     if (DEBUG)
//     {
//         std::cout << "[ ";
//         for (unsigned int v : ary)
//             std::cout << v << ", ";
//         std::cout << "]" << std::endl;
//     }
// }

// int main()
// {
//     int cnt;
//     //平文
//     std::array<unsigned int, N> m1, m2, m3;
//     //暗号文
//     std::array<unsigned int, N> b1, b2, b3;
//     //key
//     std::array<bool, N> s;
//     std::binomial_distribution<unsigned int> dist;
//     for (bool &v : s)
//         v = dist(engine);
//     //a
//     std::array<unsigned int, N> a1, a2, a3;

//     //暗号と復号1
//     for (unsigned int &v : m1)
//         v = tourus_uniform_dist_val();
//     std::cout << "plaintext >>" << std::endl;
//     dump(m1);
//     for (unsigned int &v : a1)
//         v = tourus_uniform_dist_val();
//     encrypto(b1, m1, a1, s);
//     std::cout << "ciphertext >>" << std::endl;
//     dump(b1);
//     decrypto(b1, a1, s);
//     std::cout << "deciphertext >>" << std::endl;
//     dump(b1);
//     //test
//     cnt = 0;
//     for (int i = 0; i < N; i++)
//         if (m1[i] != b1[i])
//             cnt += 1;
//     std::cout << "error : " << cnt << std::endl;

//     //暗号と復号2
//     for (unsigned int &v : m2)
//         v = tourus_uniform_dist_val();
//     std::cout << "plaintext >>" << std::endl;
//     dump(m2);
//     for (unsigned int &v : a2)
//         v = tourus_uniform_dist_val();
//     encrypto(b2, m2, a2, s);
//     std::cout << "ciphertext >>" << std::endl;
//     dump(b2);
//     decrypto(b2, a2, s);
//     std::cout << "deciphertext >>" << std::endl;
//     dump(b2);
//     //test
//     cnt = 0;
//     for (int i = 0; i < N; i++)
//         if (m2[i] != b2[i])
//             cnt += 1;
//     std::cout << "error : " << cnt << std::endl;

//     //加法性
//     for (int i = 0; i < N; i++)
//         m3[i] = m1[i] + m2[i];
//     for (int i = 0; i < N; i++)
//         a3[i] = a1[i] + a2[i];
//     std::cout << "plaintext >>" << std::endl;
//     dump(m3);
//     encrypto(b3, m3, a3, s);
//     std::cout << "ciphertext >>" << std::endl;
//     dump(b3);
//     decrypto(b3, a3, s);
//     std::cout << "deciphertext >>" << std::endl;
//     dump(b3);
//     cnt = 0;
//     for (int i = 0; i < N; i++)
//         if (m3[i] != b3[i])
//             cnt += 1;
//     std::cout << "error : " << cnt << std::endl;
// }