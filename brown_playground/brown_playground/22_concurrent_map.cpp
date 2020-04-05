//
//  22_concurrent_map.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/5/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

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
    
    template <typename K, typename V, typename Hash = std::hash<K>>
    class ConcurrentMap {
    public:
        using MapType = unordered_map<K, V, Hash>;

        struct WriteAccess {
            V& ref_to_value;
            mutex* mtx;
            
            WriteAccess(V& ref, mutex* ptr) : ref_to_value { ref }, mtx { ptr } {
                mtx->lock();
            }
            ~WriteAccess() {
                mtx->unlock();
            }
        };

        struct ReadAccess {
            const V& ref_to_value;
            mutex* mtx;
            
            ReadAccess(const V& ref, mutex* ptr) : ref_to_value { ref }, mtx { ptr } {
                mtx->lock();
            }
            ~ReadAccess() {
                mtx->unlock();
            }
        };

        explicit ConcurrentMap(size_t bucket_count) : mutexes { vector<mutex>(bucket_count) } {
            for (size_t i = 0; i < bucket_count; ++i) {
                table[i] = MapType();
            }
        }

        WriteAccess operator[](const K& key) {
            size_t i = hasher(key);

            if (Has(key)) {
                return { table[i][key], mutexes[i] };
            }
            
            V val = V();
            table[i][key] = val;
            return { table[i][key], mutexes[i] };
        }

        ReadAccess At(const K& key) const {
            size_t i = hasher(key);
            return ReadAccess(table.at(i).at(key), mutexes[i]);
        }

        bool Has(const K& key) const {
            try {
                At(key);
                return true;
            } catch (out_of_range e) {
                return false;
            } catch(system_error e) {
                cout <<  e.what() << endl;
                return false;
            }
        }

        MapType BuildOrdinaryMap() const {
            unordered_map<K, V, Hash> m;

            for (const auto& [i, map] : table) {
                for (const auto& [key, val] : map) {
                    m[key] = val;
                }
            }

            return m;
        }

    private:
        Hash hasher;
        unordered_map<size_t, MapType> table;
        mutable vector<mutex> mutexes;
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

        for (size_t i = 0; i < futures.size(); ++i) {
            futures[i].get();
        }
    }

    void TestConcurrentUpdate() {
        const size_t thread_count = 3;
        const size_t key_count = 50000;

        ConcurrentMap<int, int> cm(thread_count);
        RunConcurrentUpdates(cm, thread_count, key_count);

        const auto result = std::as_const(cm).BuildOrdinaryMap();
        assert(result.size() == key_count);
        for (auto& [k, v] : result) {
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
        int x, y;
    };

    struct PointHash {
        size_t operator()(Point p) const {
            std::hash<int> h;
            return h(p.x) * 3571 + h(p.y);
        }
    };

    bool operator==(Point lhs, Point rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

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

int main() {
    
//    Concurrent::TestConcurrentUpdate();
    //    TestReadAndWrite();
    //    TestSpeedup();
    //    TestConstAccess();
    //    TestStringKeys();
    //    TestUserType();
    //    TestHas();
    
    return 0;
}

