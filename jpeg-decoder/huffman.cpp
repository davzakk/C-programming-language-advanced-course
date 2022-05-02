#include "huffman.h"

int Sum(const std::vector<uint8_t> &vec) {
    int ans = 0;
    for (auto i : vec) {
        ans += i;
    }
    return ans;
}

void HuffmanTree::Build2(std::vector<uint8_t> &code_lengths, std::vector<uint8_t> &values,
                         int i_code, int &i_vals, int v) {
    if (Sum(code_lengths) == 0) {
        return;
    }
    int l = 2 * v + 1, r = 2 * v + 2;

    if (v == 0) {
        vec[v].val = -2;
        HuffmanTree::Build2(code_lengths, values, i_code + 1, i_vals, l);
        HuffmanTree::Build2(code_lengths, values, i_code + 1, i_vals, r);
        return;
    }

    if (code_lengths[i_code] != 0) {
        if (i_vals >= static_cast<int>(values.size())) {
            throw std::invalid_argument{"3433"};
        }
        vec[v].val = values[i_vals];
        vec[v].terminated = true;
        i_vals++;
        code_lengths[i_code]--;
    } else {
        vec[v].val = -2;
        HuffmanTree::Build2(code_lengths, values, i_code + 1, i_vals, l);
        HuffmanTree::Build2(code_lengths, values, i_code + 1, i_vals, r);
    }
}

void HuffmanTree::Build(const std::vector<uint8_t> &code_lengths,
                        const std::vector<uint8_t> &values) {
    if (code_lengths.size() > 16) {
        throw std::invalid_argument{"big"};
    }
    if (Sum(code_lengths) != static_cast<int>(values.size())) {
        throw std::invalid_argument{"unequal"};
    }
    *this = HuffmanTree();
    vec.assign(1 << 17, Node(-1, 0));
    int i_code = -1, i_vals = 0;
    std::vector<uint8_t> code_lengths2 = code_lengths, values2 = values;
    Build2(code_lengths2, values2, i_code, i_vals, 0);
    if (Sum(code_lengths2) != 0) {
        throw std::invalid_argument{"a lot"};
    }
}

bool HuffmanTree::Move(bool bit, int &value) {
    if (vec[2 * ind + 1 + bit].val != -1) {
        if (vec[2 * ind + 1 + bit].terminated) {
            value = vec[2 * ind + 1 + bit].val;
            ind = 0;
            return true;
        } else {
            ind = 2 * ind + 1 + bit;
        }
    } else {
        throw std::invalid_argument{"moveer"};
    }
    return false;
}
