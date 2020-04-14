//
//  53_variant_vs_exception.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/14/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <variant>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

enum class Fail {
    BAD,
    WRONG,
    INCONSISTENT,
    MAX_FAIL
};

variant<int, Fail> calculateTaxesVariant(int cost) {
    if (cost % 10) {
        return Fail::BAD;
    }
    return cost * 2;
}

int calculateTaxesException(int cost) {
    if (cost % 10) {
        throw Fail::BAD;
    }
    return cost * 2;
}

int main53() {
    //rule of thumb
    //use exceptions in 'very exceptional' cases and in places
    //where performance isn't so important
    
    int iterationCount = 1'000'000;
    int defaultSize = static_cast<int>(Fail::MAX_FAIL) - 1;
    
    {
        vector<int> v(defaultSize);
        auto start = system_clock::now();
        for (int i = 0; i < iterationCount; ++i) {
            if (auto const result = calculateTaxesVariant(i); holds_alternative<int>(result)) {
                //equivalent to
                //v[v.size() - 1] += get<int>(result);
                v.back() += get<int>(result);
            } else {
                v[static_cast<size_t>(get<Fail>(result))]++;
            }
        }
        //85 millisec on average
        cout << duration_cast<milliseconds>(system_clock::now() - start).count() << endl;
    }
    {
        vector<int> v(defaultSize);
        auto start = system_clock::now();
        for (int i = 0; i < iterationCount; ++i) {
            try {
                auto const result = calculateTaxesException(i);
                v.back() += result;
            } catch (const Fail& fail) {
                v[static_cast<size_t>(fail)]++;
            }
        }
        //930 millisec on average
        cout << duration_cast<milliseconds>(system_clock::now() - start).count() << endl;
    }
    
    
    return 0;
}
