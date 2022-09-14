#include "bptree.h"
#include <stdio.h>
#include <iostream>
#include "node.h"
#include <queue>
#include <vector>
#include "dummyRecord.h"
#include <stack>
#include <cmath>

BPTree::BPTree(int nodeSize)
{
    this->nodeSize = nodeSize;
}

Node **BPTree::insert(Node *parentNode, int key, void *incomingRecord)
{
    // terminal condition full
    int insertionIndex = parentNode->binarySearchInsertIndex(key);
    if (insertionIndex == -1)
    {
        std::cout << "Duplicate key: " << key << std::endl;
        throw 1;
    }

    Node *childNode = parentNode;

    // Node *childNode = ((Node **)parentNode->childrenNodes)[insertionIndex];
    if (childNode->isLeaf)
    {
        // if leaf Node is full
        if (childNode->isFull())
        {
            Node *leftNode = new Node(this->nodeSize, true);
            Node *rightNode = new Node(this->nodeSize, true);

            int minimumKeySizeLeft = ceil((this->nodeSize + 1) / 2);
            int minimumKeySizeRight = floor((this->nodeSize + 1) / 2);

            int leftCounter = 0;

            // fill the left and right subtrees
            int nodeToSplitCounter = 0;
            int newNodesCounter = 0;
            int tempInsertionIndex = insertionIndex;
            while (newNodesCounter < minimumKeySizeLeft)
            {
                if (nodeToSplitCounter == tempInsertionIndex)
                {
                    leftNode->keys[newNodesCounter] = key;
                    ((dummyRecord **)leftNode->childrenNodes)[newNodesCounter] = (dummyRecord *)incomingRecord;
                    std::cout << "here1: " << leftNode->keys[newNodesCounter] << std::endl;

                    tempInsertionIndex = -1;
                    newNodesCounter++;
                }
                else
                {
                    leftNode->keys[newNodesCounter] = childNode->keys[nodeToSplitCounter];
                    std::cout << "here2: " << leftNode->keys[newNodesCounter] << std::endl;

                    newNodesCounter++;
                    nodeToSplitCounter++;
                }
                leftNode->currentKeySize++;
                leftNode->currentPointerSize++;
            }

            // fill the right subtree
            newNodesCounter = 0;
            while (newNodesCounter < minimumKeySizeRight)
            {
                if (nodeToSplitCounter == tempInsertionIndex)
                {
                    rightNode->keys[newNodesCounter] = key;
                    ((dummyRecord **)rightNode->childrenNodes)[newNodesCounter] = (dummyRecord *)incomingRecord;
                    std::cout << "here3: " << rightNode->keys[newNodesCounter] << std::endl;
                    tempInsertionIndex = -1;
                    newNodesCounter++;
                }
                else
                {
                    rightNode->keys[newNodesCounter] = childNode->keys[nodeToSplitCounter];
                    std::cout << "here4: " << rightNode->isLeaf << std::endl;

                    newNodesCounter++;

                    nodeToSplitCounter++;
                }
                rightNode->currentKeySize++;
                rightNode->currentPointerSize++;
            }

            // if it is initial case of root node, create new parent
            if (childNode == this->rootNode)
            {
                Node *newParentNode = new Node(3, false);
                newParentNode->insertInitialInNonLeafNode(rightNode->keys[0], leftNode, rightNode);
                this->rootNode = newParentNode;
                return nullptr;
            }

            return new Node *[2]
            { leftNode, rightNode };
        }
        // leaf node is not full
        else
        {
            childNode->insertSubsequentPair(key, incomingRecord);
            return nullptr;
        }
    }
    // if childNode is not a leaf
    else
    {
        childNode = ((Node **)parentNode->childrenNodes)[insertionIndex];
        std::cout << "childnode: " << std::endl;
        std::cout << "childnode is full?: " << childNode->isFull() << std::endl;

        childNode->printNode(); // correct
        Node **splitSubTrees = this->insert(childNode, key, incomingRecord);

        // if nullptr is returned, then no need to insert
        if (splitSubTrees == nullptr)
        {
            std::cout << "jere" << std::endl;
            return nullptr;
        }

        // if [leftSub,rightSub] is returned then need to insert
        Node *leftSubTree = splitSubTrees[0];
        Node *rightSubTree = splitSubTrees[1];

        // if parent tree is full
        if (parentNode->isFull())
        {
            std::cout << "parent sub tree is leaf? " << parentNode->isLeaf << std::endl;

            // split parent node in the non root node way
            // split to 2 nodes
            Node *leftNode;
            Node *rightNode;

            // if split is happening at the non-leaf node
            leftNode = new Node(this->nodeSize, true);
            rightNode = new Node(this->nodeSize, true);

            int minimumKeySizeLeft = ceil(this->nodeSize / 2);
            int minimumKeySizeRight = floor(this->nodeSize / 2);

            int leftCounter = 0;

            // todo: create a virtual keys array and virtual pointer array with the inserted key and pointers
            std::vector<int> virtualKeyArray(this->nodeSize + 1, 0);
            std::vector<Node *> virtualPointerArray(this->nodeSize + 2, nullptr);

            int parentNodeCounter = 0;
            int virtualNodeCounter = 0;
            while (virtualNodeCounter < virtualKeyArray.size())
            {
                if (parentNodeCounter == insertionIndex)
                {
                    virtualKeyArray.at(virtualNodeCounter) = key;
                    // deallocate memory space of the original pointer
                    virtualPointerArray.at(virtualNodeCounter) = leftNode;
                    virtualNodeCounter++;
                    virtualPointerArray.at(virtualNodeCounter) = rightNode;
                    virtualNodeCounter++;
                }
                else
                {
                    virtualKeyArray.at(virtualNodeCounter) = parentNode->keys[parentNodeCounter];
                    virtualPointerArray.at(virtualNodeCounter) = ((Node **)parentNode->childrenNodes)[parentNodeCounter];
                    parentNodeCounter++;
                    virtualNodeCounter++;
                }
            }
            // todo: check whether both virtual arrays are filled correctly

            // fill the left and right subtrees
            int virtualCounter = 0;
            int newNodesCounter = 0;

            // leftpointer || incomingKey || rightPointer
            while (newNodesCounter < minimumKeySizeLeft)
            {
                if (virtualCounter == insertionIndex)
                {
                    leftNode->keys[newNodesCounter] = key;
                    ((Node **)leftNode->childrenNodes)[newNodesCounter] = virtualPointerArray.at(virtualCounter);
                    newNodesCounter++;
                }
                else
                {
                    leftNode->keys[newNodesCounter] = virtualKeyArray.at(virtualCounter);
                    newNodesCounter++;
                    virtualCounter++;
                }
            }
            // instantiate the last pointer for left subtree
            ((Node **)leftNode->childrenNodes)[newNodesCounter] = virtualPointerArray.at(virtualCounter);
            // skip this key, this key is to be inserted in parent node
            // if no parent node, create new parent node with this key inserted
            int unecessaryKey = virtualKeyArray.at(virtualCounter);
            virtualCounter++;

            newNodesCounter = 0;
            // fill the right subtree
            while (newNodesCounter < minimumKeySizeRight)
            {
                if (virtualCounter == insertionIndex)
                {
                    rightNode->keys[newNodesCounter] = key;
                    ((Node **)rightNode->childrenNodes)[newNodesCounter] = virtualPointerArray.at(virtualCounter);
                    newNodesCounter++;
                }
                else
                {
                    rightNode->keys[newNodesCounter] = virtualKeyArray.at(virtualCounter);
                    newNodesCounter++;
                    virtualCounter++;
                }
            }

            // instantiate the last pointer of right subtree
            ((Node **)rightNode->childrenNodes)[newNodesCounter] = virtualPointerArray.at(virtualCounter);

            // if the node is the root node and is full
            if (parentNode == this->rootNode)
            {
                // create new parent
                Node *temp = this->rootNode;
                this->rootNode = new Node(this->nodeSize, false);
                this->rootNode->insertInitialInNonLeafNode(rightNode->keys[0], leftNode, rightNode);
                return nullptr;
            }
            return new Node *[2]
            { leftNode, rightNode };
        }
        // if parent node is not full
        else
        {
            std::cout << "leftNode returned subtree: " << std::endl;
            leftSubTree->printNode();
            std::cout << "rightNode returned subtree: " << std::endl;
            rightSubTree->printNode();
            // deallocate memory for original left subtree
            delete ((Node **)parentNode->childrenNodes)[insertionIndex];
            ((Node **)parentNode->childrenNodes)[insertionIndex] = leftSubTree;
            parentNode->insertSubsequentPair(rightSubTree->keys[0], rightSubTree);
            return nullptr;
        }
    }
}

int BPTree::insertInternal(Node *currentSubTreeRootNode, int key)
{
    return 0;
}

void BPTree::makeTestTree()
{
    Node *rootNode = new Node(3, false);
    Node *leafNode1 = new Node(3, true);
    Node *leafNode2 = new Node(3, true);
    Node *leafNode3 = new Node(3, true);

    this->rootNode = rootNode;

    // // 3rd subtree
    leafNode3->insertInitialInLeafNode(70, nullptr, nullptr);
    leafNode3->insertSubsequentPair(60, nullptr);
    leafNode3->insertSubsequentPair(50, nullptr);

    leafNode2->insertInitialInLeafNode(40, nullptr, leafNode3);
    leafNode2->insertSubsequentPair(30, nullptr);

    leafNode1->insertInitialInLeafNode(20, nullptr, leafNode2);
    leafNode1->insertSubsequentPair(10, nullptr);

    // root node
    rootNode->insertInitialInNonLeafNode(30, leafNode1, leafNode2);
    rootNode->insertSubsequentPair(50, leafNode3);
    // testing
    rootNode->insertSubsequentPair(60, nullptr);

    // std::cout << "root children 1: " << ((Node **)rootNode->childrenNodes)[0] << std::endl;
    // std::cout << "root children 2: " << ((Node **)rootNode->childrenNodes)[1] << std::endl;
    // std::cout << "root children 3: " << ((Node **)rootNode->childrenNodes)[2] << std::endl;
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

            // displaying each element in the node
            for (j = 0; j < currentNode->currentKeySize; j++)
            {

                std::cout << currentNode->keys[j] << ",";

                // add children nodes to nextLevel nodes if it is not nullptr
                if (((Node **)currentNode->childrenNodes)[j] != nullptr && !currentNode->isLeaf)
                {
                    nextLevel.push_back(((Node **)currentNode->childrenNodes)[j]);
                }
            }
            std::cout << "]" << std::endl;
            std::cout << std::endl;

            // add the rightmost Node pointer if it exists
            if (((Node **)currentNode->childrenNodes)[j] != nullptr && !currentNode->isLeaf)
            {
                nextLevel.push_back(((Node **)currentNode->childrenNodes)[j]);
            }
        }

        pq.push(nextLevel);
        level++;
    }
    std::cout << "**** <Finished traversal> ****" << std::endl;
};

// for finding parent node
Node *BPTree::findParentNode(Node *cursor, Node *child)
{
    Node *parent;

    // when cursor reaches end of tree
    if (cursor->isLeaf)
    {
        return nullptr;
    }

    // cursor is root
    for (int i = 0; i < cursor->currentPointerSize; i++)
    {
        // if cursor's children is child, return cursor as parent
        if (((Node **)cursor->childrenNodes)[i] == child)
        {
            parent = cursor;
            return parent;
        }
        else
        {
            parent = findParentNode(((Node **)cursor->childrenNodes)[i], child);

            if (parent != nullptr)
            {
                return parent;
            }
        }
    }

    return parent;
}

// helper function for when the node size is unbalanced and requires merging
void BPTree::merge(Node *currentNode, int deletedKey){};

// splits a node into 2 nodes, left and right node and returns them
// if nodeToSplit is a leaf node, left Node: ceil (n+1)/2 keys, right Node:floor (n+1)/2 keys
// if nodeToSplit is a non-leaf node, left node: ceil(n/2) keys, right node: floor (n/2) keys
// a key, nodeToSplit[ceil(n/2)] is removed and used to insert to parent
// if leaf nodes are returned they are not linked yet
Node **BPTree::split(Node *nodeToSplit, int keyToInsert, void *recordToInsert)
{

    // split to 2 nodes
    Node *leftNode;
    Node *rightNode;

    int indexToInsert = nodeToSplit->binarySearchInsertIndex(keyToInsert);

    // if split is happening at the leaf Node
    if (nodeToSplit->isLeaf)
    {
        leftNode = new Node(this->nodeSize, true);
        rightNode = new Node(this->nodeSize, true);

        int minimumKeySizeLeft = ceil((this->nodeSize + 1) / 2);
        int minimumKeySizeRight = floor((this->nodeSize + 1) / 2);

        int leftCounter = 0;

        // fill the left and right subtrees
        int nodeToSplitCounter = 0;
        int newNodesCounter = 0;
        while (newNodesCounter < minimumKeySizeLeft)
        {
            if (nodeToSplitCounter == indexToInsert)
            {
                leftNode->keys[newNodesCounter] = keyToInsert;
                ((dummyRecord **)leftNode->childrenNodes)[newNodesCounter] = (dummyRecord *)recordToInsert;
                newNodesCounter++;
            }
            else
            {
                leftNode->keys[newNodesCounter] = nodeToSplit->keys[nodeToSplitCounter];
                newNodesCounter++;
                nodeToSplitCounter++;
            }
        }

        newNodesCounter = 0;
        // fill the right subtree
        while (newNodesCounter < minimumKeySizeRight)
        {
            if (nodeToSplitCounter == indexToInsert)
            {
                rightNode->keys[newNodesCounter] = keyToInsert;
                ((dummyRecord **)rightNode->childrenNodes)[newNodesCounter] = (dummyRecord *)recordToInsert;
                newNodesCounter++;
            }
            else
            {
                rightNode->keys[newNodesCounter] = nodeToSplit->keys[nodeToSplitCounter];
                newNodesCounter++;
                nodeToSplitCounter++;
            }
        }
    }
    // if split is happening at the non-leaf node
    else
    {
        leftNode = new Node(this->nodeSize, true);
        rightNode = new Node(this->nodeSize, true);

        int minimumKeySizeLeft = ceil(this->nodeSize / 2);
        int minimumKeySizeRight = floor(this->nodeSize / 2);

        int leftCounter = 0;

        // fill the left and right subtrees
        int nodeToSplitCounter = 0;
        int newNodesCounter = 0;
        while (newNodesCounter < minimumKeySizeLeft)
        {
            if (nodeToSplitCounter == indexToInsert)
            {
                leftNode->keys[newNodesCounter] = keyToInsert;
                ((Node **)leftNode->childrenNodes)[newNodesCounter] = (Node *)recordToInsert;
                newNodesCounter++;
            }
            else
            {
                leftNode->keys[newNodesCounter] = nodeToSplit->keys[nodeToSplitCounter];
                newNodesCounter++;
                nodeToSplitCounter++;
            }
        }
        // instantiate the last pointer for left subtree
        ((dummyRecord **)leftNode->childrenNodes)[newNodesCounter] = (dummyRecord *)recordToInsert;

        newNodesCounter = 0;

        // skip this key, this key is to be inserted in parent node
        // if no parent node, create new parent node with this key inserted
        int unecessaryKey = nodeToSplit->keys[nodeToSplitCounter];
        nodeToSplitCounter++;

        // fill the right subtree
        while (newNodesCounter < minimumKeySizeRight)
        {
            if (nodeToSplitCounter == indexToInsert)
            {
                rightNode->keys[newNodesCounter] = keyToInsert;
                ((dummyRecord **)rightNode->childrenNodes)[newNodesCounter] = (dummyRecord *)recordToInsert;
                newNodesCounter++;
            }
            else
            {
                rightNode->keys[newNodesCounter] = nodeToSplit->keys[nodeToSplitCounter];
                newNodesCounter++;
                nodeToSplitCounter++;
            }
        }
    }

    return new Node *[2]
    { leftNode, rightNode };
}