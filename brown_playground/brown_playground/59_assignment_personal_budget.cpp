//
//  59_assignment_personal_budget.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/18/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>

#include <string>
#include <map>
#include <numeric>

#include <ctime>
#include <chrono>

#define EARN "Earn"
#define COMPUTE_INCOME "ComputeIncome"
#define PAY_TAX "PayTax"

using namespace std;
using namespace std::chrono;

namespace PersonalBudget {

    class IncomeManager {
    public:
        void earn(time_t start, time_t end, double amount) {
            //+1 to include the last day from range
            size_t daysCount = (end - start) / SEC_DAY + 1;
            double amountPerDay = amount / daysCount;
            
            for (size_t i = 0; i < daysCount; i++) {
                time_t date = start + i * SEC_DAY;
    //            cout << ctime(&date);
                money[start + i * SEC_DAY] += amountPerDay;
            }
    //        cout << endl;
        }
        
        double computeIncome(time_t start, time_t end) const {
    //        cout << ctime(&start);
    //        cout << ctime(&dayAfterEnd) << endl;
            
            map<time_t, double>::const_iterator itLower = money.lower_bound(start);
            map<time_t, double>::const_iterator itUpper = money.upper_bound(end);
            
            double income = accumulate(itLower, itUpper, 0.0, [](double sum, const pair<time_t, double>& p) { return sum + p.second; });
            return income;
        }
        
        void payTax(time_t start, time_t end) {
    //        cout << ctime(&start);
    //        cout << ctime(&end) << endl;
            
            map<time_t, double>::iterator itLower = money.lower_bound(start);
            map<time_t, double>::iterator itUpper = money.upper_bound(end);
            
            for_each(itLower, itUpper, [](pair<const time_t, double>& p) { p.second *= INCOME_RESIDUAL; });
        }
    private:
        static const long SEC_DAY = 60 * 60 * 24;
        constexpr static double INCOME_RESIDUAL = 0.87;
        
        map<time_t, double> money;
    };

    void readTime(istream& is, time_t& time) {
        int year;
        int month;
        int day;
        
        is >> year;
        is.ignore(1);
        is >> month;
        is.ignore(1);
        is >> day;
        
        tm t {
            .tm_sec = 0,
            .tm_min = 0,
            .tm_hour = 0,
            .tm_mday = day,
            .tm_mon = month - 1,
            .tm_year = year - 1900,
            .tm_isdst = 0
        };
        
        time = mktime(&t);
    }

    void run(istream& in, ostream& os) {
        IncomeManager manager;
        
        string command;
        time_t startDate;
        time_t endDate;
        double money;
        
        int commandsCount;
        in >> commandsCount;
        
        for (int i = 0; i < commandsCount; ++i) {
            in >> command;
            readTime(in, startDate);
            readTime(in, endDate);
    //        cout << ctime(&startDate);
    //        cout << ctime(&endDate) << endl;
            
            if (command == EARN) {
                in >> money;
                manager.earn(startDate, endDate, money);
            } else if (command == COMPUTE_INCOME) {
                os << manager.computeIncome(startDate, endDate) << endl;
            } else if (command == PAY_TAX) {
                manager.payTax(startDate, endDate);
            }
        }
    }
}

int main() {
    using namespace PersonalBudget;
    
    string input = R"(8
Earn 2000-01-02 2000-01-06 20
ComputeIncome 2000-01-01 2001-01-01
PayTax 2000-01-02 2000-01-03
ComputeIncome 2000-01-01 2001-01-01
Earn 2000-01-03 2000-01-03 10
ComputeIncome 2000-01-01 2001-01-01
PayTax 2000-01-03 2000-01-03
ComputeIncome 2000-01-01 2001-01-01)";
    istringstream is(input);
//    ifstream ifs("59_assignment_personal_budget.txt");
    
    time_point<system_clock> startChrono = system_clock::now();
    time_t start = time(nullptr);
    run(is, cout);
    time_point<system_clock> finishChrono = system_clock::now();
    time_t finish = time(nullptr);
    
    double cpuTimeUsed = difftime(finish, start);
    long long cpuTimeUsedChrono = duration_cast<milliseconds>(finishChrono - startChrono).count();
    cout << "running time seconds " << cpuTimeUsed << endl;
    cout << "running time milliseconds " << cpuTimeUsedChrono << endl;
    
    return 0;
}
