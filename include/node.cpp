#include "node.h"
#include <cmath>
#include <vector>
#include <bits/stdc++.h>

// Node constructor
Node::Node(int nodeSize)
{
    // instantiate a vector with all -1s
    this->maxKeys = nodeSize;
    // instantiate a vector with all Nulls
    //nodeSize + 1 because it can maintain n+1 pointers if there are n keys 
    this->maxPointers = nodeSize + 1;
    this->childrenNodes = std::vector<Node*>();
    this->keys = std::vector<int>(nodeSize, -1);
};

// insertion of key into a Node
void Node::insert(int key)
{
    if (this->currentSize == this->maxKeys)
    {
        std::cout << "Node filled! Need to split!" << std::endl; 
        return; 
    }

    this->keys.push_back(key);
    sort(keys.begin(), keys.end());
    this->currentSize++;
    std::cout << "Inserting " << key << " in Node. Current Node size is " << this->currentSize << std::endl;
};

// removal of key from node within the node 
void Node::remove(int value)
{
    int index = -1;
    if (this->currentSize == 0)
    {
        throw 1;
    }
    index = binarySearch(value);
    if (index == -1){
        std::cout<<"value not found"<<std::endl;
        return;
    }
    else{
        keys.erase(keys.begin()+index); //auto adjusted vector
    }
    
    std::cout << "removed key : " << value << std::endl;
    this->keys.at(index) = 0;
    this->currentSize--;
}

// returns -1 if key not found in the node
//only within the node 
int Node::binarySearch(int key)
{
    int l = 0;
    int r = this->currentSize - 1;
    int m;

    while (l <= r)
    {

        m = (r + l) / 2;
        if (this->keys.at(m) > key)
        {
            r = m - 1;
        }
        if (this->keys.at(m) < key)
        {
            l = m + 1;
        }
        if (this->keys.at(m) == key)
        {
            return m;
        }
    }

    return -1;
}

void Node::insertChildNode(int Index, Node* child){

    if (this->currentPointerSize == this->maxPointers)
    {
        std::cout << "Node filled! Need to split!" << std::endl; 
    }
    this->childrenNodes.at(Index)=child;
    this->currentPointerSize++;


};

int Node::returnSize(){
    return this->currentSize;
};

// returns -1 if duplicate key is found in the node
// returns the index to insert the value at
int Node::binarySearchInsertIndex(int key)
{
    int l = 0;
    int r = this->currentSize - 1;
    int m;

    while (l <= r)
    {

        m = (r + l) / 2;
        if (this->keys.at(m) > key)
        {
            r = m - 1;
        }
        if (this->keys.at(m) < key)
        {
            l = m + 1;
        }
        if (this->keys.at(m) == key)
        {
            return -1;
        }
    }

    return l;
}
