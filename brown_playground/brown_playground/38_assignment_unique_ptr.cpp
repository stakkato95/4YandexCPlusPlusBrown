//
//  38_assignment_unique_ptr.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/9/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <cstddef>  // нужно для nullptr_t
#include <cassert>

using namespace std;

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
    UniquePtr(const UniquePtr&) = delete;
    
    UniquePtr& operator=(const UniquePtr&) = delete;
    
    T* pointer;
public:
    UniquePtr() {
        pointer = new T;
    }
    
    UniquePtr(T * ptr) : pointer { ptr } { }
    
    UniquePtr(UniquePtr&& other) {
        pointer = other.Release();
    }
    
    UniquePtr& operator=(nullptr_t) {
        //assignment operator with parameter 'nullptr_t' explicitly endicates
        //that we passed 'nullptr' as argument to the function
        pointer = nullptr;
    }
    
    UniquePtr& operator=(UniquePtr&& other) {
        delete pointer;
        pointer = other.Release();
    }
    
    ~UniquePtr() {
        delete pointer;
    }
    
    T& operator*() const {
        return *pointer;
    }
    
    T* operator->() const {
        return pointer;
    }
    
    T* Release() {
        T* temp = pointer;
        pointer = nullptr;
        return temp;
    }
    
    void Reset(T* ptr) {
        delete pointer;
        pointer = ptr;
    }
    
    void Swap(UniquePtr& other) {
        T* temp = other.Release();
        other.pointer = pointer;
        pointer = temp;
    }
    
    T* Get() const {
        return pointer;
    }
};


struct Item {
    static int counter;
    int value;
    Item(int v = 0): value(v) {
        ++counter;
    }
    Item(const Item& other): value(other.value) {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;


void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);
        
        ptr.Reset(new Item);
        assert(Item::counter == 1);
    }
    assert(Item::counter == 0);
    
    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);
        
        auto rawPtr = ptr.Release();
        assert(Item::counter == 1);
        
        delete rawPtr;
        assert(Item::counter == 0);
    }
    assert(Item::counter == 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    assert(ptr.Get()->value == 42);
    assert((*ptr).value == 42);
    assert(ptr->value == 42);
}

int main38() {
    TestLifetime();
    TestGetters();
    return 0;
}

