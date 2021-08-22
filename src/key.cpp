#include "util.hpp"
#include "key.hpp"
template <class P>
secret_key<P>::secret_key()
{
}

template <class P>
template <RandGen RG>
secret_key<P>::secret_key(RG &rng)
{
    std::binomial_distribution<int> dist;
    dist(rng);
    for (torus &t : secret_key<P>::level0) // s <- B_n
        t = dist(rng);
    for (torus &t : secret_key<P>::level1) // s[X] <- B_N[X]
        t = dist(rng);
}