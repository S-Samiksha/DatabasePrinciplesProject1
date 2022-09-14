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
    testBPTree->insert(testBPTree->rootNode, 91, nullptr);





    std::cout << "root Node" << std::endl;
    testBPTree->rootNode->printNode();
    std::cout << "leaf Node1" << std::endl;
    ((Node **)testBPTree->rootNode->childrenNodes)[0]->printNode();
    std::cout << "leaf Node 2" << std::endl;
    ((Node **)testBPTree->rootNode->childrenNodes)[1]->printNode();
    std::cout << "leaf Node 3" << std::endl;
    ((Node **)testBPTree->rootNode->childrenNodes)[2]->printNode();

    testBPTree->display();

    // Node *child = ((Node **)testBPTree->rootNode->childrenNodes)[2];

    // Node **results = testBPTree->split(testBPTree->rootNode, 65, nullptr);
    // std::cout<<"insertindex:" <<testBPTree->rootNode->binarySearchInsertIndex(55)<<std::endl;

    // std::cout << "contents of left: [" << results[0]->keys[0] << "," << results[0]->keys[1] << "," << results[0]->keys[2] << std::endl;
    // std::cout << "contents of right: [" << results[1]->keys[0] << "," << results[1]->keys[1] << "," << results[1]->keys[2] << std::endl;

    // std::cout << "Root Node: " << testBPTree->rootNode << std::endl;
    // std::cout << "Child address: " << child << std::endl;
    // std::cout << "Found parent: " << testBPTree->findParentNode(testBPTree->rootNode, child) << std::endl;
    return 0;
}