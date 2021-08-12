#ifndef HEADER_UTIL_
#define HEADER_UTIL_
#include <random>
using torus = uint32_t;

template <class G>
concept RandGen = std::uniform_random_bit_generator<G>;

template <RandGen RG>
torus torus_modular_normal_dist_val(RG &engine, torus alpha);

template <RandGen RG>
torus tourus_uniform_dist_val(RG &engine);
#endif