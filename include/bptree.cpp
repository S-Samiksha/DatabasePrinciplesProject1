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
    Node *rootNode = new Node(3);
    Node *leafNode1 = new Node(3);
    Node *leafNode2 = new Node(3);
    Node *leafNode3 = new Node(3);

    this->rootNode = rootNode;

    // first layer keys
    rootNode->insert(30);
    rootNode->insert(50);

    // second layer keys

    // 1st subtree
    leafNode1->insert(20);
    leafNode1->insert(10);

    // 2nd subtree
    leafNode2->insert(40);
    leafNode2->insert(30);

    // 3rd subtree
    leafNode3->insert(70);
    leafNode3->insert(60);
    leafNode3->insert(50);

    // linking layers together
    rootNode->insertChildNode(0, leafNode1);
    rootNode->insertChildNode(1,leafNode2);
    rootNode->insertChildNode(2, leafNode3);
};

// deleting a key
void BPTree::remove(int key){


};

// displaying the tree
// do a BFS of the B+tree
void BPTree::display()
{
    std::priority_queue<std::vector<Node *>> pq;

    std::vector<Node *> rootLevel = std::vector<Node *>{this->rootNode};
    pq.push(rootLevel);
    int level = 0;
    while (!pq.top().empty())
    {
        // gets current top Node and removes the top Node from the queue
        std::vector<Node *> currentLevel = (pq.top());
        pq.pop();

        // display keys in each node in each level
        std::cout << "level: " << level << std::endl;
        std::cout << "********************"<<std::endl;

        // to store nodes in next level
        std::vector<Node *> nextLevel;

        for (int i = 0; i < currentLevel.size(); i++)
        {

            Node *currentNode = currentLevel.at(i);

            std::cout << "Node " << i <<": ";

            int j;
            std::cout << "[";
            for (j = 0; j < currentNode->currentSize; j++)
            {

                std::cout << currentNode->keys.at(j)<<",";

                // add children nodes to nextLevel nodes if it is not NULL
                if (currentNode->childrenNodes.at(j) != NULL)
                {
                    nextLevel.push_back(currentNode->childrenNodes.at(j));
                }
            }
            std::cout << "]" << std::endl;
            std::cout << std::endl;

            // add the rightmost Node pointer if it exists
            if (currentNode->childrenNodes.at(j) != NULL)
            {
                nextLevel.push_back(currentNode->childrenNodes.at(j));
            }
        }

        pq.push(nextLevel);
        level++;
    }
    std::cout << "**** <Finished traversal> ****" << std::endl;
};

// helper function for when the node size is exceeded and requires splitting
void BPTree::split(Node *currentNode, int incomingKey){
    

};

// helper function for when the node size is unbalanced and requires merging
void BPTree::merge(Node *currentNode, int deletedKey){};
