//
//  61_final_proj_routes.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/19/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <sstream>

#include <string>
#include <string_view>

#include <vector>
#include <list>
#include <set>
#include <unordered_map>

#include <cmath>

#include <optional>
#include <variant>

#include <cassert>
#include <stdexcept>

using namespace std;

#define BUS "Bus"
#define STOP "Stop"

//summary of a route
struct RouteInfo {
    int id = UNSET;
    int stops = UNSET;
    int stopsUnique = UNSET;
    double length = UNSET;
    
    static const int UNSET = -1;
    
    bool operator==(const RouteInfo& other) const {
        return id == other.id && stops == other.stops && stopsUnique == other.stopsUnique && length == other.length;
    }
};

struct Position {
    double lat;
    double lon;
    
    bool operator==(const Position& other) const {
        return lat == other.lat && lon == other.lon;
    }
};

struct Stop {
    string name;
    Position position;
    
    bool operator==(const Stop& other) const {
        return tie(name, position) == tie(other.name, other.position);
    }
    
    bool operator<(const Stop& other) const {
        return name < other.name;
    }
};

/////////////////////////////////////////////////////////////////

struct StopStringCompare {
    using is_transparent = void;
    
    bool operator()(const Stop& lhs, const Stop& rhs) const {
        return lhs.name < rhs.name;
    }
    
    bool operator()(const Stop& lhs, const string& rhs) const {
        return lhs.name < rhs;
    }
    
    bool operator()(const string& lhs, const Stop& rhs) const {
        return lhs < rhs.name;
    }
};

using StopsSet = set<Stop, StopStringCompare>;

/////////////////////////////////////////////////////////////////

struct StopShortInfo {
    string name;
    optional<StopsSet::const_iterator> itStop;
    
    bool operator==(const StopShortInfo& other) const {
        return name == other.name;
    }
};

struct Route {
    int id;
    bool isCircular;
    list<StopShortInfo> stops;
    
    bool operator==(const Route& other) const {
        return tie(id, isCircular, stops) == tie(other.id, other.isCircular, other.stops);
    }
};

/////////////////////////////////////////////////////////////////

struct PositionRads {
    double lat;
    double lon;
};

PositionRads toRads(const Position& position) {
    static const double PI = 3.1415926535;
    return { position.lat * PI / 180, position.lon * PI / 180 };
}

double getDistanceBetween(const Stop& lhs, const Stop& rhs) {
    static const int EARTH_RADIUS_METERS = 6371000;
    
    PositionRads lhsRads = toRads(lhs.position);
    PositionRads rhsRads = toRads(rhs.position);
    
    return acos(sin(lhsRads.lat) * sin(rhsRads.lat) + cos(lhsRads.lat) * cos(rhsRads.lat)
                * cos(abs(lhsRads.lon - rhsRads.lon)) ) * EARTH_RADIUS_METERS;
}

void throwEmptyStop(const string& stopName) {
    ostringstream os;
    os << "stop" << stopName << " was not added";
    throw logic_error(os.str());
}

double getRouteDistance(const Route& route) {
    double distance = 0;
    const list<StopShortInfo>& stops = route.stops;
    
    for (auto it = begin(stops), itNext = next(begin(stops), 1); itNext != end(stops); ++it, ++itNext) {
        if (!it->itStop) {
            throwEmptyStop(it->name);
        } else if (!itNext->itStop) {
            throwEmptyStop(itNext->name);
        }
        
        distance += getDistanceBetween(*(it->itStop.value()), *(itNext->itStop.value()));
    }
    
    return route.isCircular ? distance : distance * 2;
}

void fillEmptyStops(Route& route, const StopsSet& stops) {
    list<StopShortInfo>& routeStops = route.stops;
    
    for (list<StopShortInfo>::iterator it = begin(routeStops); it != end(routeStops); ++it) {
        if (it->itStop) {
            //itStop already has an initialised iterator
            continue;
        }
        
        if (StopsSet::const_iterator itStopFound = stops.find(it->name); itStopFound != end(stops)) {
            //stop is in StopsSet, we need to get iterator to a specific value
            it->itStop = itStopFound;
        } else {
            //stop is NOT in StopsSet, we CAN NOT get iterator to a specific value
            throwEmptyStop(it->name);
        }
    }
}


/////////////////////////////////////////////////////////////////

double roundWithPrecision(double val, int precision) {
    double correction = pow(10, precision);
    return (int)(round(val * correction)) / correction;
}

/////////////////////////////////////////////////////////////////

class RouteManager {
public:
    void addStop(Stop stop) {
        stops.insert(move(stop));
    }
    void addRoute(Route route) {
        for (StopShortInfo& stopInfo : route.stops) {
            if (StopsSet::iterator it = stops.find(stopInfo.name); it != stops.end()) {
                stopInfo.itStop = it;
            }
        }
        
        routes[route.id] = move(route);
    }
    
    RouteInfo getRouteInfo(int id) const {
        if (routes.count(id) == 0) {
            return RouteInfo { id };
        }
        
        Route& route = routes.at(id);
        
        fillEmptyStops(route, stops);
        
        return RouteInfo {
            route.id,
            static_cast<int>(route.isCircular ? route.stops.size() : route.stops.size() * 2 - 1),
            static_cast<int>(route.isCircular ? route.stops.size() - 1 : route.stops.size()),
            roundWithPrecision(getRouteDistance(route), 2)
        };
    }
private:
    StopsSet stops;
    mutable unordered_map<int, Route> routes;
};

vector<string> splitBy(string_view str, const string& delimiter) {
    if (str.find(delimiter) == string::npos) {
        return { };
    }
    
    vector<string> v;
    
    while (true) {
        size_t i = str.find(delimiter);
        
        string aa(str.substr(0, i));
        v.push_back(aa);
        
        str.remove_prefix(i + delimiter.size());
        
        if (i == string::npos) {
            break;
        }
    }
    
    return v;
}

variant<Route, Stop> processInput(istream& is) {
    static string data;
    is >> data;
    
    if (data == BUS) {
        static int busId;
        
        is >> busId;
        is.ignore(2);
        
        getline(is, data);
        bool isCircular = data.find('>') != string::npos;
        
        list<StopShortInfo> stopShortInfos;
        vector<string> splittedStops = splitBy(data, isCircular ? " > " : " - ");
        for (string& stopName : splittedStops) {
            stopShortInfos.push_back({ move(stopName) });
        }
        
        return Route { busId, isCircular, stopShortInfos };
    } else if (data == STOP) {
        static double lat;
        static double lon;
        static string stopName;
        
        stopName.clear();
        is >> data;
        stopName = data;
        
        while (data[data.size() - 1] != ':') {
            is >> data;
            stopName += " " + data;
        }
        
        is.ignore(1);
        is >> lat;
        is.ignore(2);
        is >> lon;
        
        return Stop { stopName.substr(0, stopName.size() - 1), { lat, lon } };
    }
    
    return { };
}

void writeResult(ostream& os, const RouteInfo& info) {
    bool isUnknownRoute = info.stops == RouteInfo::UNSET;
    
    os << "Bus " << info.id << ": ";
    
    if (isUnknownRoute) {
        os << "not found";
    } else {
        os
        << info.stops << " stops on route, "
        << info.stopsUnique << " unique stops, "
        << info.length << " route length";
    }
    
    os << endl;
}

void run(istream& is, ostream& os) {
    int entriesCount;
    RouteManager manager;
    
    is >> entriesCount;
    for (int i = 0; i < entriesCount; ++i) {
        if (variant<Route, Stop> routeStopVariant = processInput(is); holds_alternative<Route>(routeStopVariant)) {
            manager.addRoute(get<Route>(routeStopVariant));
        } else {
            manager.addStop(get<Stop>(routeStopVariant));
        }
    }
    
    int busId;
    is >> entriesCount;
    for (int i = 0; i < entriesCount; ++i) {
        is.ignore(4);
        is >> busId;
        RouteInfo info = manager.getRouteInfo(busId);
        writeResult(os, info);
    }
}

/////////////////////////////////////////////////////////////////

void testRadiansConversion() {
    Position p { 55.611087, 37.20829 };
    PositionRads pRads = toRads(p);
    
    double lat = roundWithPrecision(pRads.lat, 4);
    double lon = roundWithPrecision(pRads.lon, 4);
    assert(lat == 0.9706 && lon == 0.6494);
}

void testGetdistanceBetween() {
    Stop a { "a", { 55.611087, 37.20829 } };
    Stop b { "b", { 55.595884, 37.209755 } };
    
    double distance = getDistanceBetween(a, b);
    assert(round(distance) == 1693);
}

/////////////////////////////////////////////////////////////////

void testRouteDistance(const StopsSet& stops, Route& route, double expectedDistance) {
    fillEmptyStops(route, stops);
    
    double distance = getRouteDistance(route);
    double distanceRounded = roundWithPrecision(distance, 2);
    assert(distanceRounded == expectedDistance);
}

void testRouteDistanceCircular() {
    const StopsSet stops {
        { "Biryulyovo Zapadnoye", { 55.574371, 37.6517 } },
        { "Biryusinka", { 55.581065, 37.64839 } },
        { "Universam", { 55.587655, 37.645687 } },
        { "Biryulyovo Tovarnaya", { 55.592028, 37.653656 } },
        { "Biryulyovo Passazhirskaya", { 55.580999, 37.659164 } },
        { "Biryulyovo Zapadnoye", { 55.574371, 37.6517 } },
    };
    
    Route route {
        .id = 1,
        .isCircular = true,
        .stops = {
            { "Biryulyovo Zapadnoye" },
            { "Biryusinka" },
            { "Universam" },
            { "Biryulyovo Tovarnaya" },
            { "Biryulyovo Passazhirskaya" },
            { "Biryulyovo Zapadnoye" }
        }
    };
    
    testRouteDistance(stops, route, 4371.02);
}

void testRouteDistanceNonCircular() {
    const StopsSet stops {
        { "Tolstopaltsevo", { 55.611087, 37.20829 } },
        { "Marushkino", { 55.595884, 37.209755 } },
        { "Rasskazovka", { 55.632761, 37.333324 } }
    };
    
    Route route {
        .id = 1,
        .isCircular = false,
        .stops = {
            { "Tolstopaltsevo" },
            { "Marushkino" },
            { "Rasskazovka" }
        }
    };
    
    testRouteDistance(stops, route, 20939.48);
}

/////////////////////////////////////////////////////////////////

void testSplitBy() {
    string str = "Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye";
    vector<string> result = splitBy(str, " > ");
    vector<string> expected = {
        "Biryulyovo Zapadnoye",
        "Biryusinka",
        "Universam",
        "Biryulyovo Tovarnaya",
        "Biryulyovo Passazhirskaya",
        "Biryulyovo Zapadnoye"
    };
    assert(result == expected);
}

/////////////////////////////////////////////////////////////////

template<typename T>
void testProcessInput(const string& input, const T& expected) {
    istringstream in(input);
    
    variant<Route, Stop> routeStopVariant = processInput(in);
    assert(holds_alternative<T>(routeStopVariant));
    
    T result = get<T>(routeStopVariant);
    assert(result == expected);
}

void testProcessBusNotCircularInput() {
    string input = "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka";
    
    Route expected {
        .id = 750,
        .isCircular = false,
        .stops = {
            { "Tolstopaltsevo" },
            { "Marushkino" },
            { "Rasskazovka" }
        }
    };
    
    testProcessInput(input, expected);
}

void testProcessBusCircularInput() {
    string input = "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye";
    
    Route expected {
        .id = 256,
        .isCircular = true,
        .stops = {
            { "Biryulyovo Zapadnoye" },
            { "Biryusinka" },
            { "Universam" },
            { "Biryulyovo Tovarnaya" },
            { "Biryulyovo Passazhirskaya" },
            { "Biryulyovo Zapadnoye" }
        }
    };
    
    testProcessInput(input, expected);
}

void testProcessBusstop() {
    string input = "Stop Tolstopaltsevo: 55.611087, 37.20829";
    
    Stop expected { "Tolstopaltsevo", { 55.611087, 37.20829 } };
    
    testProcessInput(input, expected);
}

/////////////////////////////////////////////////////////////////

void testRouteInfo(const StopsSet& stops,
                   int busId,
                   const RouteInfo& expected,
                   const Route* const route = nullptr) {
    RouteManager manager;
    
    if (route) {
        manager.addRoute(*route);
    }
    
    for (const Stop& s : stops) {
        manager.addStop(s);
    }
    
    RouteInfo result = manager.getRouteInfo(busId);
    assert(result == expected);
}

void testRouteInfoCircular() {
    const StopsSet stops {
        { "Biryulyovo Zapadnoye", { 55.574371, 37.6517 } },
        { "Biryusinka", { 55.581065, 37.64839 } },
        { "Universam", { 55.587655, 37.645687 } },
        { "Biryulyovo Tovarnaya", { 55.592028, 37.653656 } },
        { "Biryulyovo Passazhirskaya", { 55.580999, 37.659164 } },
        { "Biryulyovo Zapadnoye", { 55.574371, 37.6517 } },
    };
    
    Route route {
        .id = 256,
        .isCircular = true,
        .stops = {
            { "Biryulyovo Zapadnoye" },
            { "Biryusinka" },
            { "Universam" },
            { "Biryulyovo Tovarnaya" },
            { "Biryulyovo Passazhirskaya" },
            { "Biryulyovo Zapadnoye" }
        }
    };
    
    RouteInfo expected {
        .id = 256,
        .stops = 6,
        .stopsUnique = 5,
        .length = 4371.02
    };
    
    testRouteInfo(stops, 256, expected, &route);
}

void testRouteInfoNonCircular() {
    const StopsSet stops {
        { "Tolstopaltsevo", { 55.611087, 37.20829 } },
        { "Marushkino", { 55.595884, 37.209755 } },
        { "Rasskazovka", { 55.632761, 37.333324 } }
    };
    
    Route route {
        .id = 750,
        .isCircular = false,
        .stops = {
            { "Tolstopaltsevo" },
            { "Marushkino" },
            { "Rasskazovka" }
        }
    };
    
    const RouteInfo expected {
        .id = 750,
        .stops = 5,
        .stopsUnique = 3,
        .length = 20939.48
    };
    
    testRouteInfo(stops, 750, expected, &route);
}

void testRouteInfoUnknownRoute() {
    const StopsSet stops;
    const RouteInfo expected { .id = 751 };
    testRouteInfo(stops, 751, expected);
}

/////////////////////////////////////////////////////////////////

void testWriteOutput(const RouteManager& manager, int busId, const string& expectedOutput) {
    RouteInfo info = manager.getRouteInfo(busId);
    
    ostringstream os;
    writeResult(os, info);
    
    assert(os.str() == expectedOutput);
}

void testWriteOutputExistingBus() {
    const StopsSet stops {
        { "Tolstopaltsevo", { 55.611087, 37.20829 } },
        { "Marushkino", { 55.595884, 37.209755 } },
        { "Rasskazovka", { 55.632761, 37.333324 } }
    };
    
    Route route {
        .id = 750,
        .isCircular = false,
        .stops = {
            { "Tolstopaltsevo" },
            { "Marushkino" },
            { "Rasskazovka" }
        }
    };
    
    string expected = "Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length";
    
    RouteManager manager;
    manager.addRoute(route);
    
    for (const Stop& s : stops) {
        manager.addStop(s);
    }
    
    testWriteOutput(manager, 750, expected);
}

void testWriteOutputNonExistingBus() {
    testWriteOutput(RouteManager(), 751, "Bus 751: not found");
}

/////////////////////////////////////////////////////////////////

int main() {
//    //test utility funcitons
//    testSplitBy();
//
//    //test distance calculation
//    testRadiansConversion();
//    testGetdistanceBetween();
//    testRouteDistanceCircular();
//    testRouteDistanceNonCircular();
//
//    //test string processor
//    testProcessBusCircularInput();
//    testProcessBusNotCircularInput();
//    testProcessBusstop();
//
//    //test output methods
//    testRouteInfoCircular();
//    testRouteInfoNonCircular();
//    testRouteInfoUnknownRoute();
//    testWriteOutputExistingBus();
//    testWriteOutputNonExistingBus();
    
    string input = R"(10
Stop Tolstopaltsevo: 55.611087, 37.20829
Stop Marushkino: 55.595884, 37.209755
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
Stop Biryusinka: 55.581065, 37.64839
Stop Universam: 55.587655, 37.645687
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
3
Bus 256
Bus 750
Bus 751)";
    
    istringstream in(input);
    run(in, cout);
    
    return 0;
}
