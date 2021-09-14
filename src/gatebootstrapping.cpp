#include "gatebootstrapping.hpp"
#include "key.hpp"
#include "trgsw.hpp"
template <class P>
bootstrapping_key<P>::bootstrapping_key(std::random_device &engine, secret_key<P> &key)
{
    for (size_t i = 0; i < P::n; i++)
    {
        auto v = trgsw<P>::encrypt_bool(key.level0[i], key, engine);
        data[i] = v;
    }
}