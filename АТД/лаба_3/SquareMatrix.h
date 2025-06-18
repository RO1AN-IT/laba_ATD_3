#pragma once
#include "help_from_laba_2/ArraySequence.h"
#include "help_from_laba_2/IEnumerable.h"
#include "help_from_laba_2/IntegerRing.h"
#include <stdexcept>
#include <memory>
#include <tuple>
#include <algorithm> 
#include <utility>

template<typename T>
class SquareMatrix {
private:
    MutableArraySequence<T>* data;
    int n;

    int getIndex(int i, int j) const{
        if (i < 0 || i >= n || j < 0 || j >= n) {
            throw std::out_of_range("Matrix index out of range");
        }
        return i * n + j;
    }

    SquareMatrix<T>* getMinor(int row, int col) const {
        SquareMatrix<T>* minor = new SquareMatrix<T>(n - 1);
        for(int i = 0, mi = 0; i < n; i++){
            if(i == row) continue;
            for(int j = 0, mj = 0; j < n; j++){
                if(j == col) continue;
                minor->set(this->get(i, j), mi, mj);
                mj++;
            }
            mi++;
        }
        return minor;
    }

    class SquareEnumerator : public IEnumerator<T> {
    private:
        const MutableArraySequence<T>* data;
        int currentIndex;
        int n;

    public:
        SquareEnumerator(const MutableArraySequence<T>* d, int s) : data(d), n(s), currentIndex(-1) {}

        bool MoveNext() override {
            if (currentIndex + 1 < n) {
                currentIndex++;
                return true;
            }
            return false;
        }
        
        T Current() const override {
            if (currentIndex < 0 || currentIndex >= n) {
                throw std::out_of_range("No current element");
            }
            return data->Get(currentIndex).item;
        }

        void Reset() override {
            currentIndex = -1;
        }
    };

public: 
    SquareMatrix() : n(0) {
        data = new MutableArraySequence<T>();
    }

    SquareMatrix(int n) {
        if(n < 0){
            throw std::invalid_argument("n must be non-negative");
        }
        this->n = n;
        data = new MutableArraySequence<T>(n * n);
    }

    SquareMatrix(T* items, int n) {
        if(n < 0){
            throw std::invalid_argument("n must be non-negative");
        }
        this->n = n;
        data = new MutableArraySequence<T>(items, n * n);
    }

    SquareMatrix(const SquareMatrix<T>& other) : n(other.n) {
        data = new MutableArraySequence<T>(*other.data);
    }

    SquareMatrix(MutableArraySequence<T>* items, int n) : n(n) {
        if(n < 0){
            throw std::invalid_argument("n must be non-negative");
        }
        data = items;
    }

    ~SquareMatrix(){
        delete data;
        n = 0;
    }

    SquareMatrix<T>& operator=(const SquareMatrix<T>& other){
        if(this != &other){
            delete data;
            data = new MutableArraySequence<T>(*other.data);
            n = other.n;
        }
        return *this;
    }


    const T& operator[](int index) const{
        if(index < 0 || index >= data->GetLength()){
            throw std::invalid_argument("Index out of range");
        }
        return data->Get(index);
    }
    
    SquareMatrix<T>& operator+(const SquareMatrix<T>& other){
        if(n != other.n){
            throw std::invalid_argument("Matrices must have the same n");
        }
        int newn = data->GetLength();
        MutableArraySequence<T>* newData = new MutableArraySequence<T>(newn);
        for(int i = 0; i < newn; i++){
            newData->InsertAt(this->data->Get(i) + other.data->Get(i), i);
        }
        delete data;
        data = newData;
        return *this;
    }

    SquareMatrix<T>& operator*(const SquareMatrix<T>& other) const{
        if(n != other.n){
            throw std::invalid_argument("Matrices must have the same n");
        }
        int newn = data->GetLength();
        MutableArraySequence<T>* newData = new MutableArraySequence<T>(newn);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                int multi = 0;
                for(int k = 0; k < n; k++){
                    multi += (this->get(i, k) * other.get(k, j));
                }
                newData->InsertAt(multi, getIndex(i, j));
            }
        }
        SquareMatrix<T>* result = new SquareMatrix<T>(newData, n);
        return *result;
    }

    SquareMatrix<T>& operator*(const T item){
        MutableArraySequence<T>* newData = new MutableArraySequence<T>(n * n);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                newData->InsertAt(this->get(i, j) * item, getIndex(i, j));
            }
        }
        SquareMatrix<T>* result = new SquareMatrix<T>(newData, n);
        return *result;
    }

    bool operator==(const SquareMatrix<T>& other) const {
        if (n != other.n) return false;
        for (int i = 0; i < n * n; ++i) {
            if (data->Get(i) != other.data->Get(i)) {
                return false;
            }
        }
        return true;
    }

    T operator()(int i, int j) const {
        return get(i, j);
    }

    T get(int i, int j) const{
        return data->Get(getIndex(i, j));
    }

    void set(T value, int i, int j){
        data->InsertAt(value, getIndex(i, j));
    }

    int size() const {
        return n;
    }

    bool empty() const {
        return n == 0;
    }

    void clear() {
        delete data;
        data = new MutableArraySequence<T>();
        n = 0;
    }

    SquareMatrix<T>* Transpose() {
        SquareMatrix<T>* transpor = new SquareMatrix<T>(n);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                transpor->set(get(j, i), i, j);
            }
        }
        return transpor;
    }

    std::pair<SquareMatrix<T>, SquareMatrix<T>> LUDecomposition() const {
        if (n == 0) {
            throw std::invalid_argument("Cannot decompose empty matrix");
        }
        SquareMatrix<T> L(n);
        SquareMatrix<T> U(n);

        for (int i = 0; i < n; ++i) {
            L.set(i, i, 1);
        }

        for (int j = 0; j < n; ++j) {
            U.set(0, j, get(0, j));
        }

        for (int k = 0; k < n - 1; ++k) {
            T u_kk = U.get(k, k);

            for (int i = k + 1; i < n; ++i) {
                T sum = 0;
                for (int m = 0; m < k; ++m) {
                    sum += L.get(i, m) * U.get(m, k);
                }
                L.set(i, k, (get(i, k) - sum) / u_kk);
            }

            for (int j = k; j < n; ++j) {
                T sum = 0;
                for (int m = 0; m < k; ++m) {
                    sum += L.get(k + 1, m) * U.get(m, j);
                }
                U.set(k + 1, j, get(k + 1, j) - sum);
            }
        }

        return {L, U};
    }

    T determinant() const {
        if (n == 0) {
            throw std::invalid_argument("Determinant of empty matrix is undefined");
        }
        try {
            auto [L, U] = LUDecomposition();
            T det = 1;
            for (int i = 0; i < n; ++i) {
                det *= U.get(i, i);
            }
            return det;
        } 
        catch (...) {
            if (n == 1) {
                return get(0, 0);
            }
            if (n == 2) {
                return get(0, 0) * get(1, 1) - get(0, 1) * get(1, 0);
            }
            T det = 0;
            for (int j = 0; j < n; ++j) {
                det += (j % 2 == 0 ? 1 : -1) * get(0, j) * getMinor(0, j)->determinant();
            }
            return det;
        }
    }

    T Norm() const {
        T sum = T(0);
        for (int i = 0; i < n * n; ++i) {
            T val = data->Get(i);
            sum += val * val;
        }
        return std::sqrt(sum);
    }

    friend std::ostream& operator<<(std::ostream& os, const SquareMatrix<T>& matrix) {
        for (int i = 0; i < matrix.size(); ++i) {
            os << "[ ";
            for (int j = 0; j < matrix.size(); ++j) {
                os << matrix.get(i, j);
                if (j < matrix.size() - 1) {
                    os << ", ";
                }
            }
            os << " ]";
            if (i < matrix.size() - 1) {
                os << "\n";
            }
        }
        return os;
    }

    std::unique_ptr<IEnumerator<T>> GetEnumerator() const {
        return std::make_unique<SquareEnumerator>(data, n * n);
    }



};

