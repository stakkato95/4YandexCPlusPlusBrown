//
//  2_map.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 3/28/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <fstream>

#include <map>
#include <unordered_map>
#include <vector>

#include <string>
#include <tuple>

#include <ctime>
#include <chrono>

#include "1_head_iterator.h"

using namespace std;
using namespace std::chrono;

void trimWord(string& text) {
    if (text[0] == '\'' || text[0] == '\"') {
        text.erase(begin(text));
    }
}

template<typename K, typename V>
ostream& operator<<(ostream& os, const pair<K, V>& pair) {
    os << "\"" << pair.first << "\" - " << pair.second;
    return os;
}

template<typename Collection>
void print(const Collection& collection) {
    for (const auto& item : head(collection, 5)) {
        cout << item << endl;
    }
}

int main() {
    ifstream input("holmes.txt");
    
    auto startTime = system_clock::now();
    //create a map
    unordered_map<string, int> words;
    string text;
    while (input >> text) {
        transform(begin(text), end(text), begin(text), [](const unsigned char c) { return tolower(c); });
        trimWord(text);
        words[text]++;
    }
    //print(words);
    auto endTime = system_clock::now();
    cout << duration_cast<milliseconds>(endTime - startTime).count() << " milliseconds" << endl;
    //map - 1809 milliseconds
    //unordered_map - 578 milliseconds
    
    
    startTime = system_clock::now();
    //sort map
    vector<pair<string, int>> v(begin(words), end(words));
    sort(begin(v), end(v), [](const pair<string, int>& l, const pair<string, int>& r) { return l.second > r.second; });
    cout << endl;
    //print(v);
    endTime = system_clock::now();
    cout << duration_cast<milliseconds>(endTime - startTime).count() << " milliseconds" << endl;
    
    return 0;
}
