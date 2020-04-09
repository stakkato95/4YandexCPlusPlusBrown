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

////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////

template<int intLength = 100500, int mapSize = 100500>
struct IntHasher {
    int p;
    int a;
    int b;
    
    IntHasher() {
        default_random_engine rnd;
        uniform_int_distribution<> pDistribution { 0, 1000 };
        p = pDistribution(rnd);
        
        uniform_int_distribution<> aDistribution { 1, p - 1 };
        uniform_int_distribution<> bDistribution { 0, p - 1 };
        a = aDistribution(rnd);
        b = bDistribution(rnd);
    }
    
    size_t operator()(const int& x) const {
        size_t result = (a*x + b) % p % mapSize;
        return result;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////

template<int intLength = 100500, int mapSize = 100500>
struct StringHasher : public IntHasher<intLength, mapSize> {
    
    size_t p; //big prime number
    
    StringHasher() {
        default_random_engine rnd;
        uniform_int_distribution<> pDistribution { 1, 1'000'000 };
        p = pDistribution(rnd);
    }
    
    size_t operator()(const string& s) const {
        int hash = 0;
        int x = 31; // just a magic constant. by default 31 in Java
        
        for (size_t k = s.size(); k > 0; --k) {
            size_t i = k - 1;
            hash = (hash * x + s[i]) % p;
        }
        
        size_t result = IntHasher<intLength, mapSize>::operator()(hash);
        return result;
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
    //m - size of map's array.
    //When numbers p, a, b are chosen apply th following hash-function
    //hash(x) = ((a*x + b) % p) % m, where x is a key to be hashed.
    
    //The function above is a function from the Universal family of hash-functions,
    //since the probability of collisions is no more than 1 / m.
    
    const int maxIntDigits = 7;
    const int maxMapSize = 100;
    unordered_map<int, string, IntHasher<maxIntDigits, maxMapSize>> phoneToContact;

    phoneToContact[9999999] = "Bob";
    phoneToContact[1005000] = "Alice";
    for (const auto& [key, val] : phoneToContact) {
        cout << key << " - " << val << endl;
    }
    
    cout << endl;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    unordered_map<string, int, StringHasher<maxIntDigits, maxMapSize>> contactToPhone;
    contactToPhone["Bob"] = 9999999;
    contactToPhone["Alice"] = 1005000;
    for (const auto& [key, val] : contactToPhone) {
        cout << key << " - " << val << endl;
    }
    
    return 0;
}
