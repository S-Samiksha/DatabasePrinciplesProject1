#include "node.h"
#include <cmath>
#include <vector>
#include <bits/stdc++.h>
#include "types.h"

// Node constructor
Node::Node(){};

Node::Node(int nodeSize, bool isLeaf)
{
    this->isLeaf = isLeaf;
    this->maxKeySize = nodeSize;
    this->maxPointerSize = nodeSize + 1;
    this->keys = new int[this->maxKeySize]{0};
    this->childrenNodes = new void *[this->maxPointerSize]
    { nullptr };
};

// prints a node's info and contents
void Node::printNode(){
    std::cout<<"**********************"<<std::endl;
    std::cout<<"Node's address: "<<this<<std::endl;
    std::cout<<"currentKeySize :"<<this->currentKeySize<<std::endl;
    std::cout<<"currentPointerSize :"<<this->currentPointerSize<<std::endl;
    std::string nodeType = this->isLeaf?"LEAF":"InternalNode";
    std::cout<<"node type :"<<nodeType<<std::endl;

    std::cout<<"keyArray: [";
    for(int i=0;i<this->maxKeySize; i++){
        std::cout<<this->keys[i]<<",";
    };
    std::cout<<"]"<<std::endl;
    std::cout<<"PointerArray: [";
    for(int i=0;i<this->maxPointerSize; i++){
        std::cout<<((Node**)this->childrenNodes)[i]<<",";
    };
    std::cout<<"]"<<std::endl;
    std::cout<<"**********************"<<std::endl;

}

// if leaf node initial insert state is {key,pointer,...., next neighbour}
// if non-leaf node initial insert state is {pointerL,key,pointerR}
void Node::insertInitialInNonLeafNode(int key, Node *leftPointer, Node *rightPointer)
{
    // check if that the node is indeed a non-leaf node
    if (this->isLeaf || this->currentKeySize != 0)
    {
        std::cout << "cannot insertNonLeafNodeInitialPairs with node with size != 0 or NonLeaf" << std::endl;
        throw 1;
    }

    // insert the initial nodes
    this->insertKeyInKeyArray(key, 0);
    this->insertChildInPointerArray(leftPointer, 0);
    this->insertChildInPointerArray(rightPointer, 1);
    // std::cout << "left pointer " << ((Node **)this->childrenNodes)[0] << " right pointer " << ((Node **)this->childrenNodes)[1] << std::endl;
}

void Node::insertInitialInLeafNode(int key, void *recordPointer, Node *neighbourNode)
{

    // check if that the node is indeed a non-leaf node
    if (!this->isLeaf || this->currentKeySize != 0)
    {
        std::cout << "cannot insertLeafNodeInitialPairs with node with size != 0 or NonLeaf" << std::endl;
        throw 1;
    }

    this->insertKeyInKeyArray(key, 0);
    this->insertChildInPointerArray(recordPointer, 0);
    // link to neighbour
    this->insertChildInPointerArray(neighbourNode, this->maxPointerSize - 1);
}

// binary searches for the insertion slot and inserts a key-pointer pair
// returns and does nothing if inserted key is a dupe
void Node::insertSubsequentPair(int key, void *nodeOrRecordPointer)
{

    // if node is full, throw error
    if (this->isFull())
    {
        std::cout << "Key array is full" << std::endl;
        throw 1;
    }
    int insertionIndex = this->binarySearchInsertIndex(key);
    // in the case that the key is a duplicate
    if (insertionIndex == -1)
    {
        std::cout << "duplicate key " << key << " detected and not added into the B+ Tree" << std::endl;
        return;
    }

    this->insertKeyInKeyArray(key, insertionIndex);
    this->insertChildInPointerArray(nodeOrRecordPointer, insertionIndex + 1);
}

// inserts key,pointer pairs into the B+ tree at index
void Node::insertKeyInKeyArray(int key, int index)
{
    // if node is full
    if (this->isFull())
    {
        std::cout << "Node Key array is filled! Need to split!" << std::endl;
        std::cout << index << std::endl;
        throw 1;
    };

    // in the case that the key is a duplicate
    if (index == -1)
    {
        std::cout << "duplicate key " << key << " detected and not added into the B+ Tree" << std::endl;
        return;
    }

    // push all elements to the right of the inserted element
    int i;
    // push the elements on the right side of the insertion index 1 slot right
    for (int i = this->currentKeySize; i > index; i--)
    {
        (this->keys)[i] = (this->keys)[i - 1];
    }

    // insert key
    (this->keys)[index] = key;

    this->currentKeySize++;
};

// inserts a child pointer(record* or Node*) to the pointer array of the node in the index position
// pushes every element after the insertion index to the right
void Node::insertChildInPointerArray(void *child, int index)
{
    // if pointer node is full
    if (this->currentPointerSize == this->maxPointerSize)
    {
        std::cout << "Node pointer array is filled! Need to split!" << std::endl;
        throw 1;
    };

    // for insertion need push the rest of the pointers right
    // if child to be inserted is a Record
    if (this->isLeaf)
    {
        // push all elements to the right of the inserted element
        int i;
        // push the elements on the right side of the insertion index 1 slot right
        for (i = this->currentPointerSize; i > index; i--)
        {
            ((Address **)this->childrenNodes)[i] = ((Address **)this->childrenNodes)[i - 1];
        }

        // insert Address
        ((Address **)this->childrenNodes)[index] = (Address *)child;
    }
    else
    {
        // push all elements to the right of the inserted element
        int i;
        // push the elements on the right side of the insertion index 1 slot right
        for (i = currentPointerSize; i > index; i--)
        {
            ((Node **)this->childrenNodes)[i] = ((Node **)this->childrenNodes)[i + 1];
        }

        // insert Node
        ((Node **)this->childrenNodes)[index] = (Node *)child;
    }

    this->currentPointerSize++;
};

// check if Node is full
bool Node::isFull()
{
    if (this->currentKeySize == this->maxKeySize)
    {
        return true;
    }

    return false;
}

// returns -1 if key not found in the node
// only within the node
int Node::binarySearch(int key)
{
    int l = 0;
    int r = this->currentKeySize - 1;
    int m;

    while (l <= r)
    {

        m = (r + l) / 2;
        if (this->keys[m] > key)
        {
            r = m - 1;
        }
        if (this->keys[m] < key)
        {
            l = m + 1;
        }
        if (this->keys[m] == key)
        {
            return m;
        }
    }

    return -1;
}

int Node::returnSize()
{
    return this->currentKeySize;
};

// returns -1 if duplicate key is found in the node
// returns the index to insert the value at
int Node::binarySearchInsertIndex(int key)
{
    int l = 0;
    int r = this->currentKeySize - 1;
    int m;

    while (l <= r)
    {

        m = (r + l) / 2;
        if (this->keys[m] > key)
        {
            r = m - 1;
        }
        if (this->keys[m] < key)
        {
            l = m + 1;
        }
        if (this->keys[m] == key)
        {
            return -1;
        }
    }

    return l;
}

// removal of key from node within the node
void Node::remove(int value)
{
    int index = -1;
    if (this->currentKeySize == 0)
    {
        throw 1;
    }
    //find the index of the key 
     index = this->binarySearch(value);

    /*
    within the leaf node removal 
    1. Remove the key
    2. Remove the pointer to the block --> deallocate 
    */
   std::cout<<"index:" <<index <<"value" << keys[index]<< "childnode" << ((Node**)this->childrenNodes)[index] << std::endl;


}
