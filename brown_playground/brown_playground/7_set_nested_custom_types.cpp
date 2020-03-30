//
//  7_set_nested_custom_types.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 3/30/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <random>
#include <string>

#include <array>
#include <set>
#include <unordered_set>

#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct NewPlate {
    char c1;
    int number;
    char c2;
    char c3;
    int region;
};

ostream& operator<<(ostream& os, const NewPlate& p) {
    os
    << p.c1
    << p.number
    << p.c2
    << p.c3
    << p.region;
    return os;
}

bool operator==(const NewPlate& l, const NewPlate& r) {
    return tie(l.c1, l.number, l.c2, l.c3, l.region) == tie(r.c1, r.number, r.c2, r.c3, r.region);
}

struct NewPlateHasher {
    
    size_t operator()(const NewPlate& p) {
        size_t result = p.number;
        result *= 100;
        result += p.region;
        
        size_t s1 = p.c1 - 'A';
        size_t s2 = p.c2 - 'A';
        size_t s3 = p.c3 - 'A';
        size_t s = (s1 * 100 + s2) * 100 + s3;
        
        result *= 1'000'000; //million because 's' takes exactly 6 positions
        result += s;
        
        return result;
    }
};

class NewPlateGenerator {
public:
    NewPlate generatePlate() {
        NewPlate plate;
        
        plate.c1 = LETTERS[letters(rnd)];
        plate.number = numbers(rnd);
        plate.c2 = LETTERS[letters(rnd)];
        plate.c3 = LETTERS[letters(rnd)];
        plate.region = regions(rnd);
        
        return plate;
    }
private:
    static const int N = 12;
    const array<char, N> LETTERS { 'A', 'B', 'C', 'E', 'H', 'K', 'M', 'O', 'P', 'T', 'X', 'Y' };
    
    default_random_engine rnd;
    uniform_int_distribution<> letters { 0, N-1 };
    uniform_int_distribution<> numbers { 0, 999 };
    uniform_int_distribution<> regions { 0, 99 };
};

////////////////////////////////////////////////////////

struct TechnicalPassport {
    NewPlate plate;
    string manufacturer;
    double axesDistance;
};

bool operator==(const TechnicalPassport& l, const TechnicalPassport& r) {
    return tie(l.axesDistance, l.manufacturer, l.plate) == tie(r.axesDistance, r.manufacturer, r.plate);
}

struct TechnicalPassportHasher {
    
    size_t operator()(const TechnicalPassport& tp) {
        size_t A = pHash(tp.plate);
        size_t B = mHash(tp.manufacturer);
        size_t C = aHash(tp.axesDistance);
        
        size_t x = 37;
        
        //Ax^2 + Bx + C - apply hashing with polynomial
        return A*x*x + B*x + C;
    }
    
    NewPlateHasher pHash;
    hash<string> mHash;
    hash<double> aHash;
};

class TechnicalPassportGenerator {
public:
    TechnicalPassport generatePassport() {
        TechnicalPassport p;
        
        p.axesDistance = axesDistance(rnd);
        p.manufacturer = to_string(p.axesDistance); //stub
        p.plate = plateGenerator.generatePlate();
        
        return p;
    }
private:
    NewPlateGenerator plateGenerator;
    
    default_random_engine rnd;
    uniform_real_distribution<> axesDistance { 1.5, 2.0 };
};

////////////////////////////////////////////////////////

void testNewPlate(NewPlateGenerator& generator) {
    auto start = system_clock::now();
    
    size_t N = 1'000'000;
    unordered_set<NewPlate, NewPlateHasher> s;
    for (size_t i = 0; i < N; ++i) {
        s.insert(generator.generatePlate());
    }
    for (size_t i = 0; i < N; ++i) {
        s.find(generator.generatePlate());
    }
    
    auto finish = system_clock::now();
    cout << duration_cast<milliseconds>(finish - start).count() << " millisec" << endl;
}

void testTechnicalPassport() {
    TechnicalPassportGenerator generator;
    
    auto start = system_clock::now();
    
    size_t N = 10'000'000;
    unordered_set<TechnicalPassport, TechnicalPassportHasher> s;
    for (size_t i = 0; i < N; ++i) {
        s.insert(generator.generatePassport());
    }
    for (size_t i = 0; i < N; ++i) {
        s.find(generator.generatePassport());
    }
    
    auto finish = system_clock::now();
    cout << duration_cast<milliseconds>(finish - start).count() << " millisec" << endl;
}

int main() {
    NewPlateGenerator generator;
    
//    testNewPlate(generator);
    testTechnicalPassport();
    
    
    return 0;
}
