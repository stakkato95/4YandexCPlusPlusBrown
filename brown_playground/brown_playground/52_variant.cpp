//
//  52_variant.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/13/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <variant>

using namespace std;

enum class FailReason {
    NEGATIVE_VALUE,
    TOO_BIG_VALUE,
    ZERO_VALUE
};

ostream& operator<<(ostream& os, const FailReason& reason) {
    os << static_cast<int>(reason);
    return os;
}

//function returns either calculates taxes sum
//or returns fail reason
variant<int, FailReason> calculateTaxes(int amount) {
    if (amount < 1) {
        return FailReason::NEGATIVE_VALUE;
    }
    if (amount == 0) {
        return FailReason::ZERO_VALUE;
    }
    if (amount > 100500) {
        return FailReason::TOO_BIG_VALUE;
    }
    
    return amount * 0.3;
}

variant<monostate, int, FailReason> calculateTaxesDifferent(int amount) {
    if (amount < 1) {
        return FailReason::NEGATIVE_VALUE;
    }
    if (amount == 0) {
        return FailReason::ZERO_VALUE;
    }
    if (amount > 100500) {
        return FailReason::TOO_BIG_VALUE;
    }
    if (amount == 100) {
        return monostate();
    }
    
    return amount * 0.3;
}

void showVariant() {
    //better use 'auto'
//  if (const auto                     result = calculateTaxes(100); holds_alternative<int>(result))
    if (const variant<int, FailReason> result = calculateTaxes(100); holds_alternative<int>(result)) {
        cout << "calculated amount of taxes " << get<int>(result) << endl;
    } else {
        cout << "fail reason " << get<FailReason>(result) << endl;
    }
}

void showMonostate() {
    if (variant<monostate, int, FailReason> result = calculateTaxesDifferent(100); holds_alternative<int>(result)) {
        cout << "calculated amount of taxes " << get<int>(result) << endl;
    } else if (holds_alternative<FailReason>(result)) {
        cout << "fail reason " << get<FailReason>(result) << endl;
    } else if (holds_alternative<monostate>(result)) {
        cout << "returned monostate" << endl;
    }
}

int main52() {
    //showVariant();
    showMonostate();
    
    return 0;
}
