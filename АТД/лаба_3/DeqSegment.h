#pragma once
#include "help_from_laba_2/SegmentedList.h"
#include <stdexcept>
#include <memory>
#include <tuple>
#include <algorithm>

template<typename T>
class SegmentedDeque {
private:
    SegmentedList<T>* data;

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

public:
    SegmentedDeque() {
        data = new SegmentedList<T>();
    }

    SegmentedDeque(int size, int sizeSegment = 4) {
        if (size < 0 || sizeSegment <= 0) {
            throw std::invalid_argument("Invalid size or segment size");
        }
        data = new SegmentedList<T>(size, sizeSegment);
    }

    SegmentedDeque(T* items, int size, int sizeSegment = 4) {
        if (size < 0 || sizeSegment <= 0) {
            throw std::invalid_argument("Invalid size or segment size");
        }
        data = new SegmentedList<T>(items, size, sizeSegment);
    }

    SegmentedDeque(const SegmentedDeque<T>& other) {
        data = new SegmentedList<T>(*other.data);
    }

    SegmentedDeque(SegmentedDeque<T>&& other) noexcept : data(other.data) {
        other.data = nullptr;
    }

    ~SegmentedDeque() {
        delete data;
    }

    SegmentedDeque<T>& operator=(const SegmentedDeque<T>& other) {
        if (this != &other) {
            delete data;
            data = new SegmentedList<T>(*other.data);
        }
        return *this;
    }

    SegmentedDeque<T>& operator=(SegmentedDeque<T>&& other) noexcept {
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }

    T operator[](int index) {
        return data->Get(index);
    }

    const T& operator[](int index) const {
        return data->Get(index);
    }

    SegmentedDeque<T>& operator+(const SegmentedDeque<T>& other) {
        int newSize = std::min(data->GetLength(), other.data->GetLength());
        T* newItems = new T[newSize];
        for (int i = 0; i < newSize; i++) {
            newItems[i] = data->Get(i) + other.data->Get(i);
        }
        delete data;
        data = new SegmentedList<T>(newItems, newSize);
        delete[] newItems;
        return *this;
    }

    SegmentedDeque<T>& operator*(const SegmentedDeque<T>& other) {
        int newSize = std::min(data->GetLength(), other.data->GetLength());
        T* newItems = new T[newSize];
        for (int i = 0; i < newSize; i++) {
            newItems[i] = data->Get(i) * other.data->Get(i);
        }
        delete data;
        data = new SegmentedList<T>(newItems, newSize);
        delete[] newItems;
        return *this;
    }

    void push_front(const T& item) {
        data->Prepend(item);
    }

    void push_back(const T& item) {
        data->Append(item);
    }

    T pop_front() {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Deque is empty");
        }
        T item = data->GetFirst();
        data->RemoveAt(0);
        return item;
    }

    T pop_back() {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Deque is empty");
        }
        T item = data->GetLast();
        data->RemoveAt(data->GetLength() - 1);
        return item;
    }

    T front() const {
        return data->GetFirst();
    }

    T back() const {
        return data->GetLast();
    }

    int size() const {
        return data->GetLength();
    }

    int GetSizeSegment() const {
        return data->GetSizeSegment();
    }

    bool empty() const {
        return data->GetLength() == 0;
    }

    void clear() {
        data->Clear();
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
        data = new SegmentedList<T>(items, length);
        delete[] items;
    }


    bool Contains(const SegmentedDeque<T>& other) const {
        if (other.data->GetLength() == 0) {
            return true;
        }
        for(int i = 0; i < other.data->GetLength(); i++){
            if(!data->Contains(other.data->Get(i))) {
                return false; 
            }
        }
        return true;
    }

    SegmentedDeque<T>* GetSubSegment(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex > data->GetLength() || startIndex > endIndex) {
            throw std::invalid_argument("Invalid index range");
        }
        SegmentedDeque<T>* result = new SegmentedDeque<T>();
        for (int i = startIndex; i < endIndex; ++i) {
            result->push_back(data->Get(i));
        }
        return result;
    }

    SegmentedDeque<T>* Concat(const SegmentedDeque<T>& other) {
        SegmentedDeque<T>* result = new SegmentedDeque<T>(*this);
        for (int i = 0; i < other.data->GetLength(); ++i) {
            result->push_back(other.data->Get(i));
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const SegmentedDeque<T>& deque) {
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
};