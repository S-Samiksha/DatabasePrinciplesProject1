#include <iostream>
#include "include/node.h"
#include "include/bptree.h"

int main()
{
    BPTree* testBPTree = new BPTree(3);
    testBPTree->makeTestTree();

    std::cout << testBPTree->rootNode->keys.at(1) << std::endl;

    return 0;
}