//#include "node.h"
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
    this->rootNode = nullptr;
}

Address *BPTree::insert(Node *parentNode, int key, Address address, MemoryPool &disk)
{
    // if there is no root, just insert in root

    if (this->rootNode == nullptr)
    {

        this->rootNode = new Node(this->nodeSize, true);
        this->rootNode->addressInDisk = disk.allocate(disk.getBlockSize(),false);
        this->rootNode->keys[0] = key;

        // store address for record in Node
        this->rootNode->childrenNodes[0] = address;
        this->rootNode->currentPointerSize++;
        this->rootNode->currentKeySize++;

        // load incoming record to disk
        disk.saveToDisk(this->rootNode, disk.getBlockSize());
        // std::cout<<"hi here: \n";
        // std::cout<<this->rootNode->addressInDisk.blockAddress;
        // testAddress.getAddressNode()->printNode();

        return nullptr;
    }
    else
    {

        // search for place to insert
        int insertionIndex = parentNode->binarySearchInsertIndex(key);

        // duplicate records
        if (insertionIndex == -1)
        {
            std::cout << "Duplicate key: " << key << std::endl;
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
                Address leftNodeAddress = disk.allocate(disk.getBlockSize(),false);
                Address rightNodeAddress = disk.allocate(disk.getBlockSize(),false);

                Node *leftNode = new Node(this->nodeSize, true);
                leftNode->addressInDisk = leftNodeAddress;
                Node *rightNode = new Node(this->nodeSize, true);
                rightNode->addressInDisk = rightNodeAddress;

                //  minimum and maximum bounds of non-leaf nodes
                int minimumKeySizeRight = (int)floor((this->nodeSize / 2));
                int minimumKeySizeLeft = this->nodeSize - minimumKeySizeRight;

                // build a virtual array to help with the splitting
                int virtualKeyArray[this->nodeSize + 1]{0};
                Address virtualPointerArray[this->nodeSize + 1]{nullptr};

                int i = 0;
                for (i; i < this->nodeSize; i++)
                {
                    virtualKeyArray[i] = parentNode->keys[i];
                    virtualPointerArray[i] = parentNode->childrenNodes[i];
                }

                // insert the key and pointer to be inserted
                for (int i = this->nodeSize; i > insertionIndex; i--)
                {
                    virtualPointerArray[i] = virtualPointerArray[i - 1];
                    virtualKeyArray[i] = virtualKeyArray[i - 1];
                }

                virtualKeyArray[insertionIndex] = key;
                virtualPointerArray[insertionIndex] = address;

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

                // todo
                // std::cout << "virtual left: " << std::endl;
                // leftNode->printNode();
                // std::cout << "virtual right: " << std::endl;
                // rightNode->printNode();

                // save to filled left and right subtree to disk

                Address testLeftAddress = disk.saveToDisk(leftNode, disk.getBlockSize(), leftNodeAddress);
                Address testRightAddress = disk.saveToDisk(rightNode, disk.getBlockSize(), rightNodeAddress);

                // std::cout<<"debug left: "<<std::endl;
                // testLeftAddress.getAddressNode()->printNode();
                // std::cout<<"debug right: "<<std::endl;
                // testRightAddress.getAddressNode()->printNode();

                // terminal case where the parent is full, a leaf and a root node
                // we just create a new parent and set that as the new root Node
                if (parentNode == this->rootNode)
                {
                    Node *newParentNode = new Node(this->nodeSize, false);
                    newParentNode->insertInitialInNonLeafNode(rightNode->keys[0], leftNodeAddress, rightNodeAddress);

                    // allocate space on disk for new parent node
                    Address newParentAddress = disk.allocate(disk.getBlockSize(),false);

                    disk.saveToDisk(newParentNode, disk.getBlockSize(), newParentAddress);

                    // deallocate original parent node
                    // disk.deallocate(this->rootNode->addressInDisk,disk.getBlockSize());

                    this->rootNode = newParentAddress.getAddressNode();
                    this->rootNode->addressInDisk = newParentAddress;
                    return nullptr;
                }

                return new Address[2]{leftNodeAddress, rightNodeAddress};
            }
            // or if leaf node is not full we can just insert a key pointer pair
            else
            {
                parentNode->insertSubsequentPair(key, address);
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
                Node *leftNode = new Node(this->nodeSize, false);
                Node *rightNode = new Node(this->nodeSize, false);

                // todo: allocate space in memory for nodes
                Address leftNodeAddress = disk.allocate(disk.getBlockSize(),false);
                Address rightNodeAddress = disk.allocate(disk.getBlockSize(),false);
                leftNode->addressInDisk = leftNodeAddress;
                rightNode->addressInDisk = rightNodeAddress;

                disk.saveToDisk(leftNode, disk.getBlockSize(), leftNodeAddress);
                disk.saveToDisk(rightNode, disk.getBlockSize(), rightNodeAddress);

                leftNode = leftNodeAddress.getAddressNode();
                rightNode = rightNodeAddress.getAddressNode();

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
                    virtualPointerArray[i] = parentNode->childrenNodes[i];
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

                    // allocate new memory to node
                    Node *newRoot = new Node(this->nodeSize, false);
                    Address newRootAddress = disk.allocate(disk.getBlockSize(),false);
                    newRoot->addressInDisk = newRootAddress;
                    // std::cout<<"newly created parent Node Address (splitting internal node): "<< newRootAddress.getAddressNode()<<std::endl;
                    disk.saveToDisk(newRoot, disk.getBlockSize(), newRootAddress);

                    newRootAddress.getAddressNode()->insertInitialInNonLeafNode(parentInsertionKey, leftNodeAddress, rightNodeAddress);

                    // link to root pointer
                    this->rootNode = newRootAddress.getAddressNode();
                    this->rootNode->addressInDisk = newRootAddress;

                    return nullptr;
                }

                return new Address[2]{leftNodeAddress, rightNodeAddress};
            }

            // if parent node is not full, just insert a key-pointer pair in the parent node
            else
            {
                //  deallocate memory for original left subtree
                parentNode->childrenNodes[insertionIndex] = returnedChildSubTrees[0];
                parentNode->insertSubsequentPair(keyToInsertIntoParent, returnedChildSubTrees[1]);

                return nullptr;
            }
        }
    }
}

// displaying the tree by using BFS on the B+tree
void BPTree::display()
{
    std::cout << "enter display" << std::endl;
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
            std::cout << "Node address: " << currentNode << " \n";
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
                if (currentNode->childrenNodes[j].getAddressNode() != nullptr && !currentNode->isLeaf)
                {
                    nextLevel.push_back(currentNode->childrenNodes[j].getAddressNode());
                }
            }

            // add children nodes to nextLevel nodes if it is not nullptr
            // add last children Node to the nextLevel list cos there are n+1 children where n= number of keys
            if (currentNode->childrenNodes[j].getAddressNode() != nullptr && !currentNode->isLeaf)
            {
                nextLevel.push_back(currentNode->childrenNodes[j].getAddressNode());
            }

            std::cout << "]" << std::endl;

            std::cout << "Pointers: [";

            // displaying each element in the node
            // if its a non leaf node we need to loop through j+1
            for (int i = 0; i < currentNode->maxPointerSize; i++)
            {
                std::cout << currentNode->childrenNodes[i].getAddressNode() << ",";
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
Address BPTree::queryWithNumVotesAsKey(int key, int &nodesUpdated)
{
    Node *cursor = this->rootNode;
    nodesUpdated = 1;

    if (this->rootNode == nullptr)
    {
        std::cout << "BPTree does not exist. instantiate rootNode" << std::endl;
        return {nullptr, 0};
    }

    while (!cursor->isLeaf)
    {
        nodesUpdated += 1;
        // cursor->printNode();
        //  checks whether the key exists in the internal node
        int insertionIndex = cursor->binarySearchInsertIndex(key);
        // found the key
        if (insertionIndex == -1)
        {
            insertionIndex = cursor->binarySearch(key);
            cursor = cursor->childrenNodes[insertionIndex + 1].getAddressNode();
        }
        // key is not found in internal node
        else
        {
            // go right
            if (key >= cursor->keys[insertionIndex])
            {
                cursor = cursor->childrenNodes[insertionIndex + 1].getAddressNode();
            }
            // go left
            else
            {
                cursor = cursor->childrenNodes[insertionIndex].getAddressNode();
            }
        }
    }

    // on leaf node
    int index;
    for(int i=0;i<cursor->currentKeySize;i++){
        if(cursor->keys[i] >= key){
            index = i;
            break;
        }
    }    
    cursor->printNode();
    return cursor->childrenNodes[index];
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
    return findMinimumKeyInBPTree(node->childrenNodes[0].getAddressNode());
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
        cursor = cursor->childrenNodes[insertionIndex].getAddressNode();
        height++;
    }

    std::cout << "height of B+ Tree is: " << height << std::endl;

    return height;
}
// print answers for experiment 2
void BPTree::printBPDetails()
{
    // experiment 2
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
    // std::cout << "rootNode";
    // this->rootNode->printNode();
    // std::cout<<"first rootNode left children: "<<std::endl;
    // this->rootNode->childrenNodes[0].getAddressNode()->printNode();
    // std::cout<<"first rootNode right children: "<<std::endl;
    // this->rootNode->childrenNodes[1].getAddressNode()->printNode();

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
    int addressZeroCounter = 0;
    std::queue<Address> childrenNodesToSearch;
    // terminal condition if the node is a leaf, add node pointer into the vector
    if (currentNode.getAddressNode()->isLeaf)
    {
        // counting nodes
        nodeCount++;
        if(currentNode.getAddressNode())

        recordList.push_back(currentNode);
        // std::cout << "keys in leaf node: [ ";
        // for (int i = 0; i < currentNode.getAddressNode()->currentKeySize; i++)
        // {
        //     std::cout << currentNode.getAddressNode()->keys[i] << ", ";
        // }
        // std::cout << "]" << std::endl;
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