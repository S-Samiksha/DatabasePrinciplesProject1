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

    Node *childNode = ((Node **)parentNode->keys)[insertionIndex];
    if (childNode->isLeaf)
    {
        // if leaf Node is full
        if (childNode->isFull())
        {
            // split tje leaf Node int left and right subtree
            Node **resultsArray = this->splitLeafNode(childNode, key, incomingRecord);
            Node *leftSubTree = resultsArray[0];
            Node *rightSubTree = resultsArray[1];

            // if the parent node is the root Node, create new parent root node
            if (childNode == this->rootNode)
            {
                Node *newParentNode = new Node(this->nodeSize, false);
                newParentNode->insertInitialInNonLeafNode(rightSubTree->keys[0], leftSubTree, rightSubTree);
                delete this->rootNode;
                this->rootNode = newParentNode;
                return nullptr;
            }
            // if current Node is not rootnode return l and r value
            else
            {
                return new Node *[2]
                { leftSubTree, rightSubTree };
            }
        }
        // leaf node is not full
        else
        {
            int dummyRecordPointer;
            childNode->insertSubsequentPair(key, (void *)(dummyRecordPointer));
            return nullptr;
        }
    }
    // if childNode is not a leaf
    else
    {
        Node **splitSubTrees = insert(childNode, key);

        // if nullptr is returned, then no need to insert
        if (splitSubTrees == nullptr)
        {
            return nullptr;
        }

        // if [leftSub,rightSub] is returned then need to insert
        Node *leftSubTree = splitSubTrees[0];
        Node *rightSubTree = splitSubTrees[1];

        // if parent tree is full
        if (parentNode->isFull())
        {
            // todo:sus concept
            //  key to be inserted in parent is always rightSubTree[1]
            Node **parentSubtees = this->split(parentNode, rightSubTree->keys[1]);

            // link the initial leftSubTree to the current Splitted Left subtree
            Node *parentsLeftSubTree = parentSubtees[0];
            Node *parentsRightSubTree = parentSubtees[1];

            parentsLeftSubTree->insert
        }
        // if parent tree is not full
        int *resultArray = insert(childNode, key);

        if (resultArray == nullptr)
        {
            return nullptr;
        }
        else
        {
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

    // first layer keys
    rootNode->insert(30);
    rootNode->insert(40);
    rootNode->insert(50);

    std::cout << rootNode->keys[0] << std::endl;
    std::cout << rootNode->keys[1] << std::endl;
    std::cout << rootNode->keys[2] << std::endl;

    // // second layer keys

    // // 1st subtree
    leafNode1->insert(20);
    leafNode1->insert(10);

    std::cout << leafNode1->keys[0] << std::endl;
    std::cout << leafNode1->keys[1] << std::endl;
    std::cout << leafNode1->keys[2] << std::endl;
    // // 2nd subtree
    leafNode2->insert(40);
    leafNode2->insert(30);
    std::cout << leafNode2->keys[0] << std::endl;
    std::cout << leafNode2->keys[1] << std::endl;
    std::cout << leafNode2->keys[2] << std::endl;

    // // 3rd subtree
    leafNode3->insert(70);
    leafNode3->insert(60);
    leafNode3->insert(50);
    std::cout << leafNode3->keys[0] << std::endl;
    std::cout << leafNode3->keys[1] << std::endl;
    std::cout << leafNode3->keys[2] << std::endl;

    // // linking layers together
    rootNode->insertChildNode(0, leafNode1);
    rootNode->insertChildNode(1, leafNode2);
    rootNode->insertChildNode(2, leafNode3);

    ((Node **)leafNode1->childrenNodes)[-1] = leafNode2;
    ((Node **)leafNode2->childrenNodes)[-1] = leafNode3;
    std::cout << "value at node 2: " << ((Node **)leafNode2->childrenNodes)[-1]->keys[0] << std::endl;
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

// returns a pointer to the split left and right subTree
Node **BPTree::splitLeafNode(Node *currentNode, int incomingKey, void *incomingRecord)
{

    // check Node is full
    if (!currentNode->isFull())
    {
        std::cout << "Node is not full. cannot call split function!" << std::endl;
        throw 1;
    }

    // minimum key values for left and right subtrees
    int minimumLeftKeys = ceil((currentNode->maxKeySize) / 2);

    int minimumRightKeys = floor((currentNode->maxKeySize) / 2);

    Node *leftSubTree = new Node(this->nodeSize, false);
    Node *rightSubTree = new Node(this->nodeSize, false);

    // find the index to insert the key
    int insertionIndex = currentNode->binarySearchInsertIndex(incomingKey);

    if (insertionIndex == -1)
    {
        std::cout << "duplicate index called at split()" << std::endl;
        throw 1;
    };

    // link left leaf node to right leaf node, right leaf node to null currently
    leftSubTree->insertInitialInLeafNode(currentNode->keys[0], ((dummyRecord **)currentNode->childrenNodes)[0], rightSubTree);

    // fill the left Node to ceil(n/2) key values
    dummyRecord *tempRecord;
    int i, j;
    for (i = 0, j = 0; j < minimumLeftKeys; i++, j++)
    {
        tempRecord = ((dummyRecord **)currentNode->childrenNodes)[i];
        if (j == insertionIndex)
        {
            leftSubTree->insertSubsequentPair(incomingKey, incomingRecord);
            i--;
        }
        else
        {

            leftSubTree->insertSubsequentPair(incomingKey, tempRecord);
        }
    }

    // fill right Node with floor(n/2) key values
    rightSubTree->insertInitialInLeafNode(currentNode->keys[i], ((dummyRecord **)currentNode->childrenNodes)[i], nullptr);
    for (i, j = 0; i < minimumRightKeys; i++, j++)
    {
        if (j == insertionIndex)
        {
            leftSubTree->insertSubsequentPair(incomingKey, incomingRecord);
            i--;
        }
        else
        {

            leftSubTree->insertSubsequentPair(incomingKey, ((dummyRecord **)currentNode->childrenNodes)[i]);
        }
    }
    return new Node *[2]
    { leftSubTree, rightSubTree };
};

// todo
Node **BPTree::splitNonLeafNode(Node *currentNode, int incomingKey)
{
    // check Node is full
    if (!currentNode->isFull())
    {
        std::cout << "Node is not full. cannot call split function!" << std::endl;
        throw 1;
    }

    // minimum key values for left and right subtrees
    int minimumLeftKeys = ceil((currentNode->maxKeySize) / 2);
    int minimumRightKeys = floor((currentNode->maxKeySize) / 2);

    Node *leftSubTree = new Node(this->nodeSize, false);
    Node *rightSubTree = new Node(this->nodeSize, false);

    // find the index to insert the key
    int insertionIndex = currentNode->binarySearchInsertIndex(incomingKey);

    if (insertionIndex == -1)
    {
        std::cout << "duplicate index called at split()" << std::endl;
        throw 1;
    };

    leftSubTree->insertInitialInNonLeafNode(currentNode->keys[0], ((Node **)currentNode->childrenNodes)[0], ((Node **)currentNode->childrenNodes)[1]);
    // fill leftSubTree until insertionNode
    int i, j;
    for (i = 0, j = 0; j < minimumLeftKeys; i++, j++)
    {
        if (j == insertionIndex)
        {
            leftSubTree->insertSubsequentPair(incomingKey, ((Node **)currentNode->childrenNodes)[i]);
            i--;
        }
        else
        {

            leftSubTree->insertSubsequentPair(currentNode->keys[i], ((Node **)currentNode->childrenNodes)[i]);
        }
    }

    // continue with right subtree
    rightSubTree->insertInitialInNonLeafNode(currentNode->keys[i], ((dummyRecord **)currentNode->childrenNodes)[i],;
    for (i, j = 0; i < minimumRightKeys; i++, j++)
    {
        if (j == insertionIndex)
        {
            leftSubTree->insertSubsequentPair(incomingKey, ((Node **)currentNode->childrenNodes)[i]);
            i--;
        }
        else
        {

            leftSubTree->insertSubsequentPair(incomingKey, ((Node **)currentNode->childrenNodes)[i]);
        }
    }
}
// split into l and right nodes on the way up the recursion and link
Node **BPTree::linkParentToNewChildrenNode(Node *parentNode, Node *leftChildNode, Node *rightChildNode)
{
    // if parent node is the root Node, then make a new parentNode and insert the right minimum value
    if (parentNode == this->rootNode)
    {
        Node *newParentNode = new Node(this->nodeSize, false);
        // deallocate memory for current root node
        delete this->rootNode;
        newParentNode.insert
    }
};

// link only Node is full on insertion and requires to link left and right nodes
Node *BPTree::linkNonLeafNode(Node *parentNode, Node *leftNode, Node *rightNode){

};
Node *BPTree::linkLeafNode(Node *parentNode, Node *leftNode, Node *rightNode){

};

// for finding parent node
Node *BPTree::findParentNode(Node *cursor, Node *child)
{
    Node *parent;

    // when cursor reaches end of tree
    if (cursor->isLeaf || ((Node **)cursor->childrenNodes)[0]->isLeaf)
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
