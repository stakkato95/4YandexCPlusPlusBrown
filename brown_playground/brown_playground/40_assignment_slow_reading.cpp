//
//  40_assignment_slow_reading.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/10/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>

using namespace std;

class ReadingManager {
public:
    ReadingManager() : pagesReadByUser(MAX_USER_COUNT_, 0), pagePercentile(MAX_PAGE_, 0) { }
    
    void Read(int user_id, int page_count) {
        //in order not to overwrite the last percentile for that user
        size_t from = pagesReadByUser[user_id] == 0 ? 0 : pagesReadByUser[user_id] + 1;
        updatePercentile(from, page_count);
        pagesReadByUser[user_id] = page_count - 1;
    }
    
    double Cheer(int user_id) const {
        if (pagesReadByUser[user_id] == 0) {
            return 0;
        }
        
        size_t userCount = GetUserCount();
        if (userCount == 1) {
            return 1;
        }
        
        size_t pageIndex = pagesReadByUser[user_id];
        return 1.0 * (userCount - pagePercentile[pageIndex]) / (userCount - 1);
    }
    
private:
    // Статическое поле не принадлежит какому-то конкретному
    // объекту класса. По сути это глобальная переменная,
    // в данном случае константная.
    // Будь она публичной, к ней можно было бы обратиться снаружи
    // следующим образом: ReadingManager::MAX_USER_COUNT.
    static const int MAX_USER_COUNT_ = 100'000;
    static const int MAX_PAGE_ = 1'000;
    
    vector<int> pagePercentile;
    vector<int> pagesReadByUser;
    
    void updatePercentile(size_t from, size_t to) {
        for (size_t i = from; i < to; ++i) {
            pagePercentile[i]++;
        }
    }
    
    int GetUserCount() const {
        return pagePercentile[0];
    }
};

void start(istream& is) {
    ReadingManager manager;
    
    int query_count;
    is >> query_count;
    
    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        is >> query_type;
        int user_id;
        is >> user_id;
        
        if (query_type == "READ") {
            int page_count;
            is >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }
}

int main40() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    //only if cin is used as an input source
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    istringstream is(R"(12
                     CHEER 5
                     READ 1 10
                     CHEER 1
                     READ 2 5
                     READ 3 7
                     CHEER 2
                     CHEER 3
                     READ 3 10
                     CHEER 3
                     READ 3 11
                     CHEER 3
                     CHEER 1)");
    start(is);
    
    return 0;
}
