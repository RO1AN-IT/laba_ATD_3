#pragma once
#include "Sequence.h"
#include "IntegerRing.h"
#include <stdexcept>
#include <utility>
#include <tuple>
#include <algorithm>

template<typename T>
class AdaptiveSequence : public Sequence<T> {
private:
    T* data;
    int capacity;
    int size;
    int head;
    int tail;
    // IntegerRing<T> ring;

    class AdaptiveEnumerator : public IEnumerator<T> {
    private: 
        const T* data;
        int head;
        int currentIndex;
        int size;

    public:
        AdaptiveEnumerator(const T* d, int h, int s) : data(d), head(h), currentIndex(-1), size(s) {}

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
            return data[head + currentIndex];
        }

        void Reset() override {
            currentIndex = -1;
        }
    };


    void ResizeAndCenter(){
        int newCapacity = (capacity == 0 ? 4 : capacity * 2);
        T* newData = new T[newCapacity];
        if(!data){
            data = newData;
            size = 0;
            head = newCapacity / 4;
            tail = head + size;
            return;
        }
        int newHead = newCapacity / 4;
        int newTail = newHead + size;
        for(int i = 0; i < size; i++){
            newData[newHead + i] = data[head + i];
        }
        delete[] data;
        data = newData;
        head = newHead;
        tail = newTail;
        capacity = newCapacity;
    }

public:
public:
    AdaptiveSequence() : data(nullptr), capacity(0), size(0), head(0), tail(0) {}
    
    AdaptiveSequence(int size) : capacity(size), size(size), head(size / 4), tail(head + size) {
        data = new T[capacity];
        // Инициализация элементов по умолчанию
        for (int i = 0; i < capacity; i++) {
            data[i] = T();
        }
    }
    
    AdaptiveSequence(T* items, int size) : capacity(size), size(size), head(size / 4), tail(head + size) {
        data = new T[capacity];
        for (int i = 0; i < size; i++) {
            data[head + i] = items[i];
        }
    }
    
    // Конструктор копирования
    AdaptiveSequence(const AdaptiveSequence<T>& other) : capacity(other.capacity), size(other.size), head(other.head), tail(other.tail) {
        data = new T[capacity];
        for (int i = 0; i < capacity; i++) {
            data[i] = other.data[i];
        }
    }
    
    // Конструктор перемещения
    AdaptiveSequence(AdaptiveSequence<T>&& other) noexcept : data(other.data), capacity(other.capacity), size(other.size), head(other.head), tail(other.tail) {
        other.data = nullptr;
        other.capacity = 0;
        other.size = 0;
        other.head = 0;
        other.tail = 0;
    }

    ~AdaptiveSequence() {
        delete[] data;
    }

    // Оператор присваивания копированием
    AdaptiveSequence<T>& operator=(const AdaptiveSequence<T>& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            size = other.size;
            head = other.head;
            tail = other.tail;
            data = new T[capacity];
            for (int i = 0; i < capacity; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    // Оператор присваивания перемещением
    AdaptiveSequence<T>& operator=(AdaptiveSequence<T>&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            capacity = other.capacity;
            size = other.size;
            head = other.head;
            tail = other.tail;
            
            other.data = nullptr;
            other.capacity = 0;
            other.size = 0;
            other.head = 0;
            other.tail = 0;
        }
        return *this;
    }

    T Get(int index) const override {
        if(index < 0 || index >= size){
            throw std::out_of_range("Index out of range");
        }
        return data[head + index];
    }
    
    T GetFirst() const override {
        return data[head];
    }

    T GetLast() const override {
        return data[tail - 1];
    }

    int GetLength() const override{
        return size;
    }

    Sequence<T>* Append(T item) override {
        if(tail == capacity){
            ResizeAndCenter();
        }
        data[tail] = item;
        tail++;
        size++;
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        if(head == 0){
            ResizeAndCenter();
        }
        head--;
        data[head] = item;
        size++;
        return this;
    }

    Sequence<T>* InsertAt(T item, int index) override {
        if(index < 0 || index >= size){
            throw std::out_of_range("Index out of range");
        }
        if(size == capacity || head == 0 || tail == capacity){
            ResizeAndCenter();
        }    
        if (index == 0) {
            return Prepend(item);
        }
        if (index == size) {
            return Append(item);
        }

        // for(int i = size - 1; i >= index; i--){
        //     data[head + i + 1] = data[head + i];
        // }
        // data[head + index] = item;
        // tail++;
        // size++;
        // return this;

        T* newData = new T[size + 1];
        for(int i = 0; i < index; i++){
            newData[i] = data[head + i];
        }
        newData[index] = item;
        for(int i = index; i < size; i++){
            newData[i+1] = data[head + i];
        }
        delete[] data;
        capacity = size + 1;
        head = capacity / 4;
        size++;
        data = new T[capacity];
        for(int i = 0; i < size; i++){
            data[head + i] = newData[i];
        }
        tail = head + size;
        delete[] newData;
        return this;
    }

    void RemoveAt(int index) override {
        if(index < 0 || index >= size){
            throw std::out_of_range("Index out of range");
        }
        for(int i = index; i < size - 1; i++){
            data[head + i] = data[head + i + 1];
        }
        tail--;
        size--;
    }


    Sequence<T>* Concat(Sequence<T>* list) override {
        for(int i = 0; i < list->GetLength(); i++){
            Append(list->Get(i));
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
        T* SubItems = new T[subSize];
        for(int i = 0; i < subSize; i++){
            SubItems[i] = Get(startIndex + i);
        }
        Sequence<T>* newItem = new AdaptiveSequence<T>(SubItems, subSize);
        delete[] SubItems;
        return newItem;
    }

    Sequence<T>* From(T* items, int size) override {
        delete[] data;
        this->size = size;
        this->capacity = size;
        this->head = size / 4;
        this->tail = this->head + size;
        data = new T[capacity];
        for (int i = 0; i < size; i++) {
            data[head + i] = items[i];
        }
        return this;
    }

    template<typename U> 
    Sequence<U>* Map(U (*f)(T)) {
        U* items = new U[size];
        for(int i = 0; i < size; i++){
            items[i] = f(Get(i));
        }
        Sequence<U>* newItems = new AdaptiveSequence<U>(items, size);
        delete[] items;
        return newItems;
    }

    template<typename U>
    Sequence<U>* FlatMap(Sequence<U>* (*f)(T)) {
        int totalSize = 0;
        for(int i = 0; i < size; i++){
            Sequence<U>* seq = f(Get(i));
            totalSize += seq->GetLength();
            delete seq;
        }
        U* flatItems = new U[totalSize];
        int index = 0;
        for(int i = 0; i < size; i++){
            Sequence<U>* seq = f(Get(i));
            for(int j = 0; j < seq->GetLength(); j++){
                flatItems[index++] = seq->Get(j);
            }
            delete seq;
        }
        Sequence<U>* seq = new AdaptiveSequence<U>(flatItems, totalSize);
        delete[] flatItems;
        return seq;
    }

    Sequence<T>* Where(bool (*f)(T)){
        int newSize = 0;
        for(int i = 0; i < size; i++){
            if(f(Get(i))){
                newSize++;
            }
        }
        T* items = new T[newSize];
        int index = 0;
        for(int i = 0; i < size; i++){
            if(f(Get(i))){
                items[index++] = Get(i);
            }
        }
        Sequence<T>* seq = new AdaptiveSequence<T>(items, newSize);
        delete[] items;
        return seq;
    }

    template<typename U>
    Sequence<U>* Reduce(U (*f)(U, T), const U c) {
        U result = c;
        for(int i = 0; i < size; i++){
            result = f(result, Get(i));
        }
        U* items = new U[1];
        items[0] = result;
        Sequence<U>* seq = new AdaptiveSequence<U>(items, 1);
        delete[] items;
        return seq;
    }

    template<typename... Tuples, typename U>
    Sequence<std::tuple<Tuples..., U>>* Zip(Sequence<U>* other) {
        int zipSize = std::min(size, other->GetLength());
        std::tuple<Tuples..., U>* zippedItems = new std::tuple<Tuples..., U>[zipSize];
        
        if constexpr (sizeof...(Tuples) == 0) {
            for (int i = 0; i < zipSize; i++) {
                zippedItems[i] = std::tuple<T, U>(data[head + i], other->Get(i));
            }
        } else {
            auto create_tuple = [&]<size_t... Is>(std::index_sequence<Is...>) {
                for (int i = 0; i < zipSize; i++) {
                    T tuple = data[head + i];
                    U value = other->Get(i);
                    zippedItems[i] = std::tuple<Tuples..., U>(std::get<Is>(tuple)..., value);
                }
            };
            create_tuple(std::index_sequence_for<Tuples...>{});
        }
        
        Sequence<std::tuple<Tuples..., U>>* result = new AdaptiveSequence<std::tuple<Tuples..., U>>(zippedItems, zipSize);
        delete[] zippedItems;
        return result;
    }

    template<typename... Tuples>
    std::tuple<AdaptiveSequence<Tuples>...>* UnZip() const {
        std::tuple<AdaptiveSequence<Tuples>...>* result = new std::tuple<AdaptiveSequence<Tuples>...>();
        auto create_arrays = [&]<size_t... Is>(std::index_sequence<Is...>) {
            ((std::get<Is>(*result) = AdaptiveSequence<Tuples>(new Tuples[size], size)), ...);
        };
        create_arrays(std::index_sequence_for<Tuples...>{});
        auto fill_arrays = [&]<size_t... Is>(std::index_sequence<Is...>) {
            for (int i = 0; i < size; i++) {
                auto tuple = this->Get(i);
                ((std::get<Is>(*result)->InsertAt(i, std::get<Is>(tuple))), ...);
            }
        };
        fill_arrays(std::index_sequence_for<Tuples...>{});
        return result;
    }

    void Clear() override {
        delete[] data;
        data = nullptr;
        size = 0;
        head = size / 4;
        tail = head + size;
        capacity = 0;
    }

   bool Contains(T item) const override {
        for (int i = 0; i < size; i++) {
            if (data[head + i] == item) {
                return true;
            }
        }
        return false;
    }

    bool Remove(T item) override {
        for (int i = 0; i < size; i++) {
            if (data[head + i] == item) {
                RemoveAt(i);
                return true;
            }
        }
        return false;
    }

    std::unique_ptr<IEnumerator<T>> GetEnumerator() const override {
        return std::make_unique<AdaptiveEnumerator>(data, head, size);
    }
    
    //  T Sum() const override {
    //     if (size == 0) {
    //         return ring.Zero();
    //     }
    //     T result = ring.Zero();
    //     for (int i = 0; i < size; i++) {
    //         result = ring.Operation(result, data[head + i]);
    //     }
    //     return result;
    // }

    // T Product() const override {
    //     if (size == 0) {
    //         return ring.Identity();
    //     }
    //     T result = ring.Identity();
    //     for (int i = 0; i < size; i++) {
    //         result = ring.Multiplication(result, data[head + i]);
    //     }
    //     return result;
    // }
};