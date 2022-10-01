#ifndef _BPTREE_H
#define _BPTREE_H

//#include "node.h"
#include "types.h"
#include "memorypool.h"
#include <stack>


class BPTree
{
    friend class Node;

public:
    Node *rootNode{nullptr};
    int nodeSize;
    static MemoryPool *memoryPoolInstance;

    BPTree(int nodeSize, int poolSize, int blockSize);

    // inserting a key
    Address *insert(Node *parentNode, int key, Address recordAddress, MemoryPool &disk);

    // deleting a key
    void remove(int key, int &nodesDeleted, int &nodesUpdated, int &height, MemoryPool &disk);

    // displaying the tree
    void display();

    Address queryWithNumVotesAsKey(int key, int &nodesUpdated);

    int findMinimumKeyInBPTree(Node *node);

    void search(int key);

    //Return the number of blocks accessed
    int * searchRange(int lowKey, int highKey, MemoryPool &disk);

    void updateParent(std::stack<Node *> stack, int key, int &nodesUpdated);

    // prints details and all record keys
    void printBPDetails();

    // links all leaf nodes of the BPTree
    void linkLeafNodes();

    // DFS traversal and returns by reference an array of records and number nodes
    void DFSNodes(Address currentNode, std::vector<Address > &recordList, int &nodeCount);

    // find height of tree
    int findHeight(Node *rootNode);

private:
    // helper function for when the node size is exceeded and requires splitting
    Node *linkNonLeafNode(Node *parentNode, Node *leftNode, Node *rightNode);

    Node *linkLeafNode(Node *parentNode, Node *leftNode, Node *rightNode);

    // helper function for when the node size is unbalanced and requires merging
    void merge(Node *currentNode, int deletedKey);

};

#endif