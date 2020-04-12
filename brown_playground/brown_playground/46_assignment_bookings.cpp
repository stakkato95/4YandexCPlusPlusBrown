//
//  46_assignment_bookings.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/11/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class HotelManager {
public:
    HotelManager() {
        
    }
    
    void book(int time, const string& hotelName, int clientId, int roomsCount) {
        //assumption: hotel manager can't accept more than one request at a time
        //so for each point in time corresponds only one event
        roomEvents[hotelName][time] = { DUMMY_EVENT_ID, roomsCount };
        clientEvents[hotelName][time] = { DUMMY_EVENT_ID, clientId };
        
        lastPointInTime = time;
        DUMMY_EVENT_ID++;
    }
    
    int getClients(const string& hotelName) const {
        using Iterator = HotelClientEvents::const_iterator;
        
        if (clientEvents.count(hotelName) == 0) {
            return 0;
        }
        
        //old style vs new style
        //pair<Iterator, Iterator> bounds = getTimeBounds(clientEvents.at(hotelName));
        auto [timeLower, timeUpper] = getTimeBounds(clientEvents.at(hotelName));
        
        unordered_set<int> clients;
        for (Iterator it = timeLower; it != timeUpper; it++) {
            clients.insert(it->second.clientId);
        }
        
        return (int) clients.size();
    }
    
    int getRooms(const string& hotelName) const {
        using Iterator = HotelRoomEvents::const_iterator;
        
        if (clientEvents.count(hotelName) == 0) {
            return 0;
        }
        
        auto [timeLower, timeUpper] = getTimeBounds(roomEvents.at(hotelName));
        
        int roomsCount = 0;
        for (Iterator it = timeLower; it != timeUpper; ++it) {
            roomsCount += it->second.roomsCount;
        }
        
        return roomsCount;
    }
private:
    struct RoomEvent {
        int eventId;
        int roomsCount;
    };
    
    struct ClientEvent {
        int eventId;
        int clientId;
    };
    
    using HotelRoomEvents = map<int, RoomEvent>;
    using HotelClientEvents = map<int, ClientEvent>;
    
    static const int SEC_IN_DAY = 86400;
    
    int DUMMY_EVENT_ID = 0;
    int lastPointInTime = 0;
    
    unordered_map<string, HotelRoomEvents> roomEvents;
    unordered_map<string, HotelClientEvents> clientEvents;
    
    template<typename TEvent, typename Iterator = typename map<int, TEvent>::const_iterator>
    pair<Iterator, Iterator> getTimeBounds(const map<int, TEvent>& events) const {
        int timeStart = lastPointInTime + 1 - SEC_IN_DAY;
        Iterator timeLower = events.lower_bound(timeStart);
        Iterator timeUpper = events.upper_bound(lastPointInTime);
        
        return { timeLower, timeUpper };
    }
};

int main() {
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
