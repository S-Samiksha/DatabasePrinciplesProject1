#include "node.h"
#include <cmath>
#include <vector>

// Node constructor
Node::Node (int nodeSize){
    // instantiate a vector with all -1s
    this->keys = std::vector<int>(nodeSize,-1);
    // instantiate a vector with all Nulls
    this->childrenNodes = std::vector<Node*> (nodeSize+1,NULL);
};

// insertion of key into a Node
void Node::insert(int key)
{
    if (this->currentSize == this->keys.size())
    {
        throw 1;
    }

    this->keys.at(this->currentSize++) = key;
    std::cout << "Inserting " << key << " in Node. Current Node size is " << this->currentSize << std::endl;
};

// removal of key from node
void Node::remove(int index)
{
    if (this->currentSize == 0)
    {
        throw 1;
    }

    std::cout << "removed" << keys.at(index) << std::endl;
    this->keys.at(index) = 0;
    this->currentSize--;
}

// returns -1 if key not found in the node
int Node::binarySearch(int key)
{
    int l = 0;
    int r = this->currentSize - 1;
    int m;

    while (l <= r)
    {

        m = (r - l) / 2;
        if (this->keys.at(m) > key)
        {
            l = m + 1;
        }
        if (this->keys.at(m) < key)
        {
            r = m - 1;
        }
        if (this->keys.at(m) == key)
        {
            return m;
        }
    }

    return -1;
}