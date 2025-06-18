#pragma once
#include <iostream>
#include <string>
#include <stdexcept>

typedef std::string TeacherId; 

class Teacher {
private:
    TeacherId id;
    std::string name;
    std::string surname;
    int age;

public:
    Teacher(const TeacherId& id, const std::string& name, const std::string& surname, int age) : id(id), name(name), surname(surname), age(age) {
        if (age < 0) {
            throw std::invalid_argument("Age cannot be negative");
        }
    }

    TeacherId getId() const {
        return id;
    }

    std::string getName() const {
        return name;
    }

    std::string getSurname() const {
        return surname;
    }

    int getAge() const {
        return age;
    }

    void setName(const std::string& newName) {
        name = newName;
    }

    void setSurname(const std::string& newSurname) {
        surname = newSurname;
    }

    void setAge(int newAge) {
        if (newAge < 0) {
            throw std::invalid_argument("Age cannot be negative");
        }
        age = newAge;
    }

    friend std::ostream& operator<<(std::ostream& os, const Teacher& Teacher) {
        os << "Teacher ID: " << Teacher << ", Name: " << Teacher.name<< ", Surname: " << Teacher.surname << ", Age: " << Teacher.age;
        return os;
    }

    bool operator==(const Teacher& other) const {
        return id == other.id && name == other.name && surname == other.surname && age == other.age;
    }
    
    bool operator!=(const Teacher& other) const {
        return !(*this == other);
    }

    bool operator<(const Teacher& other) const {
        return id < other.id; 
    }
    bool operator>(const Teacher& other) const {
        return id > other.id; 
    }
    bool operator<=(const Teacher& other) const {
        return id <= other.id; 
    }
    bool operator>=(const Teacher& other) const {
        return id >= other.id; 
    }
};
