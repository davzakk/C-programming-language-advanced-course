#pragma once
#include <iterator>
#include <exception>

template <typename F, typename Iterator, typename P>
void TransformIf(Iterator begin, Iterator end, P p, F f) {
    auto x = *begin;
    for (Iterator begin_copy = begin; begin_copy != end; begin_copy++) {
        try {
            x = *begin_copy;
        } catch (...) {
        }
        if (p(x)) {
            f(x);
        }
    }
    for (Iterator begin_copy = begin; begin_copy != end; begin_copy++) {
        if (p(*begin_copy)) {
            f(*begin_copy);
        }
    }
}
