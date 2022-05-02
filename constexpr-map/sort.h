#pragma once

#include <constexpr_map.h>

#include <type_traits>

#include <algorithm>
#include <type_traits>

template <class K, class V, int S>
constexpr auto Sort(ConstexprMap<K, V, S> map) {
    ConstexprMap<K, V, S> map2 = map;
    for (int i = map2.Size() - 1; i > -1; --i) {
        for (int j = 0; j < i; ++j) {
            if (std::is_integral<K>::value) {
                if (map2.map_[j] < map2.map_[j + 1]) {
                    auto a = map2.map_[j];
                    map2.map_[j] = map2.map_[j + 1];
                    map2.map_[j + 1] = a;
                }
            } else {
                if (!(map2.map_[j] < map2.map_[j + 1])) {
                    auto a = map2.map_[j];
                    map2.map_[j] = map2.map_[j + 1];
                    map2.map_[j + 1] = a;
                }
            }
        }
    }
    return map2;
}
