#include <iostream>
#include <string>
#include <functional>
#include "DeqSegment.h"
#include "DioganalMatrix.h"
#include "TriangularMatrix.h"
#include "que.h"
#include "stack.h"
#include "deq.h"
#include "RectangularMatrix.h"
#include "vec.h"
#include "SquareMatrix.h"
#include "priorQue.h"

using namespace std;

// Templated input function for any type T
template<typename T>
T getInput(const string& prompt) {
    T value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << "Invalid input. Please enter a valid value: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return value;
}

// Templated array input function
template<typename T>
void getArrayInput(T* arr, int size) {
    cout << "Enter " << size << " elements:\n";
    for (int i = 0; i < size; ++i) {
        cout << "Element " << i << ": ";
        arr[i] = getInput<T>("");
    }
}

template<typename T>
void segmentedDequeUI() {
    SegmentedDeque<T> deque;
    while (true) {
        cout << "\nSegmentedDeque Operations:\n";
        cout << "1. Push front\n2. Push back\n3. Pop front\n4. Pop back\n5. Front\n6. Back\n";
        cout << "7. Size\n8. Empty\n9. Clear\n10. Get element\n";
        cout << "11. Sort\n12. Contains\n13. Get SubSegment\n14. Concat\n15. Add another deque\n";
        cout << "16. Multiply by another deque\n17. Display\n18. Back to main menu\n";
        int choice = getInput<int>("Choose an option: ");

        try {
            if (choice == 1) {
                T item = getInput<T>("Enter item to push front: ");
                deque.push_front(item);
            } else if (choice == 2) {
                T item = getInput<T>("Enter item to push back: ");
                deque.push_back(item);
            } else if (choice == 3) {
                cout << "Popped front: " << deque.pop_front() << endl;
            } else if (choice == 4) {
                cout << "Popped back: " << deque.pop_back() << endl;
            } else if (choice == 5) {
                cout << "Front: " << deque.front() << endl;
            } else if (choice == 6) {
                cout << "Back: " << deque.back() << endl;
            } else if (choice == 7) {
                cout << "Size: " << deque.size() << endl;
            } else if (choice == 8) {
                cout << "Empty: " << (deque.empty() ? "Yes" : "No") << endl;
            } else if (choice == 9) {
                deque.clear();
                cout << "Deque cleared.\n";
            } else if (choice == 10) {
                int index = getInput<int>("Enter index: ");
                cout << "Element at " << index << ": " << deque[index] << endl;
            } else if (choice == 11) {
                deque.Sort();
                cout << "Deque sorted.\n";
            } else if (choice == 12) {
                int size = getInput<int>("Enter size of other deque: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                SegmentedDeque<T> other(arr, size);
                cout << "Contains: " << (deque.Contains(other) ? "Yes" : "No") << endl;
                delete[] arr;
            } else if (choice == 13) {
                int start = getInput<int>("Enter start index: ");
                int end = getInput<int>("Enter end index: ");
                auto* sub = deque.GetSubSegment(start, end);
                cout << "Subsegment: " << *sub << endl;
                delete sub;
            } else if (choice == 14) {
                int size = getInput<int>("Enter size of other deque: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                SegmentedDeque<T> other(arr, size);
                auto* result = deque.Concat(other);
                cout << "Concatenated: " << *result << endl;
                delete result;
                delete[] arr;
            } else if (choice == 15) {
                int size = getInput<int>("Enter size of other deque: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                SegmentedDeque<T> other(arr, size);
                deque + other;
                cout << "Added: " << deque << endl;
                delete[] arr;
            } else if (choice == 16) {
                int size = getInput<int>("Enter size of other deque: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                SegmentedDeque<T> other(arr, size);
                deque * other;
                cout << "Multiplied: " << deque << endl;
                delete[] arr;
            } else if (choice == 17) {
                cout << "Deque: " << deque << endl;
            } else if (choice == 18) {
                break;
            } else {
                cout << "Invalid option.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

template<typename T>
void diagonalMatrixUI() {
    DiagonalMatrix<T> matrix;
    while (true) {
        cout << "\nDiagonalMatrix Operations:\n";
        cout << "1. Initialize\n2. Get element\n3. Set element\n4. Add matrix\n5. Multiply matrix\n";
        cout << "6. Multiply by scalar\n7. Equals\n8. Transpose\n9. Determinant\n10. Size\n";
        cout << "11. Empty\n12. Clear\n13. Norm\n14. Display\n15. Back to main menu\n";
        int choice = getInput<int>("Choose an option: ");

        try {
            if (choice == 1) {
                int size = getInput<int>("Enter size: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                matrix = DiagonalMatrix<T>(arr, size);
                delete[] arr;
            } else if (choice == 2) {
                int i = getInput<int>("Enter row: ");
                int j = getInput<int>("Enter column: ");
                cout << "Element (" << i << "," << j << "): " << matrix(i, j) << endl;
            } else if (choice == 3) {
                int i = getInput<int>("Enter row: ");
                int j = getInput<int>("Enter column: ");
                T value = getInput<T>("Enter value: ");
                matrix.set(i, j, value);
            } else if (choice == 4) {
                int size = getInput<int>("Enter size of other matrix: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                DiagonalMatrix<T> other(arr, size);
                matrix = matrix + other;
                cout << "Added: " << matrix << endl;
                delete[] arr;
            } else if (choice == 5) {
                int size = getInput<int>("Enter size of other matrix: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                DiagonalMatrix<T> other(arr, size);
                matrix = matrix * other;
                cout << "Multiplied: " << matrix << endl;
                delete[] arr;
            } else if (choice == 6) {
                T scalar = getInput<T>("Enter scalar: ");
                matrix = matrix * scalar;
                cout << "Scaled: " << matrix << endl;
            } else if (choice == 7) {
                int size = getInput<int>("Enter size of other matrix: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                DiagonalMatrix<T> other(arr, size);
                cout << "Equal: " << (matrix == other ? "Yes" : "No") << endl;
                delete[] arr;
            } else if (choice == 8) {
                matrix = matrix.transpose();
                cout << "Transposed: " << matrix << endl;
            } else if (choice == 9) {
                cout << "Determinant: " << matrix.determinant() << endl;
            } else if (choice == 10) {
                cout << "Size: " << matrix.size() << endl;
            } else if (choice == 11) {
                cout << "Empty: " << (matrix.empty() ? "Yes" : "No") << endl;
            } else if (choice == 12) {
                matrix.clear();
                cout << "Matrix cleared.\n";
            } else if (choice == 13) {
                cout << "Norm: " << matrix.Norm() << endl;
            } else if (choice == 14) {
                cout << "Matrix:\n" << matrix << endl;
            } else if (choice == 15) {
                break;
            } else {
                cout << "Invalid option.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

template<typename T>
void triangularMatrixUI() {
    TriangularMatrix<T> matrix;
    bool isUpper = true;
    while (true) {
        cout << "\nTriangularMatrix Operations:\n";
        cout << "1. Initialize\n2. Get element\n3. Set element\n4. Add matrix\n5. Multiply matrix\n";
        cout << "6. Multiply by scalar\n7. Equals\n8. Transpose\n9. Determinant\n10. Size\n";
        cout << "11. Empty\n12. Clear\n13. Norm\n14. Display\n15. Toggle Upper/Lower\n16. Back to main menu\n";
        int choice = getInput<int>("Choose an option: ");

        try {
            if (choice == 1) {
                int size = getInput<int>("Enter size: ");
                T* arr = new T[(size * (size + 1)) / 2];
                getArrayInput(arr, (size * (size + 1)) / 2);
                matrix = TriangularMatrix<T>(arr, size, isUpper);
                delete[] arr;
            } else if (choice == 2) {
                int i = getInput<int>("Enter row: ");
                int j = getInput<int>("Enter column: ");
                cout << "Element (" << i << "," << j << "): " << matrix(i, j) << endl;
            } else if (choice == 3) {
                int i = getInput<int>("Enter row: ");
                int j = getInput<int>("Enter column: ");
                T value = getInput<T>("Enter value: ");
                matrix.set(i, j, value);
            } else if (choice == 4) {
                int size = getInput<int>("Enter size of other matrix: ");
                T* arr = new T[(size * (size + 1)) / 2];
                getArrayInput(arr, (size * (size + 1)) / 2);
                TriangularMatrix<T> other(arr, size, isUpper);
                matrix = matrix + other;
                cout << "Added: " << matrix << endl;
                delete[] arr;
            } else if (choice == 5) {
                int size = getInput<int>("Enter size of other matrix: ");
                T* arr = new T[(size * (size + 1)) / 2];
                getArrayInput(arr, (size * (size + 1)) / 2);
                TriangularMatrix<T> other(arr, size, isUpper);
                matrix = matrix * other;
                cout << "Multiplied: " << matrix << endl;
                delete[] arr;
            } else if (choice == 6) {
                T scalar = getInput<T>("Enter scalar: ");
                matrix = matrix * scalar;
                cout << "Scaled: " << matrix << endl;
            } else if (choice == 7) {
                int size = getInput<int>("Enter size of other matrix: ");
                T* arr = new T[(size * (size + 1)) / 2];
                getArrayInput(arr, (size * (size + 1)) / 2);
                TriangularMatrix<T> other(arr, size, isUpper);
                cout << "Equal: " << (matrix == other ? "Yes" : "No") << endl;
                delete[] arr;
            } else if (choice == 8) {
                TriangularMatrix<T>* transposed = matrix.transpose();
                cout << "Transposed: " << *transposed << endl;
                delete transposed;
            } else if (choice == 9) {
                cout << "Determinant: " << matrix.determinant() << endl;
            } else if (choice == 10) {
                cout << "Size: " << matrix.size() << endl;
            } else if (choice == 11) {
                cout << "Empty: " << (matrix.empty() ? "Yes" : "No") << endl;
            } else if (choice == 12) {
                matrix.clear();
                cout << "Matrix cleared.\n";
            } else if (choice == 13) {
                cout << "Norm: " << matrix.Norm() << endl;
            } else if (choice == 14) {
                cout << "Matrix:\n" << matrix << endl;
            } else if (choice == 15) {
                isUpper = !isUpper;
                cout << "Switched to " << (isUpper ? "Upper" : "Lower") << " triangular matrix.\n";
                matrix.clear();
            } else if (choice == 16) {
                break;
            } else {
                cout << "Invalid option.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

template<typename T>
void queueUI() {
    Queue<T> queue;
    while (true) {
        cout << "\nQueue Operations:\n";
        cout << "1. Enqueue\n2. Dequeue\n3. Front\n4. Back\n5. Size\n6. Empty\n7. Clear\n";
        cout << "8. Add queue\n9. Multiply queue\n";
        cout << "10. Concat\n11. Get SubQueue\n12. Contains\n13. Map\n14. Where\n";
        cout << "15. Split by predicate\n16. Display\n17. Back to main menu\n";
        int choice = getInput<int>("Choose an option: ");

        try {
            if (choice == 1) {
                T item = getInput<T>("Enter item: ");
                queue.enqueue(item);
            } else if (choice == 2) {
                cout << "Dequeued: " << queue.dequeue() << endl;
            } else if (choice == 3) {
                cout << "Front: " << queue.front() << endl;
            } else if (choice == 4) {
                cout << "Back: " << queue.back() << endl;
            } else if (choice == 5) {
                cout << "Size: " << queue.size() << endl;
            } else if (choice == 6) {
                cout << "Empty: " << (queue.empty() ? "Yes" : "No") << endl;
            } else if (choice == 7) {
                queue.clear();
                cout << "Queue cleared.\n";
            } else if (choice == 8) {
                int size = getInput<int>("Enter size of other queue: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Queue<T> other(arr, size);
                queue + other;
                cout << "Added: " << queue << endl;
                delete[] arr;
            } else if (choice == 9) {
                int size = getInput<int>("Enter size of other queue: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Queue<T> other(arr, size);
                queue * other;
                cout << "Multiplied: " << queue << endl;
                delete[] arr;
            } else if (choice == 10) {
                int size = getInput<int>("Enter size of other queue: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Queue<T> other(arr, size);
                queue.Concat(&other);
                cout << "Concatenated: " << queue << endl;
                delete[] arr;
            } else if (choice == 11) {
                int start = getInput<int>("Enter start index: ");
                int end = getInput<int>("Enter end index: ");
                auto* sub = queue.GetSubQueue(start, end);
                cout << "Subqueue: " << *sub << endl;
                delete sub;
            } else if (choice == 12) {
                int size = getInput<int>("Enter size of other queue: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Queue<T> other(arr, size);
                cout << "Contains: " << (queue.Contains(&other) ? "Yes" : "No") << endl;
                delete[] arr;
            } else if (choice == 13) {
                Queue<T>* mapped = queue.template Map<T>([](T x) { return static_cast<T>(x * 2); });
                cout << "Mapped: " << *mapped << endl;
                delete mapped;
            } else if (choice == 14) {
                Queue<T>* filtered = queue.Where([](T x) { return static_cast<T>(x) > 5 && static_cast<T>(x) < 10; });
                cout << "Filtered (evens): " << *filtered << endl;
                delete filtered;
            } else if (choice == 15) {
                auto [trueQueue, falseQueue] = queue.SplitByPredicate([](const T& x) { return static_cast<T>(x) > 5 && static_cast<T>(x) < 10; });
                cout << "True queue (evens): " << *trueQueue << endl;
                cout << "False queue (odds): " << *falseQueue << endl;
                delete trueQueue;
                delete falseQueue;
            } else if (choice == 16) {
                cout << "Queue: " << queue << endl;
            } else if (choice == 17) {
                break;
            } else {
                cout << "Invalid option.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

template<typename T>
void stackUI() {
    Stack<T> stack;
    while (true) {
        cout << "\nStack Operations:\n";
        cout << "1. Push\n2. Pop\n3. Top\n4. Size\n5. Empty\n6. Clear\\n";
        cout << "7. Add stack\n8. Multiply stack\n9. Concat\n10. Get SubStack\n";
        cout << "11. Contains\n12. Map\n13. Where\n14. Display\n15. Back to main menu\n";
        int choice = getInput<int>("Choose an option: ");

        try {
            if (choice == 1) {
                T item = getInput<T>("Enter item: ");
                stack.push(item);
            } else if (choice == 2) {
                cout << "Popped: " << stack.pop() << endl;
            } else if (choice == 3) {
                cout << "Top: " << stack.top() << endl;
            } else if (choice == 4) {
                cout << "Size: " << stack.size() << endl;
            } else if (choice == 5) {
                cout << "Empty: " << (stack.empty() ? "Yes" : "No") << endl;
            } else if (choice == 6) {
                stack.clear();
                cout << "Stack cleared.\n";
            } else if (choice == 7) {
                int size = getInput<int>("Enter size of other stack: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Stack<T> other(arr, size);
                stack + other;
                cout << "Added: " << stack << endl;
                delete[] arr;
            } else if (choice == 8) {
                int size = getInput<int>("Enter size of other stack: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Stack<T> other(arr, size);
                stack * other;
                cout << "Multiplied: " << stack << endl;
                delete[] arr;
            } else if (choice == 9) {
                int size = getInput<int>("Enter size of other stack: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Stack<T> other(arr, size);
                stack.Concat(other);
                cout << "Concatenated: " << stack << endl;
                delete[] arr;
            } else if (choice == 10) {
                int start = getInput<int>("Enter start index: ");
                int end = getInput<int>("Enter end index: ");
                auto* sub = stack.GetSubStack(start, end);
                cout << "Substack: " << *sub << endl;
                delete sub;
            } else if (choice == 11) {
                int size = getInput<int>("Enter size of other stack: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Stack<T> other(arr, size);
                cout << "Contains: " << (stack.Contains(other) ? "Yes" : "No") << endl;
                delete[] arr;
            } else if (choice == 12) {
                Stack<T>* mapped = stack.template Map<T>([](T x) { return static_cast<T>(x * 2); });
                cout << "Mapped: " << *mapped << endl;
                delete mapped;
            } else if (choice == 13) {
                Stack<T>* filtered = stack.Where([](T x) { return static_cast<T>(x) > 2 && static_cast<T>(x) < 10; });
                cout << "Filtered (evens): " << *filtered << endl;
                delete filtered;
            } else if (choice == 14) {
                cout << "Stack: " << stack << endl;
            } else if (choice == 15) {
                break;
            } else {
                cout << "Invalid option.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

template<typename T>
void dequeUI() {
    Deque<T> deque;
    while (true) {
        cout << "\nDeque Operations:\n";
        cout << "1. Push front\n2. Push back\n3. Pop front\n4. Pop back\n5. Front\n6. Back\n";
        cout << "7. Size\n8. Empty\n9. Clear\n10. Add deque\n";
        cout << "11. Multiply deque\n12. Concat\n13. Get SubDeque\n14. Contains\n15. Map\n";
        cout << "16. Where\n17. Sort\n18. Sum\n19. Product\n20. Display\n21. Back to main menu\n";
        int choice = getInput<int>("Choose an option: ");

        try {
            if (choice == 1) {
                T item = getInput<T>("Enter item: ");
                deque.push_front(item);
            } else if (choice == 2) {
                T item = getInput<T>("Enter item: ");
                deque.push_back(item);
            } else if (choice == 3) {
                cout << "Popped front: " << deque.pop_front() << endl;
            } else if (choice == 4) {
                cout << "Popped back: " << deque.pop_back() << endl;
            } else if (choice == 5) {
                cout << "Front: " << deque.front() << endl;
            } else if (choice == 6) {
                cout << "Back: " << deque.back() << endl;
            } else if (choice == 7) {
                cout << "Size: " << deque.size() << endl;
            } else if (choice == 8) {
                cout << "Empty: " << (deque.empty() ? "Yes" : "No") << endl;
            } else if (choice == 9) {
                deque.clear();
                cout << "Deque cleared.\n";
            } else if (choice == 10) {
                int size = getInput<int>("Enter size of other deque: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Deque<T> other(arr, size);
                deque + other;
                cout << "Added: " << deque << endl;
                delete[] arr;
            } else if (choice == 11) {
                int size = getInput<int>("Enter size of other deque: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Deque<T> other(arr, size);
                deque * other;
                cout << "Multiplied: " << deque << endl;
                delete[] arr;
            } else if (choice == 12) {
                int size = getInput<int>("Enter size of other deque: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Deque<T> other(arr, size);
                deque.Concat(other);
                cout << "Concatenated: " << deque << endl;
                delete[] arr;
            } else if (choice == 13) {
                int start = getInput<int>("Enter start index: ");
                int end = getInput<int>("Enter end index: ");
                auto* sub = deque.GetSubDeque(start, end);
                cout << "Subdeque: " << *sub << endl;
                delete sub;
            } else if (choice == 14) {
                int size = getInput<int>("Enter size of other deque: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Deque<T> other(arr, size);
                cout << "Contains: " << (deque.Contains(other) ? "Yes" : "No") << endl;
                delete[] arr;
            } else if (choice == 15) {
                Deque<T>* mapped = deque.template Map<T>([](T x) { return static_cast<T>(x * 2); });
                cout << "Mapped: " << *mapped << endl;
                delete mapped;
            } else if (choice == 16) {
                Deque<T>* filtered = deque.Where([](T x) { return static_cast<T>(x) > 2 && static_cast<T>(x) < 2; });
                cout << "Filtered (evens): " << *filtered << endl;
                delete filtered;
            } else if (choice == 17) {
                deque.Sort();
                cout << "Sorted: " << deque << endl;
            } else if (choice == 18) {
                cout << "Sum: " << deque.Sum() << endl;
            } else if (choice == 19) {
                cout << "Product: " << deque.Product() << endl;
            } else if (choice == 20) {
                cout << "Deque: " << deque << endl;
            } else if (choice == 21) {
                break;
            } else {
                cout << "Invalid option.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

template<typename T>
void rectangularMatrixUI() {
    RectangularMatrix<T> matrix;
    while (true) {
        cout << "\nRectangularMatrix Operations:\n";
        cout << "1. Initialize\n2. Get element\n3. Set element\n4. Add matrix\n5. Multiply matrix\n";
        cout << "6. Multiply by scalar\n7. Equals\n8. Transpose\n9. Get Rows\n10. Get Columns\n";
        cout << "11. Empty\n12. Clear\n13. Norm\n14. Display\n15. Back to main menu\n";
        int choice = getInput<int>("Choose an option: ");

        try {
            if (choice == 1) {
                int rows = getInput<int>("Enter rows: ");
                int cols = getInput<int>("Enter columns: ");
                T* arr = new T[rows * cols];
                getArrayInput(arr, rows * cols);
                matrix = RectangularMatrix<T>(arr, rows, cols);
                delete[] arr;
            } else if (choice == 2) {
                int i = getInput<int>("Enter row: ");
                int j = getInput<int>("Enter column: ");
                cout << "Element (" << i << "," << j << "): " << matrix(i, j) << endl;
            } else if (choice == 3) {
                int i = getInput<int>("Enter row: ");
                int j = getInput<int>("Enter column: ");
                T value = getInput<T>("Enter value: ");
                matrix.set(i, j, value);
            } else if (choice == 4) {
                int rows = getInput<int>("Enter rows of other matrix: ");
                int cols = getInput<int>("Enter columns of other matrix: ");
                T* arr = new T[rows * cols];
                getArrayInput(arr, rows * cols);
                RectangularMatrix<T> other(arr, rows, cols);
                matrix = matrix + other;
                cout << "Added: " << matrix << endl;
                delete[] arr;
            } else if (choice == 5) {
                int rows = getInput<int>("Enter rows of other matrix: ");
                int cols = getInput<int>("Enter columns of other matrix: ");
                T* arr = new T[rows * cols];
                getArrayInput(arr, rows * cols);
                RectangularMatrix<T> other(arr, rows, cols);
                matrix = matrix * other;
                cout << "Multiplied: " << matrix << endl;
                delete[] arr;
            } else if (choice == 6) {
                T scalar = getInput<T>("Enter scalar: ");
                matrix = matrix * scalar;
                cout << "Scaled: " << matrix << endl;
            } else if (choice == 7) {
                int rows = getInput<int>("Enter rows of other matrix: ");
                int cols = getInput<int>("Enter columns of other matrix: ");
                T* arr = new T[rows * cols];
                getArrayInput(arr, rows * cols);
                RectangularMatrix<T> other(arr, rows, cols);
                cout << "Equal: " << (matrix == other ? "Yes" : "No") << endl;
                delete[] arr;
            } else if (choice == 8) {
                matrix = matrix.transpose();
                cout << "Transposed: " << matrix << endl;
            } else if (choice == 9) {
                cout << "Rows: " << matrix.getRows() << endl;
            } else if (choice == 10) {
                cout << "Columns: " << matrix.getCols() << endl;
            } else if (choice == 11) {
                cout << "Empty: " << (matrix.empty() ? "Yes" : "No") << endl;
            } else if (choice == 12) {
                matrix.clear();
                cout << "Matrix cleared.\n";
            } else if (choice == 13) {
                cout << "Norm: " << matrix.Norm() << endl;
            } else if (choice == 14) {
                cout << "Matrix:\n" << matrix << endl;
            } else if (choice == 15) {
                break;
            } else {
                cout << "Invalid option.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

template<typename T>
void vectorUI() {
    Vector<T> vector;
    while (true) {
        cout << "\nVector Operations:\n";
        cout << "1. Initialize\n2. Get element\n3. Set element\n4. Add vector\n5. Subtract vector\n";
        cout << "6. Multiply by scalar\n7. Dot product\n8. Norm\n9. Normalize\n10. Is Collinear\n";
        cout << "11. Size\n12. Empty\n13. Clear\n14. Map\n15. Where\n16. Display\n17. Back to main menu\n";
        int choice = getInput<int>("Choose an option: ");

        try {
            if (choice == 1) {
                int size = getInput<int>("Enter size: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                vector = Vector<T>(arr, size);
                delete[] arr;
            } else if (choice == 2) {
                int index = getInput<int>("Enter index: ");
                cout << "Element at " << index << ": " << vector.Get(index) << endl;
            } else if (choice == 3) {
                int index = getInput<int>("Enter index: ");
                T value = getInput<T>("Enter value: ");
                vector.Set(index, value);
            } else if (choice == 4) {
                int size = getInput<int>("Enter size of other vector: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Vector<T> other(arr, size);
                vector = vector + other;
                cout << "Added: " << vector << endl;
                delete[] arr;
            } else if (choice == 5) {
                int size = getInput<int>("Enter size of other vector: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Vector<T> other(arr, size);
                vector = vector - other;
                cout << "Subtracted: " << vector << endl;
                delete[] arr;
            } else if (choice == 6) {
                T scalar = getInput<T>("Enter scalar: ");
                vector = vector * scalar;
                cout << "Scaled: " << vector << endl;
            } else if (choice == 7) {
                int size = getInput<int>("Enter size of other vector: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Vector<T> other(arr, size);
                cout << "Dot product: " << (vector * other) << endl;
                delete[] arr;
            } else if (choice == 8) {
                cout << "Norm: " << vector.norm() << endl;
            } else if (choice == 9) {
                Vector<double> normalized = vector.normalize();
                cout << "Normalized: " << normalized << endl;
            } else if (choice == 10) {
                int size = getInput<int>("Enter size of other vector: ");
                T* arr = new T[size];
                getArrayInput(arr, size);
                Vector<T> other(arr, size);
                cout << "Is Collinear: " << (vector.isCollinear(other) ? "Yes" : "No") << endl;
                delete[] arr;
            } else if (choice == 11) {
                cout << "Size: " << vector.size() << endl;
            } else if (choice == 12) {
                cout << "Empty: " << (vector.empty() ? "Yes" : "No") << endl;
            } else if (choice == 13) {
                vector.clear();
                cout << "Vector cleared.\n";
            } else if (choice == 14) {
                Vector<T>* mapped = vector.template Map<T>([](T x) { return static_cast<T>(x * 2); });
                cout << "Mapped: " << *mapped << endl;
                delete mapped;
            } else if (choice == 15) {
                Vector<T>* filtered = vector.Where([](T x) { return static_cast<T>(x) > 2 && static_cast<T>(x) < 10; });
                cout << "Filtered (evens): " << *filtered << endl;
                delete filtered;
            } else if (choice == 16) {
                cout << "Vector: " << vector << endl;
            } else if (choice == 17) {
                break;
            } else {
                cout << "Invalid option.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

template<typename T>
void squareMatrixUI() {
    SquareMatrix<T> matrix;
    while (true) {
        cout << "\nSquareMatrix Operations:\n";
        cout << "1. Initialize\n2. Get element\n3. Set element\n4. Add matrix\n5. Multiply matrix\n";
        cout << "6. Multiply by scalar\n7. Equals\n8. Transpose\n9. LU Decomposition\n10. Determinant\n";
        cout << "11. Size\n12. Empty\n13. Clear\n14. Norm\n15. Display\n16. Back to main menu\n";
        int choice = getInput<int>("Choose an option: ");

        try {
            if (choice == 1) {
                int size = getInput<int>("Enter size: ");
                T* arr = new T[size * size];
                getArrayInput(arr, size * size);
                matrix = SquareMatrix<T>(arr, size);
                delete[] arr;
            } else if (choice == 2) {
                int i = getInput<int>("Enter row: ");
                int j = getInput<int>("Enter column: ");
                cout << "Element (" << i << "," << j << "): " << matrix(i, j) << endl;
            } else if (choice == 3) {
                int i = getInput<int>("Enter row: ");
                int j = getInput<int>("Enter column: ");
                T value = getInput<T>("Enter value: ");
                matrix.set(value, i, j);
            } else if (choice == 4) {
                int size = getInput<int>("Enter size of other matrix: ");
                T* arr = new T[size * size];
                getArrayInput(arr, size * size);
                SquareMatrix<T> other(arr, size);
                SquareMatrix<T> result = matrix + other;
                cout << "Added: " << matrix << endl;
                delete[] arr;
            } else if (choice == 5) {
                int size = getInput<int>("Enter size of other matrix: ");
                T* arr = new T[size * size];
                getArrayInput(arr, size * size);
                SquareMatrix<T> other(arr, size);
                matrix * other;
                cout << "Multiplied: " << matrix << endl;
                delete[] arr;
            } else if (choice == 6) {
                T scalar = getInput<T>("Enter scalar: ");
                matrix * scalar;
                cout << "Scaled: " << matrix << endl;
            } else if (choice == 7) {
                int size = getInput<int>("Enter size of other matrix: ");
                T* arr = new T[size * size];
                getArrayInput(arr, size * size);
                SquareMatrix<T> other(arr, size);
                cout << "Equal: " << (matrix == other ? "Yes" : "No") << endl;
                delete[] arr;
            } else if (choice == 8) {
                auto* transposed = matrix.Transpose();
                cout << "Transposed:\n" << *transposed << endl;
                delete transposed;
            } else if (choice == 9) {
                auto [L, U] = matrix.LUDecomposition();
                cout << "L Matrix:\n" << L << endl;
                cout << "U Matrix:\n" << U << endl;
            } else if (choice == 10) {
                cout << "Determinant: " << matrix.determinant() << endl;
            } else if (choice == 11) {
                cout << "Size: " << matrix.size() << endl;
            } else if (choice == 12) {
                cout << "Empty: " << (matrix.empty() ? "Yes" : "No") << endl;
            } else if (choice == 13) {
                matrix.clear();
                cout << "Matrix cleared.\n";
            } else if (choice == 14) {
                cout << "Norm: " << matrix.Norm() << endl;
            } else if (choice == 15) {
                cout << "Matrix:\n" << matrix << endl;
            } else if (choice == 16) {
                break;
            } else {
                cout << "Invalid option.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

template<typename T>
void priorityQueueUI() {
    PriorityQueue<T> pq;
    while (true) {
        cout << "\nPriorityQueue Operations:\n";
        cout << "1. Enqueue\n2. Dequeue\n3. Peek\n4. Size\n5. Empty\n6. Clear\n";
        cout << "7. Get element\n8. Set element\n9. Add queue\n10. Multiply queue\n";
        cout << "11. Concat\n12. Get Subsequence\n13. Find Subsequence\n14. Split by predicate\n";
        cout << "15. Map\n16. Where\n17. Display\n18. Back to main menu\n";
        int choice = getInput<int>("Choose an option: ");

        try {
            if (choice == 1) {
                T item = getInput<T>("Enter item: ");
                int priority = getInput<int>("Enter priority: ");
                pq.enqueue(item, priority);
            } else if (choice == 2) {
                cout << "Dequeued: " << pq.dequeue() << endl;
            } else if (choice == 3) {
                cout << "Peek: " << pq.peek() << endl;
            } else if (choice == 4) {
                cout << "Size: " << pq.size() << endl;
            } else if (choice == 5) {
                cout << "Empty: " << (pq.empty() ? "Yes" : "No") << endl;
            } else if (choice == 6) {
                pq.clear();
                cout << "Priority queue cleared.\n";
            } else if (choice == 7) {
                int index = getInput<int>("Enter index: ");
                cout << "Element at " << index << ": " << pq[index] << endl;
            } else if (choice == 8) {
                cout << "Note: Setting element directly is not supported as it breaks heap property.\n";
            } else if (choice == 9) {
                int size = getInput<int>("Enter size of other queue: ");
                PriorityQueue<T> other;
                cout << "Enter " << size << " items and their priorities (must match current queue priorities):\n";
                for (int i = 0; i < size; ++i) {
                    T item = getInput<T>("Enter item " + to_string(i) + ": ");
                    int priority = getInput<int>("Enter priority for item " + to_string(i) + ": ");
                    other.enqueue(item, priority);
                }
                if (pq.size() == other.size()) {
                    pq = pq + other;
                    cout << "Added: " << pq << endl;
                } else {
                    cout << "Error: Queues must have the same size for addition.\n";
                }
            } else if (choice == 10) {
                int size = getInput<int>("Enter size of other queue: ");
                PriorityQueue<T> other;
                cout << "Enter " << size << " items and their priorities (must match current queue priorities):\n";
                for (int i = 0; i < size; ++i) {
                    T item = getInput<T>("Enter item " + to_string(i) + ": ");
                    int priority = getInput<int>("Enter priority for item " + to_string(i) + ": ");
                    other.enqueue(item, priority);
                }
                if (pq.size() == other.size()) {
                    pq = pq * other;
                    cout << "Multiplied: " << pq << endl;
                } else {
                    cout << "Error: Queues must have the same size for multiplication.\n";
                }
            } else if (choice == 11) {
                int size = getInput<int>("Enter size of other queue: ");
                PriorityQueue<T> other;
                for (int i = 0; i < size; ++i) {
                    T item = getInput<T>("Enter item " + to_string(i) + ": ");
                    int priority = getInput<int>("Enter priority for item " + to_string(i) + ": ");
                    other.enqueue(item, priority);
                }
                pq.Concat(other);
                cout << "Concatenated: " << pq << endl;
            } else if (choice == 12) {
                int start = getInput<int>("Enter start index: ");
                int end = getInput<int>("Enter end index: ");
                auto* sub = pq.GetSubsequence(start, end);
                cout << "Subsequence: " << *sub << endl;
                delete sub;
            } else if (choice == 13) {
                int size = getInput<int>("Enter size of subsequence: ");
                PriorityQueue<T> other;
                for (int i = 0; i < size; ++i) {
                    T item = getInput<T>("Enter item " + to_string(i) + ": ");
                    int priority = getInput<int>("Enter priority for item " + to_string(i) + ": ");
                    other.enqueue(item, priority);
                }
                cout << "Contains subsequence: " << (pq.FindSubsequence(other) ? "Yes" : "No") << endl;
            } else if (choice == 14) {
                auto [trueQueue, falseQueue] = pq.SplitByPredicate([](const T& x) { return static_cast<int>(x) % 2 == 0; });
                cout << "True queue (evens): " << *trueQueue << endl;
                cout << "False queue (odds): " << *falseQueue << endl;
                delete trueQueue;
                delete falseQueue;
            } else if (choice == 15) {
                PriorityQueue<T>* mapped = pq.template Map<T>([](T x) { return static_cast<T>(x * 2); });
                cout << "Mapped: " << *mapped << endl;
                delete mapped;
            } else if (choice == 16) {
                PriorityQueue<T>* filtered = pq.Where([](T x) { return static_cast<T>(x) > 2 && static_cast<T>(x) < 10; });
                cout << "Filtered (evens): " << *filtered << endl;
                delete filtered;
            } else if (choice == 17) {
                cout << "Priority Queue: " << pq << endl;
            } else if (choice == 18) {
                break;
            } else {
                cout << "Invalid option.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

int main() {
    cout << "Select data type:\n1. Integer\n2. Double\n";
    int typeChoice = getInput<int>("Choose a type (1 or 2): ");
    
    while (true) {
        cout << "\nData Structure Menu:\n";
        cout << "1. SegmentedDeque\n2. DiagonalMatrix\n3. TriangularMatrix\n4. Queue\n5. Stack\n";
        cout << "6. Deque\n7. RectangularMatrix\n8. Vector\n9. SquareMatrix\n10. PriorityQueue\n11. Exit\n";
        int choice = getInput<int>("Choose a data structure: ");

        if (typeChoice == 1) {
            if (choice == 1) {
                segmentedDequeUI<int>();
            } else if (choice == 2) {
                diagonalMatrixUI<int>();
            } else if (choice == 3) {
                triangularMatrixUI<int>();
            } else if (choice == 4) {
                queueUI<int>();
            } else if (choice == 5) {
                stackUI<int>();
            } else if (choice == 6) {
                dequeUI<int>();
            } else if (choice == 7) {
                rectangularMatrixUI<int>();
            } else if (choice == 8) {
                vectorUI<int>();
            } else if (choice == 9) {
                squareMatrixUI<int>();
            } else if (choice == 10) {
                priorityQueueUI<int>();
            } else if (choice == 11) {
                cout << "Exiting...\n";
                break;
            } else {
                cout << "Invalid option.\n";
            }
        }
        else if (typeChoice == 2) {
            if (choice == 1) {
                segmentedDequeUI<double>();
            } else if (choice == 2) {
                diagonalMatrixUI<double>();
            } else if (choice == 3) {
                triangularMatrixUI<double>();
            } else if (choice == 4) {
                queueUI<double>();
            } else if (choice == 5) {
                stackUI<double>();
            } else if (choice == 6) {
                dequeUI<double>();
            } else if (choice == 7) {
                rectangularMatrixUI<double>();
            } else if (choice == 8) {
                vectorUI<double>();
            } else if (choice == 9) {
                squareMatrixUI<double>();
            } else if (choice == 10) {
                priorityQueueUI<double>();
            } else if (choice == 11) {
                cout << "Exiting...\n";
                break;
            } else {
                cout << "Invalid option.\n";
            }
        } 
        else {
            cout << "Invalid type choice. Defaulting to Integer.\n";
            typeChoice = 1;
            continue;
        }
    }
    return 0;
}