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
void BPTree::insert(int key)
{
    if (this->rootNode == NULL)
    {
        this->rootNode = new Node(this->nodeSize);
        rootNode->insert(key);
        rootNode->isLeaf = true;
    }
    else
    {
        Node* currentSubTreeRoot = this->rootNode; 
        while(!currentSubTreeRoot->isLeaf){
            int insertionIndex = currentSubTreeRoot->binarySearchInsertIndex(key);
            currentSubTreeRoot = currentSubTreeRoot->childrenNodes.at(insertionIndex);
        }
        // search to the leaf node
        // TODO: handle duplicates
    }
};

int BPTree::insertInternal(Node *currentSubTreeRootNode, int key)
{
    if (currentSubTreeRootNode->isLeaf)
    {
        // full need to split
        if(currentSubTreeRootNode->currentSize == this->nodeSize){
            return split(currentSubTreeRootNode,key);
        }else{
            currentSubTreeRootNode->insert(key);
            return -1;
        }
    }
    else
    {
        int insertionIndex = currentSubTreeRootNode->binarySearchInsertIndex(key);
        Node *nextSubTree = currentSubTreeRootNode->childrenNodes.at(insertionIndex);
        int insertionKey = insertInternal(nextSubTree, key);
        if(currentSubTreeRootNode->currentSize == this->nodeSize){
            return split(currentSubTreeRootNode,insertionKey);
        }else{
            if(insertionKey!= -1){
                currentSubTreeRootNode->insert(insertionKey);
            }
            return -1;
        }
        
    }
}

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
        std::cout << "********************" << std::endl;

        // to store nodes in next level
        std::vector<Node *> nextLevel;

        for (int i = 0; i < currentLevel.size(); i++)
        {

            Node *currentNode = currentLevel.at(i);

            std::cout << "Node " << i << ": ";

            int j;
            std::cout << "[";
            for (j = 0; j < currentNode->currentSize; j++)
            {

                std::cout << currentNode->keys.at(j) << ",";

                // add children nodes to nextLevel nodes if it is not NULL
                if (currentNode->childrenNodes.at(j) != NULL && !currentNode->isLeaf)
                {
                    nextLevel.push_back(currentNode->childrenNodes.at(j));
                }
            }
            std::cout << "]" << std::endl;
            std::cout << std::endl;

            // add the rightmost Node pointer if it exists
            if (currentNode->childrenNodes.at(j) != NULL && !currentNode->isLeaf)
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
// returns -1 if no need to insert
int BPTree::split(Node *currentNode, int incomingKey){
    int insertionIndex = currentNode->binarySearchInsertIndex(incomingKey);
    Node* leftNode = new Node(this->nodeSize);
    Node* rightNode = new Node(this->nodeSize);
    
    int minimumLeftKeys;
    int minimumRightKeys;

    // calculate the minimum number of keys on the left and right Node
    if(currentNode->isLeaf){
        minimumLeftKeys = (this->nodeSize+1)/2;
        minimumRightKeys = this->nodeSize - minimumLeftKeys;
    }else{
        minimumLeftKeys = (this->nodeSize)/2;
        minimumRightKeys = this->nodeSize - minimumLeftKeys;
    };

    // while(leftNode->currentSize < minimumLeftKeys){
    //     leftNode.insert()
    // }

   return 0;


};

// helper function for when the node size is unbalanced and requires merging
void BPTree::merge(Node *currentNode, int deletedKey){};
