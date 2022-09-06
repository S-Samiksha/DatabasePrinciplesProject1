#ifndef _NODE_H
#define _NODE_H

#include <vector>
#include <iostream>

class Node
{

private:
    int currentSize{0};
    // n
    std::vector<int> keys;
    std::vector<Node *> childrenNodes;

public:
    bool isLeaf;

    Node(int nodeSize)
    {
        keys = std::vector<int>(nodeSize, -1);
        childrenNodes = std::vector<Node *>(nodeSize + 1);
        std::cout << "A Node has been created" << std::endl;
    };

    void insert(int key)
    {

        if (this->currentSize == this->keys.size())
        {
            throw 1;
        }

        this->keys.at(this->currentSize++) = key;
        std::cout << "Inserting " << key << " in Node. Current Node size is " << this->currentSize << std::endl;
    };

    void remove(int index)
    {
        if (this->currentSize == 0)
        {
            throw 1;
        }

        std::cout << "removed" << keys.at(index) << std::endl;
        this->keys.at(index) = 0;
        this->currentSize--;
    };

    int binarySearch(int key)
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
    };
};

#endif