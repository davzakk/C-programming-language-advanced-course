#include "ugly.h"
#include <vector>
#include <memory>
#include "bad.h"

struct SplineImpl {
    size_t sz_;
    const double *x_, *y_;
    double a_, b_;
    SplineImpl(const std::vector<double>& x, const std::vector<double>& y, double a, double b)
        : sz_(x.size()), x_(&x[0]), y_(&y[0]), a_(a), b_(b) {
    }
    double Interpolate(double x) {
        double* y2 = new double[10];
        double y;
        mySplineSnd(x_, y_, sz_, a_, b_, y2);
        mySplintCube(x_, y_, y2, sz_, x, &y);
        delete[] y2;
        return y;
    }
};

Spline::Spline(const std::vector<double>& x, const std::vector<double>& y, double a, double b)
    : impl_(std::make_shared<SplineImpl>(SplineImpl(x, y, a, b))) {
}

double Spline::Interpolate(double x) {
    return impl_->Interpolate(x);
}
