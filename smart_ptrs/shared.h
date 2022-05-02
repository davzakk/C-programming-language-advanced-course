#pragma once

#include "sw_fwd.h"  // Forward declaration

#include <cstddef>  // std::nullptr_t
#include <algorithm>

struct EnableSharedBase {};
// Look for usage examples in tests
template <typename T>
struct EnableSharedFromThis : public EnableSharedBase {
public:
    SharedPtr<T> SharedFromThis() {
        return SharedPtr<T>(self_);
    }
    SharedPtr<const T> SharedFromThis() const {
        return SharedPtr<const T>(self_);
    }

    WeakPtr<T> WeakFromThis() noexcept {
        return self_;
    }
    WeakPtr<const T> WeakFromThis() const noexcept {
        return self_;
    }
    virtual ~EnableSharedFromThis() {
    }

public:
    WeakPtr<T> self_;
    template <typename Y>
    friend struct SharedPtr;
};
struct ControlBlockBase {
    size_t GetStrong() {
        return strong;
    }
    size_t GetWeak() {
        return weak;
    }
    void operator--() {
        if (strong == 1) {
            Remove();
            --strong;
            if (!weak) {
                delete this;
            }
        } else {
            --strong;
        }
    }
    void operator++() {
        ++strong;
    }
    void WeakPlus() {
        ++weak;
    }
    void WeakMinus() {
        --weak;
        if (!strong && !weak) {
            delete this;
        }
    }
    virtual void Remove() {
    }
    virtual ~ControlBlockBase() {
    }
    size_t strong = 0;
    size_t weak = 0;
};
template <typename T>
struct ControlBlockPtr : ControlBlockBase {
    ControlBlockPtr(T* ptr) : ptr_(ptr) {
    }
    void Remove() {
        delete ptr_;
    }
    T* ptr_;
};
template <typename T>
struct ControlBlockMem : ControlBlockBase {
    template <typename... Args>
    ControlBlockMem(Args&&... args) {
        new (&storage_) T(std::forward<Args>(args)...);
    }
    T* GetPtr() {
        return reinterpret_cast<T*>(&storage_);
    }
    void Remove() {
        reinterpret_cast<T*>(&storage_)->~T();
    }
    std::aligned_storage_t<sizeof(T), alignof(T)> storage_;
};
// https://en.cppreference.com/w/cpp/memory/shared_ptr
template <typename T>
struct SharedPtr {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors
    SharedPtr() : ptr_(nullptr), ptr_block_(nullptr) {
    }
    SharedPtr(std::nullptr_t) noexcept : ptr_(nullptr), ptr_block_(nullptr) {
    }
    template <typename Y>
    explicit SharedPtr(Y* ptr) noexcept {
        ptr_ = ptr;
        ptr_block_ = new ControlBlockPtr<Y>(ptr);
        if (ptr_block_ != nullptr) {
            ++(*ptr_block_);
        }
        if constexpr (std::is_convertible_v<Y*, EnableSharedBase*>) {
            ptr->self_ = WeakPtr<Y>(ptr_block_, ptr);
        }
    }
    explicit SharedPtr(T* ptr) noexcept {
        ptr_ = ptr;
        ptr_block_ = new ControlBlockPtr(ptr);
        if (ptr_block_ != nullptr) {
            ++(*ptr_block_);
        }
        if constexpr (std::is_convertible_v<T*, EnableSharedBase*>) {
            ptr->self_ = WeakPtr<T>(*this);
        }
    }

    template <typename Y>
    SharedPtr(const SharedPtr<Y>& other) noexcept {
        ptr_ = other.ptr_;
        ptr_block_ = other.ptr_block_;
        if (ptr_block_ != nullptr) {
            ++(*ptr_block_);
        }
    }
    SharedPtr(const SharedPtr& other) noexcept {
        ptr_ = other.ptr_;
        ptr_block_ = other.ptr_block_;
        if (ptr_block_ != nullptr) {
            ++(*ptr_block_);
        }
    }
    template <typename Y>
    SharedPtr(SharedPtr<Y>&& other) noexcept {
        ptr_ = other.ptr_;
        ptr_block_ = other.ptr_block_;
        other.ptr_ = nullptr;
        other.ptr_block_ = nullptr;
    }
    SharedPtr(SharedPtr&& other) noexcept {
        ptr_ = other.ptr_;
        ptr_block_ = other.ptr_block_;
        other.ptr_ = nullptr;
        other.ptr_block_ = nullptr;
    }
    SharedPtr(ControlBlockBase* block, T* ptr) {
        ++(*block);
        ptr_block_ = block;
        ptr_ = ptr;
    }

    // Aliasing constructor
    // #8 from https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
    template <typename Y>
    SharedPtr(const SharedPtr<Y>& other, T* ptr) {
        if constexpr (std::is_convertible_v<T*, EnableSharedBase*>) {
            ptr_block_ = other.ptr_block_;
            if (ptr_block_ != nullptr) {
                ++(*ptr_block_);
            }
            ptr->self_ = WeakPtr<Y>(*this);
        } else {
            ptr_ = ptr;
            ptr_block_ = other.ptr_block_;
            if (ptr_block_ != nullptr) {
                ++(*ptr_block_);
            }
        }
    }
    void DestructMinus() {
        if (ptr_block_ == nullptr) {
            return;
        }
        if (ptr_block_->GetStrong() == 1 && ptr_block_->GetWeak() == 0) {
            --(*ptr_block_);
            ptr_block_ = nullptr;
            ptr_ = nullptr;
        } else if (ptr_block_->GetStrong() == 1) {
            --(*ptr_block_);
            // ptr_ = nullptr;
        } else {
            --(*ptr_block_);
        }
    }
    // Promote `WeakPtr`
    // #11 from https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
    explicit SharedPtr(const WeakPtr<T>& other) {
        if (other.Expired()) {
            throw BadWeakPtr();
        }
        ptr_ = other.ptr_;
        ptr_block_ = other.ptr_block_;
        if (ptr_block_ != nullptr) {
            ++(*ptr_block_);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s
    SharedPtr& operator=(const SharedPtr& other) {
        if (ptr_ != other.ptr_) {
            DestructMinus();
            ptr_ = other.ptr_;
            ptr_block_ = other.ptr_block_;
            if (ptr_block_ != nullptr) {
                ++(*ptr_block_);
            }
        }
        return *this;
    }
    template <typename Y>
    SharedPtr& operator=(const SharedPtr<Y>& other) {
        if (ptr_ != other.ptr_) {
            DestructMinus();
            ptr_ = other.ptr_;
            ptr_block_ = other.ptr_block_;
            if (ptr_block_ != nullptr) {
                ++(*ptr_block_);
            }
        }
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other) {
        if (ptr_ != other.ptr_) {
            DestructMinus();
            ptr_ = other.ptr_;
            ptr_block_ = other.ptr_block_;
        }
        other.ptr_ = nullptr;
        other.ptr_block_ = nullptr;
        return *this;
    }
    template <typename Y>
    SharedPtr& operator=(SharedPtr<Y>&& other) {
        if (ptr_ != other.ptr_) {
            DestructMinus();
            ptr_ = other.ptr_;
            ptr_block_ = other.ptr_block_;
        }
        other.ptr_ = nullptr;
        other.ptr_block_ = nullptr;
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~SharedPtr() {
        DestructMinus();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    void Reset() {
        ptr_ = nullptr;
        DestructMinus();
        ptr_block_ = nullptr;
    }
    template <typename Y>
    void Reset(Y* ptr) {
        DestructMinus();
        ptr_ = ptr;
        ptr_block_ = new ControlBlockPtr<Y>(ptr);
        if (ptr_block_ != nullptr) {
            ++(*ptr_block_);
        }
    }
    void Reset(T* ptr) {
        DestructMinus();
        ptr_ = ptr;
        ptr_block_ = new ControlBlockPtr(ptr);
        if (ptr_block_ != nullptr) {
            ++(*ptr_block_);
        }
    }
    void Swap(SharedPtr& other) {
        std::swap(*this, other);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    T* Get() const {
        return ptr_;
    }
    T& operator*() const {
        return *ptr_;
    }
    T* operator->() const {
        return ptr_;
    }
    size_t UseCount() const {
        if (ptr_block_ == nullptr) {
            return 0;
        }
        return ptr_block_->GetStrong();
    }
    explicit operator bool() const {
        return ptr_ != nullptr;
    }
    T* ptr_;
    ControlBlockBase* ptr_block_;
};

template <typename T, typename U>
inline bool operator==(const SharedPtr<T>& left, const SharedPtr<U>& right) {
    return left.ptr_ == right.ptr_;
}

// Allocate memory only once
template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    ControlBlockMem<T>* block_mem = new ControlBlockMem<T>(std::forward<Args>(args)...);
    //++(*block_mem);
    //++(*block_mem);
    SharedPtr<T> s = SharedPtr<T>(block_mem, block_mem->GetPtr());
    if constexpr (std::is_convertible_v<T*, EnableSharedBase*>) {
        // block_mem->WeakPlus();
        s.ptr_->self_ = WeakPtr<T>(block_mem, block_mem->GetPtr());
    }
    return s;
}
