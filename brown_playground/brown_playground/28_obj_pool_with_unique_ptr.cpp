//
//  28_obj_pool_with_unique_ptr.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/8/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <memory>

#include <cassert>

using namespace std;

template <class T>
class ObjectPool {
public:
    T* Allocate() {
        T* element = TryAllocate();
        if (element == nullptr) {
            element = new T;
            allocated[element] = unique_ptr<T>(element);
        }
        
        return element;
    }
    
    T* TryAllocate() {
        if (deallocated.empty()) {
            return nullptr;
        }
        
        unique_ptr<T> ptr = move(deallocated.front());
        deallocated.pop();
        T* rawPtr = ptr.get();
        allocated[rawPtr] = move(ptr);
        
        return rawPtr;
    }
    
    void Deallocate(T* object) {
        typename unordered_map<T*, unique_ptr<T>>::iterator it = allocated.find(object);
        
        if (it == end(allocated)) {
            throw invalid_argument("unknown object");
        }
        
        deallocated.push(move(it->second));
        allocated.erase(it);
    }
    
    //destructor is no longer needed
    //~Allocate() { }
    
private:
    queue<unique_ptr<T>> deallocated;
    unordered_map<T*, unique_ptr<T>> allocated;
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

int main() {
    TestObjectPool();
    return 0;
}

