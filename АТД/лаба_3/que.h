#pragma once
#include "help_from_laba_2/ListSequence.h"
#include "help_from_laba_2/IEnumerable.h"
#include "help_from_laba_2/IntegerRing.h"
#include <stdexcept>
#include <memory>
#include <tuple>
#include <algorithm>

template<typename T>
class Queue {
public:
    Queue() {
        data = new MutableListSequence<T>();
    }

    Queue(int size) {
        if (size < 0) {
            throw std::invalid_argument("Size must be non-negative");
        }
        data = new MutableListSequence<T>(size);
    }

    Queue(T* items, int size) {
        if (size < 0) {
            throw std::invalid_argument("Size must be non-negative");
        }
        data = new MutableListSequence<T>(items, size);
    }

    Queue(const Queue<T>& other) {
        data = new MutableListSequence<T>(*other.data);
    }

    ~Queue() {
        delete data;
    }

    Queue<T>& operator=(const Queue<T>& other) {
        if (this != &other) {
            delete data;
            data = new MutableListSequence<T>(*other.data);
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

    Queue<T>& operator+(const Queue<T>& other) {
        int newSize = std::min(data->GetLength(), other.data->GetLength());
        MutableListSequence<T>* newList = new MutableListSequence<T>(newSize);
        for (int i = 0; i < newSize; i++) {
            newList->Append(data->Get(i) + other.data->Get(i));
        }
        delete data;
        data = newList;
        return *this;
    }

    Queue<T>& operator*(const Queue<T>& other) {
        int newSize = std::min(data->GetLength(), other.data->GetLength());
        MutableListSequence<T>* newList = new MutableListSequence<T>(newSize);
        for (int i = 0; i < newSize; i++) {
            newList->Append(data->Get(i) * other.data->Get(i));
        }
        delete data;
        data = newList;
        return *this;
    }

    void enqueue(const T& item) {
        data->Append(item);
    }

    T dequeue() {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Queue is empty");
        }
        T first = data->GetFirst();
        data->RemoveAt(0); 
        return first;
    }

    T front() const {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Queue is empty");
        }
        return data->GetFirst();
    }

    T back() const {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Queue is empty");
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

    friend std::ostream& operator<<(std::ostream& os, const Queue<T>& queue) {
        os << "[ ";
        for (int i = 0; i < queue.data->GetLength(); ++i) {
            os << queue.data->Get(i);
            if (i < queue.data->GetLength() - 1) {
                os << ", ";
            }
        }
        os << " ]";
        return os;
    }

    Queue<T>* Concat(Queue<T>* other){
        this->data = static_cast<MutableListSequence<T>*>(this->data->Concat(other->data));
        return this;
    }

    Queue<T>* GetSubQueue(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex >= data->GetLength() || startIndex > endIndex) {
            throw std::out_of_range("Invalid range for subqueue");
        }
        MutableListSequence<T>* subData = dynamic_cast<MutableListSequence<T>*>(data->GetSubSequence(startIndex, endIndex));
        Queue<T>* subQueue = new Queue<T>();
        subQueue->data = subData;
        return subQueue;
    }

    bool Contains(Queue<T>* other) const {
        for(int i = 0; i < other->size(); i++) {
            if(!data->Contains(other->data->Get(i))) {
                return false;
            }
        }
        return true;
    }

    template<typename U>
    Queue<U>* Map(U (*f)(T)) {
        this->data = static_cast<MutableListSequence<U>*>(this->data->Map(f));
        return this;
    }

    template<typename U>
    Queue<U>* Reduce(U (*f)(U, T), const U c) {
        this->data = static_cast<MutableListSequence<U>*>(this->data->Reduce(f, c));
        return this;
    }

    template<typename... Tuples, typename U>
    Queue<std::tuple<Tuples..., U>>* Zip(Queue<U>* other) {
        this->data = static_cast<MutableListSequence<std::tuple<Tuples..., U>>*>(this->data->Zip(other->data));
        return this;
    }

    template<typename... Tuples>
    std::tuple<Queue<Tuples>...>* UnZip() const {
        auto tuple = data->UnZip();
        std::tuple<Queue<Tuples>...>* result = new std::tuple<Queue<Tuples>...>();
        return result;
    }

    template<typename V>
    Queue<Queue<V>*>* Split(bool (*f)(V)) {
        this->data = static_cast<MutableListSequence<MutableListSequence<V>>*>(this->data->Split(f));
        return this;
    }

    std::pair<Queue<T>*, Queue<T>*> SplitByPredicate(bool (*f)(const T&)) const {
        Queue<T>* trueQueue = new Queue<T>();
        Queue<T>* falseQueue = new Queue<T>();
        for (int i = 0; i < data->GetLength(); ++i) {
            if (f(data->Get(i))) {
                trueQueue->enqueue(data->Get(i));
            } else {
                falseQueue->enqueue(data->Get(i));
            }
        }
        return {trueQueue, falseQueue};
    }
    
    Queue<T>* Where(bool (*f)(T)) {
        this->data = static_cast<MutableListSequence<T>*>(this->data->Where(f));
        return this;
    }

    Queue<T>* From(T* items, int size) {
        this->data = static_cast<MutableListSequence<T>*>(this->data->From(items, size));
        return this;
    }

    std::unique_ptr<IEnumerator<T>> GetEnumerator() const {
        return std::make_unique<QueueEnumerator>(data, data->GetLength());
    }

protected:
    MutableListSequence<T>* data;

    class QueueEnumerator : public IEnumerator<T> {
    private:
        const MutableListSequence<T>* data;
        int currentIndex;
        int size;

    public:
        QueueEnumerator(const MutableListSequence<T>* d, int s) : data(d), size(s), currentIndex(-1) {}

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