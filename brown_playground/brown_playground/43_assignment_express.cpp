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
        
    private:
        int* parent;
        int* rank;
        int offset;
        int size;
        
        int find_(int i) {
            if (i != parent[i]) {
                parent[i] = find(parent[i]);
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
}

int main() {
    express::DisjointSet ds(-2, 11);
    ds.unionOf(-2, 5);
    ds.unionOf(10, 4);
    ds.unionOf(5, 8);
    
    return 0;
}
