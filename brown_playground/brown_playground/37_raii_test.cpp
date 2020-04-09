
//
//  37_test.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/9/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class File {
public:
    File() {
        //if constructor throws an exception, its destructor is not called
        val = new int;
        throw runtime_error("er");
    }
    
    ~File() {
        //destructor should be finished w/o exceptions
        delete val;
        cout << "deleted" << endl;
    }
private:
    int* val;
};


int main() {
    try {
        File();
    } catch (const runtime_error& e) {
        cout << "error called" << endl;
    }
    
    return 0;
}
