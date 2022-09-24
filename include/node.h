#ifndef _NODE_H
#define _NODE_H

#include <vector>
#include <iostream>

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
    void *childrenNodes;

    bool isLeaf;

    int minkeys{0}, minpointers{0};

    Node();

    Node(int nodeSize, bool isLeaf);
    void printNode();

    void insertInitialInNonLeafNode(int key, Node *leftPointer, Node *rightPointer);

    void insertInitialInLeafNode(int key, void *recordPointer, Node *neighbourNode);

    void insertSubsequentPair(int key, void *NodeOrRecordPointer);

    void insertKeyInKeyArray(int key, int index);

    void remove(int remove);

    int binarySearch(int remove);

    void insertChildInPointerArray(void *child, int index);
    
    void linkToAnotherLeafNode(Node *child);

    int returnSize();

    bool isFull();

    // binary search the index to slot in the incoming key
    int binarySearchInsertIndex(int key);
};
#endif