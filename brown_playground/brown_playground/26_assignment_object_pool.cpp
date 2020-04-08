//
//  26_assignment_object_pool.cpp
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
#include <set>

#include <cassert>

using namespace std;

namespace obj_pool {
    
    template <class T>
    class ObjectPool {
    public:
        T* Allocate() {
            T* element = TryAllocate();
            if (element == nullptr) {
                element = new T;
                allocated.insert(element);
            }
            
            return element;
        }
        
        T* TryAllocate() {
            if (deallocated.empty()) {
                return nullptr;
            }
            
            T* element = deallocated.front();
            deallocated.pop();
            allocated.insert(element);
            return element;
        }
        
        void Deallocate(T* object) {
            if (allocated.find(object) == end(allocated)) {
                throw invalid_argument("unknown object");
            }
            
            allocated.erase(object);
            deallocated.push(object);
        }
        
        ~ObjectPool() {
            while (!deallocated.empty()) {
                T* element = deallocated.front();
                deallocated.pop();
                delete element;
            }
            
            for (T* element : allocated) {
                delete element;
            }
        }
        
    private:
        queue<T*> deallocated;
        set<T*> allocated;
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
}

int main26() {
    obj_pool::TestObjectPool();
    return 0;
}
