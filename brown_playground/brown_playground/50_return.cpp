//
//  50_return.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/12/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct VeryHeavyStruct {
    string str1;
    string str2;
    string str3;
    vector<double> vec1;
    vector<double> vec2;
};

using HeavyStructPtr = unique_ptr<VeryHeavyStruct>;

HeavyStructPtr doSomeWork() {
    //if there are a lot of data on stack (i.e. struct is very heavy)
    //'copy elision' will not work, so use unique_ptr to avoid copying these big objects
    
    HeavyStructPtr ptr = unique_ptr<VeryHeavyStruct>();
    //...
    return ptr;
}

/////////////////////////////////////////////////////////

struct ComputationResult {
    vector<int> updatedItems;
    vector<int> oldItems;
};

ComputationResult compute() {
    vector<int> updatedItems;
    vector<int> oldItems;
    //...
    //'move()' should be explicitly called
    //because NRVO (named return value optiomisation) doesn't work, when multiple values are returned
    return { move(updatedItems), move(oldItems) };
}

int main() {
    HeavyStructPtr ptr = doSomeWork();
    
    auto [updatedItems, oldItems] = compute();
    
    return 0;
}
