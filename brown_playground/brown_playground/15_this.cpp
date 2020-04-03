//
//  15_this.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/3/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <utility>

using namespace std;

class boxedInt {
public:
    boxedInt(int v) {
        val = new int(v);
    }
    
    boxedInt() {
        delete val;
    }
    
    boxedInt(const boxedInt& other) {
        cout << "ctor copy" << endl;
        val = new int(*other.val);
    }
    
    boxedInt(boxedInt&& other) {
        cout << "ctor move" << endl;
        val = other.val;
        other.val = nullptr;
    }
    
    boxedInt& operator=(const boxedInt& other) {
        cout << "assign copy" << endl;
        
        //&other return address of other in memory
        if (&other != this) {
            boxedInt temp(other);
            *val = *temp.val;
        }
        
        return *this;
    }
    
    boxedInt& operator=(boxedInt&& rhs) {
        cout << "assign move" << endl;
        
        if (&rhs != this) {
            val = rhs.val;
            rhs.val = nullptr;
        }
        
        return *this;
    }
    
    int getVal() {
        if (val == nullptr) {
            return -1;
        }
        return *val;
    }
private:
    int* val;
};

int main() {
    boxedInt i = boxedInt(3);
    boxedInt j = boxedInt(5);
    
    cout << "assign copy" << endl;
    j = i;
    cout << "i = " << i.getVal() << ", " << "j = " << j.getVal() << endl;
    
    cout << endl << "assign move" << endl;
    j = move(i);
    cout << "i = " << i.getVal() << ", " << "j = " << j.getVal() << endl;
    
    cout << endl << "ctor copy" << endl;
    boxedInt kk = boxedInt(6);
    boxedInt k = kk;
    cout << "k = " << k.getVal() << ", " << "kk = " << kk.getVal() << endl;
    
    cout << endl << "ctor move" << endl;
    boxedInt n = boxedInt(6);
    boxedInt m = move(n);
    cout << "m = " << m.getVal() << ", " << "n = " << n.getVal() << endl;
    
    return 0;
}
