#include "json.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <cassert>

using namespace std;
using namespace json;

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

int CalculateTotalSpendings(
                            const vector<Spending>& spendings
                            ) {
    int result = 0;
    for (const Spending& s : spendings) {
        result += s.amount;
    }
    return result;
}

string MostExpensiveCategory(
                             const vector<Spending>& spendings
                             ) {
    auto compare_by_amount =
    [](const Spending& lhs, const Spending& rhs) {
        return lhs.amount < rhs.amount;
    };
    return max_element(begin(spendings), end(spendings),
                       compare_by_amount)->category;
}

vector<Spending> LoadFromJson(istream& input) {
    Document doc = Load(input);
    
    vector<Spending> v;
    for (const auto& child : doc.GetRoot().AsArray()) {
        const map<string, Node> childMap = child.AsMap();
        v.push_back({
            childMap.at("category").AsString(),
            childMap.at("amount").AsInt()
        });
    }
    
    return v;
}

void TestLoadFromJson() {
    istringstream json_input(R"([
                             {"amount": 2500, "category": "food"},
                             {"amount": 1150, "category": "transport"},
                             {"amount": 5780, "category": "restaurants"},
                             {"amount": 7500, "category": "clothes"},
                             {"amount": 23740, "category": "travel"},
                             {"amount": 12000, "category": "sport"}
                             ])");
    
    const vector<Spending> spendings = LoadFromJson(json_input);
    
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

void TestJsonLibrary() {
    // Тест демонстрирует, как пользоваться библиотекой из файла json.h
    
    istringstream json_input(R"([
                             {"amount": 2500, "category": "food"},
                             {"amount": 1150, "category": "transport"},
                             {"amount": 12000, "category": "sport"}
                             ])");
    
    Document doc = Load(json_input);
    const vector<Node>& root = doc.GetRoot().AsArray();
    assert(root.size() == 3u);
    
    const map<string, Node>& food = root.front().AsMap();
    assert(food.at("category").AsString() == "food");
    assert(food.at("amount").AsInt() == 2500);
    
    const map<string, Node>& sport = root.back().AsMap();
    assert(sport.at("category").AsString() == "sport");
    assert(sport.at("amount").AsInt() == 12000);
    
    Node transport(map<string, Node>{{"category", Node("transport")}, {"amount", Node(1150)}});
    Node array_node(vector<Node>{transport});
    assert(array_node.AsArray().size() == 1u);
}

int main() {
    TestJsonLibrary();
    TestLoadFromJson();
    
    return 0;
}
