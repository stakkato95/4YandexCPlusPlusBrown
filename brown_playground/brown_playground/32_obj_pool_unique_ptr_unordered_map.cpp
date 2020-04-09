//
//  32_obj_pool_unique_ptr_unordered_map.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/9/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <unordered_set>
#include <memory>

#include <cassert>

using namespace std;

template <class T>
class ObjectPool {
public:
    T* Allocate() {
        if (free.empty()) {
            free.push(make_unique<T>());
        }
        
        unique_ptr<T> ptr = move(free.front());
        free.pop();
        T* rawPtr = ptr.get();
        allocated.insert(move(ptr));
        
        return rawPtr;
    }
    
    T* TryAllocate() {
        if (free.empty()) {
            return nullptr;
        }
        
        return Allocate();
    }
    
    void Deallocate(T* object) {
        unique_ptr<T> ptr = unique_ptr<T>(object);
        typename unordered_set<unique_ptr<T>>::iterator it = allocated.find(ptr);
        ptr.release();
        
        if (it == end(allocated)) {
            throw invalid_argument("unknown object");
        }
        
        free.push(move(allocated.extract(it).value()));
    }
    
private:
    unordered_set<unique_ptr<T>> allocated;
    queue<unique_ptr<T>> free;
};

void TestObjectPool() {
    ObjectPool<string> pool;
    
    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();
    
    *p1 = "first";
    *p2 = "second";
    *p3 = "third";
    
    pool.Deallocate(p2);
    assert(*pool.Allocate() == "second");
    
    pool.Deallocate(p3);
    pool.Deallocate(p1);
    assert(*pool.Allocate() == "third");
    assert(*pool.Allocate() == "first");
    
    pool.Deallocate(p1);
}

int main32() {
    TestObjectPool();
    
    return 0;
}


