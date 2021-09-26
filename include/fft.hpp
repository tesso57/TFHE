#pragma once

#include "util.hpp"
#include <array>

template <class P>
void polymult_fft(std::array<torus, P::N> &res, std::array<torus, P::N> &a, std::array<torus, P::N> &b);