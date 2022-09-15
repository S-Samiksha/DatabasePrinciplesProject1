#include <iostream>
#include "include/node.h"
#include "include/bptree.h"

int main()
{
    BPTree *testBPTree = new BPTree(3);
    // testBPTree->makeTestTree();
    testBPTree->rootNode = new Node(3, true);
    testBPTree->rootNode->insertInitialInLeafNode(70, nullptr, nullptr);

    testBPTree->insert(testBPTree->rootNode, 1, nullptr);
    testBPTree->insert(testBPTree->rootNode, 2, nullptr);
    testBPTree->insert(testBPTree->rootNode, 3, nullptr);
    testBPTree->insert(testBPTree->rootNode, 4, nullptr);
    testBPTree->insert(testBPTree->rootNode, 5, nullptr);
    testBPTree->insert(testBPTree->rootNode, 80, nullptr);
    testBPTree->insert(testBPTree->rootNode, 90, nullptr);
    testBPTree->insert(testBPTree->rootNode, 85, nullptr);
    testBPTree->insert(testBPTree->rootNode, 34, nullptr);
    std::cout<< "*******************inserting 91*******************\n\n"<<std::endl;
    testBPTree->insert(testBPTree->rootNode, 91, nullptr);
    testBPTree->insert(testBPTree->rootNode, 6, nullptr);
    // testBPTree->insert(testBPTree->rootNode, 7, nullptr);
    // testBPTree->insert(testBPTree->rootNode, 8, nullptr);
    // testBPTree->insert(testBPTree->rootNode, 9, nullptr);
    // testBPTree->insert(testBPTree->rootNode, 10, nullptr);
    // testBPTree->insert(testBPTree->rootNode, 11, nullptr);
    // testBPTree->insert(testBPTree->rootNode, 12, nullptr);



    // testBPTree->insert(testBPTree->rootNode, 91, nullptr);


    testBPTree->display();
    return 0;
}