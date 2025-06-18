#pragma once
#include "help_from_laba_2/ArraySequence.h"
#include "help_from_laba_2/IEnumerable.h"
#include "help_from_laba_2/IntegerRing.h"
#include <stdexcept>
#include <memory>
#include <tuple>
#include <cmath>
#include <algorithm>

template<typename T>
class Vector {
public:
    Vector() {
        data = new MutableArraySequence<T>();
    }

    Vector(int size) {
        if (size < 0) {
            throw std::invalid_argument("size must be non-negative");
        }
        data = new MutableArraySequence<T>(size);
    }

    Vector(T* coords, int size) {
        if (size < 0) {
            throw std::invalid_argument("size must be non-negative");
        }
        data = new MutableArraySequence<T>(coords, size);
    }

    Vector(const Vector<T>& other) {
        data = new MutableArraySequence<T>(*other.data);
    }

    ~Vector() {
        delete data;
    }

    T Get(int index) {
        if (index < 0 || index >= data->GetLength()) {
            throw std::out_of_range("Index out of range");
        }
        return data->Get(index);
    }

    const T& Get(int index) const {
        if (index < 0 || index >= data->GetLength()) {
            throw std::out_of_range("Index out of range");
        }
        return data->Get(index);
    }

    Vector<T>& Set(int index, const T& value) {
        if (index < 0 || index >= data->GetLength()) {
            throw std::out_of_range("Index out of range");
        }
        data->InsertAt(value, index);
        return *this;
    }

    Vector<T>& operator=(const Vector<T>& other) {
        if (this != &other) {
            delete data;
            data = new MutableArraySequence<T>(*other.data);
        }
        return *this;
    }

    T& operator[](int index) {
        if (index < 0 || index >= data->GetLength()) {
            throw std::out_of_range("Index out of range");
        }
        return data->Get(index);
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= data->GetLength()) {
            throw std::out_of_range("Index out of range");
        }
        return data->Get(index);
    }

    Vector<T> operator+(const Vector<T>& other) const {
        if (data->GetLength() != other.data->GetLength()) {
            throw std::invalid_argument("Vectors must have the same size");
        }
        Vector<T> result(data->GetLength());
        for (int i = 0; i < data->GetLength(); ++i) {
            result.Set(i, data->Get(i) + other.data->Get(i));
        }
        return result;
    }

    Vector<T> operator-(const Vector<T>& other) const {
        if (data->GetLength() != other.data->GetLength()) {
            throw std::invalid_argument("Vectors must have the same size");
        }
        Vector<T> result(data->GetLength());
        for (int i = 0; i < data->GetLength(); ++i) {
            result.Set(i, data->Get(i) - other.data->Get(i));
        }
        return result;
    }

    Vector<T> operator*(T scalar) const {
        Vector<T> result(data->GetLength());
        for (int i = 0; i < data->GetLength(); ++i) {
            result.Set(i, data->Get(i) * scalar);
        }
        return result;
    }

    T operator*(const Vector<T>& other) const {
        if (data->GetLength() != other.data->GetLength()) {
            throw std::invalid_argument("Vectors must have the same size");
        }
        T result = 0;
        for (int i = 0; i < data->GetLength(); ++i) {
            result += data->Get(i) * other.data->Get(i);
        }
        return result;
    }

    double norm() const {
        T sum = 0;
        for (int i = 0; i < data->GetLength(); ++i) {
            sum += data->Get(i) * data->Get(i);
        }
        return std::sqrt(static_cast<double>(sum));
    }

    Vector<double> normalize() const {
        double n = norm();
        if (n == 0) {
            throw std::runtime_error("Cannot normalize zero vector");
        }
        Vector<double> result(data->GetLength());
        for (int i = 0; i < data->GetLength(); ++i) {
            result.Set(i, static_cast<double>(data->Get(i)) / n);
        }
        return result;
    }

    bool isCollinear(const Vector<T>& other) const {
        if (data->GetLength() != other.data->GetLength()) {
            return false;
        }
        if (data->GetLength() == 0) {
            return true; 
        }

        int firstNonZero = -1;
        for (int i = 0; i < data->GetLength(); ++i) {
            if (data->Get(i) != 0) {
                firstNonZero = i;
                break;
            }
        }

        if (firstNonZero == -1) {
            for (int i = 0; i < other.data->GetLength(); ++i) {
                if (other.data->Get(i) != 0) {
                    return false;
                }
            }
            return true;
        }

        double ratio = static_cast<double>(other.data->Get(firstNonZero)) / data->Get(firstNonZero);
        for (int i = 0; i < data->GetLength(); ++i) {
            if (std::abs(static_cast<double>(other.data->Get(i)) - ratio * data->Get(i)) > 1e-10) {
                return false;
            }
        }
        return true;
    }

    int size() const {
        return data->GetLength();
    }

    bool empty() const {
        return data->GetLength() == 0;
    }

    void clear() {
        data->Clear();
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& vector) {
        os << "[ ";
        for (int i = 0; i < vector.data->GetLength(); ++i) {
            os << vector.data->Get(i);
            if (i < vector.data->GetLength() - 1) {
                os << ", ";
            }
        }
        os << " ]";
        return os;
    }

    friend Vector<T> operator*(T scalar, const Vector<T>& vector) {
        return vector * scalar;
    }

    template<typename U>
    Vector<U>* Map(U (*f)(T)) {
        this->data = static_cast<MutableArraySequence<U>*>(this->data->Map(f));
        return this;
    }

    template<typename U>
    Vector<U>* Reduce(U (*f)(U, T), const U c) {
        this->data = static_cast<MutableArraySequence<U>*>(this->data->Reduce(f, c));
        return this;
    }

    template<typename... Tuples, typename U>
    Vector<std::tuple<Tuples..., U>>* Zip(Vector<U>* other) {
        this->data = static_cast<MutableArraySequence<std::tuple<Tuples..., U>>*>(this->data->Zip(other->data));
        return this;
    }

    template<typename... Tuples>
    std::tuple<Vector<Tuples>*...>* UnZip() const {
        auto tuple = data->UnZip();
        std::tuple<Vector<Tuples>...>* result = new std::tuple<Vector<Tuples>...>();
        return result;
    }

    template<typename V>
    Vector<Vector<V>*>* Split(bool (*f)(V)) {
        this->data = static_cast<MutableArraySequence<MutableArraySequence<V>>*>(this->data->Split(f));
        return this;
    }

    Vector<T>* Where(bool (*f)(T)) {
        this->data = static_cast<MutableArraySequence<T>*>(this->data->Where(f));
        return this;
    }

    Vector<T>* From(T* items, int size) {
        this->data = static_cast<MutableArraySequence<T>*>(this->data->From(items, size));
        return this;
    }

    std::unique_ptr<IEnumerator<T>> GetEnumerator() const {
        return std::make_unique<VectorEnumerator>(data, data->GetLength());
    }

protected:
    MutableArraySequence<T>* data;

    class VectorEnumerator : public IEnumerator<T> {
    private:
        const MutableArraySequence<T>* data;
        int currentIndex;
        int size;

    public:
        VectorEnumerator(const MutableArraySequence<T>* d, int s) : data(d), size(s), currentIndex(-1) {}

        bool MoveNext() override {
            if (currentIndex + 1 < size) {
                currentIndex++;
                return true;
            }
            return false;
        }

        T Current() const override {
            if (currentIndex < 0 || currentIndex >= size) {
                throw std::out_of_range("No current element");
            }
            return data->Get(currentIndex);
        }

        void Reset() override {
            currentIndex = -1;
        }
    };
};