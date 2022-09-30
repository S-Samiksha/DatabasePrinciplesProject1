#ifndef _NODE_H
#define _NODE_H

#include <vector>
#include <iostream>
#include "types.h"

// struct Address;
// 200B block size
class Node
{
    friend class BPTree;

public:
    // current number of keys and pointers a Node is holding
    int currentKeySize{0}; 
    int currentPointerSize{0}; 
    int maxKeySize{0}; 
    int maxPointerSize{0};
    // switch all vectors to arrays
    // keys instantiated to 0 initially
    int *keys;
    // childrenNodes can either be &Record &Node or nullptr(initially)
    Address *childrenNodes{nullptr};

    bool isLeaf;
    Address addressInDisk{NULL};

    Node();

    Node(int nodeSize, bool isLeaf);
    void printNode();

    void insertInitialInNonLeafNode(int key, Address leftPointer, Address rightPointer);

    void insertInitialInLeafNode(int key, Address recordPointer, Address neighbourNode);

    void insertSubsequentPair(int key, Address NodeOrRecordPointer);

    void insertKeyInKeyArray(int key, int index);

    Address remove(int remove);

    int binarySearch(int remove);

    void insertChildInPointerArray(Address child, int index);

    void linkToAnotherLeafNode(Address child);

    bool hasRightNeighbour(int index);

    bool hasLeftNeighbour(int index);

    int removeFirstKeyFromNode();

    int returnSize();

    bool isFull();

    // binary search the index to slot in the incoming key
    int binarySearchInsertIndex(int key);
};
#endif

