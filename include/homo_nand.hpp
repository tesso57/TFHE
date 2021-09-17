#pragma once

#include "tlwe.hpp"
#include "trlwe.hpp"
#include "trgsw.hpp"
#include "gatebootstrapping.hpp"
#include "key.hpp"
#include "params.hpp"
#include "keyswitchingkey.hpp"

template <class P>
tlwe<P, 0> homo_nand(tlwe<P, 0> &lhs, tlwe<P, 0> &rhs, bootstrapping_key<P> &bkey, key_switching_key<P> &ks);
