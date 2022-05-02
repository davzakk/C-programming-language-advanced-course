
#pragma once

#include <utility>

template <std::size_t N>
constexpr auto Fib() {
    if constexpr (N < 2) {
        return [] { return N; };
    } else {
        return [] { return Fib<N - 1>()() + Fib<N - 2>()(); };
    }
}

template <std::size_t N>
constexpr auto Fact() {
    if constexpr (N < 2) {
        return [] { return 1; };
    } else {
        return [] { return Fact<N - 1>()() * N; };
    }
}

template <std::size_t A, std::size_t B>
constexpr auto GCD() {
    if constexpr (A * B == 0) {
        return [] { return A + B; };
    } else if constexpr (A < B) {
        return [] { return GCD<A, B % A>()(); };
    } else {
        return [] { return GCD<B, A % B>()(); };
    }
}

template <std::size_t A, std::size_t H = 2>
constexpr auto Prime() -> bool (*)() {
    if constexpr (A == 0) {
        return [] { return false; };
    } else if constexpr (A == 1) {
        return [] { return false; };
    } else if constexpr (A < H * H) {
        return [] { return true; };
    } else if constexpr (A % H == 0) {
        return [] { return false; };
    } else {
        return [] { return Prime<A, H + 1>()(); };
    }
}
