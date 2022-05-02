#pragma once

#include <cstddef>
#include <vector>

class DctCalculator {
public:
    DctCalculator(size_t width, std::vector<double> *input, std::vector<double> *output);

    void Inverse();

    size_t width_;
    std::vector<double> *input_, *output_;
};
