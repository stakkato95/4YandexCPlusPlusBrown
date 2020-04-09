//
//  24_hashing_integers.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/6/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

struct PhoneNumber : public string {
    PhoneNumber(const string& s) : string(s) { }
};

bool operator==(const PhoneNumber& l, const PhoneNumber& r) {
    return l.compare(r) == 0;
}

template<int PhoneNumberLength = 19>
struct PhoneNumberHasher {
    size_t p;
    size_t a;
    size_t b;
    
    PhoneNumberHasher() {
        default_random_engine rnd;
        uniform_int_distribution<> aDistribution { 1, PhoneNumberLength - 1 };
        uniform_int_distribution<> bDistribution { 0, PhoneNumberLength - 1 };
        uniform_int_distribution<> pDistribution { 0, 1000 };
        
        p = pDistribution(rnd);
        a = aDistribution(rnd);
        b = bDistribution(rnd);
    }
    
    size_t operator()(const PhoneNumber& number) const {
        string numberStr = static_cast<string>(number);
        string::iterator it = remove(begin(numberStr), end(numberStr), '-');
        numberStr.erase(it, end(numberStr));
        
        size_t x = stoi(numberStr);
        return (a*x + b) / p;
    }
};

int main() {
    //Assignment statement
    //You have a mapping of phone numbers of format 999-99-99 to name of contacts.
    //Implement hashing of integers in the most effective way.
    
    //Solution description
    //1 Map keeps load factor under 90% (n / m = 0.9). n - number of stored elements, m - size of map's array.
    //When load factor is exceeded, rehashing happens.
    
    //2 When rehashing happens cardinality is changed from size of the previous map's array,
    //to the size of the new map's array. That way load factor decreases roughly twice.
    
    //3 Choice of a hashing function. Choose following numbers:
    //p - prime number which is equal 10^L, where L is the max number of digits in phone number.
    //p is chosen randomly and is a little bit bigger, than 10^L
    //a - hashing param. Chosen randomly from range [1, p - 1]
    //b - hashing param. Chosen randomly from range [0, p - 1]
    //When numbers p, a, b are chosen apply th following hash-function
    //hash(x) = ((a*x + b) % p) % m, where x is a key to be hashed.
    
    //The function above is a function from the Universal family of hash-functions,
    //since the probability of collisions is no more than 1 / m.
    
    const int maxPhoneNumberLength = 7;
    unordered_map<PhoneNumber, string, PhoneNumberHasher<maxPhoneNumberLength>> m;
    
    m[PhoneNumber("999-99-99")] = "Bob";
    m[PhoneNumber("100-50-00")] = "Alice";
    
    for (const auto& [key, val] : m) {
        cout << key << " - " << val << endl;
    }
    
    return 0;
}
