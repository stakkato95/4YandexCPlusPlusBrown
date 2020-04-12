#include <iostream>

#include <array>
#include <vector>
#include <valarray>

#include <string>
#include <span>

using namespace std;

template<typename T>
void printContainer(span<T> container, const string& title) {
    cout << title << endl;
    for (const T& item : container) {
        cout << item << " ";
    }
    cout << endl;
}

template<typename T>
void multiply(span<T> container, const string& title) {
    for (T& item : container) {
        item *= 2;
    }
}

void showHowSpanWorks() {
    int cArray[] { 1, 2, 3, 4 };
    printContainer<int>(cArray, "c array");

    vector<int> v { 5, 6, 7, 8 };
    printContainer<int>(v, "vector");
    printContainer<int>(span(v).first(2), "vector short span");

    array stackArray { 9, 10, 11, 12 };
    printContainer<int>(stackArray, "stack array");
    printContainer<int>(span(stackArray).subspan(1, 2), "stack array subspan");
    cout << endl;

    valarray<int> val1 { 1, 2 };
    valarray<int> val2 { 3, 4 };
    valarray<int> val3 = val1 * val2;
    //apply span to a not commonly used container
    //span corresponds to a vector like string_view to a string
    //span can replace iterators
    printContainer(span<int>(&val3[0], val3.size()), "valarray");
}

void modifyContainerViaSpan() {
    vector<double> d { 1.5, 2.6 };
    multiply<double>(d, "multiplying vector...");
    printContainer<double>(d, "multiplied vector");
}

int main() {
    //showHowSpanWorks();
    //modifyContainerViaSpan();

    //two main applications for spans
    //1 interfacing with C code - pass span instead of pointer to 1st element and N-number of elements
    //2 work with spans of memory of serialized objects

    return 0;
}
