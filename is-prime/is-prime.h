#include "is_prime.h"
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <thread>
#include <future>

bool IsPrime(uint64_t x) {
    if (x <= 1) {
        return false;
    }
    if (x == 2) {
        return true;
    }
    uint64_t root = sqrt(x);
    auto bound = std::min(root + 6, x);
    std::atomic<bool> returned = 0;
    auto f1 = std::thread([&] {
        for (auto i = 2ull; i < bound / 2; ++i) {
            if (returned) {
                break;
            }
            if (x % i == 0) {
                returned = 1;
            }
        }
    }),
         f2 = std::thread([&] {
             for (auto i = bound / 2; i < bound; ++i) {
                 if (returned) {
                     break;
                 }
                 if (x % i == 0) {
                     returned = 1;
                 }
             }
         });
    f1.join();
    f2.join();
    return !returned;
}
