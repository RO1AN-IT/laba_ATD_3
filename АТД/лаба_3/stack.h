#pragma once
#include "help_from_laba_2/ArraySequence.h"
#include "help_from_laba_2/IEnumerable.h"
#include "help_from_laba_2/IntegerRing.h"
#include <stdexcept>
#include <memory>
#include <tuple>
#include <algorithm> 

template<typename T>
class Stack {
public:
    Stack() {
        data = new MutableArraySequence<T>();
    }

    Stack(int size) {
        if (size < 0) {
            throw std::invalid_argument("Size must be non-negative");
        }
        data = new MutableArraySequence<T>(size);
    }

    Stack(T* items, int size) {
        if (size < 0) {
            throw std::invalid_argument("Size must be non-negative");
        }
        data = new MutableArraySequence<T>(items, size);
    }

    Stack(const Stack<T>& other) {
        data = new MutableArraySequence<T>(*other.data);
    }

    ~Stack() {
        delete data;
    }

    Stack<T>& operator=(const Stack<T>& other) {
        if (this != &other) {
            delete data;
            data = new MutableArraySequence<T>(*other.data);
        }
        return *this;
    }

    T& operator[](int index) {
        if (index < 0 || index >= data->GetLength()) {
            throw std::invalid_argument("Index out of range");
        }
        return data->Get(index);
    }
    
    const T& operator[](int index) const {
        if (index < 0 || index >= data->GetLength()) {
            throw std::invalid_argument("Index out of range");
        }
        return data->Get(index);
    }

    Stack<T>& operator+(const Stack<T>& other) {
        int newSize = std::min(data->GetLength(), other.data->GetLength());
        MutableArraySequence<T>* newData = new MutableArraySequence<T>(newSize);
        for (int i = 0; i < newSize; i++) {
            newData->Append(data->Get(i) + other.data->Get(i));
        }
        delete data;
        data = newData;
        return *this;
    }

    Stack<T>& operator*(const Stack<T>& other) {
        int newSize = std::min(data->GetLength(), other.data->GetLength());
        MutableArraySequence<T>* newData = new MutableArraySequence<T>(newSize);
        for (int i = 0; i < newSize; i++) {
            newData->Append(data->Get(i) * other.data->Get(i));
        }
        delete data;
        data = newData;
        return *this;
    }

    void push(const T& item) {
        data->Append(item); 
    }

    T pop() {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Stack is empty");
        }
        T last = data->GetLast();
        data->RemoveAt(data->GetLength() - 1);
        return last;
    }

    T top() const {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Stack is empty");
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

    friend std::ostream& operator<<(std::ostream& os, const Stack<T>& stack) {
        os << "[ ";
        for (int i = stack.data->GetLength() - 1; i >= 0 ; i--) {
            os << stack.data->Get(i);
            if (i > 0) {
                os << ", ";
            }
        }
        os << " ]";
        return os;
    }

    template<typename U>
    Stack<U>* Map(U (*f)(T)) {
        this->data = static_cast<MutableArraySequence<U>*>(this->data->Map(f));
        return this;
    }

    template<typename U>
    Stack<U>* Reduce(U (*f)(U, T), const U c) {
        this->data = static_cast<MutableArraySequence<U>*>(this->data->Reduce(f, c));
        return this;
    }

    template<typename... Tuples, typename U>
    Stack<std::tuple<Tuples..., U>>* Zip(Stack<U>* other) {
        this->data = static_cast<MutableArraySequence<std::tuple<Tuples..., U>>*>(this->data->Zip(other->data));
        return this;
    }

    template<typename... Tuples>
    std::tuple<Stack<Tuples>...>* UnZip() const {
        auto tuple = data->UnZip();
        std::tuple<Stack<Tuples>...>* result = new std::tuple<Stack<Tuples>...>();
        return result;
    }

    template<typename V>
    Stack<Stack<V>*>* Split(bool (*f)(V)) {
        this->data = static_cast<MutableArraySequence<MutableArraySequence<V>>*>(this->data->Split(f));
        return this;
    }

    Stack<T>* Where(bool (*f)(T)) {
        this->data = static_cast<MutableArraySequence<T>*>(this->data->Where(f));
        return this;
    }

    Stack<T>* From(T* items, int size) {
        this->data = static_cast<MutableArraySequence<T>*>(this->data->From(items, size));
        return this;
    }

    Stack<T>* Concat(const Stack<T>& other){
        this->data = static_cast<MutableArraySequence<T>*>(this->data->Concat(other.data));
        return this;
    }

    Stack<T>* GetSubStack(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex > data->GetLength() || startIndex > endIndex) {
            throw std::invalid_argument("Invalid index range");
        }
        Stack<T>* result = new Stack<T>();
        for (int i = startIndex; i < endIndex; ++i) {
            result->push(data->Get(i));
        }
        return result;
    }

    bool Contains(const Stack<T>& other) const {
        for (int i = 0; i < other.size(); i++) {
            if (!data->Contains(other.data->Get(i))) {
                return false;
            }
        }
        return true;
    }

    std::unique_ptr<IEnumerator<T>> GetEnumerator() const {
        return std::make_unique<StackEnumerator>(data, data->GetLength());
    }

protected:
    MutableArraySequence<T>* data;

    class StackEnumerator : public IEnumerator<T> {
    private:
        const MutableArraySequence<T>* data;
        int currentIndex;
        int size;

    public:
        StackEnumerator(const MutableArraySequence<T>* d, int s) : data(d), size(s), currentIndex(-1) {}

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