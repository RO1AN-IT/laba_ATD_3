#pragma once
#include "IGroup.h"

template<typename T>
class IntegerRing : public IRing<T> {
public:
    T Identity() const override {
        return 1;
    }

    T Operation(T a, T b) const override {
        return a + b;
    }

    T Inverse(T a) const override {
        return -a;
    }

    T Zero() const override {
        return 0;
    }

    T Multiplication(T a, T b) const override {
        return a * b;
    }
};