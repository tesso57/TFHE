#include "util.hpp"

template <RandGen RG>
torus torus_modular_normal_dist_val(RG &engine, torus alpha)
{
    std::normal_distribution<> dist{0.0, alpha};
    return (torus)(fmod(dist(engine), 1.0) * std::pow(2, std::numeric_limits<torus>::digits));
}
template <RandGen RG>
torus tourus_uniform_dist_val(RG &engine)
{
    std::uniform_int_distribution<torus> dist{0, std::numeric_limits<torus>::max()};
    return dist(engine);
}
