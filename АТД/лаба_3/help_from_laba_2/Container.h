#pragma once 

template<typename T>
class Container{
public:
    virtual void append(T element) = 0;
    virtual void prepend(T element) = 0;
    virtual T& get(int index) const = 0;
    virtual int getSize() const = 0;
    virtual T getFirst() const = 0;
    virtual T getLast() const = 0;
    virtual T* getData() const = 0;
    virtual void insertAt(T item, int index) = 0;

    std::string toString(){
        std::string res;
        res += "[ ";
        for(int i = 0; i < getSize(); i++){
            res += std::to_string(get(i));
            if(i < getSize() - 1){
                res += ", ";
            }
        }
        res += " ]";
        return res;
    }
    
    
    virtual ~Container() {}
};

template<typename T>
void printContainer(Container<T>& container){
    std::cout << container.toString() << std::endl;
}