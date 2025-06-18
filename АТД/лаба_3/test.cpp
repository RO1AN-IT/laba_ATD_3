#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "DeqSegment.h"
#include "que.h"
#include "deq.h"
#include "stack.h"
#include "priorQue.h"
#include "vec.h"
#include "DioganalMatrix.h"
#include "TriangularMatrix.h"
#include "RectangularMatrix.h"
#include "SquareMatrix.h"
#include "Hanoi.h"
#include "complex.h"
#include "Student.h"
#include "Prepod.h"

void writeResult(const std::string& testName, bool passed, const std::string& error = "") {
    std::cout << "Test: " << testName << " - " << (passed ? "PASSED" : "FAILED") << std::endl;
    if (!passed) std::cout << "  Error: " << error << std::endl;
}

template<typename T>
std::vector<T> parseArray(const std::string& line) {
    std::vector<T> result;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ',')) {
        result.push_back(std::stoi(token));
    }
    return result;
}

template<typename T>
void testSegmentedDeque(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line != "# SegmentedDeque Tests") {}
    
    SegmentedDeque<T> deque1;
    bool passed = deque1.empty();
    writeResult("SegmentedDeque Default Constructor and Empty", passed, passed ? "" : "Expected empty deque");

    deque1.push_back(1);
    deque1.push_front(2);
    passed = deque1.size() == 2 && deque1.front() == 2 && deque1.back() == 1;
    writeResult("SegmentedDeque Push and Pop", passed, passed ? "" : "Push or front/back failed");

    int front = deque1.pop_front();
    int back = deque1.pop_back();
    passed = front == 2 && back == 1 && deque1.empty();
    writeResult("SegmentedDeque Pop Front and Back", passed, passed ? "" : "Pop operations failed");

    std::getline(in, line); 
    std::vector<T> arr = parseArray<T>(line.substr(line.find('[') + 1, line.find(']') - line.find('[') - 1));
    SegmentedDeque<T> deque2(arr.data(), arr.size());
    const SegmentedDeque<T>& const_deque2 = deque2;
    passed = deque2.size() == arr.size() && const_deque2[0] == arr[0] && const_deque2[arr.size()-1] == arr[arr.size()-1];
    writeResult("SegmentedDeque Array Constructor", passed, passed ? "" : "Array constructor failed");

    std::cout << "Sorting SegmentedDeque..." << std::endl;
    std::cout << "Before sort: " << deque2 << std::endl;
    deque2.Sort();
    std::cout << "After sort: " << deque2 << std::endl;
    // passed = true;
    // for (size_t i = 0; i < arr.size(); ++i) {
    //     if (const_deque2[i] != static_cast<int>(i + 1)) passed = false;
    // }
    //writeResult("SegmentedDeque Sort", passed, passed ? "" : "Sort failed");

    // Test 6: Exception handling (pop empty)
    try {
        SegmentedDeque<T> emptyDeque;
        emptyDeque.pop_front();
        writeResult("SegmentedDeque Pop Empty Exception", false, "Expected out_of_range exception");
    } catch (const std::out_of_range&) {
        writeResult("SegmentedDeque Pop Empty Exception", true);
    }
}

template<typename T>
void testQueue(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line != "# Queue Tests") {}
    
    Queue<T> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    bool passed = queue.size() == 2 && queue.front() == 1 && queue.back() == 2;
    writeResult("Queue Enqueue and Access", passed, passed ? "" : "Enqueue or access failed");

    int dequeued = queue.dequeue();
    passed = dequeued == 1 && queue.size() == 1;
    writeResult("Queue Dequeue", passed, passed ? "" : "Dequeue failed");
}

template<typename T>
void testDeque(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line != "# Deque Tests") {}
    
    Deque<T> deque;
    deque.push_back(1);
    deque.push_front(2);
    bool passed = deque.size() == 2 && deque.front() == 2 && deque.back() == 1;
    writeResult("Deque Push and Access", passed, passed ? "" : "Push or access failed");

    int popped = deque.pop_front();
    passed = popped == 2 && deque.size() == 1;
    writeResult("Deque Pop Front", passed, passed ? "" : "Pop front failed");
}

template<typename T>
void testStack(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line != "# Stack Tests") {}
    
    Stack<T> stack;
    stack.push(1);
    stack.push(2);
    bool passed = stack.size() == 2 && stack.top() == 2;
    writeResult("Stack Push and Top", passed, passed ? "" : "Push or top failed");

    int popped = stack.pop();
    passed = popped == 2 && stack.size() == 1;
    writeResult("Stack Pop", passed, passed ? "" : "Pop failed");
}

template<typename T>
void testPriorityQueue(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line != "# PriorityQueue Tests") {}
    
    PriorityQueue<T> pq;
    pq.enqueue(1, 2);
    pq.enqueue(2, 1);
    bool passed = pq.peek() == 1 && pq.size() == 2;
    writeResult("PriorityQueue Enqueue and Peek", passed, passed ? "" : "Enqueue or peek failed");

    int dequeued = pq.dequeue();
    passed = dequeued == 1 && pq.size() == 1;
    writeResult("PriorityQueue Dequeue", passed, passed ? "" : "Dequeue failed");

    const PriorityQueue<T>& const_pq = pq;
    passed = const_pq[0] == 2; 
    writeResult("PriorityQueue Const Access", passed, passed ? "" : "Const access failed");
}

template<typename T>
void testVector(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line != "# Vector Tests") {}
    
    std::getline(in, line); 
    std::vector<T> arr = parseArray<T>(line.substr(line.find('[') + 1, line.find(']') - line.find('[') - 1));
    Vector<T> vec(arr.data(), arr.size());
    bool passed = vec.size() == arr.size() && vec.norm() == 5.0;
    writeResult("Vector Constructor and Norm", passed, passed ? "" : "Constructor or norm failed");

    Vector<T> vec2(arr.data(), arr.size());
    Vector<T> sum = vec + vec2;
    const Vector<T>& const_sum = sum;
    passed = const_sum[0] == 6 && const_sum[1] == 8;
    writeResult("Vector Addition", passed, passed ? "" : "Addition failed");

    try {
        Vector<T> vec3(3);
        vec + vec3;
        writeResult("Vector Invalid Addition", false, "Expected invalid_argument exception");
    } catch (const std::invalid_argument&) {
        writeResult("Vector Invalid Addition", true);
    }
}

template<typename T>
void testDiagonalMatrix(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line != "# DiagonalMatrix Tests") {}
    
    //std::getline(in, line); // Skip "Input: Array ..."
    std::getline(in, line); // Get array line
    std::vector<T> arr = parseArray<T>(line.substr(line.find('[') + 1, line.find(']') - line.find('[') - 1));
    DiagonalMatrix<T> mat(arr.data(), arr.size());
    bool passed = mat.get(0, 0) == 1 && mat.get(1, 1) == 2 && mat.get(1, 0) == 0;
    writeResult("DiagonalMatrix Constructor and Get", passed, passed ? "" : "Constructor or get failed");

    try {
        mat.set(1, 0, 5);
        writeResult("DiagonalMatrix Invalid Set", false, "Expected invalid_argument exception");
    } catch (const std::invalid_argument&) {
        writeResult("DiagonalMatrix Invalid Set", true);
    }
}

template<typename T>
void testTriangularMatrix(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line != "# TriangularMatrix Tests") {}
    
    //std::getline(in, line); // Skip "Input: Array ..."
    std::getline(in, line); // Get array line
    std::vector<T> arr = parseArray<T>(line.substr(line.find('[') + 1, line.find(']') - line.find('[') - 1));
    TriangularMatrix<T> mat(arr.data(), 3, true);
    bool passed = mat.get(0, 0) == 1 && mat.get(0, 1) == 2 && mat.get(1, 0) == 0;
    writeResult("TriangularMatrix Constructor and Get", passed, passed ? "" : "Constructor or get failed");

    try {
        TriangularMatrix<T>* mat2 = mat.transpose();
        passed = mat2->get(0, 1) == 0 && mat2->get(1, 0) == 2;
        writeResult("TriangularMatrix Transpose", passed, passed ? "" : "Transpose failed");
    }
    catch(...){
        writeResult("TriangularMatrix Transpose", false, "Expected valid transpose");
    }
}

template<typename T>
void testRectangularMatrix(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line != "# RectangularMatrix Tests") {}
    
   // std::getline(in, line); // Skip "Input: Array ..."
    std::getline(in, line); // Get array line
    std::vector<T> arr = parseArray<T>(line.substr(line.find('[') + 1, line.find(']') - line.find('[') - 1));
    RectangularMatrix<T> mat(arr.data(), 2, 2);
    bool passed = mat.get(0, 0) == 1 && mat.get(1, 1) == 4;
    writeResult("RectangularMatrix Constructor and Get", passed, passed ? "" : "Constructor or get failed");

    RectangularMatrix<T> mat2 = mat * 2;
    passed = mat2.get(0, 0) == 2 && mat2.get(1, 1) == 8;
    writeResult("RectangularMatrix Scalar Multiplication", passed, passed ? "" : "Scalar multiplication failed");
}

template<typename T>
void testSquareMatrix(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line != "# SquareMatrix Tests") {}
    
   // std::getline(in, line); // Skip "Input: Array ..."
    std::getline(in, line); // Get array line
    std::vector<T> arr = parseArray<T>(line.substr(line.find('[') + 1, line.find(']') - line.find('[') - 1));
    SquareMatrix<T> mat(arr.data(), 2);
    bool passed = mat.get(0, 0) == 1 && mat.get(1, 1) == 4;
    writeResult("SquareMatrix Constructor and Get", passed, passed ? "" : "Constructor or get failed");

    T det = mat.determinant();
    passed = det == -2; // det([[1,2],[3,4]]) = 1*4 - 2*3 = -2
    writeResult("SquareMatrix Determinant", passed, passed ? "" : "Determinant failed");
}

void testHanoi(std::ifstream& in) {
    std::cout << std::endl << "Testing Hanoi..." << std::endl;

    std::string line;
    while (std::getline(in, line) && line != "# Hanoi Tests") {}

    std::getline(in, line);
    std::vector<int> arr = parseArray<int>(line.substr(line.find('[') + 1, line.find(']') - line.find('[') - 1));

    try {
        Hanoi hanoi(arr[0], arr[1]); 
        hanoi.solve();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "Hanoi test completed." << std::endl;
}

int main() {
    std::ifstream in("input.txt");
    if (!in.is_open()) {
        std::cerr << "Error opening input.txt" << std::endl;
        return 1;
    }

    testSegmentedDeque<int>(in);
    in.clear();
    in.seekg(0);
    testQueue<int>(in);
    in.clear();
    in.seekg(0);
    testDeque<int>(in);
    in.clear();
    in.seekg(0);
    testStack<int>(in);
    in.clear();
    in.seekg(0);
    testPriorityQueue<int>(in);
    in.clear();
    in.seekg(0);
    testVector<int>(in);
    in.clear();
    in.seekg(0);
    testDiagonalMatrix<int>(in);
    in.clear();
    in.seekg(0);
    testTriangularMatrix<int>(in);
    in.clear();
    in.seekg(0);
    testRectangularMatrix<int>(in);
    in.clear();
    in.seekg(0);
    testSquareMatrix<int>(in);
    in.clear();
    in.seekg(0);
    testHanoi(in);

    in.close();
    return 0;
}