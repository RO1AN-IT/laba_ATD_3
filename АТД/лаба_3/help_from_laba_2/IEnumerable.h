#pragma once
#include <memory>

template<typename T>
class IEnumerator;

template<typename T>
class IEnumerable{
public:
    virtual std::unique_ptr<IEnumerator<T>> GetEnumerator() const = 0;
    virtual ~IEnumerable() = default; 
};

template<typename T>
class IEnumerator{
public: 
    virtual bool MoveNext() = 0;
    virtual T Current() const = 0;
    virtual void Reset() = 0;
    virtual ~IEnumerator() = default;
};
