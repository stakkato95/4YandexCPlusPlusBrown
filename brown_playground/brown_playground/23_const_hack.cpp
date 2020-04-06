//
//  23_const_hack.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/5/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <string>

#include <utility>
#include <cassert>

using namespace std;

int main23() {
    string str = "hello world";
    //ref to a const string
    const string& strRef = as_const(str);
    
    cout << strRef << endl;
    
    return 0;
}
