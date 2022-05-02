#pragma once

#include <typeinfo>
#include <algorithm>
#include <memory>

struct Var {
    Var() : inner_(new Inner<int>(0)) {
    }  // default construct to an integer

    Var(const Var& src) : inner_(src.inner_->Clone()) {
    }  // copy constructor calls clone method of concrete type

    template <typename Ty_>
    Var(Ty_ src) : inner_(new Inner<Ty_>(std::forward<Ty_>(src))) {
    }

    template <typename Ty_>
    Var& operator=(Ty_ src) {  // assign to a concrete type
        inner_ = std::make_unique<Inner<Ty_>>(std::forward<Ty_>(src));
        return *this;
    }

    Var& operator=(const Var& src) {  // assign to another Var type
        Var o_tmp(src);
        std::swap(o_tmp.inner_, this->inner_);
        return *this;
    }

    // interrogate the underlying type through the inner_base interface
    const std::type_info& Type() const {
        return inner_->Type();
    }
    bool IsPOD() const {
        return inner_->IsPOD();
    }
    size_t Size() const {
        return inner_->Size();
    }

    struct InnerBase {
        using Pointer = std::unique_ptr<InnerBase>;
        virtual ~InnerBase() {
        }
        virtual InnerBase* Clone() const = 0;
        virtual const std::type_info& Type() const = 0;
        virtual bool IsPOD() const = 0;
        virtual std::size_t Size() const = 0;
    };

    template <typename Ty_>
    struct Inner : InnerBase {
        Inner(Ty_ newval) : value_(std::move(newval)) {
        }
        virtual InnerBase* Clone() const override {
            return new Inner(value_);
        }
        virtual const std::type_info& Type() const override {
            return typeid(Ty_);
        }
        Ty_& operator*() {
            return value_;
        }
        const Ty_& operator*() const {
            return value_;
        }
        virtual bool IsPOD() const {
            return std::is_pod<Ty_>::value;
        }
        virtual size_t Size() const {
            return sizeof(Ty_);
        }

    private:
        Ty_ value_;
    };

    InnerBase::Pointer inner_;
};

class Any {
public:
    bool is_empty_ = true;
    Var v_;
    Any() {
    }

    template <class T>
    Any(const T& value) : is_empty_(false), v_(value) {
    }

    template <class T>
    Any& operator=(const T& value) {
        v_ = value;
        is_empty_ = false;
        return *this;
    }

    Any(const Any& rhs) : is_empty_(rhs.is_empty_), v_(rhs.v_) {
    }
    Any& operator=(const Any& rhs) {
        v_ = rhs.v_;
        is_empty_ = rhs.is_empty_;
        return *this;
    }
    ~Any() = default;

    bool Empty() const {
        return is_empty_;
    }

    void Clear() {
        is_empty_ = true;
    }
    void Swap(Any& rhs) {
        std::swap(v_, rhs.v_);
        std::swap(is_empty_, rhs.is_empty_);
    }

    template <class T>
    const T& GetValue() const {
        if (dynamic_cast<Var::Inner<T>*>(v_.inner_.get()) == nullptr) {
            throw std::bad_cast();
        }
        try {
            dynamic_cast<Var::Inner<T>*>(v_.inner_.get());
            **dynamic_cast<Var::Inner<T>*>(v_.inner_.get());
        } catch (...) {
            throw std::bad_cast();
        }
        return **dynamic_cast<Var::Inner<T>*>(v_.inner_.get());
    }
};
