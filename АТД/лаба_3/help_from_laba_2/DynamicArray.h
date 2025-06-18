#pragma once
#include "Container.h"

template<typename T>
class DynamicArray: public Container<T> {
public:
    DynamicArray() : data(nullptr), size(0) {}

    DynamicArray(int size) : size(size) {
        if (size < 0) {
            throw std::invalid_argument("Size must be positive");
        }
        data = size > 0 ? new T[size] : nullptr;
    }

    DynamicArray(const DynamicArray<T>& other) : DynamicArray(other.data, other.size) {}

    DynamicArray(T* items, int size) : size(size) {
        if (size < 0) {
            throw std::invalid_argument("Size must be positive");
        }
        if (size == 0) {
            data = nullptr;
            return;
        }
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = items[i];
        }
    }

    DynamicArray(DynamicArray<T>&& other) {
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
    }

    ~DynamicArray() {
        delete[] data;
    }

    T& operator[](int index) {
        if (index >= size || index < 0) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index >= size || index < 0) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    DynamicArray& operator=(const DynamicArray<T>& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data;
        size = other.size;
        data = size > 0 ? new T[size] : nullptr;
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }

    DynamicArray& operator=(DynamicArray<T>&& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data;
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
        return *this;
    }

    T* getData() const override {
        return data;
    }

    T& get(int index) const override {
        if (!data && size > 0) {
            throw std::out_of_range("DynamicArray is not initialized");
        }
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    T getFirst() const override {
        if (size == 0 || !data) {
            throw std::out_of_range("Array is empty");
        }
        return data[0];
    }

    T getLast() const override {
        if (size == 0 || !data) {
            throw std::out_of_range("Array is empty");
        }
        return data[size - 1];
    }

    int getSize() const override {
        return size;
    }

    void Set(int index, const T& value) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        data[index] = value;
    }

    void Set(int index, T&& value) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        data[index] = std::move(value);
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw std::invalid_argument("Size must be positive");
        }
        T* newData = newSize > 0 ? new T[newSize] : nullptr;
        int copySize = std::min(size, (size_t)newSize);
        for (int i = 0; i < copySize; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        size = newSize;
    }

    void append(T element) override {
        Resize(size + 1);
        data[size - 1] = element;
    }

    void prepend(T element) override {
        Resize(size + 1);
        for (int i = size - 1; i > 0; i--) {
            data[i] = data[i - 1];
        }
        data[0] = element;
    }

    void insertAt(T item, int index) override {
        if (index < 0 || index > size) {
            throw std::out_of_range("Index out of range");
        }
        Resize(size + 1);
        for (int i = size - 1; i > index; i--) {
            data[i] = data[i - 1];
        }
        data[index] = item;
    }

    T& begin() {
        if (size == 0) {
            throw std::out_of_range("Array is empty");
        }
        return data[0];
    }

    T& end() {
        if (size == 0) {
            throw std::out_of_range("Array is empty");
        }
        return data[size - 1];
    }

private:
    T* data;
    size_t size;
};