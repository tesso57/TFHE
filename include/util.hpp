#ifndef HEADER_UTIL_
#define HEADER_UTIL_
#include <random>
using torus = uint32_t;
using secret_key = std::vector<bool>;

template <class G>
concept RandGen = std::uniform_random_bit_generator<G>;

template <RandGen RG>
inline torus torus_modular_normal_dist_val(RG &engine, double alpha)
{
    std::normal_distribution<> dist{0.0, alpha};
    return (torus)(fmod(dist(engine), 1.0) * std::pow(2, std::numeric_limits<torus>::digits));
}
template <RandGen RG>
inline torus torus_uniform_dist_val(RG &engine)
{
    std::uniform_int_distribution<torus> dist{0, std::numeric_limits<torus>::max()};
    return dist(engine);
}
#endif