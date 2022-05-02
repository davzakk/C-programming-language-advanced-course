#pragma once
#include <string>
#include <vector>
#include <iterator>

class Editor {
public:
    std::string s_;
    std::size_t ind_ = 0;
    std::vector<std::pair<std::size_t, std::pair<std::string, char>>> pas_, fut_;

    const std::string &GetText() const {
        return s_;
    }

    void Type(char c) {
        fut_.clear();
        pas_.push_back({ind_, {"Type", c}});
        std::string ss;
        ss += c;
        s_.insert(ind_, ss);
        ind_++;
    }
    void Type2(char c) {
        std::string ss;
        ss += c;
        s_.insert(ind_, ss);
        ind_++;
    }

    void ShiftLeft() {
        if (ind_ > 0) {
            fut_.clear();
            pas_.push_back({ind_, {"ShiftLeft", '$'}});
            ind_--;
        }
    }
    void ShiftLeft2() {
        if (ind_ > 0) {
            ind_--;
        }
    }

    void ShiftRight() {
        if (ind_ < s_.size()) {
            fut_.clear();
            pas_.push_back({ind_, {"ShiftRight", '$'}});
            ind_++;
        }
    }
    void ShiftRight2() {
        if (ind_ < s_.size()) {
            ind_++;
        }
    }

    void Backspace() {
        if (ind_ > 0) {
            fut_.clear();
            pas_.push_back({ind_, {"Backspace", s_[ind_ - 1]}});
            s_.erase(ind_ - 1, 1);
            ind_--;
        }
    }
    void Backspace2() {
        if (ind_ > 0) {
            s_.erase(ind_ - 1, 1);
            ind_--;
        }
    }

    void Undo() {
        if (!pas_.empty()) {
            fut_.push_back(pas_.back());
            if (pas_.back().second.first == "Type") {
                Backspace2();
            } else if (pas_.back().second.first == "ShiftLeft") {
                ShiftRight2();
            } else if (pas_.back().second.first == "ShiftRight") {
                ShiftLeft2();
            } else if (pas_.back().second.first == "Backspace") {
                Type2(pas_.back().second.second);
            }
            // ind_ = pas_.back().first;
            pas_.pop_back();
        }
    }

    void Redo() {
        if (!fut_.empty()) {
            pas_.push_back(fut_.back());
            if (pas_.back().second.first == "Type") {
                Type2(pas_.back().second.second);
            } else if (pas_.back().second.first == "ShiftLeft") {
                ShiftLeft2();
            } else if (pas_.back().second.first == "ShiftRight") {
                ShiftRight2();
            } else if (pas_.back().second.first == "Backspace") {
                Backspace2();
            }
            // ind_ = fut_.back().first;
            fut_.pop_back();
        }
    }
};
