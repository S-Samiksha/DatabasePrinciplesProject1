#ifndef _BPTREE_H
#define _BPTREE_H

#include "node.h"
#include "types.h"
#include <stack>

class BPTree
{

public:
    Node *rootNode{nullptr};
    int nodeSize;
    BPTree(int nodeSize);

    // inserting a key
    Node **insert(Node *parentNode, int key, Address *incomingRecord);

    // deleting a key
    Address* remove(int key, int *nodesDeleted, int *nodesUpdated, int *height);

    // displaying the tree
    void display();

    Node **linkParentToNewChildrenNode(Node *parentNode, Node *leftChildNode, Node *rightChildNode);

    Node *findParentNode(Node *cursor, Node *child);

    Address* queryWithNumVotesAsKey(int key, int &nodesUpdated);

    int findMinimumKeyInBPTree(Node *node);

    void search(int key);

    void searchRange(int lowKey,int highKey,MemoryPool &disk);

    void updateParent(std::stack<Node *> stack, int key, int * nodesUpdated);

    // prints details and all record keys
    void printBPDetails();

    // links all leaf nodes of the BPTree
    void linkLeafNodes();

    void DFSNodes(Node* currentNode,std::vector<Node*> &recordList,int &nodeCount);


    int findHeight(Node *rootNode);

private:
    // helper function for when the node size is exceeded and requires splitting

    Node *linkNonLeafNode(Node *parentNode, Node *leftNode, Node *rightNode);
    Node *linkLeafNode(Node *parentNode, Node *leftNode, Node *rightNode);

    // helper function for when the node size is unbalanced and requires merging
    void merge(Node *currentNode, int deletedKey);

    // function to recursively search the subtree for the node to insert key
    int insertInternal(Node *currentSubTreeRootNode, int key);

    // remove first key-pointer pair from a split right node
    // int removeFirstKeyFromNode(Node *node);

    
};

#endif