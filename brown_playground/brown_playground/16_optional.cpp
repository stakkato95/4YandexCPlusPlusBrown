//
//  16_double_dispatching.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/3/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <optional>
#include <string>

using namespace std;

optional<string> getSomeString(bool isInitialized = true) {
    if (isInitialized) {
        return "hello world";
    }
    return nullopt;
}

int main16() {
    cout << "\"" << *getSomeString() << "\"" << endl;
    cout << "\"" << *getSomeString(false) << "\"" << endl;
    
    optional<string> str;
    if (!str) {
        cout << "no value" << endl;
    }
    str = "hello world";
    if (str) {
        cout << "value exists" << endl;
    }
    
    return 0;
}
