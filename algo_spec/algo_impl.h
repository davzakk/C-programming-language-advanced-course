#pragma once

#include <cstdint>

using AlgoRes = int;

struct Int128 {
    int64_t lo, hi;
};
struct Int256 {
    Int128 lo, hi;
};

template <typename T>
AlgoRes ComputeUnsignedIntegral(T) {
    return 1 << 3;
}

template <typename T>
AlgoRes ComputeIntegral(T) {
    return 1 << 2;
}

template <typename T>
AlgoRes ComputeFloat(T) {
    return 1 << 1;
}

template <typename T>
AlgoRes ComputeGeneral(T) {
    return 1 << 0;
}
