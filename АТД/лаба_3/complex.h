#pragma once
#include <stdexcept>
#include <cmath>
#include <iostream>

template<typename T>
class Complex {
private:
    T real; 
    T imag; 

public:
    Complex() : real(0), imag(0) {}

    Complex(T real, T imag = 0) : real(real), imag(imag) {}

    Complex(const Complex<T>& other) : real(other.real), imag(other.imag) {}

    Complex<T>& operator=(const Complex<T>& other) {
        if (this != &other) {
            real = other.real;
            imag = other.imag;
        }
        return *this;
    }

    bool operator==(const Complex<T>& other) const {
        return std::abs(real - other.real) < 1e-10 && std::abs(imag - other.imag) < 1e-10;
    }
    bool operator!=(const Complex<T>& other) const {
        return !(*this == other);
    }
    bool operator<(const Complex<T>& other) const {
        return (real < other.real) || (real == other.real && imag < other.imag);
    }
    bool operator<=(const Complex<T>& other) const {
        return *this < other || *this == other;
    }
    bool operator>(const Complex<T>& other) const {
        return !(*this <= other);
    }
    bool operator>=(const Complex<T>& other) const {
        return !(*this < other);
    }

    ~Complex() = default;

    T realPart() const { return real; }

    T imagPart() const { return imag; }

    Complex<T> operator+(const Complex<T>& other) const {
        return Complex<T>(real + other.real, imag + other.imag);
    }

    Complex<T> operator-(const Complex<T>& other) const {
        return Complex<T>(real - other.real, imag - other.imag);
    }

    Complex<T> operator*(const Complex<T>& other) const {
        return Complex<T>(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }

    Complex<T> operator/(const Complex<T>& other) const {
        T denominator = other.real * other.real + other.imag * other.imag;
        return Complex<T>((real * other.real + imag * other.imag) / denominator, (imag * other.real - real * other.imag) / denominator);
    }

    Complex<T> operator*(T scalar) const {
        return Complex<T>(real * scalar, imag * scalar);
    }

    T modulus() const {
        return std::sqrt(real * real + imag * imag);
    }

    T argument() const {
        return std::atan2(imag, real);
    }

    // Сопряжённое число
    Complex<T> conjugate() const {
        return Complex<T>(real, -imag);
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex<T>& z) {
        os << z.real;
        if (z.imag >= 0) {
            os << "+";
        }
        os << z.imag << "i";
        return os;
    }
};