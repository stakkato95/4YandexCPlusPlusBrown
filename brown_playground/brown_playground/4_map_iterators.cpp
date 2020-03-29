//
//  4_map_iterators.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 3/29/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <set>
#include <vector>

#include <algorithm>
#include <random>

#include <chrono>
#include <ctime>

using namespace std;
using namespace std::chrono;

void iteratorInUnorderedMap() {
    //iterator in unordered_map is a ForwardIterator, that is it has only '++' operation
    //because unordered_map stores elements in forward_list
    
    //after inserting to unordered_map iterators are invalidated because rehashing can happen
}

void measureSetSearch() {
    //lover_bound from <algorithm> uses linear (sequential) search for map
    //because map iterator is a Bidirectional iterator (it can make only ++ or --)
    
    //lover_bound from <algorithm> ensures O(log N) only for RandomAccessIterators
    
    ///lover_bound from map / set knows how data is structured and has complexity O(1)
    
    vector<int> v(1'000'000);
    iota(begin(v), end(v), 0);
    auto startTime = system_clock::now();
    lower_bound(begin(v), end(v), 100500);
    auto endTime = system_clock::now();
    cout << duration_cast<milliseconds>(endTime - startTime).count() << " millisec vector with <algorithm>" << endl;
    
    set<int> s(begin(v), end(v));
    startTime = system_clock::now();
    s.lower_bound(100500);
    endTime = system_clock::now();
    cout << duration_cast<milliseconds>(endTime - startTime).count() << " millisec set with built in algo" << endl;
    
    startTime = system_clock::now();
    lower_bound(begin(s), end(s), 100500);
    endTime = system_clock::now();
    cout << duration_cast<milliseconds>(endTime - startTime).count() << " millisec set with <algorithm>" << endl;
}

int main4() {
//    measureSetSearch();

    
    return 0;
}
