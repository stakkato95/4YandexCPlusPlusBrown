//
//  20_const_and_mutable.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/4/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <map>

#include <string>

#include <chrono>

using namespace std;
using namespace std::chrono;

long getCurrentTime() {
    auto nowMillisec = time_point_cast<milliseconds>(system_clock::now());
    return duration_cast<milliseconds>(nowMillisec.time_since_epoch()).count();
}

template<typename V>
struct database_entry {
    V val;
    long timestamp;
};

template<typename V>
ostream& operator<<(ostream& os, const database_entry<V>& entry) {
    os
    << "[ val = " << entry.val
    << ", timestamp = " << entry.timestamp
    << " ]";
    return os;
}

template<typename V>
struct cache {
    string id;
    database_entry<V> val;
    long accessTime;
};

template<typename V>
class database {
public:
    void put(string id, const V val, long timeStamp) {
        database_entry<V> entry = { move(val), timeStamp };
        data[id] = entry;
        lastAccess = { move(id), entry, getCurrentTime() };
    }
    
    database_entry<V> get(string id) const {
        //data[id] isn't a const method
        //data.at(id) is a const method
        const database_entry<V>& entry = data.at(id);
        lastAccess = { move(id), entry, getCurrentTime() };
        return entry;
    }
private:
    map<string, database_entry<V>> data;
    mutable cache<V> lastAccess;
};

int main20() {
    //physical constancy - none bit in object is changed
    //logical constancy - observable state of the object stays is UNchanged
    //'const' ensures logical constancy
    //to change a field inside a const method mark it with 'mutable'
    
    database<string> db;
    db.put("1", "hello", getCurrentTime() - 100500);
    db.put("2", "world", getCurrentTime() - 100800);

    database_entry<string> entry = db.get("1");
    cout << entry << endl;
    
    return 0;
}
