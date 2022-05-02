#pragma once

#include "sw_fwd.h"  // Forward declaration
#include "shared.h"
// https://en.cppreference.com/w/cpp/memory/weak_ptr
template <typename T>
class WeakPtr {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    WeakPtr() : ptr_(nullptr), ptr_block_(nullptr) {
    }
    template <typename Y>
    WeakPtr(const WeakPtr<Y>& other) noexcept {
        ptr_ = other.ptr_;
        ptr_block_ = other.ptr_block_;
        if (ptr_block_ != nullptr) {
            ptr_block_->WeakPlus();
        }
    }
    WeakPtr(const WeakPtr& other) noexcept {
        ptr_ = other.ptr_;
        ptr_block_ = other.ptr_block_;
        if (ptr_block_ != nullptr) {
            ptr_block_->WeakPlus();
        }
    }
    WeakPtr(WeakPtr&& other) noexcept {
        ptr_ = other.ptr_;
        ptr_block_ = other.ptr_block_;
        other.ptr_ = nullptr;
        other.ptr_block_ = nullptr;
    }
    // Demote `SharedPtr`
    // #2 from https://en.cppreference.com/w/cpp/memory/weak_ptr/weak_ptr
    WeakPtr(const SharedPtr<T>& other) {
        ptr_ = other.ptr_;
        ptr_block_ = other.ptr_block_;
        if (ptr_block_ != nullptr) {
            ptr_block_->WeakPlus();
        }
    }
    template <typename Y>
    WeakPtr(ControlBlockBase* block, Y* ptr) {
        block->WeakPlus();
        ptr_block_ = block;
        ptr_ = ptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s
    void DestructMinus() {
        if (ptr_block_ == nullptr) {
            return;
        }
        if (ptr_block_->GetStrong() == 0 && ptr_block_->GetWeak() == 1) {
            ptr_block_->WeakMinus();
            ptr_block_ = nullptr;
            ptr_ = nullptr;
        } else if (ptr_block_->GetWeak() == 1) {
            ptr_block_->WeakMinus();
            // ptr_ = nullptr;
        } else {
            ptr_block_->WeakMinus();
        }
    }

    WeakPtr& operator=(const WeakPtr& other) {
        if (ptr_ != other.ptr_) {
            DestructMinus();
            ptr_ = other.ptr_;
            ptr_block_ = other.ptr_block_;
            if (ptr_block_ != nullptr) {
                ptr_block_->WeakPlus();
            }
        }
        return *this;
    }
    WeakPtr& operator=(WeakPtr&& other) {
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
    WeakPtr& operator=(WeakPtr<Y>&& other) {
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

    ~WeakPtr() {
        DestructMinus();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    void Reset() {
        ptr_ = nullptr;
        DestructMinus();
        ptr_block_ = nullptr;
    }
    void Swap(WeakPtr& other) {
        std::swap(*this, other);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    size_t UseCount() const {
        if (ptr_block_ == nullptr) {
            return 0;
        }
        return ptr_block_->GetStrong();
    }
    bool Expired() const {
        return UseCount() == 0;
    }
    SharedPtr<T> Lock() const {
        if (this->Expired()) {
            return nullptr;
        }
        return SharedPtr<T>(*this);
    }
    T* ptr_;
    ControlBlockBase* ptr_block_;
};
