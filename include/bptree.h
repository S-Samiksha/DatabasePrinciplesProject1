#ifndef _BPTREE_H
#define _BPTREE_H

#include "node.h"

class BPTree
{

public:
    Node *rootNode{NULL};
    int nodeSize;
    BPTree(int nodeSize);

    // inserting a key
    void insert(int key);

    // deleting a key
    void remove(int key);

    // displaying the tree
    void display();

    // make a test BPTree
    void makeTestTree();

private:
    // helper function for when the node size is exceeded and requires splitting
    int split(Node *currentNode, int incomingKey);

    // helper function for when the node size is unbalanced and requires merging
    void merge(Node *currentNode, int deletedKey);

    // function to recursively search the subtree for the node to insert key
    int BPTree::insertInternal(Node *currentSubTreeRootNode, int key);
};

#endif