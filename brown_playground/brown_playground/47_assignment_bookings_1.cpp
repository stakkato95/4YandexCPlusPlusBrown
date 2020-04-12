//
//  47_assignment_bookings_1.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/12/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <queue>
#include <string>

using namespace std;

class HotelManager {
public:
    void book(int time, const string& hotelName, int clientId, int roomsCount) {
        lastPointInTime = time;
        hotels[hotelName].book(time, clientId, roomsCount);
    }
    
    int getClients(const string& hotelName) const {
        if (hotels.count(hotelName) == 0) {
            return 0;
        }
        
        return hotels.at(hotelName).getClients(getSmallestAllowedTime());
    }
    
    int getRooms(const string& hotelName) const {
        if (hotels.count(hotelName) == 0) {
            return 0;
        }
        
        return hotels.at(hotelName).getRooms(getSmallestAllowedTime());
    }
private:
    struct Booking {
        int time;
        int clientId;
        int rooms;
    };
    
    class Hotel {
    public:
        void book(int time, int clientId, int roomsCount) {
            bookings.push({ time, clientId, roomsCount });
            
            currentlyBookedRooms += roomsCount;
            clientsToBookingsMapping[clientId] += roomsCount;
        }
        
        int getClients(int smallestAllowedTime) const {
            popOldBookings(smallestAllowedTime);
            return (int)(clientsToBookingsMapping.size());
        }
        
        int getRooms(int smallestAllowedTime) const {
            popOldBookings(smallestAllowedTime);
            return currentlyBookedRooms;
        }
    private:
        mutable int currentlyBookedRooms = 0;
        mutable queue<Booking> bookings;
        
        //one client can make multiple booking-transactions
        //so we also need to keep track of number of booked rooms
        mutable map<int, int> clientsToBookingsMapping;
        
        void popOldBookings(int smallestAllowedTime) const {
            while (bookings.front().time <= smallestAllowedTime) {
                const Booking& booking = bookings.front();
                bookings.pop();
                
                currentlyBookedRooms -= booking.rooms;
                clientsToBookingsMapping[booking.clientId] -= booking.rooms;
                if (clientsToBookingsMapping[booking.clientId] == 0) {
                    clientsToBookingsMapping.erase(booking.clientId);
                }
            }
        }
    };
    
    static const int SEC_IN_DAY = 86400;
    
    int lastPointInTime = 0;
    unordered_map<string, Hotel> hotels;
    
    int getSmallestAllowedTime() const {
        return lastPointInTime - SEC_IN_DAY;
    }
};

int main47() {
    //faster reading cin
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string input = R"(11
    CLIENTS Marriott
    ROOMS Marriott
    BOOK 10 FourSeasons 1 2
    BOOK 10 Marriott 1 1
    BOOK 86409 FourSeasons 2 1
    CLIENTS FourSeasons
    ROOMS FourSeasons
    CLIENTS Marriott
    BOOK 86410 Marriott 2 10
    ROOMS FourSeasons
    ROOMS Marriott
    )";
    istringstream is(input);
    
    HotelManager manager;
    
    int commandsCount;
    is >> commandsCount;
    
    string data;
    for (int i = 0; i < commandsCount; ++i) {
        is >> data;
        if (data == "BOOK") {
            int time;
            string hotelName;
            int clientId;
            int roomsCount;
            
            is >> time;
            is >> hotelName;
            is >> clientId;
            is >> roomsCount;
            
            manager.book(time, hotelName, clientId, roomsCount);
        } else if (data == "CLIENTS") {
            is >> data;
            cout << manager.getClients(data) << endl;
        } else {
            is >> data;
            cout << manager.getRooms(data) << endl;
        }
    }
    
    return 0;
}
