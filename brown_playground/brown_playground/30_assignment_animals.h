//
//  30_assignment_animals.hpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/8/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#ifndef _0_assignment_animals_hpp
#define _0_assignment_animals_hpp

#include <string>

class Animal {
public:
    virtual std::string Voice() const {
        return "Not implemented yet";
    }
    virtual ~Animal() {
    }
};


class Tiger: public Animal {
    std::string Voice() const override {
        return "Rrrr";
    }
};


class Wolf: public Animal {
    std::string Voice() const override {
        return "Wooo";
    }
};


class Fox: public Animal {
    std::string Voice() const override {
        return "Tyaf";
    }
};



#endif /* _0_assignment_animals_hpp */
