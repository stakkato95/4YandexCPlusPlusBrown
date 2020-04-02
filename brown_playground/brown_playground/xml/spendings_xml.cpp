#include "xml.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <utility>

using namespace std;

struct Spending {
    string category;
    int amount;
};

bool operator == (const Spending& lhs, const Spending& rhs) {
    return lhs.category == rhs.category && lhs.amount == rhs.amount;
}

ostream& operator << (ostream& os, const Spending& s) {
    return os << '(' << s.category << ": " << s.amount << ')';
}

int CalculateTotalSpendings(const vector<Spending>& spendings) {
    int result = 0;
    for (const Spending& s : spendings) {
        result += s.amount;
    }
    return result;
}

string MostExpensiveCategory(const vector<Spending>& spendings) {
    auto compare_by_amount =
    [](const Spending& lhs, const Spending& rhs) {
        return lhs.amount < rhs.amount;
    };
    return max_element(begin(spendings), end(spendings),
                       compare_by_amount)->category;
}

vector<Spending> LoadFromXml(istream& input) {
    Document doc = Load(input);
    
    vector<Spending> v;
    for (const auto& child : doc.GetRoot().Children()) {
        v.push_back({
            child.AttributeValue<string>("category"),
            child.AttributeValue<int>("amount")
        });
    }
    
    return v;
}

void TestLoadFromXml() {
    istringstream xml_input(R"(<july>
                            <spend amount="2500" category="food"></spend>
                            <spend amount="1150" category="transport"></spend>
                            <spend amount="5780" category="restaurants"></spend>
                            <spend amount="7500" category="clothes"></spend>
                            <spend amount="23740" category="travel"></spend>
                            <spend amount="12000" category="sport"></spend>
                            </july>)");
    
    const vector<Spending> spendings = LoadFromXml(xml_input);
    
    const vector<Spending> expected = {
        {"food", 2500},
        {"transport", 1150},
        {"restaurants", 5780},
        {"clothes", 7500},
        {"travel", 23740},
        {"sport", 12000}
    };
    assert(spendings == expected);
}

void TestXmlLibrary() {
    // Тест демонстрирует, как пользоваться библиотекой из файла xml.h
    
    istringstream xml_input(R"(<july>
                            <spend amount="2500" category="food"></spend>
                            <spend amount="23740" category="travel"></spend>
                            <spend amount="12000" category="sport"></spend>
                            </july>)");
    
    Document doc = Load(xml_input);
    const Node& root = doc.GetRoot();
    assert(root.Name() == "july");
    assert(root.Children().size() == 3u);
    
    const Node& food = root.Children().front();
    assert(food.AttributeValue<string>("category") == "food");
    assert(food.AttributeValue<int>("amount") ==  2500);
    
    const Node& sport = root.Children().back();
    assert(sport.AttributeValue<string>("category") == "sport");
    assert(sport.AttributeValue<int>("amount") == 12000);
    
    Node july("july", {});
    Node transport("spend", {{"category", "transport"}, {"amount", "1150"}});
    july.AddChild(transport);
    assert(july.Children().size() == 1u);
}

int main() {
    TestXmlLibrary();
    TestLoadFromXml();
}
