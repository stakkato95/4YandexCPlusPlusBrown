//
//  10_assignment_secondary_db_index_1.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/1/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <map>
#include <unordered_map>

#include <string>
#include <utility>

#include <cassert>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

struct RecordHasher {
    
    size_t operator()(const Record& r) {
        size_t x = 37;
        
        size_t A = strHasher(r.id) + strHasher(r.title);
        size_t B = strHasher(r.user) + intHasher(r.timestamp);
        size_t C = intHasher(r.karma);
        
        return A*x*x + B*x + C;
    }
    
    hash<string> strHasher;
    hash<int> intHasher;
};

struct DatabaseEntry {
    Record record;
    multimap<int, string>::iterator timestampIt;
    multimap<int, string>::iterator karmaIt;
    multimap<string, string>::iterator userIt;
};

class Database {
public:
    bool Put(const Record& record) {
        if (GetById(record.id)) {
            return false;
        }
        
        DatabaseEntry entry;
        entry.record = move(record);
        entry.timestampIt = timestampIndex.insert(decltype(timestampIndex)::value_type(record.timestamp, record.id));
        entry.karmaIt = karmaIndex.insert(decltype(karmaIndex)::value_type(record.karma, record.id));
        entry.userIt = userIndex.insert(decltype(userIndex)::value_type(record.user, record.id));
        records[entry.record.id] = entry;
        
        return true;
    }
    
    const Record* GetById(const string& id) const {
        //const iterator points to a const value
        //dereferencing it returns pointer to a const value
        unordered_map<string, DatabaseEntry>::const_iterator it = records.find(id);
        if (it == end(records)) {
            return nullptr;
        }
        
        return &(it->second.record);
    }
    
    bool Erase(const string& id) {
        if (!GetById(id)) {
            return false;
        }
        
        unordered_map<string, DatabaseEntry>::iterator it = records.find(id);
        timestampIndex.erase(it->second.timestampIt);
        karmaIndex.erase(it->second.karmaIt);
        userIndex.erase(it->second.userIt);
        records.erase(id);
        
        return true;
    }
    
    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        multimap<int, string>::const_iterator it = timestampIndex.lower_bound(low);
        if (it == end(timestampIndex)) {
            return;
        }
        
        while (it != end(timestampIndex) && it->first <= high && callback(*GetById(it->second))) {
            ++it;
        }
    }
    
    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        multimap<int, string>::const_iterator it = karmaIndex.lower_bound(low);
        if (it == end(karmaIndex)) {
            return;
        }
        
        while (it != end(karmaIndex) && it->first <= high && callback(*GetById(it->second))) {
            ++it;
        }
    }
    
    template <typename Callback>
    void AllByUser(const string& user, Callback callback) const {
        multimap<string, string>::const_iterator it = userIndex.find(user);
        
        while (it != end(userIndex) && callback(*GetById(it->second))) {
            it++;
        }
    }
private:
    unordered_map<string, DatabaseEntry> records;
    multimap<int, string> timestampIndex;
    multimap<int, string> karmaIndex;
    multimap<string, string> userIndex;
};

void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;
    
    Database db;
    db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
    db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});
    
    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
    });
    
    assert(2 == count);
}

void TestSameUser() {
    Database db;
    db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
    db.Put({"id2", "Rethink life", "master", 1536107260, 2000});
    
    int count = 0;
    db.AllByUser("master", [&count](const Record&) {
        ++count;
        return true;
    });
    
    assert(2 == count);
}

void TestReplacement() {
    const string final_body = "Feeling sad";
    
    Database db;
    db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    db.Erase("id");
    db.Put({"id", final_body, "not-master", 1536107260, -10});
    
    auto record = db.GetById("id");
    assert(record != nullptr);
    assert(final_body == record->title);
}

int main() {
    TestRangeBoundaries();
    TestSameUser();
    TestReplacement();
    
    return 0;
}

