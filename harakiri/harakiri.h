#pragma once

#include <string>

// Should not allow reuse and yell under sanitizers.
// Fix the interface and implementation.
// AwesomeCallback should add "awesomeness".

class OneTimeCallback {
public:
    virtual ~OneTimeCallback() {
    }
    virtual std::string operator()() const&& = 0;
};

// Implement ctor, operator(), maybe something else...
class AwesomeCallback : public OneTimeCallback {
private:
    std::string str_;

public:
    AwesomeCallback(std::string s) {
        str_ = s;
    }
    virtual std::string operator()() const&& {
        std::string str_copy = str_ + "awesomeness";
        delete this;
        return str_copy;
    }
};
