#pragma once

struct Sum {
    template <class T>
    void operator()(T& a, T b) {
        a += b;
    }
};

struct Prod {
    template <class T>
    void operator()(T& a, T b) {
        a *= b;
    }
};
struct Concat {
    template <class T>
    void operator()(T& a, T b) {
        for (auto i : b) {
            a.push_back(i);
        }
    }
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    for (; first != last; ++first) {
        func(init, *first);
    }
    return init;
}
class Length {
public:
    Length(int* sz) {
        sz_ = sz;
    }
    template <class T>
    void operator()(T, T) {
        ++(*sz_);
    }

private:
    int* sz_;
};
