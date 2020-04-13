//
//  51_optional.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/13/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <optional>
#include <vector>
#include <limits>

using namespace std;

struct Date {
    int year;
    int month;
    int day;
};

enum class DateValidation {
    NEGATIVE_YEAR,
    NEGATIVE_MONTH,
    NEGATIVE_DAY,
    TOO_BIG
};

ostream& operator<<(ostream& os, const DateValidation& validation) {
    os << static_cast<int>(validation);
    return os;
}

//function checks date.
//if there is a problem, optional will contain reason
//if there is no problem, optional will be empty
optional<DateValidation> validateDate(const Date& date) {
    //DateValidation values are automatically cast to optinal
    //in modern compilers copy elision works perfectly even for heavy objects, so no worries
    
    if (date.day < 0) {
        return DateValidation::NEGATIVE_DAY;
    }
    if (date.month < 0) {
        return DateValidation::NEGATIVE_DAY;
    }
    if (date.year < 0) {
        return DateValidation::NEGATIVE_DAY;
    }
    
    return nullopt;
}

void printVector(const vector<int>& v, optional<size_t> firstElementsCount = nullopt) {
    size_t sizeBound = min(v.size(), firstElementsCount.value_or(numeric_limits<size_t>::min()));
    for (size_t i = 0; i < sizeBound; ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
}

void showOptionalReturn() {
    Date problematicDate { -1, -1, -1 };
    optional<DateValidation> validationResult = validateDate(problematicDate);
    
    cout << boolalpha;
    cout << "problem with date = " << validationResult.has_value() << endl;
    
    //optional is automatically cast to bool
    if (validationResult) {
        cout << "Error code = " << validationResult.value() << endl;
        cout << "Error code = " << *validationResult << endl;
    }
    
    if (const auto result = validateDate(problematicDate)) {
        cout << "Validation in short form, error code = " << *result << endl;
    }
}

void showOptionalParam() {
    printVector({ 1, 2, 3, 4, 5, 6 }, 3);
}

int main() {
    //showOptionalReturn();
    showOptionalParam();
    
    return 0;
}
