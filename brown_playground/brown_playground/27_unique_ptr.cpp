//
//  27_unique_ptr.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/8/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <memory>

using namespace std;

namespace actor {
    
    struct Actor {
        Actor() { cout << "created" << endl; }
        ~Actor() { cout << "deleted" << endl; }
        void doWork() { cout << "work" << endl; }
    };
    
    void runActor(Actor* ptr) {
        if (ptr) {
            ptr->doWork();
        } else {
            cout << "Actor war expected" << endl;
        }
    }
}

int main27() {
    using namespace actor;
    
    unique_ptr<Actor> ptr = make_unique<Actor>();
    runActor(ptr.get());
    
    cout << endl << "moved" << endl;
    unique_ptr<Actor> ptr1 = move(ptr);
    runActor(ptr1.get());
    runActor(ptr.get());
    
    return 0;
}
