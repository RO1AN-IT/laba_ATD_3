#pragma once
#include <utility>
#include <iostream>
#include "ICollection.h"
#include "IGroup.h"


template<typename T>
class Sequence : public ICollection<T> {
public: 
    virtual T Get(int index) const = 0;
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual int GetLength() const = 0;
    virtual Sequence<T>* Append(T item) = 0;
    virtual Sequence<T>* Prepend(T item) = 0;
    virtual Sequence<T>* InsertAt(T item, int index) = 0;
    virtual Sequence<T>* Concat(Sequence<T>* list) = 0;
    virtual void RemoveAt(int index) = 0;
    virtual Sequence<T>* GetSubSequence(int startIndex, int endIndex) const = 0;
    virtual Sequence<T>* From(T* items, int size) = 0;

    void Add(T item) override {
        Append(item);
    }

    int Count() const override {
        return GetLength();
    }

    // virtual T Sum() const = 0;
    // virtual T Product() const = 0;

    std::string toString(){
        std::string res;
        res += "[ ";
        for(int i = 0; i < GetLength(); i++){
            res += std::to_string(Get(i));
            if(i < GetLength() - 1){
                res += ", ";
            }
        }
        res += " ]";
        return res;
    }
    

    virtual ~Sequence() {}

};

template<typename T>
void printSequence(Sequence<T>& sequence){
    std::cout << sequence.toString() << std::endl;
}

