#pragma once
#include <vector>
#include <string>
struct State {
    int ref_count;  // сколько векторов делят этот State между собой.
    std::vector<std::string> vec;
    // другие поля
    State(std::vector<std::string> v) {
        vec = v;
        ref_count = 1;
    }
    State() {
        vec = {};
        ref_count = 1;
    }
};

class COWVector {
public:
    COWVector() {
        sh_ = new State();
    }
    COWVector(std::vector<std::string> vec) {
        sh_ = new State(vec);
    }
    ~COWVector() {
        sh_->ref_count--;
        if (sh_->ref_count == 0) {
            delete sh_;
        }
    }

    COWVector(const COWVector& other) {
        sh_ = other.sh_;
        other.sh_->ref_count++;
    }
    COWVector& operator=(const COWVector& other) {
        sh_->ref_count--;
        if (!sh_->ref_count) {
            delete sh_;
        }
        sh_ = other.sh_;
        other.sh_->ref_count++;
        return *this;
    }

    size_t Size() const {
        return sh_->vec.size();
    }

    void Resize(size_t size) {
        if (sh_->ref_count != 1) {
            sh_->ref_count--;
            sh_ = new State(sh_->vec);
        }
        sh_->vec.resize(size);
    }

    const std::string& Get(size_t at) {
        return sh_->vec[at];
    }

    const std::string& Back() {
        return sh_->vec.back();
    }

    void PushBack(const std::string& value) {
        if (sh_->ref_count != 1) {
            sh_->ref_count--;
            sh_ = new State(sh_->vec);
        }
        sh_->vec.push_back(value);
    }

    void Set(size_t at, const std::string& value) {
        if (sh_->ref_count != 1) {
            sh_->ref_count--;
            sh_ = new State(sh_->vec);
        }
        sh_->vec[at] = value;
    }

private:
    State* sh_;
};
