#pragma once

constexpr long double operator"" _kHz(long double value)
{
    return value * 1'000;
}
constexpr long double operator"" _MHz(long double value)
{
    return value * 1'000'000;
}
constexpr long double operator"" _GHz(long double value)
{
    return value * 1'000'000'000;
}

constexpr unsigned long long operator"" _kHz(unsigned long long value)
{
    return value * 1'000;
}
constexpr unsigned long long operator"" _MHz(unsigned long long value)
{
    return value * 1'000'000;
}
constexpr unsigned long long operator"" _GHz(unsigned long long value)
{
    return value * 1'000'000'000;
}
