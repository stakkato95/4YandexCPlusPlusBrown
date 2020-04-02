//
//  13_ini.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/2/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include "13_ini.h"

namespace Ini {
    
    Section& Document::AddSection(string name) {
        return sections[name];
    }
    
    const Section& Document::GetSection(const string& name) const {
        return sections.at(name);
    }
    
    size_t Document::SectionCount() const {
        return sections.size();
    }
    
    pair<string_view, string_view> split(string_view str, char by) {
        size_t pos = str.find(by);
        string_view left = str.substr(0, pos);
        
        if (pos != string_view::npos) {
            return { left, str.substr(pos + 1) };
        }
        
        return { left, string_view() };
    }
    
    string_view trimSpaces(string_view str) {
        while (isspace(str[0])) {
            str.remove_prefix(1);
        }
        
        return str;
    }
    
    string_view trimBraces(string_view str) {
        str.remove_prefix(1);
        str.remove_suffix(1);
        return str;
    }
    
    Document Load(istream& input) {
        Document doc;
        
        string data;
        string_view trimmedData;
        while (getline(input, data)) {
            trimmedData = trimBraces(trimSpaces(data));
            string kk = string(trimmedData);
            Section& s = doc.AddSection(kk);
            
            while (getline(input, data)) {
                trimmedData = trimSpaces(data);
                if (trimmedData.size() == 0) {
                    break;
                }
                auto [key, value] = split(trimmedData, '=');
                
                string key1 = string(key);
                string val1 = string(value);
                
                s[key1] = val1;
            }
        }
        
        return doc;
    }
}
