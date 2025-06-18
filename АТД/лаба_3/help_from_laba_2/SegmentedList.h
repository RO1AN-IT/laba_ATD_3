#pragma once
#include "Sequence.h"
#include "DynamicArray.h"
#include "LinkedList.h"
#include "IntegerRing.h"
#include <stdexcept>
#include <utility>
#include <tuple>
#include <algorithm>

template<typename T>
class SegmentedList : public Sequence<T> {
    
    private:
        struct Node {
            DynamicArray<T>* segment;
            int validCount;
            Node() : segment(nullptr), validCount(0) {} 
            Node(int count) : segment(new DynamicArray<T>(count)), validCount(count) {}
            Node(DynamicArray<T>* arr) : segment(arr), validCount(arr->getSize()) {}
            ~Node() { delete segment; }
            
            Node(const Node& other) : segment(new DynamicArray<T>(*(other.segment))), validCount(other.validCount) {}
            
            Node& operator=(const Node& other) {
                if (this != &other) {
                    delete segment;
                    segment = new DynamicArray<T>(*(other.segment));
                    validCount = other.validCount;
                }
                return *this;
            }
        };
    
    LinkedList<Node>* data;
    int size;
    int sizeSegment;
    // IntegerRing<T> ring;

    class SegmentedListEnumerator : public IEnumerator<T> {
    private:
        const LinkedList<Node>* data;
        int currentIndex;
        int size;
        int currentSegmentIndex;
        int currentLocalIndex;

    public:
        SegmentedListEnumerator(const LinkedList<Node>* d, int s) : data(d), currentIndex(-1), size(s), currentSegmentIndex(0), currentLocalIndex(-1) {}

        bool MoveNext() override {
            if (currentIndex + 1 < size) {
                currentIndex++;
                currentLocalIndex++;
                
                if (currentSegmentIndex < data->getSize() && 
                    currentLocalIndex >= data->get(currentSegmentIndex).validCount) {
                    currentSegmentIndex++;
                    currentLocalIndex = 0;
                }
                return true;
            }
            return false;
        }

        T Current() const override {
            if (currentIndex < 0 || currentIndex >= size) {
                throw std::out_of_range("No current element");
            }
            return data->get(currentSegmentIndex).segment->get(currentLocalIndex);
        }

        void Reset() override {
            currentIndex = -1;
            currentSegmentIndex = 0;
            currentLocalIndex = -1;
        }
    };

    void getSegmentAndLocalIndex(int index, int& segmentIndex, int& localIndex) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        int currentIndex = 0;
        for (int i = 0; i < data->getSize(); i++) {
            if (currentIndex + data->get(i).validCount > index) {
                segmentIndex = i;
                localIndex = index - currentIndex;
                return;
            }
            currentIndex += data->get(i).validCount;
        }
        throw std::out_of_range("Index out of range");
    }

public:
    SegmentedList(): size(0), sizeSegment(4) {
        data = new LinkedList<Node>();
    }
    
    SegmentedList(int size, int sizeSegment = 4): size(size), sizeSegment(sizeSegment) {
        if (size < 0 || sizeSegment <= 0) {
            throw std::invalid_argument("Invalid size or segment size");
        }
        int numSegments = (size + sizeSegment - 1) / sizeSegment;
        data = new LinkedList<Node>();
        for (int i = 0; i < numSegments; i++) {
            int segmentSize = std::min(sizeSegment, size - i * sizeSegment);
            data->append(Node(segmentSize));
        }
    }
    
    SegmentedList(T* items, int size, int sizeSegment = 4): size(size), sizeSegment(sizeSegment) {
        if (size < 0 || sizeSegment <= 0) {
            throw std::invalid_argument("Invalid size or segment size");
        }
        int numSegments = (size + sizeSegment - 1) / sizeSegment;
        data = new LinkedList<Node>();
        for (int i = 0; i < numSegments; i++) {
            int segmentSize = std::min(sizeSegment, size - i * sizeSegment);
            DynamicArray<T>* segment = new DynamicArray<T>(segmentSize);
            for (int j = 0; j < segmentSize; j++) {
                segment->Set(j, items[i * sizeSegment + j]);
            }
            data->append(Node(segment));
        }
    }
    
    SegmentedList(const SegmentedList<T>& other): size(other.size), sizeSegment(other.sizeSegment) {
        data = new LinkedList<Node>();
        for (int i = 0; i < other.data->getSize(); i++) {
            data->append(Node(other.data->get(i)));
        }
    }
    
    SegmentedList(SegmentedList<T>&& other) noexcept : data(other.data), size(other.size), sizeSegment(other.sizeSegment) {
        other.data = nullptr;
        other.size = 0;
        other.sizeSegment = 4;
    }

    ~SegmentedList() {
        delete data;
    }

    SegmentedList<T>& operator=(const SegmentedList<T>& other) {
        if (this != &other) {
            delete data;
            size = other.size;
            sizeSegment = other.sizeSegment;
            data = new LinkedList<Node>();
            for (int i = 0; i < other.data->getSize(); i++) {
                data->append(Node(other.data->get(i)));
            }
        }
        return *this;
    }
    
    SegmentedList<T>& operator=(SegmentedList<T>&& other) noexcept {
        if (this != &other) {
            delete data;
            data = other.data;
            size = other.size;
            sizeSegment = other.sizeSegment;
            other.data = nullptr;
            other.size = 0;
            other.sizeSegment = 4;
        }
        return *this;
    }

    T Get(int index) const override {
        int segmentIndex, localIndex;
        getSegmentAndLocalIndex(index, segmentIndex, localIndex);
        return data->get(segmentIndex).segment->get(localIndex);
    }

    T GetFirst() const override {
        if (size == 0) {
            throw std::out_of_range("Sequence is empty");
        }
        return data->get(0).segment->get(0);
    }

    T GetLast() const override {
        if (size == 0) {
            throw std::out_of_range("Sequence is empty");
        }
        int lastSegment = data->getSize() - 1;
        return data->get(lastSegment).segment->get(data->get(lastSegment).validCount - 1);
    }

    int GetLength() const override {
        return size;
    }

    int GetSizeSegment() const {
        return sizeSegment;
    }

    Sequence<T>* Append(T item) override {
        if (data->getSize() == 0 || data->get(data->getSize() - 1).validCount >= sizeSegment) {
            data->append(Node(new DynamicArray<T>(0)));
        }
        int lastSegment = data->getSize() - 1;
        Node& node = data->get(lastSegment);
        node.segment->append(item);
        node.validCount++;
        size++;
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        if (data->getSize() == 0 || data->get(0).validCount >= sizeSegment) {
            data->prepend(Node(new DynamicArray<T>(0)));
        }
        Node& node = data->get(0);
        DynamicArray<T>* newSeg = new DynamicArray<T>(node.validCount + 1);
        newSeg->Set(0, item);
        for (int i = 0; i < node.validCount; i++) {
            newSeg->Set(i + 1, node.segment->get(i));
        }
        delete node.segment;
        node.segment = newSeg;
        node.validCount++;
        size++;
        return this;
    }

    Sequence<T>* InsertAt(T item, int index) override {
        if (index < 0 || index > size) {
            throw std::out_of_range("Index out of range");
        }
        if (index == 0) {
            return Prepend(item);
        }
        if (index == size) {
            return Append(item);
        }

        int segmentIndex, localIndex;
        getSegmentAndLocalIndex(index, segmentIndex, localIndex);
        Node& node = data->get(segmentIndex);
        
        if (node.validCount < sizeSegment) {
            DynamicArray<T>* newSeg = new DynamicArray<T>(node.validCount + 1);
            for (int i = 0; i < localIndex; i++) {
                newSeg->Set(i, node.segment->get(i));
            }
            newSeg->Set(localIndex, item);
            for (int i = localIndex; i < node.validCount; i++) {
                newSeg->Set(i + 1, node.segment->get(i));
            }
            delete node.segment;
            node.segment = newSeg;
            node.validCount++;
        } else {
            DynamicArray<T>* newSegment = new DynamicArray<T>(sizeSegment);
            for (int i = 0; i < localIndex; i++) {
                newSegment->Set(i, node.segment->get(i));
            }
            newSegment->Set(localIndex, item);
            for (int i = localIndex; i < sizeSegment - 1; i++) {
                newSegment->Set(i + 1, node.segment->get(i));
            }
            T carry = node.segment->get(sizeSegment - 1);
            delete node.segment;
            node.segment = newSegment;
            node.validCount = sizeSegment;

            int currentSegmentIndex = segmentIndex + 1;
            while (currentSegmentIndex < data->getSize() && data->get(currentSegmentIndex).validCount >= sizeSegment) {
                Node& nextNode = data->get(currentSegmentIndex);
                T nextCarry = nextNode.segment->get(sizeSegment - 1);
                DynamicArray<T>* shiftedSegment = new DynamicArray<T>(sizeSegment);
                shiftedSegment->Set(0, carry);
                for (int i = 0; i < sizeSegment - 1; i++) {
                    shiftedSegment->Set(i + 1, nextNode.segment->get(i));
                }
                delete nextNode.segment;
                nextNode.segment = shiftedSegment;
                nextNode.validCount = sizeSegment;
                carry = nextCarry;
                currentSegmentIndex++;
            }
            if (currentSegmentIndex >= data->getSize()) {
                data->append(Node(new DynamicArray<T>(0)));
            }
            Node& lastNode = data->get(currentSegmentIndex);
            DynamicArray<T>* newLastSegment = new DynamicArray<T>(lastNode.validCount + 1);
            newLastSegment->Set(0, carry);
            for (int i = 0; i < lastNode.validCount; i++) {
                newLastSegment->Set(i + 1, lastNode.segment->get(i));
            }
            delete lastNode.segment;
            lastNode.segment = newLastSegment;
            lastNode.validCount++;
        }
        size++;
        return this;
    }


    Sequence<T>* Concat(Sequence<T>* other) override {
        for (int i = 0; i < other->GetLength(); i++) {
            Append(other->Get(i));
        }
        return this;
    }

    Sequence<T>* GetSubSequence(int startIndex, int endIndex) const override {
        if (size == 0) {
            throw std::out_of_range("Sequence is empty");
        }
        if (startIndex < 0 || startIndex >= size || endIndex < startIndex || endIndex >= size) {
            throw std::out_of_range("Invalid range");
        }
        int subSize = endIndex - startIndex + 1;
        T* subItems = new T[subSize];
        for (int i = 0; i < subSize; i++) {
            subItems[i] = Get(startIndex + i);
        }
        SegmentedList<T>* subseq = new SegmentedList<T>(subItems, subSize, sizeSegment);
        delete[] subItems;
        return subseq;
    }

    void RemoveAt(int index) override {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        int segmentIndex, localIndex;
        getSegmentAndLocalIndex(index, localIndex, segmentIndex);
        Node& node = data->get(segmentIndex);
        DynamicArray<T>* newSeg = new DynamicArray<T>(node.validCount - 1);
        for (int j = 0, k = 0; j < node.validCount; j++) {
            if (j != localIndex) {
                newSeg->Set(k++, node.segment->get(j));
            }
        }
        delete node.segment;
        node.segment = newSeg;
        node.validCount--;
        size--;
        if (node.validCount == 0) {
            T* tempData = new T[size];
            for (int j = 0, k = 0; j < size + 1; j++) {
                if (j != index) {
                    tempData[k++] = Get(j);
                }
            }
            From(tempData, size);
            delete[] tempData;
        }
    }

    void Clear() override {
        delete data;
        data = new LinkedList<Node>();
        size = 0;
    }

    bool Contains(T item) const override {
        for (int i = 0; i < data->getSize(); i++) {
            Node node = data->get(i);
            for (int j = 0; j < node.validCount; j++) {
                if (node.segment->get(j) == item) {
                    return true;
                }
            }
        }
        return false;
    }

    bool Remove(T item) override {
        for (int i = 0; i < size; i++) {
            int segmentIndex, localIndex;
            getSegmentAndLocalIndex(i, localIndex, segmentIndex);
            if (data->get(segmentIndex).segment->get(localIndex) == item) {
                Node& node = data->get(segmentIndex);
                DynamicArray<T>* newSeg = new DynamicArray<T>(node.validCount - 1);
                for (int j = 0, k = 0; j < node.validCount; j++) {
                    if (j != localIndex) {
                        newSeg->Set(k++, node.segment->get(j));
                    }
                }
                delete node.segment;
                node.segment = newSeg;
                node.validCount--;
                size--;
                if (node.validCount == 0) {
                    T* tempData = new T[size];
                    for (int j = 0, k = 0; j < size + 1; j++) {
                        if (j != i) {
                            tempData[k++] = Get(j);
                        }
                    }
                    From(tempData, size);
                    delete[] tempData;
                }
                return true;
            }
        }
        return false;
    }

    Sequence<T>* From(T* items, int size) override {
        delete data;
        this->size = size;
        this->sizeSegment = 4;
        int numSegments = (size + sizeSegment - 1) / sizeSegment;
        data = new LinkedList<Node>();
        for (int i = 0; i < numSegments; i++) {
            int segmentSize = std::min(sizeSegment, size - i * sizeSegment);
            DynamicArray<T>* segment = new DynamicArray<T>(segmentSize);
            for (int j = 0; j < segmentSize; j++) {
                if (i * sizeSegment + j < size) {
                    segment->Set(j, items[i * sizeSegment + j]);
                }
            }
            data->append(Node(segment));
        }
        return this;
    }

    std::unique_ptr<IEnumerator<T>> GetEnumerator() const override {
        return std::make_unique<SegmentedListEnumerator>(data, size);
    }

    
    // T Sum() const override {
    //     if (size == 0) {
    //         return ring.Zero();
    //     }
    //     T result = ring.Zero();
    //     for (int i = 0; i < data->getSize(); i++) {
    //         Node node = data->get(i);
    //         for (int j = 0; j < node.validCount; j++) {
    //             result = ring.Operation(result, node.segment->get(j));
    //         }
    //     }
    //     return result;
    // }

    // T Product() const override {
    //     if (size == 0) {
    //         return ring.Identity();
    //     }
    //     T result = ring.Identity();
    //     for (int i = 0; i < data->getSize(); i++) {
    //         Node node = data->get(i);
    //         for (int j = 0; j < node.validCount; j++) {
    //             result = ring.Multiplication(result, node.segment->get(j));
    //         }
    //     }
    //     return result;
    // }
};

