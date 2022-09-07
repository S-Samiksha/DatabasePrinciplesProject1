#ifndef _NODE_H
#define _NODE_H

#include <vector>
#include <iostream>

class Node
{

public:
    int currentSize{0};
    // n
    std::vector<int> keys;
    
    std::vector<Node *> childrenNodes;
    
    bool isLeaf;

    Node(int nodeSize);

    void insert(int key);

    void remove(int index);

    int binarySearch(int key);
};
#endif