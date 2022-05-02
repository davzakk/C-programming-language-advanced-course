#pragma once

#include <string>
#include <algorithm>
#include <cstring>
class StringView {

private:
    size_t sz_;
    const char* c_;

public:
    StringView(const std::string& s, size_t c = 0, size_t siz = std::string::npos) {  //
        sz_ = siz;
        if (siz > s.size() - c) {
            sz_ = s.size() - c;
        }
        c_ = s.data() + c;
    }
    StringView(const char* c) {
        c_ = c;
        sz_ = strlen(c);
    }
    StringView(const char* c, size_t sz) {
        c_ = c;
        sz_ = sz;
    }
    char operator[](size_t i) {
        return *(c_ + i);
    }
    char operator[](size_t i) const {
        return *(c_ + i);
    }
    size_t Size() const {
        return sz_;
    }
};
