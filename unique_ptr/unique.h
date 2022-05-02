#pragma once

#include "compressed_pair.h"

#include <cstddef>  // std::nullpair_.GetFirst()t
#include <algorithm>
#include <memory>

template <typename T>
class MainDeleter {
public:
    MainDeleter() = default;
    template <typename D>
    MainDeleter(const MainDeleter<D>& deleter) {}
    void operator()(T* ptr) noexcept {
        delete ptr;
    }
};
template <typename T>
class MainDeleter<T[]> {
public:
    MainDeleter() = default;
    template <typename D>
    MainDeleter(const MainDeleter<D[]>& deleter) {}
    void operator()(T* ptr) noexcept {
        delete[] ptr;
    }
};

// Primary template
template <typename T, typename Deleter = MainDeleter<T>>
class UniquePtr {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    explicit UniquePtr(T* ptr = nullptr) noexcept : pair_(ptr, Deleter()) {}
    UniquePtr(T* ptr, Deleter deleter) noexcept : pair_(ptr, deleter) {}
    template <typename D>
    UniquePtr(T* ptr, D deleter) noexcept : pair_(ptr, deleter) {}

    UniquePtr(UniquePtr&& other) noexcept {
        pair_.GetFirst() = other.pair_.GetFirst();
        pair_.GetSecond() = std::forward<Deleter>(other.GetDeleter());
        other.pair_.GetFirst() = nullptr;
    }
    template <typename Y, typename D>
    UniquePtr(UniquePtr<Y, D>&& other) noexcept {
        pair_.GetFirst() = other.Release();
        pair_.GetSecond() = std::forward<Deleter>(other.GetDeleter());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this->pair_.GetFirst() != other.pair_.GetFirst()) {
            pair_.GetSecond()(pair_.GetFirst());
            pair_.GetFirst() = other.pair_.GetFirst();
            other.pair_.GetFirst() = nullptr;
        }
        pair_.GetSecond() = std::forward<Deleter>(other.pair_.GetSecond());
        return *this;
    }
    UniquePtr& operator=(std::nullptr_t) noexcept {
        pair_.GetSecond()(pair_.GetFirst());
        pair_.GetFirst() = nullptr;
        pair_.GetSecond() = Deleter();
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~UniquePtr() {
        pair_.GetSecond()(pair_.GetFirst());
        pair_.GetFirst() = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    T* Release() {
        T* ptr_copy = pair_.GetFirst();
        pair_.GetFirst() = nullptr;
        return ptr_copy;
    }
    void Reset(T* ptr = nullptr) {
        T* ptr_copy = pair_.GetFirst();
        pair_.GetFirst() = ptr;
        pair_.GetSecond()(ptr_copy);
    }
    void Swap(UniquePtr& other) {
        std::swap(other.pair_, pair_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    T* Get() const {
        return pair_.GetFirst();
    }
    Deleter& GetDeleter() {
        return pair_.GetSecond();
    }
    const Deleter& GetDeleter() const {
        return pair_.GetSecond();
    }
    explicit operator bool() const {
        return pair_.GetFirst();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Single-object dereference operators

    std::add_lvalue_reference_t<T> operator*() const {
        return *pair_.GetFirst();
    }
    T* operator->() const {
        return pair_.GetFirst();
    }

private:
    CompressedPair<T*, Deleter> pair_; // pair_.GetFirst() deleter_
};

// Specialization for arrays
template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
public:
    explicit UniquePtr(T* ptr = nullptr) noexcept : pair_(ptr, Deleter()) {}
    UniquePtr(T* ptr, Deleter deleter) noexcept : pair_(ptr, deleter) {}
    template <typename D>
    UniquePtr(T* ptr, D deleter) noexcept : pair_(ptr, deleter) {}

    UniquePtr(UniquePtr&& other) noexcept {
        pair_.GetFirst() = other.pair_.GetFirst();
        pair_.GetSecond() = other.GetDeleter();
        other.pair_.GetFirst() = nullptr;
    }
    template <typename Y, typename D>
    UniquePtr(UniquePtr<Y, D>&& other) noexcept {
        pair_.GetFirst() = other.Release();
        pair_.GetSecond() = std::forward<Deleter>(other.GetDeleter());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this->pair_.GetFirst() != other.pair_.GetFirst()) {
            pair_.GetSecond()(pair_.GetFirst());
            pair_.GetFirst() = other.pair_.GetFirst();
            other.pair_.GetFirst() = nullptr;
        }
        pair_.GetSecond() = std::forward<Deleter>(other.pair_.GetSecond());
        return *this;
    }
    UniquePtr& operator=(std::nullptr_t) noexcept {
        pair_.GetSecond()(pair_.GetFirst());
        pair_.GetFirst() = nullptr;
        pair_.GetSecond() = Deleter();
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~UniquePtr() {
        pair_.GetSecond()(pair_.GetFirst());
        pair_.GetFirst() = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    T* Release() {
        T* ptr_copy = pair_.GetFirst();
        pair_.GetFirst() = nullptr;
        return ptr_copy;
    }
    void Reset(T* ptr = nullptr) {
        T* ptr_copy = pair_.GetFirst();
        pair_.GetFirst() = ptr;
        pair_.GetSecond()(ptr_copy);
    }
    void Swap(UniquePtr& other) {
        std::swap(other.pair_, pair_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    T* Get() const {
        return pair_.GetFirst();
    }
    Deleter& GetDeleter() {
        return pair_.GetSecond();
    }
    const Deleter& GetDeleter() const {
        return pair_.GetSecond();
    }
    explicit operator bool() const {
        return pair_.GetFirst();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Single-object dereference operators

    std::add_lvalue_reference_t<T> operator[](size_t ind) const {
        return pair_.GetFirst()[ind];
    }
    T* operator->() const {
        return pair_.GetFirst();
    }

private:
    CompressedPair<T*, Deleter> pair_; // pair_.GetFirst() deleter_
};
