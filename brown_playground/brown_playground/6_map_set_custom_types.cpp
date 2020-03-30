//
//  6_map_set_custom_types.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 3/29/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <iomanip>

#include <set>
#include <unordered_set>
#include <array>

#include <random>

#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Plate {
    char c1;
    int number;
    char c2;
    char c3;
    int region;
};

ostream& operator<<(ostream& os, const Plate& plate) {
    os
    << plate.c1
    << setw(3) << setfill('0') << plate.number
    << plate.c2
    << plate.c3
    << setw(2) << setfill('0') << plate.region;
    
    return os;
}

bool operator<(const Plate& l, const Plate& r) {
    return tie(l.c1, l.number, l.c2, l.c3, l.region) < tie(r.c1, r.number, r.c2, r.c3, r.region);
}

bool operator==(const Plate& l, const Plate& r) {
    return tie(l.c1, l.number, l.c2, l.c3, l.region) == tie(r.c1, r.number, r.c2, r.c3, r.region);
}

struct PlateHasher {
    size_t operator()(const Plate& p) const {
        //very simple hash. only for small tables. range of numbers 0 - 1000
        //with number of elements > 1000 there'll be collisions
        //return p.number;
        
        //for number of elements 0 - 100 000
        //return p.number * 100 + p.region;
        
        //for number of elements 0 - 1 000 000
        size_t result = p.number * 100 + p.region;
        
        int s1 = p.c1 - 'A';
        int s2 = p.c2 - 'A';
        int s3 = p.c3 - 'A';
        int s = (s1 * 100 + s2) * 100 + s3;
        
        result *= 1'000'000;
        result += s;
        
        return result;
    }
};

class PlateGenerator {
public:
    Plate generatePlate() {
        Plate plate;
        
        plate.c1 = LETTERS[letters(rnd)];
        plate.number = numbers(rnd);
        plate.c2 = LETTERS[letters(rnd)];
        plate.c3 = LETTERS[letters(rnd)];
        plate.region = regions(rnd);
        
        return plate;
    }
private:
    const static int N = 12;
    const array<char, N> LETTERS = { 'A', 'B', 'C', 'E', 'H', 'K', 'M', 'O', 'P', 'T', 'X', 'Y' };
    
    default_random_engine rnd;
    uniform_int_distribution<> letters { 0, N-1 };
    uniform_int_distribution<> numbers { 0, 999 };
    uniform_int_distribution<> regions { 0, 99 };
};

void testSet(PlateGenerator& generator) {
    //set requires to overload function '<', because it store elements in a tree
    set<Plate> plates;
    
    cout << "set init" << endl;
    Plate plate;
    for (size_t i = 0; i < 10; ++i) {
        plate = generator.generatePlate();
        plates.insert(move(plate));
        cout << plate << endl;
    }
    cout << endl;
    
    cout << "set print" << endl;
    for (const Plate& plate : plates) {
        cout << plate << endl;
    }
    cout << endl;
}

void testUnorderedSet(PlateGenerator& generator) {
    //unordered_set requires to overload hash function, because it store elements in lists
    unordered_set<Plate, PlateHasher> plates;
    
    cout << "unordered_set init" << endl;
    Plate plate;
    for (size_t i = 0; i < 10; ++i) {
        plate = generator.generatePlate();
        plates.insert(move(plate));
        cout << plate << endl;
    }
    cout << endl;
    
    cout << "unordered_set print" << endl;
    for (const Plate& plate : plates) {
        cout << plate << endl;
    }
    cout << endl;
}

void compareSearch(PlateGenerator& generator) {
    size_t N = 1'000'000;
    
    auto startTime = system_clock::now();
    
    set<Plate> plates;
    for (size_t i = 0; i < N; ++i) {
        plates.insert(generator.generatePlate());
    }
    for (size_t i = 0; i < N; ++i) {
        plates.find(generator.generatePlate());
    }
    
    auto endTime = system_clock::now();
    cout << duration_cast<milliseconds>(endTime - startTime).count() << " millisec - set" << endl;
    
    
    startTime = system_clock::now();
    
    unordered_set<Plate, PlateHasher> platesUnordered;
    for (size_t i = 0; i < N; ++i) {
        platesUnordered.insert(generator.generatePlate());
    }
    for (size_t i = 0; i < N; ++i) {
        platesUnordered.find(generator.generatePlate());
    }
    
    endTime = system_clock::now();
    cout << duration_cast<milliseconds>(endTime - startTime).count() << " millisec - unordered_set" << endl;
}

int main() {
    PlateGenerator generator;
    
//    testSet(generator);
//    testUnorderedSet(generator);
    compareSearch(generator);
    
    return 0;
}
