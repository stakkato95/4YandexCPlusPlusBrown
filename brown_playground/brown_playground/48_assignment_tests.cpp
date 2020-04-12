//
//  48_assignment_tests.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/12/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

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

bool operator==(const Person& lhs, const Person& rhs) {
    return lhs.age == rhs.age
    && lhs.gender == rhs.gender
    && lhs.is_employed == rhs.is_employed;
}

ostream& operator<<(ostream& stream, const Person& person) {
    return stream << "Person(age=" << person.age
    << ", gender=" << static_cast<int>(person.gender)
    << ", is_employed=" << boolalpha << person.is_employed << ")";
}

struct AgeStats {
    int total;
    int females;
    int males;
    int employed_females;
    int unemployed_females;
    int employed_males;
    int unemployed_males;
};

bool operator==(const AgeStats& l, const AgeStats& r) {
    auto lVals = tie(l.total, l.females, l.males, l.employed_females, l.unemployed_females, l.employed_males, l.unemployed_males);
    auto rVals = tie(r.total, r.females, r.males, r.employed_females, r.unemployed_females, r.employed_males, r.unemployed_males);
    return lVals == rVals;
}

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename iterator_traits<InputIt>::value_type> range_copy(range_begin, range_end);
    auto middle = begin(range_copy) + range_copy.size() / 2;
    nth_element(begin(range_copy), middle, end(range_copy), [](const Person& lhs, const Person& rhs) { return lhs.age < rhs.age; });
    return middle->age;
}

vector<Person> ReadPersons(istream& in_stream = cin) {
    int person_count;
    in_stream >> person_count;
    vector<Person> persons;
    persons.reserve(person_count);
    for (int i = 0; i < person_count; ++i) {
        int age, gender, is_employed;
        in_stream >> age >> gender >> is_employed;
        Person person{
            age,
            static_cast<Gender>(gender),
            is_employed == 1
        };
        persons.push_back(person);
    }
    return persons;
}

AgeStats ComputeStats(vector<Person> persons) {
    //                 persons
    //                |       |
    //          females        males
    //         |       |      |     |
    //      empl.  unempl. empl.   unempl.
    
    auto females_end = partition(begin(persons), end(persons),
                                 [](const Person& p) { return p.gender == Gender::FEMALE; });
    auto employed_females_end = partition(begin(persons), females_end,
                                          [](const Person& p) { return p.is_employed; });
    auto employed_males_end = partition(females_end, end(persons),
                                        [](const Person& p) { return p.is_employed; });
    
    return {
        ComputeMedianAge(begin(persons), end(persons)),
        ComputeMedianAge(begin(persons), females_end),
        ComputeMedianAge(females_end, end(persons)),
        ComputeMedianAge(begin(persons),  employed_females_end),
        ComputeMedianAge(employed_females_end, females_end),
        ComputeMedianAge(females_end, employed_males_end),
        ComputeMedianAge(employed_males_end, end(persons))
    };
}

void PrintStats(const AgeStats& stats, ostream& out_stream = cout) {
    out_stream << "Median age = "
    << stats.total              << endl
    << "Median age for females = "
    << stats.females            << endl
    << "Median age for males = "
    << stats.males              << endl
    << "Median age for employed females = "
    << stats.employed_females   << endl
    << "Median age for unemployed females = "
    << stats.unemployed_females << endl
    << "Median age for employed males = "
    << stats.employed_males     << endl
    << "Median age for unemployed males = "
    << stats.unemployed_males   << endl;
}

void testRead() {
    string input = R"(8
    31 1 0
    40 0 1
    24 1 1
    20 0 1
    80 0 0
    78 1 0
    10 0 0
    55 1 1)";
    istringstream is(input);
    
    vector<Person> expected = {
        {31, Gender::MALE, false},
        {40, Gender::FEMALE, true},
        {24, Gender::MALE, true},
        {20, Gender::FEMALE, true},
        {80, Gender::FEMALE, false},
        {78, Gender::MALE, false},
        {10, Gender::FEMALE, false},
        {55, Gender::MALE, true},
    };
    
    assert(expected == ReadPersons(is));
}

void testEmptyRange() {
    vector<Person> empty;
    assert(ComputeMedianAge(begin(empty), end(empty)) == 0);
}

void testMedian() {
    vector<Person> people = {
        {1, Gender::MALE, false},
        {8, Gender::FEMALE, true},
        {3, Gender::MALE, true},
        {3, Gender::FEMALE, true},
        {10, Gender::FEMALE, false}
    };
    assert(ComputeMedianAge(begin(people), end(people)) == 3);
}

void testComputeStats() {
    vector<Person> people = {
        {31, Gender::MALE, false},
        {40, Gender::FEMALE, true},
        {24, Gender::MALE, true},
        {20, Gender::FEMALE, true},
        {80, Gender::FEMALE, false},
        {78, Gender::MALE, false},
        {10, Gender::FEMALE, false},
        {55, Gender::MALE, true},
    };
    
    AgeStats expected { 40, 40, 55, 40, 80, 55, 78 };
    
    AgeStats stats = ComputeStats(people);
    
    assert(expected == stats);
}

int main48() {
    testRead();
    testEmptyRange();
    testMedian();
    testComputeStats();
    //PrintStats(ComputeStats(ReadPersons()));
    return 0;
}
