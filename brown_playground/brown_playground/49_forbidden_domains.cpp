//
//  49_forbidden_domains.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/12/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <sstream>

#include <vector>
#include <tuple>

#include <string>
#include <cassert>

using namespace std;

struct Domains {
    vector<string> forbidden;
    vector<string> toCheck;
};

bool operator==(const Domains& l, const Domains& r) {
    return l.forbidden == r.forbidden && l.toCheck == r.toCheck;
}

/////////////////////////////////////////////////////////////////

bool anyDotAtStartFinish(const string& str) {
    if (str.size() == 0) {
        return false;
    }
    
    return str[0] == '.' || str[str.size()] == '.';
}

bool anyTwoDotsInARow(const string& str) {
    return str.find("..") != string::npos;
}

bool isValidDomain(const string& str) {
    return !anyDotAtStartFinish(str) && !anyTwoDotsInARow(str);
}

/////////////////////////////////////////////////////////////////

Domains readInput(istream& is) {
    Domains readResult = { vector<string>(), vector<string>() };
    bool readForbiddenDomains = true;
    
    string data;
    while (!is.eof()) {
        getline(is, data);
        int itemsToRead = stoi(data);
        vector<string>& outputVector = readForbiddenDomains ? readResult.forbidden : readResult.toCheck;
        
        for (int i = 0; i < itemsToRead; ++i) {
            getline(is, data);
            if (isValidDomain(data)) {
                outputVector.push_back(data);
            }
        }
        
        readForbiddenDomains = !readForbiddenDomains;
    }
    
    return readResult;
}

vector<bool> processDomains(const Domains& domains) {
    vector<bool> result;
    result.reserve(domains.toCheck.size());
    
    for (const string& toCheck : domains.toCheck) {
        bool isValidDomain = true;
        
        for (const string& forbidden : domains.forbidden) {
            if (toCheck.find(forbidden) != string::npos) {
                isValidDomain = false;
                break;
            }
        }
        
        result.push_back(isValidDomain);
    }
    
    return result;
}

void printResult(ostream& os, const vector<bool>& result) {
    for (bool isGood : result) {
        os << (isGood ? "Good" : "Bad") << endl;
    }
}

/////////////////////////////////////////////////////////////////

void testIsValidDomain() {
    assert(isValidDomain("me.com"));
    assert(isValidDomain("com"));
    
    assert(!isValidDomain("google..com"));
    assert(!isValidDomain(".google.com"));
}


void testReadInput() {
    istringstream is(R"(4
ya.ru
maps.me
m.ya.ru
com
7
ya.ru
ya.com
m.maps.me
moscow.m.ya.ru
maps.com
maps.ru
ya.ya)");
    
    Domains expected = {
        { "ya.ru", "maps.me", "m.ya.ru", "com" },
        { "ya.ru", "ya.com", "m.maps.me", "moscow.m.ya.ru", "maps.com", "maps.ru", "ya.ya" }
    };
    Domains result = readInput(is);
    
    assert(result == expected);
}

void testProcessDomains() {
    Domains domains = {
        { "ya.ru", "maps.me", "m.ya.ru", "com" },
        { "ya.ru", "ya.com", "m.maps.me", "moscow.m.ya.ru", "maps.com", "maps.ru", "ya.ya" }
    };
    vector<bool> expected { false, false, false, false, false, true, true };
    
    vector<bool> result = processDomains(domains);
    assert(expected == result);
}

void testPrintResult() {
    ostringstream os;
    vector<bool> input { false, false, false, false, false, true, true };
    
    string expected = R"(Bad
Bad
Bad
Bad
Bad
Good
Good
)";
    
    printResult(os, input);
    string result = os.str();
    assert(expected == result);
}

void runTests() {
    testIsValidDomain();
    testReadInput();
    testProcessDomains();
    testPrintResult();
}

int main49() {
    runTests();
    
    return 0;
}
