#pragma once
#include "help_from_laba_2/ListSequence.h"
#include "help_from_laba_2/IEnumerable.h"
#include "help_from_laba_2/IntegerRing.h"
#include <stdexcept>
#include <memory>
#include <tuple>
#include <algorithm>

template<typename T>
class Deque {
public:
    Deque(){
        data = new MutableListSequence<T>();
    }

    Deque(int size) {
        if (size < 0) {
            throw std::invalid_argument("Size must be non-negative");
        }
        data = new MutableListSequence<T>(size);
    }

    Deque(T* items, int size) {
        if (size < 0) {
            throw std::invalid_argument("Size must be non-negative");
        }
        data = new MutableListSequence<T>(items, size);
    }

    Deque(const Deque<T>& other) {
        data = new MutableListSequence<T>(*other.data);
    }

    ~Deque() {
        delete data;
    }

    Deque<T>& operator=(const Deque<T>& other) {
        if (this != &other) {
            delete data;
            data = new MutableListSequence<T>(*other.data);
        }
        return *this;
    }

    T& operator[](int index) {
        if (index < 0 || index >= data->GetLength()) {
            throw std::invalid_argument("Incompatible types in assignment");
        }
        return data->Get(index);
    }
    
    const T& operator[](int index) const {
        if (index < 0 || index >= data->GetLength()) {
            throw std::invalid_argument("Incompatible types in assignment");
        }
        return data->Get(index);
    }

    Deque<T>& operator+(const Deque<T>& other) {
        MutableListSequence<T>* newList = new MutableListSequence<T>(min(this->data->GetLength(), other.data->GetLength()));

        for(int i = 0; i < min(this->data->GetLength(), other.data->GetLength()); i++){
            newList->Append(this->data->Get(i) + other.data->Get(i));
        }

        this->data = newList;

        return *this;
    }

    Deque<T>& operator*(const Deque<T>& other) {
        MutableListSequence<T>* newList = new MutableListSequence<T>(min(this->data->GetLength(), other.data->GetLength()));

        for(int i = 0; i < min(this->data->GetLength(), other.data->GetLength()); i++){
            newList->Append(this->data->Get(i) * other.data->Get(i));
        }

        this->data = newList;

        return *this;
    }

    int min(int a, int b) const {
        return (a < b) ? a : b;
    }

    void push_back(const T& item) {
        data->Append(item);
    }

    void push_front(const T& item) {
        data->Prepend(item);
    }

    T pop_back() {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Deque is empty");
        }
        T last = data->GetLast();
        remove_last();
        return last;
    }

    T pop_front() {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Deque is empty");
        }
        T first = data->GetFirst();
        data->RemoveAt(0);
        return first;
    }

    T front() const {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Deque is empty");
        }
        return data->GetFirst();
    }

    T back() const {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Deque is empty");
        }
        return data->GetLast();
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

    friend std::ostream& operator<<(std::ostream& os, const Deque<T>& deque) {
        os << "[ ";
        for (int i = 0; i < deque.data->GetLength(); ++i) {
            os << deque.data->Get(i);
            if (i < deque.data->GetLength() - 1) {
                os << ", ";
            }
        }
        os << " ]";
        return os;
    }

    template<typename U>
    Deque<U>* Map(U (*f)(T)) {
        this->data = static_cast<MutableListSequence<U>*>(this->data->Map(f));
        return this;
    }

    template<typename U>
    Deque<U>* Reduce(U (*f)(U, T), const U c) {
        this->data = static_cast<MutableListSequence<U>*>(this->data->Reduce(f, c));
        return this;
    }

    template<typename... Tuples, typename U>
    Deque<std::tuple<Tuples..., U>>* Zip(Deque<U>* other) {
        this->data = static_cast<MutableListSequence<std::tuple<Tuples..., U>>*>(this->data->Zip(other->data));
        return this;
    }

    template<typename... Tuples>
    std::tuple<Deque<Tuples>...>* UnZip() const {
        auto tuple = data->UnZip();
        std::tuple<Deque<Tuples>...>* result = new std::tuple<Deque<Tuples>...>();
        return result;
    }

    Deque<Deque<T>*>* Split(bool (*f)(T)) {
        this->data = static_cast<MutableListSequence<MutableListSequence<T>>*>(this->data->Split(f));
        return this;
    }

    Deque<T>* Where(bool (*f)(T)){
        this->data = static_cast<MutableListSequence<T>*>(this->data->Where(f));
        return this;
    }

    Deque<T>* From(T* items, int size){
        this->data = static_cast<MutableListSequence<T>*>(this->data->From(items, size));
        return this;
    }

    Deque<T>* Concat(const Deque<T>& other) {
        this->data = static_cast<MutableListSequence<T>*>(this->data->Concat(other.data));
        return this;
    }

    Deque<T>* GetSubDeque(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex > data->GetLength() || startIndex > endIndex) {
            throw std::invalid_argument("Invalid index range");
        }
        Deque<T>* result = new Deque<T>();
        for (int i = startIndex; i < endIndex; ++i) {
            result->push_back(data->Get(i));
        }
        return result;
    }

    bool Contains(const Deque<T>& other) const {
        if(other.data->GetLength() == 0) {
            return true;
        }
        for(int i = 0; i < other.data->GetLength(); i++){
            if(!data->Contains(other.data->Get(i))) {
                return false; 
            }
        }
        return true;
    }

    void Sort(bool (*cmp)(const T&, const T&) = [](const T& a, const T& b) { return a < b; }) {
        if (!data) {
            throw std::runtime_error("Data pointer is null");
        }
        int length = data->GetLength();
        if (length <= 1) {
            return;
        }
        T* items = new T[length];
        for (int i = 0; i < length; ++i) {
            items[i] = data->Get(i);
        }
        QuickSort(items, 0, length - 1, cmp);
        delete data;
        data = new MutableListSequence<T>(items, length);
        delete[] items;
    }

    std::unique_ptr<IEnumerator<T>> GetEnumerator() const {
        return std::make_unique<DeqEnumerator>(data, data->GetLength());
    }

    T Sum() const {
        if (data->GetLength() == 0) {
            return ring.Zero();
        }
        T result = ring.Zero();
        for (int i = 0; i < data->GetLength(); i++) {
            result = ring.Operation(result, data->Get(i));
        }
        return result;
    }

    T Product() const {
        if (data->GetLength() == 0) {
            return ring.Identity();
        }
        T result = ring.Identity();
        for (int i = 0; i < data->GetLength(); i++) {
            result = ring.Multiplication(result, data->Get(i));
        }
        return result;
    }

protected:
    MutableListSequence<T>* data;
    IntegerRing<T> ring;

    class DeqEnumerator : public IEnumerator<T> {
        private:
            const MutableListSequence<T>* data;
            int currentIndex;
            int size;
        public:
            DeqEnumerator(const MutableListSequence<T>* d, int size) : data(d), size(size), currentIndex(-1) {}

            bool MoveNext() override {
                if(currentIndex + 1 < size){
                    currentIndex++;
                    return true;
                }
                return false;
            }
            
            T Current() const override{
                if(currentIndex + 1 < 0 || currentIndex >= size){
                    throw std::out_of_range("No current element");
                }
                return data->Get(currentIndex + 1);
            }

            void Reset() override {
                currentIndex = -1;
            }
    };  

    void QuickSort(T* arr, int low, int high, bool (*cmp)(const T&, const T&)) {
        if (low < high) {
            if (low < 0 || high < 0 || high >= data->GetLength()) {
                throw std::out_of_range("Invalid array bounds in QuickSort");
            }
            int pi = Partition(arr, low, high, cmp);
            QuickSort(arr, low, pi - 1, cmp);
            QuickSort(arr, pi + 1, high, cmp);
        }
    }

    int Partition(T* arr, int low, int high, bool (*cmp)(const T&, const T&)) {
        if (low < 0 || high < 0 || high >= data->GetLength()) {
            throw std::out_of_range("Invalid array bounds in Partition");
        }
        T pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (cmp(arr[j], pivot) || (!cmp(pivot, arr[j]) && !cmp(arr[j], pivot))) {
                i++;
                if (i >= data->GetLength() || j >= data->GetLength()) {
                    throw std::out_of_range("Index out of bounds in Partition");
                }
                std::swap(arr[i], arr[j]);
            }
        }
        if (i + 1 >= data->GetLength()) {
            throw std::out_of_range("Index out of bounds in final swap");
        }
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    }


    void remove_last(){
        if (data->GetLength() == 0) {
            throw std::out_of_range("Deque is empty");
        }
        data->RemoveAt(data->GetLength() - 1);
    }

};