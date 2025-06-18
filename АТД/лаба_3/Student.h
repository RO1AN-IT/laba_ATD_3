#pragma once
#include <iostream>
#include <string>
#include <stdexcept>

typedef std::string StudentId;

class Student {
private:
    StudentId id;
    std::string name;
    std::string surname;
    int age;

public:
    Student(const StudentId& id, const std::string& name, const std::string& surname, int age)
        : id(id), name(name), surname(surname), age(age) {
        if (age < 0) {
            throw std::invalid_argument("Age cannot be negative");
        }
    }

    StudentId getId() const {
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

    friend std::ostream& operator<<(std::ostream& os, const Student& student) {
        os << "Student ID: " << student.id << ", Name: " << student.name
           << ", Surname: " << student.surname << ", Age: " << student.age;
        return os;
    }

    bool operator==(const Student& other) const {
        return id == other.id && name == other.name && surname == other.surname && age == other.age;
    }

    bool operator<(const Student& other) const {
        return id < other.id; 
    }
    bool operator>(const Student& other) const {
        return id > other.id; 
    }
    bool operator<=(const Student& other) const {
        return !(*this > other);
    }
    bool operator>=(const Student& other) const {
        return !(*this < other);
    }

    
    bool operator!=(const Student& other) const {
        return !(*this == other);
    }
};
