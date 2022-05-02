#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

struct Node {
    int val;
    bool terminated;
    Node(int vall, bool termm) {
        val = vall;
        terminated = termm;
    }
};

class HuffmanTree {
public:
    void Build(const std::vector<uint8_t>& code_lengths, const std::vector<uint8_t>& values);

    bool Move(bool bit, int& value);

    void Build2(std::vector<uint8_t>& code_lengths, std::vector<uint8_t>& values, int i_code,
                int& i_vals, int v);

    int ind = 0;
    std::vector<Node> vec = {Node(-1, 0), Node(-1, 0), Node(-1, 0)};
};
