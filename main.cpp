#include <iostream>
#include "include/node.h"
#include "include/bptree.h"

int main()
{
    BPTree *testBPTree = new BPTree(3);
    testBPTree->rootNode = new Node(3, true);
    testBPTree->insert(testBPTree->rootNode, 1, nullptr);
    testBPTree->insert(testBPTree->rootNode, 4, nullptr);
    testBPTree->insert(testBPTree->rootNode, 7, nullptr);
    testBPTree->insert(testBPTree->rootNode, 10, nullptr);
    testBPTree->insert(testBPTree->rootNode, 17, nullptr);
    testBPTree->insert(testBPTree->rootNode, 21, nullptr);
    testBPTree->insert(testBPTree->rootNode, 31, nullptr);
    testBPTree->insert(testBPTree->rootNode, 25, nullptr);
    testBPTree->insert(testBPTree->rootNode, 19, nullptr);
    testBPTree->insert(testBPTree->rootNode, 20, nullptr);
    testBPTree->insert(testBPTree->rootNode, 28, nullptr);
    testBPTree->insert(testBPTree->rootNode, 42, nullptr);
    testBPTree->insert(testBPTree->rootNode, 5, nullptr);
    testBPTree->insert(testBPTree->rootNode, 6, nullptr);
    testBPTree->insert(testBPTree->rootNode, 8, nullptr);
    testBPTree->insert(testBPTree->rootNode, 90, nullptr);
    testBPTree->insert(testBPTree->rootNode, 91, nullptr);
    testBPTree->insert(testBPTree->rootNode, 26, nullptr);
    testBPTree->insert(testBPTree->rootNode, 27, nullptr);
    testBPTree->insert(testBPTree->rootNode, 3, nullptr);
    testBPTree->insert(testBPTree->rootNode, 2, nullptr);
    testBPTree->insert(testBPTree->rootNode, 9, nullptr);
    testBPTree->insert(testBPTree->rootNode, 36, nullptr);
    testBPTree->insert(testBPTree->rootNode, 38, nullptr);
    testBPTree->linkLeafNodes();

    testBPTree->display();
    // testBPTree->printBPDetails();
   // testBPTree->remove(42);

    testBPTree->display();

    return 0;
}