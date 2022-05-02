#pragma once

#include <array>
#include <stdexcept>
#include <algorithm>

template<class K, class V, int MaxSize = 8>
class ConstexprMap {
public:
    std::array<std::pair<K, V>, MaxSize> map_ = std::array<std::pair<K, V>, MaxSize>();
    size_t sz = 0;
    constexpr ConstexprMap() = default;

    constexpr V& operator[](const K& key) {
        if (sz == MaxSize) {
            throw "---";
        }
        sz++;
        for (size_t i = 0; i < map_.size(); ++i) {
            if (map_[i].first == key) {
                return map_[i].second;
            }
        }
        map_[sz - 1] = {key, V()};
        return map_[sz - 1].second;
    }

    constexpr const V& operator[](const K& key) const {
        for (size_t i = 0; i < map_.size(); ++i) {
            if (map_[i].first == key) {
                return map_[i].second;
            }
        }
    }

    constexpr bool Erase(const K& key) {
        bool founded = 0;
        sz--;
        for (size_t i = 0; i < map_.size() - 1; ++i) {
            if (map_[i].first == key) {
                founded = 1;
                std::swap(map_[i], map_[i + 1]);
            }
        }
        return founded;
    }

    constexpr bool Find(const K& key) const {
        for (auto i : map_) {
            if (i.first == key) {
                return true;
            }
        }
        return false;
    }

    constexpr size_t Size() const {
        return sz;
    }

    constexpr std::pair<K, V>& GetByIndex(size_t pos) {
        return map_[pos];
    }

    constexpr const std::pair<K, V>& GetByIndex(size_t pos) const {
        return map_[pos];
    }
};
