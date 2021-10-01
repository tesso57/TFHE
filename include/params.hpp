#pragma once

#include <cstdint>

struct Test
{
    //TLWE params
    static constexpr uint32_t n = 630;
    static constexpr double alpha = 0.0;
    //TRLWE params
    static constexpr uint32_t Nbit = 10;
    static constexpr uint32_t N = 1 << Nbit;
    //TRGSW params
    static constexpr uint32_t l = 3;
    static constexpr double alpha_bk = 0.0;
    static constexpr uint32_t Bgbit = 6;
    static constexpr uint32_t Bg = 1 << Bgbit;
    //IDENTITY KEY SWITCHING params
    static constexpr uint32_t t = 8;
    static constexpr uint32_t basebit = 2;
};

struct CASE1
{
    //TLWE params
    static constexpr uint32_t n = 630;
    static constexpr double alpha = 3.0517578125e-05; // 2^(-15)
    //TRLWE params
    static constexpr uint32_t Nbit = 10;
    static constexpr uint32_t N = 1 << Nbit;
    //TRGSW params
    static constexpr uint32_t l = 3;
    static constexpr double alpha_bk = 2.98023223876953125e-08; // 2^(-25)
    static constexpr uint32_t Bgbit = 6;
    static constexpr uint32_t Bg = 1 << Bgbit;
    //IDENTITY KEY SWITCHING params
    static constexpr uint32_t t = 8;
    static constexpr uint32_t basebit = 2;
};