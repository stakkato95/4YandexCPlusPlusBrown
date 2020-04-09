//
//  29_obj_pool_unique_ptr_compare.cpp
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
#include <memory>

#include <cassert>

using namespace std;

namespace obj_pool_unique_ptr_compare {
    
    template <class T>
    class ObjectPool {
    public:
        T* Allocate() {
            T* element = TryAllocate();
            if (element == nullptr) {
                element = new T;
                allocated.insert(unique_ptr<T>(element));
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
            allocated.insert(move(ptr));
            
            return rawPtr;
        }
        
        void Deallocate(T* object) {
            auto it = allocated.find(object);
            
            if (it == end(allocated)) {
                throw invalid_argument("unknown object");
            }
            
            deallocated.push(move(allocated.extract(it).value()));
        }
        
        //destructor is no longer needed
        //~Allocate() { }
        
    private:
        struct Compare {
            using is_transparent = void;
            
            bool operator()(const unique_ptr<T>& lhs, const unique_ptr<T>& rhs) const {
                return lhs < rhs;
            }
            
            bool operator()(const unique_ptr<T>& lhs, const T* rhs) const {
                //'less<const T*>()' creates an instance of class 'less' for pointers with type 'const T*'
                //it eliminates problem for platforms, where with a NON-linear memory model
                //normal PCs have a linera memory model (memory cells go one after another)
                return less<const T*>()(lhs.get(), rhs);
            }
            
            bool operator()(const T* lhs, const unique_ptr<T>& rhs) {
                return less<const T*>()(lhs, rhs.get());
            }
        };
        
        queue<unique_ptr<T>> deallocated;
        set<unique_ptr<T>, Compare> allocated;
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

int main29() {
    obj_pool_unique_ptr_compare::TestObjectPool();
    
    return 0;
}

