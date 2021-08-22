#include <chrono>
#include <cstdlib>
#ifndef HEADER_TEST_
#define HEADER_TEST_
template <class Proc>
inline auto timeit(Proc &&proc)
{
    auto begin = std::chrono::high_resolution_clock::now();
    proc();
    auto end = std::chrono::high_resolution_clock::now();
    return end - begin;
}

template <class... Args>
inline void debug_log(Args &&...args)
{
    const char *envvar = std::getenv("TFHE_VERBOSE");
    if (envvar != nullptr && std::strcmp(envvar, "1") == 0)
        (std::cerr << ... << args) << std::endl;
}

inline void failwith(bool cond, const char *expr, unsigned long line)
{
    if (cond)
        return;
    debug_log("\e[1;31mASSERTION FAILED! (L.", line, ") ", expr, "\e[0m");
    std::exit(1);
}

#endif