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
#include <array>

#include <random>

using namespace std;

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

int main() {
    PlateGenerator generator;
    cout << generator.generatePlate() << endl;
    return 0;
}
