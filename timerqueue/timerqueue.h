#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <map>
#include <set>

template <class T>
class TimerQueue {
public:
    std::mutex mutex_;
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;
    std::set<std::pair<TimePoint, T>> set_;
    void Add(const T& item, TimePoint at) {
        std::unique_lock lock(mutex_);
        set_.insert({at, item});
    }
    bool IsEmpty() {
        std::unique_lock lock(mutex_);
        return set_.empty();
    }

    bool Hz() {
        std::unique_lock lock(mutex_);
        return set_.begin()->first > std::chrono::system_clock::now();
    }

    T Pop() {
        while (IsEmpty() || Hz()) {
        }
        std::unique_lock lock(mutex_);
        std::pair<TimePoint, T> a = *set_.begin();
        set_.erase(a);
        return a.second;
    }
};
