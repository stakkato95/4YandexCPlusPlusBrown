//
//  43_assignment_express.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/11/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <set>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

using namespace std;

namespace express {
    
    //unable to use compact disjoint set (storing height as a negative value)
    //because stations can also be negative
    class DisjointSet {
    public:
        //range of values [minValue, maxValue)
        DisjointSet(int minValue, int maxValue) : size { maxValue - minValue }, offset { minValue < 0 ? abs(minValue) : -minValue } {
            parent = new int[size];
            rank = new int[size]();
            makeSet();
        }
        
        int find(int i) {
            return find_(getOrdinal(i));
        }
        
        void unionOf(int x, int y) {
            int xParent = find(x);
            int yParent = find(y);
            
            if (xParent == yParent) {
                return;
            }
            
            if (rank[xParent] > rank[yParent]) {
                parent[yParent] = xParent;
            } else {
                parent[xParent] = yParent;
                if (rank[xParent] == rank[yParent]) {
                    rank[yParent]++;
                }
            }
            
            printSet();
        }
        
        ~DisjointSet() {
            delete [] parent;
            delete [] rank;
        }
    private:
        int* parent;
        int* rank;
        int offset;
        int size;
        
        int find_(int i) {
            if (i != parent[i]) {
                parent[i] = find_(parent[i]);
            }
            
            return parent[i];
        }
        
        int getOrdinal(int i) {
            return i + offset;
        }
        
        void makeSet() {
            for (int i = 0; i < size; ++i) {
                parent[i] = i;
            }
        }
        
        void printSet() {
            for (int i = 0; i < size; ++i) {
                cout << setw(2) << parent[i] - offset << " ";
            }
            cout << "parent" << endl;
            
            for (int i = 0; i < size; ++i) {
                cout << setw(2) << rank[i] << " ";
            }
            cout << "rank" << endl << endl;
        }
    };
            
    class ExpressScheduler {
    public:
        ExpressScheduler(int minValue, int maxValue) : routes(minValue, maxValue) {
            
        }
        
        void addConnection(int from, int to) {
            routes.unionOf(from, to);
            allStations.insert(from);
            allStations.insert(to);
        }
        
        //works only if both stations exist in the system
        int getMinimalDistance(int x, int y) {
            if (allStations.count(x) == 0 && allStations.count(y) == 0) {
                throw invalid_argument("unknown stations");
            }
            
            if (routeExists(x, y)) {
                return 0;
            }
            
            int from = min(x, y);
            int to = max(x, y);
            
            set<int>::iterator itLow = next(allStations.lower_bound(from), 1);
            set<int>::iterator itUp = prev(allStations.upper_bound(to), 2);
            
            //start at lower station an move closer to upper station
            int bestDistanceStartingAtFrom = NOT_INITED;
            while (*itLow != to) {
                if (routeExists(*itLow, to)) {
                    bestDistanceStartingAtFrom = to - *itLow;
                    break;
                }
                
                itLow++;
            }
            
            //start at lower station an move closer to upper station
            int bestDistanceStartingAtTo = NOT_INITED;
            while (*itUp != from) {
                if (routeExists(*itUp, to)) {
                    bestDistanceStartingAtTo = from - *itUp;
                    break;
                }
                
                if (itUp == begin(allStations)) {
                    break;
                }
                
                itUp--;
            }
            
            if ((bestDistanceStartingAtFrom == NOT_INITED) && (bestDistanceStartingAtTo == NOT_INITED)) {
                return to - from;
            }
            
            return min(bestDistanceStartingAtFrom, bestDistanceStartingAtTo);
        }
        
    private:
        static const int NOT_INITED = numeric_limits<int>::max();
        
        DisjointSet routes;
        set<int> allStations;
        
        bool routeExists(int from, int to) {
            return routes.find(from) == routes.find(to);
        }
    };
}

int main43() {
    express::ExpressScheduler scheduler(-2, 11);
    scheduler.addConnection(-2, 5);
    scheduler.addConnection(10, 4);
    scheduler.addConnection(5, 8);
    
    cout << scheduler.getMinimalDistance(4, 10) << endl;
    cout << scheduler.getMinimalDistance(4, -2) << endl;
    cout << scheduler.getMinimalDistance(5, 0) << endl;
    cout << scheduler.getMinimalDistance(5, 100) << endl;
    
    
//    express::DisjointSet ds(-2, 11);
//    ds.unionOf(-2, 5);
//    ds.unionOf(10, 4);
//    ds.unionOf(5, 8);
    
    vector<int> v { 1 };
    cout << boolalpha << (lower_bound(begin(v), end(v), 100) == end(v)) << endl;
    
    return 0;
}
