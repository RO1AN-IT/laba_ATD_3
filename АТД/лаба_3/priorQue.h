#pragma once
#include "help_from_laba_2/ArraySequence.h"
#include "help_from_laba_2/IEnumerable.h"
#include "help_from_laba_2/IntegerRing.h"
#include <stdexcept>
#include <memory>
#include <tuple>
#include <algorithm> 

template<typename T>
class PriorityQueue {
protected:
    struct Node {
        T item;
        int priority;
        Node(const T& i, int p) : item(i), priority(p) {}
        Node() : item(T()), priority(0) {}
        Node(const Node& other) : item(other.item), priority(other.priority) {}
        Node(int subSize) : item(T()), priority(subSize) {}

        T& GetItem() const {
            return const_cast<T&>(item);
        }
        int& GetPriority() const {
            return const_cast<int&>(priority);
        }

        bool operator==(const Node& other) const {
            return item == other.item && priority == other.priority;
        }
        bool operator!=(const Node& other) const {
            return !(*this == other);
        }
        bool operator<(const Node& other) const {
            return priority < other.priority;
        }
        bool operator<=(const Node& other) const {
            return priority <= other.priority;
        }
        bool operator>(const Node& other) const {
            return priority > other.priority;
        }
        bool operator>=(const Node& other) const {
            return priority >= other.priority;
        }
        Node& operator=(const Node& other) {
            if (this != &other) {
                item = other.item;
                priority = other.priority;
            }
            return *this;
        }
    };

    MutableArraySequence<Node>* data; 

    //если что это просто сортировки куч
    void siftUp(int index) { 
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (data->Get(parent).priority >= data->Get(index).priority) {
                break;
            }
            std::swap(data->Get(index).GetItem(), data->Get(parent).GetItem());
            std::swap(data->Get(index).GetPriority(), data->Get(parent).GetPriority());
            index = parent;
        }
    }

    void siftDown(int index) {
        int maxIndex = index;
        int size = data->GetLength();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            if (left < size && data->Get(left).priority > data->Get(maxIndex).priority) {
                maxIndex = left;
            }
            if (right < size && data->Get(right).priority > data->Get(maxIndex).priority) {
                maxIndex = right;
            }
            if (maxIndex == index) {
                break;
            }
            std::swap(data->Get(index).GetItem(), data->Get(maxIndex).GetItem());
            std::swap(data->Get(index).GetPriority(), data->Get(maxIndex).GetPriority());
            index = maxIndex;
        }
    }

    class PQEnumerator : public IEnumerator<T> {
    private:
        const MutableArraySequence<Node>* data;
        int currentIndex;
        int size;

    public:
        PQEnumerator(const MutableArraySequence<Node>* d, int s) : data(d), size(s), currentIndex(-1) {}

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
            return data->Get(currentIndex).item;
        }

        void Reset() override {
            currentIndex = -1;
        }
    };

public:
    PriorityQueue() {
        data = new MutableArraySequence<Node>();
    }

    PriorityQueue(int size) {
        if (size < 0) {
            throw std::invalid_argument("Size must be non-negative");
        }
        data = new MutableArraySequence<Node>(size);
    }

    PriorityQueue(Node* items, int size) {
        if (size < 0) {
            throw std::invalid_argument("Size must be non-negative");
        }
        data = new MutableArraySequence<Node>(items, size);
        for (int i = size / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
    }

    PriorityQueue(const PriorityQueue<T>& other) {
        data = new MutableArraySequence<Node>(*other.data);
    }

    ~PriorityQueue() {
        delete data;
    }

    PriorityQueue<T>& operator=(const PriorityQueue<T>& other) {
        if (this != &other) {
            delete data;
            data = new MutableArraySequence<Node>(*other.data);
        }
        return *this;
    }
    
    const T& operator[](int index) const {
        if (index < 0 || index >= data->GetLength()) {
            throw std::invalid_argument("Index out of range");
        }
        return data->Get(index).priority;
    }

    PriorityQueue<T>& operator+(const PriorityQueue<T>& other) {
        int newSize = std::min(data->GetLength(), other.data->GetLength());
        MutableArraySequence<Node>* newData = new MutableArraySequence<Node>(newSize);
        for (int i = 0; i < newSize; i++) {
            if (data->Get(i).priority == other.data->Get(i).priority) {
                newData->Append(Node(data->Get(i).item + other.data->Get(i).item, data->Get(i).priority));
            } else {
                throw std::invalid_argument("Priorities do not match");
            }
        }
        delete data;
        data = newData;
        for (int i = newSize / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
        return *this;
    }

    PriorityQueue<T>& operator*(const PriorityQueue<T>& other) {
        int newSize = std::min(data->GetLength(), other.data->GetLength());
        MutableArraySequence<Node>* newData = new MutableArraySequence<Node>(newSize);
        for (int i = 0; i < newSize; i++) {
            if (data->Get(i).priority == other.data->Get(i).priority) {
                newData->Append(Node(data->Get(i).item * other.data->Get(i).item, data->Get(i).priority));
            } else {
                throw std::invalid_argument("Priorities do not match");
            }
        }
        delete data;
        data = newData;
        for (int i = newSize / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
        return *this;
    }

    void Set(int index, const T& value) {
        if (index < 0 || index >= data->GetLength()) {
            throw std::invalid_argument("Index out of range");
        }
        data->Get(index).item = value;
        siftUp(index);
        siftDown(index);
    }

    T Get(int index) const {
        if (index < 0 || index >= data->GetLength()) {
            throw std::invalid_argument("Index out of range");
        }
        return data->Get(index).item;
    }

    void enqueue(const T& item, int priority) {
        data->Append(Node(item, priority)); 
        siftUp(data->GetLength() - 1);
    }

    T dequeue() {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Priority queue is empty");
        }
        T maxItem = data->Get(0).item;
        if (data->GetLength() == 1) {
            data->RemoveAt(0);
            return maxItem;
        }
        data->Get(0) = data->Get(data->GetLength() - 1);
        data->RemoveAt(data->GetLength() - 1);
        siftDown(0);
        return maxItem;
    }

    T peek() const {
        if (data->GetLength() == 0) {
            throw std::out_of_range("Priority queue is empty");
        }
        return data->Get(0).item;
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

    friend std::ostream& operator<<(std::ostream& os, const PriorityQueue<T>& pq) {
        os << "[ ";
        for (int i = 0; i < pq.data->GetLength(); ++i) {
            os << "(" << pq.data->Get(i).item << ", " << pq.data->Get(i).priority << ")";
            if (i < pq.data->GetLength() - 1) {
                os << ", ";
            }
        }
        os << " ]";
        return os;
    }

    

    template<typename U>
    PriorityQueue<U>* Map(U (*f)(T)) {
        MutableArraySequence<typename PriorityQueue<U>::Node>* newData = new MutableArraySequence<typename PriorityQueue<U>::Node>(data->GetLength());
        for (int i = 0; i < data->GetLength(); ++i) {
            newData->Append(typename PriorityQueue<U>::Node(f(data->Get(i).item), data->Get(i).priority));
        }
        delete data;
        data = reinterpret_cast<MutableArraySequence<Node>*>(newData);
        for (int i = data->GetLength() / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
        return this;
    }

    template<typename U>
    PriorityQueue<U>* Reduce(U (*f)(U, T), const U c) {
        U result = c;
        for (int i = 0; i < data->GetLength(); ++i) {
            result = f(result, data->Get(i).item);
        }
        delete data;
        data = reinterpret_cast<MutableArraySequence<Node>*>(new MutableArraySequence<typename PriorityQueue<U>::Node>(1));
        data->Get(0) = typename PriorityQueue<U>::Node(result, 0);
        return this;
    }

    template<typename... Tuples, typename U>
    PriorityQueue<std::tuple<Tuples..., U>>* Zip(PriorityQueue<U>* other) {
        int newSize = std::min(data->GetLength(), other->data->GetLength());
        MutableArraySequence<typename PriorityQueue<std::tuple<Tuples..., U>>::Node>* newData = new MutableArraySequence<typename PriorityQueue<std::tuple<Tuples..., U>>::Node>(newSize);
        for (int i = 0; i < newSize; ++i) {
            newData->Append(typename PriorityQueue<std::tuple<Tuples..., U>>::Node(
                std::tuple_cat(std::make_tuple(data->Get(i).item), std::make_tuple(other->data->Get(i).item)),
                std::max(data->Get(i).priority, other->data->Get(i).priority)
            ));
        }
        delete data;
        data = reinterpret_cast<MutableArraySequence<Node>*>(newData);
        for (int i = newSize / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
        return this;
    }

    template<typename V>
    PriorityQueue<PriorityQueue<V>*>* Split(bool (*f)(V)) {
        int newSize = 0;
        for (int i = 0; i < data->GetLength(); ++i) {
            if (f(data->Get(i).item)) {
                newSize++;
            }
        }
        MutableArraySequence<typename PriorityQueue<PriorityQueue<V>*>::Node>* newData = new MutableArraySequence<typename PriorityQueue<PriorityQueue<V>*>::Node>(newSize);
        for (int i = 0; i < data->GetLength(); ++i) {
            if (f(data->Get(i).item)) {
                PriorityQueue<V>* subQueue = new PriorityQueue<V>();
                subQueue->enqueue(data->Get(i).item, data->Get(i).priority);
                newData->Append(typename PriorityQueue<PriorityQueue<V>*>::Node(subQueue, data->Get(i).priority));
            }
        }
        delete data;
        data = reinterpret_cast<MutableArraySequence<Node>*>(newData);
        for (int i = newSize / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
        return this;
    }

    PriorityQueue<T>* Where(bool (*f)(T)) {
        MutableArraySequence<Node>* newData = new MutableArraySequence<Node>();
        for (int i = 0; i < data->GetLength(); ++i) {
            if (f(data->Get(i).item)) {
                newData->Append(data->Get(i));
            }
        }
        delete data;
        data = newData;
        for (int i = data->GetLength() / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
        return this;
    }

    PriorityQueue<T>* From(Node* items, int size) {
        delete data;
        data = new MutableArraySequence<Node>(items, size);
        for (int i = size / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
        return this;
    }

    PriorityQueue<T>& Concat(const PriorityQueue<T>& other) {
        for (int i = 0; i < other.size(); ++i) {
            data->Append(Node(other[i], other.data->Get(i).priority));
            siftUp(data->GetLength() - 1);
        }
        return *this;
    }

    PriorityQueue<T>* GetSubsequence(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex > data->GetLength() || startIndex > endIndex) {
            throw std::invalid_argument("Invalid index range");
        }
        PriorityQueue<T>* result = new PriorityQueue<T>();
        for (int i = startIndex; i < endIndex; ++i) {
            result->enqueue(data->Get(i).item, data->Get(i).priority);
        }
        return result;
    }

    bool FindSubsequence(const PriorityQueue<T>& sub) const {
        if (sub.size() == 0) return 0;
        if (sub.size() > size()) return -1;

        for (int i = 0; i <= size() - sub.size(); ++i) {
            bool found = true;
            for (int j = 0; j < sub.size(); ++j) {
                if (data->Get(i + j).item != sub[j] || data->Get(i + j).priority != sub.data->Get(j).priority) {
                    found = false;
                    break;
                }
            }
            if (found) return true;
        }
        return false;
    }

    std::pair<PriorityQueue<T>*, PriorityQueue<T>*> SplitByPredicate(bool (*f)(const T&)) const {
        PriorityQueue<T>* trueQueue = new PriorityQueue<T>();
        PriorityQueue<T>* falseQueue = new PriorityQueue<T>();
        for (int i = 0; i < data->GetLength(); ++i) {
            if (f(data->Get(i).item)) {
                trueQueue->enqueue(data->Get(i).item, data->Get(i).priority);
            } else {
                falseQueue->enqueue(data->Get(i).item, data->Get(i).priority);
            }
        }
        return {trueQueue, falseQueue};
    }

    std::unique_ptr<IEnumerator<T>> GetEnumerator() const {
        return std::make_unique<PQEnumerator>(data, data->GetLength());
    }


};

