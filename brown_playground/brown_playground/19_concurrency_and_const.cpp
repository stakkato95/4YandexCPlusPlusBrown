//
//  19_concurrency_and_const.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/4/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

int main19() {
    //any thread cann accress 'v' and 'v' is thread safe. It's guaranteed by the language itself.
    const vector<int> v;
    
    //if you use a variable with multiple threads, check its implementation
    //to make sure that it's thread safe / unsafe
    
    return 0;
}
