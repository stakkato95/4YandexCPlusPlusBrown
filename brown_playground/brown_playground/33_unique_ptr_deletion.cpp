//
//  33_unique_ptr_deletion.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/9/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <string>

using namespace std;

struct Actor {
public:
    Actor(string n) : name { n } { cout << name << " created" << endl; }
    
    ~Actor() { cout << name << " deleted" << endl; }
    
    void doWork() { cout << name << " does work" << endl; }
    
private:
    string name;
};

void run(Actor* ptr) {
    if (ptr) {
        ptr->doWork();
    } else {
        cout << "actor expected" << endl;
    }
}

int main33() {
    unique_ptr<Actor> alice = make_unique<Actor>("Alice");
    unique_ptr<Actor> bob = make_unique<Actor>("Bob");
    
    cout << "------------" << endl;
    alice = move(bob);
    cout << "------------" << endl;
    run(alice.get());
    run(bob.get());
    cout << "------------" << endl;
    
    return 0;
}
