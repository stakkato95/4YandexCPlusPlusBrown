
//
//  39_demographics.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/10/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

enum class Gender {
    FEMALE,
    MALE
};

struct Person {
    int age;
    Gender gender;
    bool is_employed;
};

using PersonPredicate = function<bool(const Person&)>;

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = begin(range_copy) + range_copy.size() / 2;
    nth_element(begin(range_copy), middle, end(range_copy), [](const Person& lhs, const Person& rhs) { return lhs.age < rhs.age; });
    return middle->age;
}

void printNaive(vector<Person>& persons, const string& text, PersonPredicate predicate) {
    cout << text << " = " << ComputeMedianAge(begin(persons), partition(begin(persons), end(persons), predicate)) << endl;
}

void PrintStatsNaive(vector<Person> persons) {
    cout << "Median age = " << ComputeMedianAge(begin(persons), end(persons)) << endl;
    
    printNaive(persons, "Median age for females", [](const Person& p) { return p.gender == Gender::FEMALE; });
    printNaive(persons, "Median age for males", [](const Person& p) { return p.gender == Gender::MALE; });
    printNaive(persons, "Median age for employed females", [](const Person& p) { return p.gender == Gender::FEMALE && p.is_employed; });
    printNaive(persons, "Median age for unemployed females", [](const Person& p) { return p.gender == Gender::FEMALE && !p.is_employed; });
    printNaive(persons, "Median age for employed males", [](const Person& p) { return p.gender == Gender::MALE && p.is_employed; });
    printNaive(persons, "Median age for unemployed males", [](const Person& p) { return p.gender == Gender::MALE && !p.is_employed; });
}

void PrintStats(vector<Person> persons) {
    vector<Person>::iterator femalesEnd = partition(begin(persons), end(persons), [](const Person& p) { return p.gender == Gender::FEMALE; });
    vector<Person>::iterator femalesEmployedEnd = partition(begin(persons), femalesEnd, [](const Person& p) { return p.is_employed; });
    vector<Person>::iterator malesEmployedEnd = partition(femalesEnd, end(persons), [](const Person& p) { return p.is_employed; });
    
    cout << "Median age = " << ComputeMedianAge(begin(persons), end(persons)) << endl;
    cout << "Median age for females = " << ComputeMedianAge(begin(persons), femalesEnd) << endl;
    cout << "Median age for males = " << ComputeMedianAge(femalesEnd, end(persons)) << endl;
    cout << "Median age for employed females = " << ComputeMedianAge(begin(persons), femalesEmployedEnd) << endl;
    cout << "Median age for unemployed females = " << ComputeMedianAge(femalesEmployedEnd, femalesEnd) << endl;
    cout << "Median age for employed males = " << ComputeMedianAge(femalesEnd, malesEmployedEnd) << endl;
    cout << "Median age for unemployed males = " << ComputeMedianAge(malesEmployedEnd, end(persons)) << endl;
}

int main39() {
    vector<Person> persons = {
        {31, Gender::MALE, false},
        {40, Gender::FEMALE, true},
        {24, Gender::MALE, true},
        {20, Gender::FEMALE, true},
        {80, Gender::FEMALE, false},
        {78, Gender::MALE, false},
        {10, Gender::FEMALE, false},
        {55, Gender::MALE, true},
    };
//    PrintStatsNaive(persons);
    PrintStats(persons);
    
    return 0;
}
