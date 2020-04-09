//
//  34_thread_safe_shared_ptr.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/9/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <memory>
#include <string>

using namespace std;

struct Data {
    string data;
    
    Data(string s) : data { s } { }
};

int main34() {
    //shared_ptr is by default thread safe (incremeanting reference count)
    //shared_ptr can have a pointer to a const object. good for concurrency
    
    shared_ptr<const Data> ptr = make_shared<const Data>("const string");
    cout << ptr->data << endl;
    //ptr->data.push_back('a');//compile error
    
    return 0;
}
