#include "bptree.h"
#include <stdio.h>
#include <iostream>
#include "node.h"
#include <queue>
#include <vector>

BPTree::BPTree(int nodeSize)
{
    this->nodeSize = nodeSize;
}

// inserting a key
void BPTree::insert(int key){

};

void BPTree::makeTestTree()
{
    Node rootNode = Node(3);
    Node leafNode1 = Node(3);
    Node leafNode2 = Node(3);
    Node leafNode3 = Node(3);

    this->rootNode = &rootNode;

    // first layer keys
    rootNode.insert(30);
    rootNode.insert(50);

    // second layer keys

    // 1st subtree
    leafNode1.insert(10);
    leafNode1.insert(20);

    // 2nd subtree
    leafNode2.insert(30);
    leafNode2.insert(40);

    // 3rd subtree
    leafNode3.insert(50);
    leafNode3.insert(60);
    leafNode3.insert(70);

    // linking layers together
    rootNode.childrenNodes.at(0) = &leafNode1;
    rootNode.childrenNodes.at(1) = &leafNode2;
    rootNode.childrenNodes.at(2) = &leafNode3;

    // std::cout << this->rootNode->keys.at(1) <<std::endl;
};

// deleting a key
void BPTree::remove(int key){};

// displaying the tree
// do a BFS of the B+tree
void BPTree::display()
{
    std::priority_queue<std::vector<Node *>> pq;

    std::vector<Node *> rootLevel = std::vector<Node *>{this->rootNode};
    pq.push(rootLevel);
    int level = 0;
    while (!pq.empty())
    {
        // gets current top Node and removes the top Node from the queue
        std::vector<Node *> currentLevel = (pq.top());
        pq.pop();

        // display keys in each node in each level
        std::cout << "level: " << level << std::endl;

        // to store nodes in next level
        std::vector<Node *> nextLevel;

        for (int i = 0; i < currentLevel.size(); i++)
        {

            Node *currentNode = currentLevel.at(i);

            std::cout << "Node: " << i << std::endl;
            std::cout << "here" <<currentNode->keys.at(0)<< std::endl;

            for (int j = 0; j < currentNode->currentSize; j++)
            {
                std::cout << currentNode->keys.at(i) << std::endl;

                // add children nodes to nextLevel nodes if it is not NULL
                if (currentNode->childrenNodes.at(i) != NULL)
                {
                    nextLevel.push_back(currentNode->childrenNodes.at(i));
                }
            }
            // add the rightmost Node pointer if it exists
            if (currentNode->childrenNodes.at(-1) != NULL)
            {
                nextLevel.push_back(currentNode->childrenNodes.at(-1));
            }
        }

        pq.push(nextLevel);
    }
    std::cout << "Finished traversal" << std::endl;
};

// helper function for when the node size is exceeded and requires splitting
void BPTree::split(Node *currentNode, int incomingKey){};

// helper function for when the node size is unbalanced and requires merging
void BPTree::merge(Node *currentNode, int deletedKey){};
