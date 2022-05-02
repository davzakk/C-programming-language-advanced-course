#pragma once

#include <type_traits>
#include <cstddef>
#include <utility>

// Me think, why waste time write lot code, when few code do trick.

template <typename T, std::size_t I, bool = std::is_empty_v<T> && !std::is_final_v<T>>
struct CompressedPairElement {
    // Constructors.
    CompressedPairElement() {
        value = T();
    }
    CompressedPairElement(T& val) {
        value = val;
    }
    CompressedPairElement(T&& val) : value(std::forward<T&&>(val)) {
    }
    // Getters: return value.
    T& Get() {
        return value;
    }
    const T& Get() const {
        return value;
    }
    T value;
};

template <typename T, std::size_t I>
struct CompressedPairElement<T, I, true> : public T {
    // Constructors.
    CompressedPairElement() {
    }
    CompressedPairElement(T&) {
    }
    CompressedPairElement(T&&) {
    }
    // Getters: return *this.
    T& Get() {
        return *this;
    }
    const T& Get() const {
        return *this;
    }
};

template <typename F, typename S>
class CompressedPair : private CompressedPairElement<F, 0>, private CompressedPairElement<S, 1> {
    using First = CompressedPairElement<F, 0>;
    using Second = CompressedPairElement<S, 1>;

public:
    // Constructors.
    CompressedPair() {
    }
    CompressedPair(F& fir, S& sec) : First(fir), Second(sec) {
    }
    CompressedPair(F& fir, S&& sec) : First(fir), Second(std::forward<S&&>(sec)) {
    }
    CompressedPair(F&& fir, S& sec) : First(std::forward<F&&>(fir)), Second(sec) {
    }
    CompressedPair(F&& fir, S&& sec)
        : First(std::forward<F&&>(fir)), Second(std::forward<S&&>(sec)) {
    }

    // Getters for first and second.
    F& GetFirst() {
        return First::Get();
    }
    const F& GetFirst() const {
        return First::Get();
    }
    S& GetSecond() {
        return Second::Get();
    }
    const S& GetSecond() const {
        return Second::Get();
    }
};
