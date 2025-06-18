#pragma once
#include "help_from_laba_2/ArraySequence.h"
#include "help_from_laba_2/IEnumerable.h"
#include <stdexcept>
#include <memory>
#include <tuple>
#include <algorithm>
#include <utility>

template<typename T>
class TriangularMatrix {
protected:
    MutableArraySequence<T>* data; 
    int n;
    bool isUpper; 

    int getIndex(int i, int j) const {
        if (i < 0 || i >= n || j < 0 || j >= n) {
            throw std::out_of_range("Matrix index out of range");
        }
        if (isUpper && i > j) return -1; 
        if (!isUpper && i < j) return -1; 
        if (isUpper) {
            return i * n - (i * (i - 1)) / 2 + j;
        } else {
            return (i * (i + 1)) / 2 + j;
        }
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
    TriangularMatrix() : n(0), isUpper(true) {
        data = new MutableArraySequence<T>();
    }

    TriangularMatrix(int size, bool isUpper = true) : n(size), isUpper(isUpper) {
        if (size < 0) {
            throw std::invalid_argument("Size must be non-negative");
        }
        data = new MutableArraySequence<T>(size * size);
    }

    TriangularMatrix(T* items, int size, bool isUpper = true) : n(size), isUpper(isUpper) {
        if (size < 0) {
            throw std::invalid_argument("Size must be non-negative");
        }
        data = new MutableArraySequence<T>(items, size * size);
    }

    TriangularMatrix(const TriangularMatrix<T>& other) : n(other.n), isUpper(other.isUpper) {
        data = new MutableArraySequence<T>(*other.data);
    }

    ~TriangularMatrix() {
        delete data;
    }

    TriangularMatrix<T>& operator=(const TriangularMatrix<T>& other) {
        if (this != &other) {
            delete data;
            n = other.n;
            isUpper = other.isUpper;
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
            throw std::invalid_argument("Cannot set non-zero value in zero region");
        }
        if (index != -1) {
            data->InsertAt(value, index);
        }
    }

    T operator()(int i, int j) const {
        return get(i, j);
    }

    TriangularMatrix<T> operator+(const TriangularMatrix<T>& other) const {
        if (n != other.n || isUpper != other.isUpper) {
            throw std::invalid_argument("Matrices must have the same size and type");
        }
        TriangularMatrix<T> result(n, isUpper);
        for (int i = 0; i < (n * (n + 1)) / 2; ++i) {
            result.data->InsertAt(data->Get(i) + other.data->Get(i), i);
        }
        return result;
    }

    TriangularMatrix<T> operator*(const TriangularMatrix<T>& other) const {
        if (n != other.n || isUpper != other.isUpper) {
            throw std::invalid_argument("Matrices must have the same size and type");
        }
        TriangularMatrix<T> result(n, isUpper);
        for (int i = 0; i < n; ++i) {
            for (int j = (isUpper ? i : 0); j < (isUpper ? n : i + 1); ++j) {
                T sum = 0;
                for (int k = (isUpper ? i : j); k < (isUpper ? j + 1 : n); ++k) {
                    sum += get(i, k) * other.get(k, j);
                }
                result.set(i, j, sum);
            }
        }
        return result;
    }

    TriangularMatrix<T> operator*(T scalar) const {
        TriangularMatrix<T> result(n, isUpper);
        for (int i = 0; i < (n * (n + 1)) / 2; ++i) {
            result.data->InsertAt(data->Get(i) * scalar, i);
        }
        return result;
    }

    bool operator==(const TriangularMatrix<T>& other) const {
        if (n != other.n || isUpper != other.isUpper) return false;
        for (int i = 0; i < (n * (n + 1)) / 2; ++i) {
            if (data->Get(i) != other.data->Get(i)) {
                return false;
            }
        }
        return true;
    }
    
    TriangularMatrix<T>* transpose() const {
        TriangularMatrix<T>* result = new TriangularMatrix<T>(n, !isUpper);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if ((isUpper && i <= j) || (!isUpper && i >= j)) {
                    result->set(j, i, get(i, j));
                }
            }
        }
        return result;
    }

    T determinant() const {
        if (n == 0) {
            throw std::runtime_error("Determinant of empty matrix is undefined");
        }
        T det = T(1);
        for (int i = 0; i < n; ++i) {
            det *= get(i, i);
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
        for (int i = 0; i < (n * (n + 1)) / 2; ++i) {
            T val = data->Get(i);
            sum += val * val;
        }
        return std::sqrt(sum);
    }

    friend std::ostream& operator<<(std::ostream& os, const TriangularMatrix<T>& matrix) {
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
        return std::make_unique<MatrixEnumerator>(data, (n * (n + 1)) / 2);
    }
};