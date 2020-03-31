//
//  9_set_extract_merge.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 3/30/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <set>
#include <string>

using namespace std;

struct NCString : public string {
    using string::string; //all constructors of that string are made visible to NCString
    
    NCString(const NCString&) = delete;
    NCString(NCString&&) = default;
    
//    bool operator<(const NCString& other) const {
//        return this->compare(other) < 0;
//    }
};

bool operator<(const NCString& l, const NCString& r) {
    return l.compare(r) < 0;
}

template<typename T>
void printSet(const set<T>& s) {
    for (const T& item : s) {
        cout << item << " ";
    }
    cout << endl;
}

void showSetUpdateBadPraxis() {
    set<string> ss;
    ss.insert("Aaa");
    ss.insert("Bbb");
    ss.insert("Ccc");
    printSet(ss);
    
    set<string>::iterator it = begin(ss);
    //doesn't compile because elements in tree are sorted
    //and iterator can't move updated object into another place
    //string& first = *it;
    
    string first = *it;
    first[0] = tolower(first[0]);
    
    ss.erase(it);
    ss.insert(first);
    printSet(ss);
}

void showSetUpdateGoodPraxis() {
    set<NCString> ss;
    ss.insert("Aaa");
    ss.insert("Bbb");
    ss.insert("Ccc");
    printSet(ss);
    
    //advantage - node is not removed / added again
    //in this case the node is only moved
    set<NCString>::iterator it = begin(ss);
    auto node = ss.extract(it);
    NCString& val = node.value();
    val[0] = tolower(val[0]);
    ss.insert(move(node));
    printSet(ss);
}

void showMergeTrees() {
    set<NCString> s1;
    s1.insert("aaa");
    s1.insert("Bbb");
    s1.insert("Ccc");

    set<NCString> s2;
    s2.insert("Xxx");
    s2.insert("Yyy");
    s2.insert("yyy");

    //s1.merge(s2); //not compiling on mac os
}

int main() {
//    showSetUpdateBadPraxis();
//    showSetUpdateGoodPraxis();
    showMergeTrees();
    
    return 0;
}
