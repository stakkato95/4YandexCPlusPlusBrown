//
//  54_return_multiple_objects.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/14/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <set>

using namespace std;

template<typename BackInsertableCollection>
void returnWithBackInserter(back_insert_iterator<BackInsertableCollection> backInserter) {
    for (int i = 4; i < 10; ++i) {
        *backInserter = i;
        backInserter++;
    }
}

template<typename FrontInsertableCollection>
void returnWithFrontInserter(front_insert_iterator<FrontInsertableCollection> backInserter) {
    for (int i = 4; i > 0; --i) {
        *backInserter = i;
        backInserter++;
    }
}

void returnWithInserter(insert_iterator<set<int>> insertIterator) {
    for (int i = 1; i < 10; ++i) {
        *insertIterator = i;
        insertIterator++;
    }
}

template<typename Callback>
void returnWithCallback(Callback callback) {
    for (int i = 1; i < 10; ++i) {
        callback(i);
    }
}

int main54() {
    //when possible return 'view-types' from a function, which doesn't change the underlying collection
    //such 'view-types' are 'range', 'span', 'string_view'
    //TODO 'range' is not available yet=(
    
    //use inserter_iterator
    cout << "inserter" << endl;
    vector<int> v { 1, 2, 3 };
    returnWithBackInserter(back_inserter(v));
    copy(begin(v), end(v), ostream_iterator<int>(cout, " "));
    cout << endl;
    
    list<int> linkedList { 5, 6, 7, 8, 9 };
    returnWithFrontInserter(front_inserter(linkedList));
    copy(begin(linkedList), end(linkedList), ostream_iterator<int>(cout, " "));
    cout << endl;
    
    set<int> s;
    returnWithInserter(inserter(s, end(s)));
    copy(begin(s), end(s), ostream_iterator<int>(cout, " "));
    cout << endl;
    
    //use callback
    cout << endl << "callback" << endl;
    vector<int> v1 { 1, 2, 3 };
    returnWithCallback([&v1] (int i) {
        v1.push_back(i);
        cout << i << " ";
    });
    cout << endl;
    
    return 0;
}
