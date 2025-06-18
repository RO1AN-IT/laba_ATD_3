#include "stack.h"
#include <iostream>

struct Ring {
    int shape; 
    int color;
    Ring(int s = 0, int c = 0) : shape(s), color(c) {}
    bool operator==(const Ring& other) const {
        return shape == other.shape && color == other.color;
    }
    friend std::ostream& operator<<(std::ostream& os, const Ring& ring) {
        os << "Ring(shape: " << ring.shape << ", color: " << ring.color << ")";
        return os;
    }
};

class Hanoi {
private:
    Stack<Ring> A;  
    Stack<Ring> B; 
    Stack<Ring> C; 
    int disks;      
    int target;     

    void move(Stack<Ring>& source, Stack<Ring>& target) {
        if (source.empty()) return;
        Ring ring = source.pop();
        target.push(ring);
        std::cout << "Move ring (shape: " << ring.shape << ", color: " << ring.color << ") from ";
        if (&source == &A) std::cout << "A";
        else if (&source == &B) std::cout << "B";
        else std::cout << "C";
        std::cout << " to ";
        if (&target == &A) std::cout << "A";
        else if (&target == &B) std::cout << "B";
        else std::cout << "C";
        std::cout << std::endl;
    }

    void solveHanoi(int n, Stack<Ring>& source, Stack<Ring>& auxiliary, Stack<Ring>& target) {
        if (n == 1) {
            move(source, target);
            return;
        }
        solveHanoi(n - 1, source, target, auxiliary);
        move(source, target);
        solveHanoi(n - 1, auxiliary, source, target);
    }

public:
    Hanoi(int n, int targetRod) : disks(n), target(targetRod) {
        if (n < 1 || target < 1 || target > 3) {
            throw std::invalid_argument("Invalid number of disks or target rod");
        }
        for (int i = n; i >= 1; --i) {
            A.push(Ring(i, i % 5)); 
        }
    }

    void solve() {
        Stack<Ring>* source = &A;
        Stack<Ring>* auxiliary = &B;
        Stack<Ring>* targetStack = &C;

        if (target == 1) targetStack = &A;
        else if (target == 2) targetStack = &B;
        else if (target == 3) targetStack = &C;

        if (targetStack == &A) auxiliary = &B;
        else if (targetStack == &B) auxiliary = (source == &A) ? &C : &A;
        else auxiliary = (source == &A) ? &B : &A;

        solveHanoi(disks, *source, *auxiliary, *targetStack);

        std::cout << "Final state:" << std::endl;
        std::cout << "A: " << A << std::endl;
        std::cout << "B: " << B << std::endl;
        std::cout << "C: " << C << std::endl;
    }
};