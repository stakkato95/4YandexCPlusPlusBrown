//
//  36_non_owning_shared_ptr.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/9/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <string>
#include <memory>

using namespace std;

struct Widget {
    string value;
};

Widget* getNonOwningPtr() {
    //get widget from C code
    return new Widget;
}

shared_ptr<Widget> getOwningPtr() {
    //get widget from C++ code
    return make_shared<Widget>();
}

shared_ptr<Widget> getPtr(bool isOwning) {
    if (isOwning) {
        return getOwningPtr();
    }
    
    auto dummyDeleter = [](Widget*) { };
    return shared_ptr<Widget>(getNonOwningPtr(), dummyDeleter);
}

int main36() {
    //non woning shared_ptr should be used when interfacing with C code
    shared_ptr<Widget> ptr = getPtr(false);
    
    return 0;
}
