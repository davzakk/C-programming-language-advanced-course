#pragma once

#include "algo_impl.h"
#include <typeinfo>

/// Use one of the following:
/// AlgoRes ComputeIntegral(data);
/// AlgoRes ComputeUnsignedIntegral(data);
/// AlgoRes ComputeFloat(data);
/// AlgoRes ComputeGeneral(data);

template <class T>
AlgoRes Run(T data) {
    if (typeid(data) == typeid(40)) {
        return ComputeIntegral(data);
    } else if (typeid(data) == typeid(0.0f) || typeid(data) == typeid(-3.1415926)) {
        return ComputeFloat(data);
    } else if (typeid(data) == typeid(50u)) {
        return ComputeUnsignedIntegral(data);
    } else {
        return ComputeGeneral(data);
    }
}
