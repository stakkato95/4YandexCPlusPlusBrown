//
//  42_disjoint_set_2.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/10/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

namespace CompactDisjointSet {
    
    class UnionByHeightDisjointSet {
    public:
        UnionByHeightDisjointSet(size_t setSize) : size { setSize } {
            parent = new int[setSize];
            //at the very beginning each element is in a set of its own (single element set)
            //-1 also denotes, that the height of the tree is 0
            memset(parent, ROOT, setSize * sizeof(int));
        }
        
        int find(int i) {
            //check for root elements
            if (parent[i] < 0) {
                return i;
            }
            
            if (i != parent[i]) {
                parent[i] = find(parent[i]);
            }
            
            //returns itself (i.e. 'i') for a single set element
            //returns root for 1st child
            //returns root for 2nd child AND causes one recursion call (1st child has value of the root,
            //and recursive call writes same value of the root to 1st child)
            return parent[i];
        }
        
        //implements strategy 'union by height', i.e. keep height as low as possible (max height = 2)
        void unionOf(int x, int y) {
            int xParent = find(x);
            int yParent = find(y);
            
            if (xParent == yParent) {
                //elements are in the same set
                return;
            }
            
            int xHeight = getHeight(xParent);
            int yHeight = getHeight(yParent);
            if (xHeight > yHeight) {
                parent[yParent] = xParent;
            } else {
                parent[xParent] = yParent;
                if (xHeight == yHeight) {
                    incrementHeight(yParent);
                }
            }
            
            printState();
        }
        
        ~UnionByHeightDisjointSet() {
            delete [] parent;
        }
        
    private:
        static const int ROOT = -1;
        
        int* parent;
        size_t size;
        
        void printState() {
            for (size_t i = 0; i < size; ++i) {
                cout << setw(2) << parent[i] << " ";
            }
            cout << endl;
        }
        
        //height of the tree is denoted by negative values
        bool getHeight(int i) {
            return abs(parent[i] + 1);
        }
        
        void incrementHeight(int i) {
            parent[i]--;
        }
    };
}

int main42() {
    CompactDisjointSet::UnionByHeightDisjointSet ds(6);
    ds.unionOf(0, 2);
    ds.unionOf(4, 2);
    ds.unionOf(3, 1);
    ds.unionOf(5, 0);
    ds.unionOf(1, 5);
    
    return 0;
}
