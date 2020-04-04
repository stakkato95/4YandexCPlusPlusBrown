//
//  17_smth.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/4/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <vector>

#include <algorithm>

#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

vector<int> my_sorted(vector<int> v) {
    sort(begin(v), end(v));
    return v;
}

vector<int> my_unique(vector<int> v) {
    vector<int>::iterator it = unique(begin(v), end(v));
    v.erase(it, end(v));
    return v;
}

void printVector(const vector<int>& v) {
    for (const int i : v) {
        cout << i << " ";
    }
    cout << endl;
}

void problemStatement() {
    //1 we want 'v' to stay const
    //2 we wouldn't like 'my_unique' to be a separate function, since it's used only in this file and nowhere else
    const vector<int> v = my_unique(my_sorted({ 5, 4, 2, 1, 5, 1, 3, 4, 5, 6, 8 }));
    
    //some code that ptentially could change 'v'
    
    //at the end of the function we want 'v' to be as it was defined at the beginning
    //'const vector<int>' helps to achieve it
    printVector(v);
}

void problemSolution() {
    //use IILE - Immediatelly Invoked Lambda Expression
    
    //now we don't need 'my_unique' as a separate function
    //our namespace stays clean, without dozens of functions, which are used only once
    const vector<int> v = [] {
        vector<int> sorted = my_sorted({ 5, 4, 2, 1, 5, 1, 3, 4, 5, 6, 8 });
        vector<int>::iterator it = unique(begin(sorted), end(sorted));
        sorted.erase(it, end(sorted));
        return sorted;
    }();
    
    //some code that ptentially could change 'v'
    
    printVector(v);
}

void objectCreationTime() {
    //IILE is very useful to measure objection creation time
    vector<int> v = [] {
        auto start = system_clock::now();
        vector<int> sorted = my_sorted({ 5, 4, 2, 1, 5, 1, 3, 4, 5, 6, 8 });
        cout << duration_cast<milliseconds>(system_clock::now() - start).count() << " millisec" << endl;
        return sorted;
    }();
    
    printVector(v);
}

int main17() {
//    problemStatement();
//    problemSolution();
    objectCreationTime();
    
    return 0;
}
