#pragma once
#include "help_from_laba_2/ArraySequence.h"
#include "help_from_laba_2/IEnumerable.h"
#include <stdexcept>
#include <memory>
#include <tuple>
#include <algorithm>
#include <utility>

template<typename T>
class DiagonalMatrix {
protected:
    MutableArraySequence<T>* data;
    int n;

    int getIndex(int i, int j) const {
        if (i < 0 || i >= n || j < 0 || j >= n) {
            throw std::out_of_range("Matrix index out of range");
        }
        if (i != j) return -1;
        return i;
    }

    class MatrixEnumerator : public IEnumerator<T> {
    private:
        const MutableArraySequence<T>* data;
        int currentIndex;
        int size;

    public:
        MatrixEnumerator(const MutableArraySequence<T>* d, int s) : data(d), size(s), currentIndex(-1) {}

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

public:
    DiagonalMatrix() : n(0) {
        data = new MutableArraySequence<T>();
    }

    DiagonalMatrix(int size) : n(size) {
        if (size < 0) {
            throw std::invalid_argument("Size must be non-negative");
        }
        data = new MutableArraySequence<T>(size);
    }

    DiagonalMatrix(T* items, int size) : n(size) {
        if (size < 0) {
            throw std::invalid_argument("Size must be non-negative");
        }
        data = new MutableArraySequence<T>(items, size);
    }

    DiagonalMatrix(const DiagonalMatrix<T>& other) : n(other.n) {
        data = new MutableArraySequence<T>(*other.data);
    }

    ~DiagonalMatrix() {
        delete data;
    }

    DiagonalMatrix<T>& operator=(const DiagonalMatrix<T>& other) {
        if (this != &other) {
            delete data;
            n = other.n;
            data = new MutableArraySequence<T>(*other.data);
        }
        return *this;
    }

    T get(int i, int j) const {
        int index = getIndex(i, j);
        if (index == -1) return T(0);
        return data->Get(index);
    }

    void set(int i, int j, T value) {
        int index = getIndex(i, j);
        if (index == -1 && value != T(0)) {
            throw std::invalid_argument("Cannot set non-zero value off diagonal");
        }
        if (index != -1) {
            data->InsertAt(value, index);
        }
    }

    T operator()(int i, int j) const {
        return get(i, j);
    }

    DiagonalMatrix<T> operator+(const DiagonalMatrix<T>& other) const {
        if (n != other.n) {
            throw std::invalid_argument("Matrices must have the same size");
        }
        DiagonalMatrix<T> result(n);
        for (int i = 0; i < n; ++i) {
            result.data->InsertAt(data->Get(i) + other.data->Get(i), i);
        }
        return result;
    }

    DiagonalMatrix<T> operator*(const DiagonalMatrix<T>& other) const {
        if (n != other.n) {
            throw std::invalid_argument("Matrices must have the same size");
        }
        DiagonalMatrix<T> result(n);
        for (int i = 0; i < n; ++i) {
            result.data->InsertAt(data->Get(i) * other.data->Get(i), i);
        }
        return result;
    }

    DiagonalMatrix<T> operator*(T scalar) const {
        DiagonalMatrix<T> result(n);
        for (int i = 0; i < n; ++i) {
            result.data->InsertAt(data->Get(i) * scalar, i);
        }
        return result;
    }
    
    bool operator==(const DiagonalMatrix<T>& other) const {
        if (n != other.n) return false;
        for (int i = 0; i < n; ++i) {
            if (data->Get(i) != other.data->Get(i)) {
                return false;
            }
        }
        return true;
    }

    DiagonalMatrix<T> transpose() const {
        return *this;
    }

    T determinant() const {
        if (n == 0) {
            throw std::runtime_error("Determinant of empty matrix is undefined");
        }
        T det = T(1);
        for (int i = 0; i < n; ++i) {
            det *= data->Get(i);
        }
        return det;
    }

    int size() const { return n; }

    bool empty() const { return n == 0; }

    void clear() {
        delete data;
        data = new MutableArraySequence<T>();
        n = 0;
    }

    T Norm() const {
        T sum = T(0);
        for (int i = 0; i < n; ++i) {
            T val = data->Get(i);
            sum += val * val;
        }
        return std::sqrt(sum);
    }

    friend std::ostream& operator<<(std::ostream& os, const DiagonalMatrix<T>& matrix) {
        for (int i = 0; i < matrix.n; ++i) {
            os << "[ ";
            for (int j = 0; j < matrix.n; ++j) {
                os << matrix.get(i, j);
                if (j < matrix.n - 1) {
                    os << ", ";
                }
            }
            os << " ]";
            if (i < matrix.n - 1) {
                os << "\n";
            }
        }
        return os;
    }

    std::unique_ptr<IEnumerator<T>> GetEnumerator() const {
        return std::make_unique<MatrixEnumerator>(data, n);
    }
};