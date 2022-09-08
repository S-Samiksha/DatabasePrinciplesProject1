#include <iostream>
#include "include/node.h"
#include "include/bptree.h"

int main()
{
    BPTree* testBPTree = new BPTree(3);
    testBPTree->makeTestTree();

    testBPTree->display();

    return 0;
}