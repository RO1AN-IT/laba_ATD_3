#pragma once
#include "IEnumerable.h"

template<typename T>
class ICollection: public IEnumerable<T> {
public: 
    virtual void Add(T item) = 0;
    virtual void Clear() = 0;
    virtual bool Contains(T item) const = 0;
    virtual bool Remove(T item) = 0;
    virtual int Count() const = 0;
    virtual ~ICollection() = default;
};