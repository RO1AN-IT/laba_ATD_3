#pragma once
#include "help_from_laba_2/ArraySequence.h"
#include "help_from_laba_2/IEnumerable.h"
#include <stdexcept>
#include <memory>
#include <tuple>
#include <algorithm>
#include <utility>

template<typename T>
class RectangularMatrix {
protected:
    MutableArraySequence<T>* data; 
    int rows; 
    int cols; 

    int getIndex(int i, int j) const {
        if (i < 0 || i >= rows || j < 0 || j >= cols) {
            throw std::out_of_range("Matrix index out of range");
        }
        return i * cols + j;
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
    RectangularMatrix() : rows(0), cols(0) {
        data = new MutableArraySequence<T>();
    }

    RectangularMatrix(int rows, int cols) : rows(rows), cols(cols) {
        if (rows < 0 || cols < 0) {
            throw std::invalid_argument("Rows and columns must be non-negative");
        }
        data = new MutableArraySequence<T>(rows * cols);
    }

    RectangularMatrix(T* items, int rows, int cols) : rows(rows), cols(cols) {
        if (rows < 0 || cols < 0) {
            throw std::invalid_argument("Rows and columns must be non-negative");
        }
        data = new MutableArraySequence<T>(items, rows * cols);
    }

    RectangularMatrix(const RectangularMatrix<T>& other) : rows(other.rows), cols(other.cols) {
        data = new MutableArraySequence<T>(*other.data);
    }

    ~RectangularMatrix() {
        delete data;
        rows = 0;
        cols = 0;
    }

    RectangularMatrix<T>& operator=(const RectangularMatrix<T>& other) {
        if (this != &other) {
            delete data;
            rows = other.rows;
            cols = other.cols;
            data = new MutableArraySequence<T>(*other.data);
        }
        return *this;
    }

    T get(int i, int j) const {
        return data->Get(getIndex(i, j));
    }

    void set(int i, int j, T value) {
        data->InsertAt(value, getIndex(i, j));
    }

    T operator()(int i, int j) const {
        return get(i, j);
    }

    RectangularMatrix<T> operator+(const RectangularMatrix<T>& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrices must have the same dimensions");
        }
        RectangularMatrix<T> result(rows, cols);
        for (int i = 0; i < rows * cols; ++i) {
            result.data->InsertAt(data->Get(i) + other.data->Get(i), i);
        }
        return result;
    }

    RectangularMatrix<T> operator*(const RectangularMatrix<T>& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Matrix dimensions are not compatible for multiplication");
        }
        RectangularMatrix<T> result(rows, other.cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                T sum = 0;
                for (int k = 0; k < cols; ++k) {
                    sum += get(i, k) * other.get(k, j);
                }
                result.set(i, j, sum);
            }
        }
        return result;
    }

    RectangularMatrix<T> operator*(T scalar) const {
        RectangularMatrix<T> result(rows, cols);
        for (int i = 0; i < rows * cols; ++i) {
            result.data->InsertAt(data->Get(i) * scalar, i);
        }
        return result;
    }


    bool operator==(const RectangularMatrix<T>& other) const {
        if (rows != other.rows || cols != other.cols) return false;
        for (int i = 0; i < rows * cols; ++i) {
            if (data->Get(i) != other.data->Get(i)) {
                return false;
            }
        }
        return true;
    }

    RectangularMatrix<T> transpose() const {
        RectangularMatrix<T> result(cols, rows);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.set(j, i, get(i, j));
            }
        }
        return result;
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }

    bool empty() const {
        return rows == 0 || cols == 0;
    }

    void clear() {
        delete data;
        data = new MutableArraySequence<T>();
        rows = 0;
        cols = 0;
    }

    T Norm() const {
        T sum = T(0);
        for (int i = 0; i < rows * cols; ++i) {
            T val = data->Get(i);
            sum += val * val;
        }
        return std::sqrt(sum);
    }

    friend std::ostream& operator<<(std::ostream& os, const RectangularMatrix<T>& matrix) {
        for (int i = 0; i < matrix.rows; ++i) {
            os << "[ ";
            for (int j = 0; j < matrix.cols; ++j) {
                os << matrix.get(i, j);
                if (j < matrix.cols - 1) {
                    os << ", ";
                }
            }
            os << " ]";
            if (i < matrix.rows - 1) {
                os << "\n";
            }
        }
        return os;
    }

    std::unique_ptr<IEnumerator<T>> GetEnumerator() const {
        return std::make_unique<MatrixEnumerator>(data, rows * cols);
    }

};