//
//  13_ini.hpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/2/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#ifndef _3_ini_hpp
#define _3_ini_hpp

#include <unordered_map>
#include <string>
#include <istream>

using namespace std;

namespace Ini {
    
    using Section = unordered_map<string, string>;
    
    class Document {
    public:
        Section& AddSection(string name);
        const Section& GetSection(const string& name) const;
        size_t SectionCount() const;
        
    private:
        unordered_map<string, Section> sections;
    };
    
    Document Load(istream& input);
    
}

#endif /* _3_ini_hpp */
