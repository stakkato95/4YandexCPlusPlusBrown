//
//  41_disjoint_set.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/10/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

using namespace std;

namespace RankDisjointSet {
    
    class DisjointSet {
    public:
        DisjointSet(size_t setSize) : size { setSize } {
            parent = new int[size];
            rank = new int[size](); //'new int[size]();' to set all values to zero. important '()' at the end
            makeSet();
        }
        
        int find(int i) {
            //        recursive implementation
            //        //means that an element is a prent of itself
            //        if (parent[i] == i) {
            //            return i;
            //        }
            //        //an element is not a prent of itself. surch further
            //        return find(parent[i]);
            
            
            //        loop implementation
            //        while (i != parent[i]) {
            //            i = parent[i];
            //        }
            //        return i;
            
            
            //        path compression hueristic
            //root of a tree                                 - find() returns root
            //1st child of the root                          - find() returns root
            //append 'element' to 1st child                  - find('element') returns element itself, so the 'element' is appended to the 1st child
            //append 'subtree' (max length 1) to '2nd child' - find('2nd child') returns root; AND '2nd child' is appended directly to the root
            
            if (i != parent[i]) {
                parent[i] = find(parent[i]);
            }
            
            return parent[i];
        }
        
        void unionOf(int x, int y) {
            int xParent = find(x);
            int yParent = find(y);
            
            if (xParent == yParent) {
                //both elements already in the same tree
                return;
            }
            
            if (rank[xParent] > rank[yParent]) {
                parent[yParent] = xParent;
            } else {
                parent[xParent] = yParent;
                if (rank[yParent] == rank[xParent]) {
                    //y has become exactly one element taller
                    rank[yParent]++;
                }
            }
            
            printState();
        }
        
        ~DisjointSet() {
            delete [] parent;
            delete [] rank;
        }
    private:
        size_t size;
        int* parent;
        int* rank;
        
        void makeSet() {
            for (int i = 0; i < size; ++i) {
                parent[i] = i;
            }
        }
        
        void printState() {
            for (size_t i = 0; i < size; ++i) {
                cout << parent[i] << " ";
            }
            cout << "parent" << endl;
            
            for (size_t i = 0; i < size; ++i) {
                cout << rank[i] << " ";
            }
            cout << "rank" << endl << endl;
        }
    };
}

int main41() {
    //for a disjoint set (aka find-union) to be effective, it should have a very flat structure
    //if we don't use 'path compression' in the worst case a disjoint set could look like a linked list, i.e. a single chain of elements
    //'path compression' ensures max length of 2
    
    RankDisjointSet::DisjointSet ds(6);
    ds.unionOf(0, 2);
    ds.unionOf(4, 2);
    ds.unionOf(3, 1);
    ds.unionOf(5, 0);
    ds.unionOf(1, 5);
    
    cout << boolalpha; //enable 'true / false' bool output
    cout << "4 and 2 in same set " << (ds.find(4) == ds.find(0)) << endl;
    cout << "1 and 0 in same set " << (ds.find(1) == ds.find(0)) << endl;
    
    return 0;
}
