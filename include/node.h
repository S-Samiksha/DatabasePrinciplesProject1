#ifndef _NODE_H
#define _NODE_H

#include <vector>
#include <iostream>

class Node
{

public:
    int currentSize{0};
    int currentPointerSize{0};

    // maximum Number of keys and Pointers a Node can hold
    int maxKeys{0} , maxPointers{0}; 

    std::vector<int> keys;
    
    std::vector<Node *> childrenNodes;
    
    bool isLeaf;

    Node(int nodeSize);

    void insert(int key);

    void remove(int value);

    int binarySearch(int key);

    void insertChildNode(int Index, Node* child);

    int returnSize();
    
    // binary search the index to slot in the incoming key
    int binarySearchInsertIndex(int key);

};
#endif