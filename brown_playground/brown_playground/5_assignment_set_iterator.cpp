//
//  5_assignment_set_iterator.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 3/29/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <cassert>
#include <deque>
#include <iostream>

using namespace std;


struct Node {
    Node(int v, Node* p) : value(v), parent(p) { }
    
    int value;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent;
};


class NodeBuilder {
public:
    Node* CreateRoot(int value) {
        nodes.emplace_back(value, nullptr);
        return &nodes.back();
    }
    
    Node* CreateLeftSon(Node* me, int value) {
        assert( me->left == nullptr );
        nodes.emplace_back(value, me);
        me->left = &nodes.back();
        return me->left;
    }
    
    Node* CreateRightSon(Node* me, int value) {
        assert( me->right == nullptr );
        nodes.emplace_back(value, me);
        me->right = &nodes.back();
        return me->right;
    }
    
private:
    deque<Node> nodes;
};


Node* Next(Node* me) {
    Node* next = me->right;
    
    if (next) {
        while (next->left) {
            next = next->left;
        }
        return next;
    }
    
    next = me->parent;
    if (!next) {
        return nullptr;
    }
    
    while (next && next->value <= me->value) {
        next = next->parent;
    }
    
    return next;
}


void Test1() {
    NodeBuilder nb;
    
    Node* root = nb.CreateRoot(50);
    assert(root->value == 50);
    
    Node* l = nb.CreateLeftSon(root, 2);
    Node* min = nb.CreateLeftSon(l, 1);
    Node* r = nb.CreateRightSon(l, 4);
    assert(min->value == 1);
    assert(r->parent->value == 2);
    
    nb.CreateLeftSon(r, 3);
    nb.CreateRightSon(r, 5);
    
    r = nb.CreateRightSon(root, 100);
    l = nb.CreateLeftSon(r, 90);
    nb.CreateRightSon(r, 101);
    
    nb.CreateLeftSon(l, 89);
    r = nb.CreateRightSon(l, 91);
    
    assert(Next(l)->value == 91);
    assert(Next(root)->value == 89);
    assert(Next(min)->value == 2);
    assert(Next(r)->value == 100);
    
    while (min) {
        cout << min->value << '\n';
        min = Next(min);
    }
}

void TestRootOnly() {
    NodeBuilder nb;
    Node* root = nb.CreateRoot(42);
    assert(Next(root) == nullptr);
};


int main5() {
    Test1();
//    TestRootOnly();
    return 0;
}
