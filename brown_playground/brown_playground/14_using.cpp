//
//  14_using.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/2/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

using namespace std;

int main14() {
    //rule of thumb - minimize scope of 'using'
    //apply it not to the whole file, but to functions, blocks of code etc.
    //the smaller scope, the better it is
    
    //don't use 'using namespace in header files'
    //because we don't know in advance in which files the header will be included
    //and what conflicts it can cause
    
    return 0;
}
