//
//  main.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 3/28/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <vector>

#include <algorithm>

using namespace std;

template<typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator start, Iterator finish) : start_ { start }, finish_ { finish } { }

    Iterator begin() { return start_; }

    Iterator end() { return finish_; }
    
    size_t size() { return finish_ - start_; }
private:
    Iterator start_;
    Iterator finish_;
};

template<typename Container>
auto head(const Container& container, size_t top) {
    return IteratorRange(begin(container), next(begin(container), max<size_t>(0, top)));
}

void showHeadIterator() {
    vector<int> v { 1, 2, 3, 4, 5, 6, 7 };
    
    for (const int& i : head(v, 3)) {
        cout << i << endl;
    }
}

int main1(int argc, const char * argv[]) {
    showHeadIterator();
    
    return 0;
}
