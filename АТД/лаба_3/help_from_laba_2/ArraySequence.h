#pragma once
#include "Sequence.h"
#include "DynamicArray.h"
#include "IntegerRing.h"
#include <stdexcept>
#include <utility>
#include <tuple>

template<typename T>
class MutableArraySequence;
template<typename T>
class ImmutableArraySequence;

template<typename T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* data;
    int size;
    // IntegerRing<T> ring;

    class ArrayEnumerator : public IEnumerator<T> {
    private:
        const DynamicArray<T>* data;
        int currentIndex;
        int size;
    public:
        ArrayEnumerator(const DynamicArray<T>* d, int size) : data(d), size(size), currentIndex(-1) {}

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
            return data->get(currentIndex);
        }

        void Reset() override {
            currentIndex = -1;
        }
    };

    Sequence<T>* AppendInternal(T item) {
        data->Resize(size + 1);
        size++;
        data->Set(size - 1, item);
        return this;
    }

    Sequence<T>* PrependInternal(T item) {
        data->Resize(size + 1);
        size++;
        for (int i = size - 1; i > 0; i--) {
            data->Set(i, data->get(i - 1));
        }
        data->Set(0, item);
        return this;
    }

    Sequence<T>* InsertAtInternal(T item, int index) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Index out of range");
        }
        data->Resize(size + 1);
        size++;
        for (int i = size - 1; i > index; i--) {
            data->Set(i, data->get(i - 1));
        }
        data->Set(index, item);
        return this;
    }

    Sequence<T>* ConcatInternal(Sequence<T>* array) {
        int reSize = this->GetLength() + array->GetLength();
        data->Resize(reSize);
        for (int i = 0; i < array->GetLength(); i++) {
            data->Set(size + i, array->Get(i));
        }
        size = reSize;
        return this;
    }

    template<typename U>
    Sequence<U>* MapInternal(U (*f)(T)) {
        U* items = new U[size];
        for (int i = 0; i < size; i++) {
            items[i] = f(data->get(i));
        }
        Sequence<U>* result = new MutableArraySequence<U>(items, size);
        delete[] items;
        return result;
    }

    template<typename U>
    Sequence<U>* ReduceInternal(U (*f)(U, T), const U c) {
        U result = c;
        for (int i = 0; i < size; i++) {
            result = f(result, data->get(i));
        }
        U* items = new U[1];
        items[0] = result;
        Sequence<U>* resultSeq = new MutableArraySequence<U>(items, 1);
        delete[] items;
        return resultSeq;
    }

    Sequence<T>* FromInternal(T* items, int size) {
        delete data;
        data = new DynamicArray<T>(items, size);
        this->size = size;
        return this;
    }

    template<typename... Tuples, typename U>
    Sequence<std::tuple<Tuples..., U>>* ZipInternal(Sequence<U>* other) {
        int zipSize = std::min(size, other->GetLength());
        std::tuple<Tuples..., U>* zippedItems = new std::tuple<Tuples..., U>[zipSize];
        
        if constexpr (sizeof...(Tuples) == 0) {
            for (int i = 0; i < zipSize; i++) {
                zippedItems[i] = std::tuple<T, U>(data->get(i), other->Get(i));
            }
        } else {
            auto create_tuple = [&]<size_t... Is>(std::index_sequence<Is...>) {
                for (int i = 0; i < zipSize; i++) {
                    T tuple = data->get(i);
                    U value = other->Get(i);
                    zippedItems[i] = std::tuple<Tuples..., U>(std::get<Is>(tuple)..., value);
                }
            };
            create_tuple(std::index_sequence_for<Tuples...>{});
        }
        
        Sequence<std::tuple<Tuples..., U>>* result = new MutableArraySequence<std::tuple<Tuples..., U>>(zippedItems, zipSize);
        delete[] zippedItems;
        return result;
    }

    template<typename... Tuples>
    std::tuple<ArraySequence<Tuples>...>* UnZipInternal() const {
        std::tuple<ArraySequence<Tuples>...>* result = new std::tuple<ArraySequence<Tuples>...>();
        auto create_arrays = [&]<size_t... Is>(std::index_sequence<Is...>) {
            ((std::get<Is>(*result) = ArraySequence<Tuples>(new Tuples[size], size)), ...);
        };
        create_arrays(std::index_sequence_for<Tuples...>{});
        auto fill_arrays = [&]<size_t... Is>(std::index_sequence<Is...>) {
            for (int i = 0; i < size; i++) {
                auto tuple = data->get(i);
                ((std::get<Is>(*result).GetData()->Set(i, std::get<Is>(tuple))), ...);
            }
        };
        fill_arrays(std::index_sequence_for<Tuples...>{});
        return result;
    }

    template<typename U>
    Sequence<U>* FlatMapInternal(Sequence<U>* (*f)(T)) {
        int totalSize = 0;
        for (int i = 0; i < size; i++) {
            Sequence<U>* seq = f(data->get(i));
            totalSize += seq->GetLength();
            delete seq;
        }
        U* flatItems = new U[totalSize];
        int index = 0;
        for (int i = 0; i < size; i++) {
            Sequence<U>* seq = f(data->get(i));
            for (int j = 0; j < seq->GetLength(); j++) {
                flatItems[index++] = seq->Get(j);
            }
            delete seq;
        }
        Sequence<U>* result = new MutableArraySequence<U>(flatItems, totalSize);
        delete[] flatItems;
        return result;
    }

    Sequence<T>* WhereInternal(bool (*f)(T)) {
        int newSize = 0;
        for (int i = 0; i < size; i++) {
            if (f(data->get(i))) {
                newSize++;
            }
        }
        T* filteredItems = new T[newSize];
        int index = 0;
        for (int i = 0; i < size; i++) {
            if (f(data->get(i))) {
                filteredItems[index++] = data->get(i);
            }
        }
        Sequence<T>* result = new MutableArraySequence<T>(filteredItems, newSize);
        delete[] filteredItems;
        return result;
    }

public:
    ArraySequence() : size(0) {
        data = new DynamicArray<T>(0);
    }
    ArraySequence(int size) : size(size) {
        data = new DynamicArray<T>(size);
    }
    ArraySequence(const ArraySequence<T>& other) : size(other.size) {
        data = new DynamicArray<T>(other.data->getData(), other.size);
    }
    ArraySequence(T* items, int size) : size(size) {
        data = new DynamicArray<T>(items, size);
    }

    virtual ~ArraySequence() { delete data; }

    T& operator[](int index) {
        if (index < 0 || index >= GetLength()) {
            throw std::out_of_range("Index out of range");
        }
        return data->get(index);
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= GetLength()) {
            throw std::out_of_range("Index out of range");
        }
        return data->get(index);
    }

    Sequence<T>& operator=(const Sequence<T>& other) {
        if (this == &other) {
            return *this;
        }
        delete data;
        const ArraySequence<T>* arrSeq = dynamic_cast<const ArraySequence<T>*>(&other);
        if (!arrSeq) {
            throw std::invalid_argument("Incompatible types in assignment");
        }
        int length = arrSeq->GetLength();
        T* newData = new T[length];
        for (int i = 0; i < length; ++i) {
            newData[i] = arrSeq->Get(i);
        }
        data = new DynamicArray<T>(newData, length);
        size = length;
        delete[] newData;
        return *this;
    }

    DynamicArray<T>* GetData() const {
        return data;
    }

    T Get(int index) const override {
        if (size == 0) {
            throw std::out_of_range("Sequence is empty");
        }
        if (!data) {
            throw std::out_of_range("ArraySequence is not initialized");
        }
        if (data == nullptr) std::cout << "nullptr";
        return data->get(index);
    }

    T GetFirst() const override {
        if (size == 0) {
            throw std::out_of_range("Sequence is empty");
        }
        return data->get(0);
    }

    T GetLast() const override {
        if (size == 0) {
            throw std::out_of_range("Sequence is empty");
        }
        return data->get(size - 1);
    }

    int GetLength() const override {
        return size;
    }

    Sequence<T>* GetSubSequence(int startIndex, int endIndex) const override {
        if (data->getSize() == 0) {
            throw std::out_of_range("Sequence is empty");
        }
        if (startIndex < 0 || startIndex >= data->getSize() || endIndex < startIndex || endIndex >= data->getSize()) {
            throw std::out_of_range("Invalid range");
        }
        int subSize = endIndex - startIndex + 1;
        T* subItems = new T[subSize];
        for (int i = 0; i < subSize; i++) {
            subItems[i] = data->get(startIndex + i);
        }
        MutableArraySequence<T>* subseq = new MutableArraySequence<T>(subItems, subSize);
        delete[] subItems;
        return subseq;
    }

    Sequence<T>* Append(T item) override {
        return static_cast<ArraySequence<T>*>(Instance())->AppendInternal(item);
    }

    Sequence<T>* Prepend(T item) override {
        return static_cast<ArraySequence<T>*>(Instance())->PrependInternal(item);
    }

    Sequence<T>* InsertAt(T item, int index) override {
        return static_cast<ArraySequence<T>*>(Instance())->InsertAtInternal(item, index);
    }

    Sequence<T>* Concat(Sequence<T>* array) override {
        return static_cast<ArraySequence<T>*>(Instance())->ConcatInternal(array);
    }

    template<typename U>
    Sequence<U>* Map(U (*f)(T)) {
        return MapInternal(f);
    }

    template<typename U>
    Sequence<U>* Reduce(U (*f)(U, T), const U c) {
        return ReduceInternal(f, c);
    }

    template<typename U>
    Sequence<std::tuple<T, U>>* Zip(Sequence<U>* other) {
        return ZipInternal<U>(other);
    }

    template<typename... Tuples>
    std::tuple<ArraySequence<Tuples>...>* UnZip() const {
        return UnZipInternal<Tuples...>();
    }

    Sequence<T>* From(T* items, int size) override {
        return FromInternal(items, size);
    }

    template<typename U>
    Sequence<U>* FlatMap(Sequence<U>* (*f)(T)) {
        return FlatMapInternal(f);
    }

    Sequence<T>* Where(bool (*f)(T)) {
        return WhereInternal(f);
    }

    Sequence<Sequence<T>*>* Split(bool (*f)(T)) {
        DynamicArray<Sequence<T>*>* subsequences = new DynamicArray<Sequence<T>*>(0);
        DynamicArray<T>* current = new DynamicArray<T>(0);
        for (int i = 0; i < size; i++) {
            T item = data->get(i);
            if (f(item)) {
                if (current->getSize() > 0) {
                    Sequence<T>* subseq = new MutableArraySequence<T>(current->getData(), current->getSize());
                    subsequences->append(subseq);
                    delete current;
                    current = new DynamicArray<T>(0);
                }
            } else {
                current->append(item);
            }
        }
        if (current->getSize() > 0) {
            Sequence<T>* subseq = new MutableArraySequence<T>(current->getData(), current->getSize());
            subsequences->append(subseq);
        } else if (subsequences->getSize() == 0) {
            Sequence<T>* subseq = new MutableArraySequence<T>();
            subsequences->append(subseq);
        }
        delete current;
        Sequence<Sequence<T>*>* result = new MutableArraySequence<Sequence<T>*>(subsequences->getData(), subsequences->getSize());
        delete subsequences;
        return result;
    }

    void RemoveAt(int index) override {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        T* newData = new T[size - 1];
        int newIndex = 0;
        for (int i = 0; i < size; i++) {
            if (i != index) {
                newData[newIndex++] = data->get(i);
            }
        }
        delete data;
        data = new DynamicArray<T>(newData, size - 1);
        size--;
        delete[] newData;
    }

    void Clear() override {
        delete data;
        data = new DynamicArray<T>(0);
        size = 0;
    }

    bool Contains(T item) const override {
        for (int i = 0; i < size; i++) {
            if (this->data->get(i) == item) {
                return true;
            }
        }
        return false;
    }

    bool Remove(T item) override {
        for (int i = 0; i < size; i++) {
            if (this->data->get(i) == item) {
                RemoveAt(i);
                return true;
            }
        }
        return false;
    }

    std::unique_ptr<IEnumerator<T>> GetEnumerator() const override {
        return std::make_unique<ArrayEnumerator>(data, size);
    }
    
    // T Sum() const override {
    //     if (size == 0) {
    //         return ring.Zero();
    //     }
    //     T result = ring.Zero();
    //     for (int i = 0; i < size; i++) {
    //         result = ring.Operation(result, data->get(i));
    //     }
    //     return result;
    // }

    // T Product() const override {
    //     if (size == 0) {
    //         return ring.Identity();
    //     }
    //     T result = ring.Identity();
    //     for (int i = 0; i < size; i++) {
    //         result = ring.Multiplication(result, data->get(i));
    //     }
    //     return result;
    // }
    
    virtual Sequence<T>* Instance() = 0;
};

template<typename T>
class MutableArraySequence : public ArraySequence<T> {
public:
    MutableArraySequence() : ArraySequence<T>() {}
    MutableArraySequence(int size) : ArraySequence<T>(size) {}
    MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequence<T>(other) {}
    MutableArraySequence(T* items, int size) : ArraySequence<T>(items, size) {}

    Sequence<T>* Instance() override {
        return this;
    }

    Sequence<T>& operator=(const Sequence<T>& other) {
        return ArraySequence<T>::operator=(other);
    }
};

template<typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence() : ArraySequence<T>() {}
    ImmutableArraySequence(int size) : ArraySequence<T>(size) {}
    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>(other) {}
    ImmutableArraySequence(T* items, int size) : ArraySequence<T>(items, size) {}

    Sequence<T>* Instance() override {
        return new ImmutableArraySequence<T>(*this);
    }

    Sequence<T>& operator=(const Sequence<T>& other) {
        return ArraySequence<T>::operator=(other);
    }
};