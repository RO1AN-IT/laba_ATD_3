#pragma once

template<typename T>
class IGroup {
public:
    virtual T Identity() const = 0;
    virtual T Operation(T a, T b) const = 0;
    virtual T Inverse(T a) const = 0;
    virtual ~IGroup() = default;
};

template<typename T>
class IRing : public IGroup<T> {
public:
    virtual T Zero() const = 0;
    virtual T Multiplication(T a, T b) const = 0;
    virtual ~IRing() = default;
};