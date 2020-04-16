//
//  55_massive_constructor.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/16/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

class MassiveCtorClass {
public:
    MassiveCtorClass(int maxUserCount,
                     int maxUserFriendsCount,
                     int maxPostsCount,
                     int maxTimeOnlineCount,
                     int maxAvailability) :
    _maxUserCount(maxUserCount),
    _maxUserFriendsCount(maxUserFriendsCount),
    _maxPostsCount(maxPostsCount),
    _maxTimeOnlineCount(maxTimeOnlineCount),
    _maxAvailability(maxAvailability) { }
private:
    vector<int> _maxUserCount;
    vector<int> _maxUserFriendsCount;
    vector<int> _maxPostsCount;
    vector<int> _maxTimeOnlineCount;
    vector<int> _maxAvailability;
};

////////////////////////////////////////////////////////////////

class MassiveCtorClass1 {
public:
    friend class MassiveCtorClass1Builder;
    
private:
    MassiveCtorClass1(int param1, int param2) { }
};

////////////////////////////////////////////////////////////////

class MassiveCtorClass1Builder {
public:
    MassiveCtorClass1Builder& setParam1(int p1) {
        param1 = p1;
        return *this;
    }
    
    MassiveCtorClass1Builder& setParam2(int p2) {
        param2 = p2;
        return *this;
    }
    
    MassiveCtorClass1 build() {
        return { param1, param2 };
    }
private:
    int param1;
    int param2;
};

////////////////////////////////////////////////////////////////

struct MassiveCtorClass2Params {
    int param1;
    int param2;
};

class MassiveCtorClass2 {
public:
    MassiveCtorClass2(const MassiveCtorClass2Params& params) { }
};


int main55() {
    //Approach 1
    //since c++ doesn't have named function / ctor parameters, use comments
    MassiveCtorClass obj(/* maxUserCount */100,
                         /* maxUserFriendsCount */90,
                         /* maxPostsCount */190,
                         /* maxTimeOnlineCount */100500,
                         /* maxAvailability */ 10);
    
    //Approach 2
    //use builder
    MassiveCtorClass1 obj1 = MassiveCtorClass1Builder()
    .setParam1(100)
    .setParam2(500)
    .build();
    
    //Approach 3 (c++ 20 style)
    //create a struct with parameters and pass them to constructro of the class
    MassiveCtorClass2 obj3({ .param1 = 100, .param2 = 500 });
    
    return 0;
}
