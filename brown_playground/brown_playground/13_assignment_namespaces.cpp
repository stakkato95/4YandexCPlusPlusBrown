//
//  13_assignment_namespaces.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/2/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include "13_ini.h"

#include <iostream>
#include <sstream>

#include <cassert>

using namespace std;

void TestLoadIni() {
    istringstream input(
                        R"([july]
                        food=2500
                        sport=12000
                        travel=23400
                        clothes=5200

                        [august]
                        food=3250
                        sport=10000
                        travel=0
                        clothes=8300
                        jewelery=25000
                        )"
    );

    const Ini::Document doc = Ini::Load(input);

    assert(doc.SectionCount() == 2u);

    const Ini::Section expected_july = {
        {"food", "2500"},
        {"sport", "12000"},
        {"travel", "23400"},
        {"clothes", "5200"},
    };

    const Ini::Section expected_august = {
        {"food", "3250"},
        {"sport", "10000"},
        {"travel", "0"},
        {"clothes", "8300"},
        {"jewelery", "25000"},
    };
    
    assert(doc.GetSection("july") == expected_july);
    assert(doc.GetSection("august") == expected_august);
}

void TestDocument() {
    Ini::Document doc;
    assert(doc.SectionCount() == 0u);

    // Обратите внимание, как мы используем указатель для работы
    // с последней добавленной секцией. Эта техника может вам пригодиться
    // для реализации функции Load
    Ini::Section* section = &doc.AddSection("one");
    assert(doc.SectionCount() == 1u);

    section->insert({"name_1", "value_1"});
    section->insert({"name_2", "value_2"});

    section = &doc.AddSection("two");
    section->insert({"name_1", "value_1"});
    section->insert({"name_2", "value_2"});
    section->insert({"name_3", "value_3"});

    section = &doc.AddSection("three");
    section->insert({"name_1", "value_1"});

    assert(doc.SectionCount() == 3u);
    const Ini::Section expected_one = {{"name_1", "value_1"}, {"name_2", "value_2"}};
    const Ini::Section expected_two = {{"name_1", "value_1"}, {"name_2", "value_2"}, {"name_3", "value_3"}};
    const Ini::Section expected_three = {{"name_1", "value_1"}};

    assert(doc.GetSection("one") == expected_one);
    assert(doc.GetSection("two") == expected_two);
    assert(doc.GetSection("three") == expected_three);
}

void TestUnknownSection() {
    Ini::Document doc;
    doc.AddSection("primary");

    try {
        doc.GetSection("secondary");
        //"Ini::Document::GetSection() should throw std::out_of_range for unknown section"
        assert(false);
    } catch (out_of_range&) {
    } catch (...) {
//        "Ini::Document::GetSection() throws unexpected exception for unknown section"
        assert(false);
    }
}

void TestDuplicateSections() {
    Ini::Document doc;
    doc.AddSection("one").insert({"key_1", "value_1"});
    doc.AddSection("one").insert({"key_2", "value_2"});

    const Ini::Section expected = {{"key_1", "value_1"}, {"key_2", "value_2"}};
    assert(doc.GetSection("one") == expected);
}

int main13() {
    TestLoadIni();
    TestDocument();
    TestUnknownSection();
    TestDuplicateSections();
    return 0;
}
