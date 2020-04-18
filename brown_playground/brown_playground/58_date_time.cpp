//
//  58_date_time.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/17/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <ctime>

#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Date {
public:
    static const int DAY_SECONDS = 86400;
    
    Date(int d, int m, int y) : day { d }, month { m }, year { y } { }
    
    time_t asTimestamp() const {
//        struct tm {
//           int tm_sec;   // seconds of minutes from 0 to 61
//           int tm_min;   // minutes of hour from 0 to 59
//           int tm_hour;  // hours of day from 0 to 24
//           int tm_mday;  // day of month from 1 to 31
//           int tm_mon;   // month of year from 0 to 11
//           int tm_year;  // year since 1900
//           int tm_wday;  // days since sunday
//           int tm_yday;  // days since January 1st
//           int tm_isdst; // hours of daylight savings time
//        }
        
        //ALWAYS set other values to 0. Otherweise unexpected behavior
        tm t {
            .tm_sec = 0,
            .tm_min = 0,
            .tm_hour = 0,
            .tm_mday = day,
            .tm_mon = month - 1,
            .tm_year = year - 1900,
            .tm_isdst = 0
        };
        
        return mktime(&t);
    }
private:
    int year;
    int month;
    int day;
};

double operator-(const Date& l, const Date& r) {
    time_t difference = l.asTimestamp() - r.asTimestamp(); //'time_t' is 'long' on mac os
    return difference / Date::DAY_SECONDS;
}

void showStandardTimeConversion() {
    //clock_t, size_t and time_t are capable of representing the system time and date as some sort of integer.
    //holds result of a call to 'time(&rawTime)'
    time_t rawTime;
    
    //This returns the current calendar time of the system in number of seconds
    //elapsed since January 1, 1970. If the system has no time, .1 is returned.
    time(&rawTime);
    //same as above
    //time_t now = time(0);
    time_t now = time(nullptr);
    
    //Uses the value pointed by timer to fill a tm structure with the values
    //that represent the corresponding time, expressed for the local timezone.
    tm* timeinfo = localtime(&rawTime);
    tm* timeinfoGMT = gmtime(&rawTime);
    
    cout << ctime(&now); //with time_t
    cout << asctime(timeinfo); //with tm struct
    cout << asctime(timeinfoGMT); //with tm struct (Greenwich Mean Time (GMT))
}

void showDataClassInAction() {
    Date d1(10, 11, 1995);
    Date d2(24, 11, 2000);
    
    int diff = d2 - d1;
    cout << diff << endl;
}

void showTimeManipulation() {
    time_t start = time(nullptr);
    auto strt = system_clock::now();
    
    this_thread::sleep_for(seconds(1));
    
    time_t end = time(nullptr);
    auto nd = system_clock::now();
    
    long cpuTimeUsed = end - start;
    long long cpuTimeUsedChrono = duration_cast<seconds>(nd - strt).count();
    
    cout << fixed;
    cout << "seconds " << cpuTimeUsed << endl;
    cout << "seconds " << cpuTimeUsedChrono << endl;
}

void showChronoTime() {
    time_point<system_clock> start = system_clock::now();
    this_thread::sleep_for(seconds(1));
    time_point<system_clock> end = system_clock::now();
    
    time_t endTimestamp = system_clock::to_time_t(end);
    
    duration<double> elapsedTimeSec = end - start;
    milliseconds elapsedTimeMillisec = duration_cast<milliseconds>(end - start);
    cout << "elapsedTime sec " << elapsedTimeSec.count() << endl;
    cout << "elapsedTime millisec " << elapsedTimeMillisec.count() << endl;
    cout << "time elapsed at " << ctime(&endTimestamp) << endl;
}

int main58() {
    //c++ time
    //1 struct tm
    //2 time_t (bridge between c-style time and chrono-style time)
    //3 time_point<system_clock>
    //4 milliseconds / seconds / etc.
    
    //showStandardTimeConversion();
    //showDataClassInAction();
    //showClockTimeManipulation();
    showChronoTime();
    
    return 0;
}
