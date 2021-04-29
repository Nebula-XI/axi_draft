#pragma once

constexpr long double operator"" _kHz(long double value) {
  return value * 1000;
}
constexpr long double operator"" _MHz(long double value) {
  return value * 1000000;
}
constexpr long double operator"" _GHz(long double value) {
  return value * 1000000000;
}

constexpr unsigned long long operator"" _kHz(unsigned long long value) {
  return value * 1000;
}
constexpr unsigned long long operator"" _MHz(unsigned long long value) {
  return value * 1000000;
}
constexpr unsigned long long operator"" _GHz(unsigned long long value) {
  return value * 1000000000;
}

#include <thread>
#include <chrono>

using ipc_time_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

inline ipc_time_t ipc_get_time() {
    return std::chrono::high_resolution_clock::now();
}

inline double ipc_get_difftime(ipc_time_t start, ipc_time_t end) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

inline void ipc_delay(int ms) {
    std::this_thread::sleep_for(std::chrono_literals::operator""ms(ms));
}