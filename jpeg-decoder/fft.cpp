#include "fft.h"

#include <fftw3.h>
#include <cmath>
#include <stdexcept>

DctCalculator::DctCalculator(size_t width, std::vector<double> *input, std::vector<double> *output)
    : width_(width), input_(input), output_(output) {
    if (input && output && (width * width != input->size() || width * width != output->size())) {
        throw std::invalid_argument("");
    }
}

void DctCalculator::Inverse() {
    for (size_t i = 0; i < width_; ++i) {
        (*input_)[width_ * 0 + i] *= sqrt(2);
        (*input_)[width_ * i + 0] *= sqrt(2);
    }
    fftw_plan p = fftw_plan_r2r_2d(width_, width_, input_->data(), output_->data(), FFTW_REDFT01,
                                   FFTW_REDFT01, 0);
    fftw_execute(p);
    for (double &elem : (*output_)) {
        elem /= 16;
    }
    fftw_destroy_plan(p);
}
