#include "node.h"
#include "bptree.h"
#include "memorypool.h"
#include "types.h"
#include <stdio.h>
#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include <cmath>

BPTree::BPTree(int nodeSize, int poolSize, int blockSize)
{
    this->nodeSize = nodeSize;
    // this->memoryPoolInstance = new MemoryPool(poolSize,blockSize);
}

Address *BPTree::insert(Node *parentNode, int key, Address address, MemoryPool &disk)
{
    // std::cout << "record address blockAddress: " << address.blockAddress;
    // std::cout << "record address offset: " << address.offset;
    // void *recordAddress = (char*) address.blockAddress + address.offset;

    // std::cout << "record address: " << recordAddress;

    // allocate space for the record
    // Address recordAddress = BPTree::memoryPoolInstance->allocate(sizeof(Record));

    // if there is no root, just insert in root
    if (this->rootNode == nullptr)
    {
        this->rootNode = new Node(this->nodeSize, true);
        this->rootNode->keys[0] = key;

        // store address for record in Node
        (this->rootNode->childrenNodes)[0] = address;
        // load incoming record to disk
        // BPTree::memoryPoolInstance->saveToDisk(incomingRecord,sizeof(Record),recordAddress);

        this->rootNode->currentPointerSize++;
        this->rootNode->currentKeySize++;
        return nullptr;
    }
    else
    {
        // search for place to insert
        int insertionIndex = parentNode->binarySearchInsertIndex(key);

        // todo: duplicate index
        // insert record in the index
        if (insertionIndex == -1)
        {
            // todo: when inserting a duplicate key, traverse to the leaf node containing duplicate key
            // Node* cursor = parentNode;
            // int duplicateIndex;
            // while(!cursor->isLeaf){
            //     duplicateIndex= parentNode->binarySearch(key);
            //     cursor = ((Node**)parentNode->childrenTypes)[duplicateIndex+1];
            // };
            // duplicateIndex = cursor->binarySearch(key);

            // // if its a record
            // if(cursor->childrenTypes[duplicateIndex]==0){
            //     // todo: deallocate record and allocate special structure and copy the record pointer over

            //     // todo: change the childrenTypes array to be a special struct
            //     cursor->childrenTypes[duplicateIndex] = true;
            // }
            // // if its already a struct
            // else{
            //     // get free address from same block of the struct
            //     // insert record in struct
            //     // struct.insert(recordPointer)
            //     // insert record in memory
            //     // memory.insert(freeAdress, recordData)
            // }
            // return nullptr;

            // std::cout << "Duplicate key: " << key << std::endl;
            return nullptr;
        }

        // terminal case for recursion: parentNode is a Leaf
        if (parentNode->isLeaf)
        {
            // if leaf Node is full, split and return left and right leaf node
            if (parentNode->isFull())
            {
                // left And right nodes are leafs

                // allocate space for left and right nodes
                Address leftNodeAddress = BPTree::memoryPoolInstance->allocate(disk.getBlockSize());
                Address rightNodeAddress = BPTree::memoryPoolInstance->allocate(disk.getBlockSize());
                BPTree::memoryPoolInstance->saveToDisk(new Node(this->nodeSize, true), sizeof(Node), leftNodeAddress);
                BPTree::memoryPoolInstance->saveToDisk(new Node(this->nodeSize, true), sizeof(Node), rightNodeAddress);

                Node *leftNode = leftNodeAddress.getAddressNode();
                Node *rightNode = rightNodeAddress.getAddressNode();

                int minimumKeySizeLeft = ceil((this->nodeSize + 1) / 2);
                int minimumKeySizeRight = this->nodeSize + 1 - minimumKeySizeLeft;

                // fill the left and right subtrees
                // filling left subtree
                int nodeToSplitCounter = 0;
                int newNodesCounter = 0;
                int tempInsertionIndex = insertionIndex;

                while (newNodesCounter < minimumKeySizeLeft)
                {
                    if (nodeToSplitCounter == tempInsertionIndex)
                    {
                        leftNode->keys[newNodesCounter] = key;
                        (leftNode->childrenNodes)[newNodesCounter] = address;
                        tempInsertionIndex = -1;
                        newNodesCounter++;
                    }
                    else
                    {
                        leftNode->keys[newNodesCounter] = parentNode->keys[nodeToSplitCounter];
                        (leftNode->childrenNodes)[newNodesCounter] = (parentNode->childrenNodes)[nodeToSplitCounter];

                        newNodesCounter++;
                        nodeToSplitCounter++;
                    }
                    leftNode->currentKeySize++;
                    leftNode->currentPointerSize++;
                }

                // filling the right subtree
                newNodesCounter = 0;

                while (newNodesCounter < minimumKeySizeRight)
                {
                    if (nodeToSplitCounter == tempInsertionIndex)
                    {
                        rightNode->keys[newNodesCounter] = key;
                        (rightNode->childrenNodes)[newNodesCounter] = address;
                        tempInsertionIndex = -1;
                        newNodesCounter++;
                    }
                    else
                    {
                        rightNode->keys[newNodesCounter] = parentNode->keys[nodeToSplitCounter];
                        (leftNode->childrenNodes)[newNodesCounter] = (parentNode->childrenNodes)[nodeToSplitCounter];

                        newNodesCounter++;
                        nodeToSplitCounter++;
                    }
                    rightNode->currentKeySize++;
                    rightNode->currentPointerSize++;
                    std::cout << "Left Node: \n";
                    leftNode->printNode();
                    std::cout << "Right Node: \n";
                    rightNode->printNode();
                }

                // BPTree::memoryPoolInstance->saveToDisk(incomingRecord,sizeof(Record),recordAddress);

                // terminal case where the parent is full, a leaf and a root node
                // we just create a new parent and set that as the new root Node
                if (parentNode == this->rootNode)
                {
                    Node *newParentNode = new Node(3, false);
                    newParentNode->insertInitialInNonLeafNode(rightNode->keys[0], leftNodeAddress, rightNodeAddress);

                    this->rootNode = newParentNode;
                    return nullptr;
                }

                return new Address[2]{leftNodeAddress, rightNodeAddress};
            }
            // or if leaf node is not full we can just insert a key pointer pair
            else
            {
                parentNode->insertSubsequentPair(key, address);

                // todo: save the incoming Record to disk
                // BPTree::memoryPoolInstance->saveToDisk(incomingRecord,sizeof(Record),recordAddress);

                return nullptr;
            }
        }
        // if parentNode is not a leaf
        // this is the recursive call, we recursively find the index to insert on the node according to the key with binary search
        // then we recursively call that child using insert()
        else
        {
            Address childNodeAddress = parentNode->childrenNodes[insertionIndex];
            Node *childNode = childNodeAddress.getAddressNode();

            // recursive call on the child
            // if nullptr is returned from insert() it means that the parent need not do any insertions
            // if [ Node* leftSubTree, Node* rightSubTree] is returned, it means that the child node was full
            // and requires insertion on the parent
            Address *returnedChildSubTrees = this->insert(childNode, key, address, disk);

            // if nullptr is returned, that means child was not split
            if (returnedChildSubTrees == nullptr)
            {
                return nullptr;
            }

            // if [leftSub,rightSub] is returned this means the child was full and split
            Node *leftChildSubTree = returnedChildSubTrees[0].getAddressNode();
            Node *rightChildSubTree = returnedChildSubTrees[1].getAddressNode();
            int keyToInsertIntoParent;

            // if child is a leaf, insert the first key into the parent Node
            if (rightChildSubTree->isLeaf)
            {
                // insert key into parent
                keyToInsertIntoParent = rightChildSubTree->keys[0];
            }
            else
            {
                keyToInsertIntoParent = rightChildSubTree->removeFirstKeyFromNode();
            }

            if (parentNode->isFull())
            {
                // split parent node in the non root node way into 2 non-leaf nodes
                Node *leftNode;
                Node *rightNode;

                // todo: allocate space in memory for nodes
                Address leftNodeAddress = BPTree::memoryPoolInstance->allocate(disk.getBlockSize());
                Address rightNodeAddress = BPTree::memoryPoolInstance->allocate(disk.getBlockSize());

                // todo: save new nodes into disk
                BPTree::memoryPoolInstance->saveToDisk(new Node(this->nodeSize, false), disk.getBlockSize());
                BPTree::memoryPoolInstance->saveToDisk(new Node(this->nodeSize, false), disk.getBlockSize());

                //  minimum and maximum bounds of non-leaf nodes
                int minimumKeySizeRight = (this->nodeSize / 2);
                int minimumKeySizeLeft = this->nodeSize - minimumKeySizeRight;

                // build a virtual array to help with the splitting
                int virtualKeyArray[this->nodeSize + 1]{0};
                Address virtualPointerArray[this->nodeSize + 2]{nullptr};

                int i = 0;
                for (i; i < this->nodeSize; i++)
                {
                    virtualKeyArray[i] = parentNode->keys[i];
                    virtualPointerArray[i] = (parentNode->childrenNodes)[i];
                }
                virtualPointerArray[i] = parentNode->childrenNodes[i];

                // insert the key to be inserted
                for (int i = this->nodeSize; i > insertionIndex; i--)
                {
                    virtualKeyArray[i] = virtualKeyArray[i - 1];
                }

                virtualKeyArray[insertionIndex] = keyToInsertIntoParent;
                virtualPointerArray[insertionIndex] = returnedChildSubTrees[0];

                // insert the pointer to be inserted
                for (int i = this->nodeSize + 1; i > insertionIndex + 1; i--)
                {
                    virtualPointerArray[i] = virtualPointerArray[i - 1];
                }
                virtualPointerArray[insertionIndex + 1] = returnedChildSubTrees[1];

                // fill the left and right subtrees
                int virtualKeyCounter = 0;
                int virtualPtrCounter = 0;
                int newNodesKeyCounter = 0;

                // fill the left and right subtrees we want to return to the upper level
                while (newNodesKeyCounter < minimumKeySizeLeft)
                {
                    leftNode->keys[newNodesKeyCounter] = virtualKeyArray[virtualKeyCounter];
                    (leftNode->childrenNodes)[newNodesKeyCounter] = virtualPointerArray[virtualPtrCounter];

                    newNodesKeyCounter++;
                    virtualKeyCounter++;
                    virtualPtrCounter++;
                    leftNode->currentKeySize++;
                    leftNode->currentPointerSize++;
                }

                // add the right most pointer of left subtree
                (leftNode->childrenNodes)[newNodesKeyCounter] = virtualPointerArray[virtualPtrCounter];

                leftNode->currentPointerSize++;

                // fill the right subtree
                newNodesKeyCounter = 0;
                while (newNodesKeyCounter < minimumKeySizeRight + 1)
                {
                    rightNode->keys[newNodesKeyCounter] = virtualKeyArray[virtualKeyCounter];
                    (rightNode->childrenNodes)[newNodesKeyCounter] = virtualPointerArray[virtualPtrCounter];

                    newNodesKeyCounter++;
                    virtualPtrCounter++;
                    virtualKeyCounter++;
                    rightNode->currentKeySize++;
                    rightNode->currentPointerSize++;
                }

                // add the right most pointer of right subtree
                (rightNode->childrenNodes)[newNodesKeyCounter] = virtualPointerArray[virtualKeyCounter];
                rightNode->currentPointerSize++;

                // if parent node is root node and is full create new root node
                if (parentNode == this->rootNode)
                {
                    // remove first element of right subtree
                    int parentInsertionKey = rightNode->removeFirstKeyFromNode();
                    // std::cout << "Left subtree to go up up" << std::endl;
                    // leftNode->printNode();
                    // std::cout << "Right subtree to go up up" << std::endl;
                    // rightNode->printNode();
                    //  create new parent

                    // allocate new memory to node
                    Address newRootAddress = BPTree::memoryPoolInstance->allocate(disk.getBlockSize());
                    // todo: write data into that address
                    BPTree::memoryPoolInstance->saveToDisk(new Node(this->nodeSize, false), disk.getBlockSize(), newRootAddress);

                    newRootAddress.getAddressNode()->insertInitialInNonLeafNode(parentInsertionKey, leftNodeAddress, rightNodeAddress);

                    // dealloc old memory to prevent leaks
                    BPTree::memoryPoolInstance->deallocate(this->rootNode->addressInDisk, disk.getBlockSize());

                    // link to root pointer
                    this->rootNode = newRootAddress.getAddressNode();

                    return nullptr;
                }

                return new Address[2]{leftNodeAddress, rightNodeAddress};
            }

            // if parent node is not full, just insert a key-pointer pair in the parent node
            else
            {
                //  deallocate memory for original left subtree
                BPTree::memoryPoolInstance->deallocate((parentNode->childrenNodes)[insertionIndex], disk.getBlockSize());

                delete (parentNode->childrenNodes)[insertionIndex].getAddressNode();
                (parentNode->childrenNodes)[insertionIndex] = returnedChildSubTrees[0];
                parentNode->insertSubsequentPair(keyToInsertIntoParent, returnedChildSubTrees[1]);

                return nullptr;
            }
        }
    }
}

// displaying the tree by using BFS on the B+tree
void BPTree::display()
{
    std::priority_queue<std::vector<Node *>> pq;

    std::vector<Node *> rootLevel = std::vector<Node *>{this->rootNode};
    pq.push(rootLevel);
    std::cout << "*********display() debug********" << std::endl;
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

            std::cout << "Node " << i << "\n";
            std::cout << "Node address: " << currentNode << "\n";
            std::cout << "currentKeySize: " << currentNode->currentKeySize << "\n";
            std::cout << "currentPointerSize: " << currentNode->currentPointerSize << "\n";
            std::string nodeType = currentNode->isLeaf ? "LEAF" : "INTERNAL NODE";
            std::cout << "nodeType: " << nodeType << "\n";

            int j;
            std::cout << "keys: [";

            // displaying each element in the node
            // if its a non leaf node we need to loop through j+1

            for (j = 0; j < currentNode->maxKeySize; j++)
            {

                std::cout << currentNode->keys[j] << ",";

                // add children nodes to nextLevel nodes if it is not nullptr
                if (((Node **)currentNode->childrenNodes)[j] != nullptr && !currentNode->isLeaf)
                {
                    nextLevel.push_back(((Node **)currentNode->childrenNodes)[j]);
                }
            }

            // add children nodes to nextLevel nodes if it is not nullptr
            // add last children Node to the nextLevel list cos there are n+1 children where n= number of keys
            if (((Node **)currentNode->childrenNodes)[j] != nullptr && !currentNode->isLeaf)
            {
                nextLevel.push_back(((Node **)currentNode->childrenNodes)[j]);
            }

            std::cout << "]" << std::endl;

            std::cout << "Pointers: [";

            // displaying each element in the node
            // if its a non leaf node we need to loop through j+1
            for (int i = 0; i < currentNode->maxPointerSize; i++)
            {
                std::cout << ((Node **)currentNode->childrenNodes)[i] << ",";
            }
            // add last children Node to the nextLevel list cos there are n+1 children where n= number of keys
            std::cout << "]" << std::endl;
            std::cout << std::endl;
        }

        pq.push(nextLevel);
        level++;
    }
    std::cout << "**** <Finished traversal> ****" << std::endl;
};

// returns start address of the starting value
Address *BPTree::queryWithNumVotesAsKey(int key, int &nodesUpdated)
{
    Node *cursor = this->rootNode;
    nodesUpdated = 1;

    if (this->rootNode == nullptr)
    {
        std::cout << "BPTree does not exist. instantiate rootNode" << std::endl;
        return nullptr;
    }

    while (!cursor->isLeaf)
    {
        nodesUpdated += 1;
        cursor->printNode();
        // checks whether the key exists in the internal node
        int insertionIndex = cursor->binarySearchInsertIndex(key);
        // found the key
        if (insertionIndex == -1)
        {
            insertionIndex = cursor->binarySearch(key);
            cursor = ((Node **)cursor->childrenNodes)[insertionIndex + 1];
        }
        // key is not found in internal node
        else
        {
            cursor = ((Node **)cursor->childrenNodes)[insertionIndex];
        }
    }

    int index = cursor->binarySearch(key);

    return ((Address **)cursor->childrenNodes)[index];
}

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

// finds the minimum key for the BPTree
int BPTree::findMinimumKeyInBPTree(Node *node)
{
    if (node->isLeaf)
    {
        return node->keys[0];
    }
    return findMinimumKeyInBPTree(((Node **)node->childrenNodes)[0]);
}

int BPTree::findHeight(Node *rootNode)
{
    int height = 1;
    // perform DFS of from the root node to
    Node *cursor = rootNode;

    // traverse to leaf node
    while (!cursor->isLeaf)
    {
        int insertionIndex = cursor->binarySearchInsertIndex(0);
        cursor = cursor->childrenNodes[insertionIndex]->getAddressNode();
        height++;
    }

    std::cout << "height of B+ Tree is: " << height << std::endl;

    return height;
}
// print answers for experiment 2
void BPTree::printBPDetails()
{
    // experiment 2
    std::cout << "================= Experiment 2 =================" << std::endl;
    std::vector<Address> leafNodes;
    int nodeCount = 1;
    this->DFSNodes(this->rootNode->addressInDisk, leafNodes, nodeCount);
    std::cout << "******BPTREE DETAILS******" << std::endl;
    std::cout << "Parameter n of B+ Tree (number of keys in Node): " << this->nodeSize << std::endl;
    std::cout << "Total NodeSize: " << nodeCount << " nodes" << std::endl;

    // find max height of B+ tree
    int height = this->findHeight(this->rootNode);
    std::cout << "height of B+ Tree is: " << height << std::endl;

    // std::cout << "List of Records: [ ";

    // int count = 0;
    // while (cursor != nullptr)
    // {
    //     for (int i = 0; i < cursor->currentKeySize; i++)
    //     {
    //         std::cout << cursor->keys[i] << ",";
    //         count++;
    //     }

    //     cursor = ((Node **)cursor->childrenNodes)[cursor->maxPointerSize - 1];
    // }
    // std::cout << "]" << std::endl;
    // ;
    // std::cout << "number of records: " << count << std::endl;

    std::cout << "\n\nContent Of Root Node:" << std::endl;
    this->rootNode->printNode();
    std::cout << "\n\nContent of First Child Of Root Node" << std::endl;
    ((Node **)this->rootNode->childrenNodes)[0]->printNode();
}

// does DFS traversal and links all leafnodes together
void BPTree::linkLeafNodes()
{
    std::vector<Address> leafNodes;
    int nodeCount = 1;
    this->DFSNodes(this->rootNode->addressInDisk, leafNodes, nodeCount);

    // link the leaf nodes tgt
    for (int i = 0, j = 1; j < leafNodes.size(); i++, j++)
    {
        leafNodes.at(i).getAddressNode()->linkToAnotherLeafNode(leafNodes.at(j));
    }
    std::cout << "Finished linking leaf nodes" << std::endl;
}

// does DFS traversals and returns a list of leafnodes if order, and also the number of nodes in the B+ tree
void BPTree::DFSNodes(Address currentNode, std::vector<Address> &recordList, int &nodeCount)
{
    std::queue<Address> childrenNodesToSearch;
    // terminal condition if the node is a leaf, add node pointer into the vector
    if (currentNode.getAddressNode()->isLeaf)
    {
        // counting nodes
        nodeCount++;

        recordList.push_back(currentNode);
        std::cout << "keys in leaf node: [ ";
        for (int i = 0; i < currentNode.getAddressNode()->currentKeySize; i++)
        {
            std::cout << currentNode.getAddressNode()->keys[i] << ", ";
        }
        std::cout << "]" << std::endl;
        return;
    }
    // if its not a leaf node add all its children nodes to count
    nodeCount += currentNode.getAddressNode()->currentPointerSize;

    // keep track of all the children nodes to search in this parent node in a queue
    for (int i = 0; i < currentNode.getAddressNode()->currentPointerSize; i++)
    {
        childrenNodesToSearch.push(currentNode.getAddressNode()->childrenNodes[i]);
    }
    // search the first children node in that queue
    while (!childrenNodesToSearch.empty())
    {
        Address childrenNodeToTraverse = childrenNodesToSearch.front();
        childrenNodesToSearch.pop();
        this->DFSNodes(childrenNodeToTraverse, recordList, nodeCount);
        // going up the recursion
    }
}