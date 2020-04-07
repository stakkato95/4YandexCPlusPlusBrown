//
//  22_concurrent_map.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/5/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <future>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <cassert>
#include <chrono>
#include <memory>

using namespace std;
using namespace std::chrono;

namespace Concurrent {
    
    template<typename V>
    struct WriteAccess {
        V& ref_to_value;
        lock_guard<mutex> g;
    };
    
    template<typename V>
    struct ReadAccess {
        const V& ref_to_value;
    };
    
    template <typename K, typename V, typename Hash = std::hash<K>>
    class ConcurrentMapModule {
    public:
        WriteAccess<V> operator[](const K& key) {
            //double check pattern
            if (!Has(key)) {
                lock_guard<mutex> g(m);
                if (!HasThreadUnsafe(key)) {
                    table.insert({ key, V() });
                }
            }
            
            return { table[key], lock_guard<mutex>(m) };
        }
        
        ReadAccess<V> At(const K& key) const {
            return { table.at(key) };
        }
        
        bool Has(const K& key) const {
            lock_guard<mutex> g(m);
            // similar approaches, BUT 'table.at(key)' works faster.
            //return table.count(key) > 0;
            //return table.find(key) != table.end();
            
            try {
                table.at(key);
                return true;
            } catch (out_of_range e) {
                return false;
            }
        }
        
        const unordered_map<K, V, Hash>& getTable() const { return table; }
    private:
        bool HasThreadUnsafe(const K& key) const {
            try {
                table.at(key);
                return true;
            } catch (out_of_range e) {
                return false;
            }
        }
        
        mutable mutex m;
        unordered_map<K, V, Hash> table;
    };
    
    template <typename K, typename V, typename Hash = std::hash<K>>
    class ConcurrentMap {
    public:
        
        explicit ConcurrentMap(size_t bucket_count) {
            for (size_t i = 0; i < bucket_count; ++i) {
                table[i];
            }
        }
        
        WriteAccess<V> operator[](const K& key) {
            return table[getTableIndex(key)][key];
        }
        
        ReadAccess<V> At(const K& key) const {
            return table.at(getTableIndex(key)).At(key);
        }
        
        bool Has(const K& key) const {
            return table.at(getTableIndex(key)).Has(key);
        }
        
        unordered_map<K, V, Hash> BuildOrdinaryMap() const {
            unordered_map<K, V, Hash> result;
            
            for (auto const& [superKey, mapModule] : table) {
                for (auto const& [key, val] : mapModule.getTable()) {
                    result[key] = val;
                }
            }
            
            return result;
        }
        
    private:
        size_t getTableIndex(const K& key) const { return hasher(key) % table.size(); }
        
        Hash hasher;
        unordered_map<size_t, ConcurrentMapModule<K, V, Hash>> table;
    };
    
    void RunConcurrentUpdates(ConcurrentMap<int, int>& cm, size_t thread_count, int key_count) {
        auto kernel = [&cm, key_count](int seed) {
            vector<int> updates(key_count);
            iota(begin(updates), end(updates), -key_count / 2);
            shuffle(begin(updates), end(updates), default_random_engine(seed));
            
            for (int i = 0; i < 2; ++i) {
                for (auto key : updates) {
                    cm[key].ref_to_value++;
                }
            }
        };
        
        vector<future<void>> futures;
        for (size_t i = 0; i < thread_count; ++i) {
            futures.push_back(async(kernel, i));
        }
    }
    
    void TestConcurrentUpdate() {
        const size_t thread_count = 3;
        const size_t key_count = 50000;
        
        ConcurrentMap<int, int> cm(thread_count);
        RunConcurrentUpdates(cm, thread_count, key_count);
        
        const auto result = std::as_const(cm).BuildOrdinaryMap();
        assert(result.size() == key_count);
        
        //ofstream os("result.txt");
        for (auto& [k, v] : result) {
            //os << "[" << k << ", " << v << "]" << endl;
            assert(v == 6);
        }
    }
    
    void TestReadAndWrite() {
        ConcurrentMap<size_t, string> cm(5);
        
        auto updater = [&cm] {
            for (size_t i = 0; i < 50000; ++i) {
                cm[i].ref_to_value += 'a';
            }
        };
        auto reader = [&cm] {
            vector<string> result(50000);
            for (size_t i = 0; i < result.size(); ++i) {
                result[i] = cm[i].ref_to_value;
            }
            return result;
        };
        
        auto u1 = async(updater);
        auto r1 = async(reader);
        auto u2 = async(updater);
        auto r2 = async(reader);
        
        u1.get();
        u2.get();
        
        for (auto f : {&r1, &r2}) {
            auto result = f->get();
            assert(all_of(result.begin(), result.end(), [](const string& s) { return s.empty() || s == "a" || s == "aa"; }));
        }
    }
    
    void TestSpeedup() {
        {
            ConcurrentMap<int, int> single_lock(1);
            
            auto start = system_clock::now();
            RunConcurrentUpdates(single_lock, 4, 50000);
            cout << duration_cast<milliseconds>(system_clock::now() - start).count() << " single_lock" << endl;
        }
        {
            ConcurrentMap<int, int> many_locks(100);
            
            auto start = system_clock::now();
            RunConcurrentUpdates(many_locks, 4, 50000);
            cout << duration_cast<milliseconds>(system_clock::now() - start).count() << " many_locks" << endl;
        }
    }
    
    void TestConstAccess() {
        const unordered_map<int, string> expected = {
            {1, "one"},
            {2, "two"},
            {3, "three"},
            {31, "thirty one"},
            {127, "one hundred and twenty seven"},
            {1598, "fifteen hundred and ninety eight"}
        };
        
        const ConcurrentMap<int, string> cm = [&expected] {
            ConcurrentMap<int, string> result(3);
            for (const auto& [k, v] : expected) {
                result[k].ref_to_value = v;
            }
            return result;
        }();
        
        vector<future<string>> futures;
        for (int i = 0; i < 10; ++i) {
            futures.push_back(async([&cm, i] {
                try {
                    return cm.At(i).ref_to_value;
                } catch (exception&) {
                    return string();
                }
            }));
        }
        futures.clear();
        
        assert(cm.BuildOrdinaryMap() == expected);
    }
    
    void TestStringKeys() {
        const unordered_map<string, string> expected = {
            {"one", "ONE"},
            {"two", "TWO"},
            {"three", "THREE"},
            {"thirty one", "THIRTY ONE"},
        };
        
        const ConcurrentMap<string, string> cm = [&expected] {
            ConcurrentMap<string, string> result(2);
            for (const auto& [k, v] : expected) {
                result[k].ref_to_value = v;
            }
            return result;
        }();
        
        assert(cm.BuildOrdinaryMap() == expected);
    }
    
    struct Point {
        int x;
        int y;
        
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };
    
    struct PointHash {
        size_t operator()(const Point& p) const {
            std::hash<int> h;
            size_t x = 3571;
            size_t a = h(p.x);
            size_t b = h(p.y);
            return a * x + b;
        }
    };
    
    
    
    void TestUserType() {
        ConcurrentMap<Point, size_t, PointHash> point_weight(5);
        
        vector<future<void>> futures;
        for (int i = 0; i < 1000; ++i) {
            futures.push_back(async([&point_weight, i] {
                point_weight[Point{i, i}].ref_to_value = i;
            }));
        }
        
        futures.clear();
        
        for (int i = 0; i < 1000; ++i) {
            assert(point_weight.At(Point{i, i}).ref_to_value == i);
        }
        
        const auto weights = point_weight.BuildOrdinaryMap();
        for (int i = 0; i < 1000; ++i) {
            assert(weights.at(Point{i, i}) == i);
        }
    }
    
    void TestHas() {
        ConcurrentMap<int, int> cm(2);
        cm[1].ref_to_value = 100;
        cm[2].ref_to_value = 200;
        
        const auto& const_map = std::as_const(cm);
        assert(const_map.Has(1));
        assert(const_map.Has(2));
        assert(!const_map.Has(3));
    }
}

int main22() {
    Concurrent::TestConcurrentUpdate();
    Concurrent::TestReadAndWrite();
    Concurrent::TestSpeedup();
    Concurrent::TestConstAccess();
    Concurrent::TestStringKeys();
    Concurrent::TestUserType();
    Concurrent::TestHas();
    
    return 0;
}

