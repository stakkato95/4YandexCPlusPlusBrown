//
//  24_hashing_integers.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/6/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <list>

#include <string>

#include <random>

using namespace std;

class integer_hasher {
public:
    explicit integer_hasher(size_t digitsMaxLength) {
        
    }
private:
    size_t p;
    size_t a;
    size_t b;
};

class phones_unordered_map {
public:
    explicit phones_unordered_map(size_t digitsMaxLength) {
        
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
    //p is chose randomly and is a little bit bigger, than 10^L
    //a - hashing param. Chosen randomly from range [1, p - 1]
    //b - hashing param. Chosen randomly from range [0, p - 1]
    //When numbers p, a, b are chosen apply th following hash-function
    //hash(x) = ((a*x + b) % p) % m, where x is a key to be hashed.
    
    //The function above is a function from the Universal family of hash-functions,
    //since the probability of collisions is no more than 1 / m.
    
    return 0;
}
