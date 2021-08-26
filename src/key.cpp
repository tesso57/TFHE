#include "util.hpp"
#include "key.hpp"
#include "params.hpp"
#include <random>
template <class P>
secret_key<P>::secret_key()
{
}

template <class P>
secret_key<P>::secret_key(std::random_device &engine)
{
    std::binomial_distribution<unsigned int> dist;
    for (torus &t : secret_key<P>::level0) // s <- B_n
        t = dist(engine);
    for (torus &t : secret_key<P>::level1) // s[X] <- B_N[X]
        t = dist(engine);
}

template struct secret_key<Test>;