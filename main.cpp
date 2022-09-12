#include <iostream>
#include "include/node.h"
#include "include/bptree.h"

int main()
{
    BPTree* testBPTree = new BPTree(3);
    // testBPTree->makeTestTree();
    testBPTree->insert(10);
    testBPTree->insert(20);
    testBPTree->insert(20);



    testBPTree->display();

    return 0;
}