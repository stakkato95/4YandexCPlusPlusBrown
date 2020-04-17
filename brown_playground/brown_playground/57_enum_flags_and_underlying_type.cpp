//
//  57_enum_flags_and_underlying_type.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/16/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

using namespace std;

enum class BitFlag {
    VISIBLE   = 1 << 0, //001
    INVISIBLE = 1 << 1, //010
    GONE      = 1 << 2  //100
};

BitFlag operator|(BitFlag l, BitFlag r) {
    return static_cast<BitFlag>(static_cast<int>(l) | static_cast<int>(r));
}

void operator|=(BitFlag& l, BitFlag r) {
    l = r | l;
}

BitFlag operator&(BitFlag l, BitFlag r) {
    return static_cast<BitFlag>(static_cast<int>(l) & static_cast<int>(r));
}

ostream& operator<<(ostream& os, const BitFlag& flag) {
    bool anyValue = false;
    
    if ((flag & BitFlag::VISIBLE) == BitFlag::VISIBLE) {
        anyValue = true;
        os << "VISIBLE";
    }
    if ((flag & BitFlag::INVISIBLE) == BitFlag::INVISIBLE) {
        anyValue = true;
        os << (anyValue ? " | " : "") << "INVISIBLE";
    }
    if ((flag & BitFlag::GONE) == BitFlag::GONE) {
        anyValue = true;
        os << (anyValue ? " | " : "") << "GONE";
    }
    
    return os;
}

int main57() {
    BitFlag flag = BitFlag::GONE;
    flag |= BitFlag::VISIBLE;
    cout << flag << endl;
    
    if ((flag & BitFlag::GONE) == BitFlag::GONE) {
        cout << "GONE is set" << endl;
    }
    if ((flag & BitFlag::INVISIBLE) != BitFlag::INVISIBLE) {
        cout << "INVISIBLE is not set" << endl;
    }
    
    //check underlying type of 'BitFlag'
    bool same = is_same<int, typename underlying_type<BitFlag>::type>::value;
    cout << boolalpha;
    cout << "BitFlag is same as int = " << same << endl;
    
    return 0;
}
