#ifndef BPTREE_H_
#define BPTREE_H_
#include "node.h"

class BPTree {
    Node *rootNode;
    
    public:
        // inserting a key
        void insert(int key);
        
        // deleting a key
        void remove(int key);

        // displaying the tree
        void display();
    private:
        // helper function for when the node size is exceeded and requires splitting
        void split(Node *currentNode, int incomingKey);
        // helper function for when the node size is unbalanced and requires merging
        void merge(Node *currentNode, int deletedKey);

};


#endif