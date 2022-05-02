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
    template <typename Z>
    CompressedPairElement(Z&& val) : value(std::forward<Z>(val)) {}
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
    template <typename Z>
    CompressedPairElement(Z&& val) {
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
    template <typename Z, typename Y>
    CompressedPair(Z&& fir, Y&& sec) : First(std::forward<F>(fir)), Second(std::forward<S>(sec))  {
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
