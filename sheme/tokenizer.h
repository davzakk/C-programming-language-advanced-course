#pragma once

#include <variant>
#include <optional>
#include <istream>

#include <iostream>
#include <set>
//#include "../error.h"
#include "error.h"

// char all_sym_start[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM<=>*#",
// all_sym_in[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM<=>*#01234789?!-",
// digitss[] = "01234789";

struct SymbolToken {
    std::string name;

    bool operator==(const SymbolToken& other) const {
        return name == other.name;
    }
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const {
        return true;
    }
};

struct DotToken {
    bool operator==(const DotToken&) const {
        return true;
    }
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    bool operator==(const ConstantToken& other) const {
        return value == other.value;
    }
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

class Tokenizer {
public:
    std::set<char> digitss = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'},
                   all_sym_in = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's',
                                 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b',
                                 'n', 'm', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
                                 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C',
                                 'V', 'B', 'N', 'M', '<', '=', '>', '*', '#', '0', '1', '2',
                                 '3', '4', '7', '8', '9', '?', '!', '-'},
                   all_sym_start = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's',
                                    'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b',
                                    'n', 'm', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
                                    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C',
                                    'V', 'B', 'N', 'M', '<', '=', '>', '*', '#'};
    std::istream* in_;
    std::string s;
    char c;
    Token token;
    int isend = 0;

    Tokenizer(std::istream* in) {
        in_ = in;
        IsEof();
        c = in_->get();
        Next();
    }
    void IsEof() {
        if (in_->peek() == EOF) {
            isend++;
        }
    }
    bool IsEnd() {
        return isend >= 2;
        // in_->peek() == EOF;
    }

    void Next() {
        if (c == -1) {
            isend++;
            return;
        }
        while ((c == ' ' || c == '\n') && !IsEnd()) {
            if (in_->peek() == EOF) {
                isend = true;
            }
            IsEof();
            c = in_->get();
        }
        if (IsEnd()) {
            return;
        }
        if (c == '.') {
            IsEof();
            c = in_->get();
            token = Token{DotToken{}};
        } else if (c == '\'') {
            IsEof();
            c = in_->get();
            token = Token{QuoteToken{}};
        } else if (c == '(') {
            IsEof();
            c = in_->get();
            token = Token{BracketToken::OPEN};
        } else if (c == ')') {
            IsEof();
            c = in_->get();
            token = Token{BracketToken::CLOSE};
        } else {
            if (c == '+' || c == '-') {
                s += c;
                IsEof();
                c = in_->get();
            }
            if (digitss.find(c) != digitss.end()) {
                while (digitss.find(c) != digitss.end() && !IsEnd()) {
                    s += c;
                    IsEof();
                    c = in_->get();
                }
                token = Token{ConstantToken{stoi(s)}};
            } else {
                if (s.empty() && all_sym_start.find(c) == all_sym_start.end()) {
                    throw SyntaxError("");
                }
                while (c != ' ' && c != '\n' && all_sym_in.find(c) != all_sym_in.end() &&
                       !IsEnd()) {
                    s += c;
                    IsEof();
                    c = in_->get();
                }
                token = Token{SymbolToken{s}};
            }
        }
        s = "";
    }

    Token GetToken() {
        return token;
    }
};